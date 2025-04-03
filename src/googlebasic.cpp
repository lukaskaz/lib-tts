#include "tts/interfaces/googlebasic.hpp"

#include "shell/interfaces/linux/bash/shell.hpp"

#include <algorithm>
#include <filesystem>
#include <map>
#include <mutex>
#include <source_location>

namespace tts::googlebasic
{

using namespace std::string_literals;

static const std::string audioDirectory = "audio/";
static const std::string playbackName = "playback.mp3";
static const std::string audioFilePath = audioDirectory + playbackName;
static const std::string playAudioCmd =
    "play --no-show-progress " + audioFilePath + " --type alsa";
static constexpr const char* convUri =
    "https://translate.google.com/translate_tts?client=tw-ob";

static const std::map<voice_t, std::string> voiceMap = {
    {{language::polish, gender::male, 1}, "pl"},
    {{language::polish, gender::female, 1}, "pl"},
    {{language::english, gender::male, 1}, "en"},
    {{language::english, gender::female, 1}, "en"},
    {{language::german, gender::male, 1}, "de"},
    {{language::german, gender::female, 1}, "de"}};

struct TextToVoice::Handler : public std::enable_shared_from_this<Handler>
{
  public:
    explicit Handler(const configmin_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{shell::Factory::create<shell::lnx::bash::Shell>()},
        helpers{helpers::HelpersFactory::create()},
        filesystem{audioDirectory}, google{helpers, std::get<voice_t>(config),
                                           audioDirectory + playbackName}
    {}

    explicit Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        helpers{std::get<std::shared_ptr<helpers::HelpersIf>>(config)},
        filesystem{audioDirectory}, google{helpers, std::get<voice_t>(config),
                                           audioDirectory + playbackName}
    {}

    bool speak(const std::string& text)
    {
        return speak(text, getvoice());
    }

    bool speak(const std::string& text, const voice_t& voice)
    {
        if (std::unique_lock<std::mutex>(mtx, std::try_to_lock).owns_lock())
        {
            google.getaudio(text, voice);
            shell->run(playAudioCmd);
            return true;
        }
        return false;
    }

    bool speakasync(const std::string& text)
    {
        return speakasync(text, getvoice());
    }

    bool speakasync(const std::string& text, const voice_t& voice)
    {
        helpers->createasync([weak = weak_from_this(), text, voice]() {
            if (auto self = weak.lock())
                self->speak(text, voice);
        });
        return true;
    }

    void setvoice(const voice_t& voice)
    {
        google.setvoice(voice);
    }

    voice_t getvoice() const
    {
        return google.getvoice();
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const std::shared_ptr<shell::ShellIf> shell;
    const std::shared_ptr<helpers::HelpersIf> helpers;
    std::mutex mtx;
    class Filesystem
    {
      public:
        explicit Filesystem(const std::string& dirname) : basedir{dirname}
        {
            createdirectory();
        }

        ~Filesystem()
        {
            removedirectory();
        }

        void createdirectory()
        {
            direxist = !std::filesystem::create_directories(basedir);
        }
        void removedirectory() const
        {
            direxist ? false : std::filesystem::remove_all(basedir);
        }

      private:
        const std::string basedir;
        bool direxist;
    } filesystem;

    class Google
    {
      public:
        Google(std::shared_ptr<helpers::HelpersIf> helpers,
               const voice_t& voice, const std::string& audiopath) :
            helpers{helpers},
            audiopath{audiopath}
        {
            setvoice(voice);
        }

        void getaudio(const std::string& text)
        {
            helpers->downloadFile(audiourl, text, audiopath);
        }

        void getaudio(const std::string& text, const voice_t& tmpvoice)
        {
            auto mainVoice{voice};
            setvoice(tmpvoice);
            getaudio(text);
            setvoice(mainVoice);
        }

        voice_t getvoice() const
        {
            return voice;
        }

        void setvoice(const voice_t& voice)
        {
            this->voice = voice;
            decltype(voice) defaultvoice = {std::get<language>(voice),
                                            std::get<gender>(voice), 1};
            const auto& usevoice = voiceMap.contains(voice)
                                       ? voiceMap.at(voice)
                                       : voiceMap.at(defaultvoice);
            audiourl = std::string(convUri) + "&tl=" + usevoice + "&q=";
        }

      private:
        const std::shared_ptr<helpers::HelpersIf> helpers;
        std::string audiourl;
        std::string audiopath;
        voice_t voice;
    } google;

    void log(
        logs::level level, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(level, std::string{loc.function_name()}, msg);
    }
};

TextToVoice::TextToVoice(const config_t& config)
{
    handler = std::visit(
        [](const auto& config) -> decltype(TextToVoice::handler) {
            if constexpr (!std::is_same<const std::monostate&,
                                        decltype(config)>())
            {
                return std::make_unique<TextToVoice::Handler>(config);
            }
            throw std::runtime_error(
                std::source_location::current().function_name() +
                "-> config not supported"s);
        },
        config);
}
TextToVoice::~TextToVoice() = default;

bool TextToVoice::speak(const std::string& text)
{
    return handler->speak(text);
}

bool TextToVoice::speak(const std::string& text, const voice_t& voice)
{
    return handler->speak(text, voice);
}

bool TextToVoice::speakasync(const std::string& text)
{
    return handler->speakasync(text);
}

bool TextToVoice::speakasync(const std::string& text, const voice_t& voice)
{
    return handler->speakasync(text, voice);
}

voice_t TextToVoice::getvoice()
{
    return handler->getvoice();
}

void TextToVoice::setvoice(const voice_t& voice)
{
    handler->setvoice(voice);
}

} // namespace tts::googlebasic
