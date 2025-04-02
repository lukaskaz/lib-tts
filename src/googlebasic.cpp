#include "tts/interfaces/googlebasic.hpp"

#include "shell/interfaces/linux/bash/shell.hpp"

#include <algorithm>
#include <filesystem>
#include <map>
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

struct TextToVoice::Handler
{
  public:
    explicit Handler(const configmin_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{shell::Factory::create<shell::lnx::bash::Shell>()},
        filesystem{audioDirectory}, google{ttshelpers::HelpersFactory::create(),
                                           std::get<voice_t>(config),
                                           audioDirectory + playbackName}
    {}

    explicit Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        filesystem{audioDirectory},
        google{std::get<std::shared_ptr<ttshelpers::HelpersIf>>(config),
               std::get<voice_t>(config), audioDirectory + playbackName}
    {}

    void speak(const std::string& text)
    {
        speak(text, getvoice());
    }

    void speak(const std::string& text, const voice_t& voice)
    {
        google.getaudio(text, voice);
        shell->run(playAudioCmd);
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
        Google(std::shared_ptr<ttshelpers::HelpersIf> helpers,
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
        std::shared_ptr<ttshelpers::HelpersIf> helpers;
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

void TextToVoice::speak(const std::string& text)
{
    handler->speak(text);
}

void TextToVoice::speak(const std::string& text, const voice_t& voice)
{
    handler->speak(text, voice);
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
