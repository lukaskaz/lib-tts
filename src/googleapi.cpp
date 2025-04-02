#include "tts/interfaces/googleapi.hpp"

#include "shell/interfaces/linux/bash/shell.hpp"

#include <boost/beast/core/detail/base64.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <source_location>

namespace tts::googleapi
{

using namespace std::string_literals;
using json = nlohmann::json;

static const std::string audioDirectory = "audio/";
static const std::string playbackName = "playback.mp3";
static const std::string playAudioCmd =
    "play --no-show-progress " + audioDirectory + playbackName + " --type alsa";
static const std::string configFile = "../conf/init.json";
static const std::string convUri =
    "https://texttospeech.googleapis.com/v1/text:synthesize";

static const std::map<voice_t,
                      std::tuple<std::string, std::string, std::string>>
    voiceMap = {{{language::polish, gender::female, 1},
                 {"pl-PL", "pl-PL-Standard-E", "FEMALE"}},
                {{language::polish, gender::female, 2},
                 {"pl-PL", "pl-PL-Standard-A", "FEMALE"}},
                {{language::polish, gender::female, 3},
                 {"pl-PL", "pl-PL-Standard-D", "FEMALE"}},
                {{language::polish, gender::male, 1},
                 {"pl-PL", "pl-PL-Standard-B", "MALE"}},
                {{language::polish, gender::male, 2},
                 {"pl-PL", "pl-PL-Standard-C", "MALE"}},
                {{language::english, gender::female, 1},
                 {"en-US", "en-US-Standard-C", "FEMALE"}},
                {{language::english, gender::male, 1},
                 {"en-US", "en-US-Standard-A", "MALE"}},
                {{language::german, gender::female, 1},
                 {"de-DE", "de-DE-Standard-C", "FEMALE"}},
                {{language::german, gender::male, 1},
                 {"de-DE", "de-DE-Standard-B", "MALE"}}};

struct TextToVoice::Handler
{
  public:
    explicit Handler(const configmin_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{shell::Factory::create<shell::lnx::bash::Shell>()},
        filesystem{audioDirectory, playbackName},
        google{configFile, ttshelpers::HelpersFactory::create(),
               std::get<voice_t>(config)}
    {}

    explicit Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        filesystem{audioDirectory, playbackName},
        google{configFile,
               std::get<std::shared_ptr<ttshelpers::HelpersIf>>(config),
               std::get<voice_t>(config)}
    {}

    void speak(const std::string& text)
    {
        speak(text, getvoice());
    }

    void speak(const std::string& text, const voice_t& voice)
    {
        auto audio = google.getaudio(text, voice);
        filesystem.savetofile(audio);
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
        Filesystem(const std::string& dirname, const std::string& filename) :
            basedir{dirname}, filename{filename}
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

        void savetofile(const std::string& content) const
        {
            std::ofstream ofs(basedir + filename, std::ios::binary);
            ofs << content;
        }

      private:
        const std::string basedir;
        const std::string filename;
        bool direxist;
    } filesystem;
    class Google
    {
      public:
        Google(const std::string& configfile,
               std::shared_ptr<ttshelpers::HelpersIf> helpers,
               const voice_t& voice) :
            helpers{helpers},
            audiourl{[](const std::string& file) {
                std::ifstream ifs(file);
                if (!ifs.is_open())
                {
                    throw std::runtime_error("Cannot open config file for TTS");
                }
                auto content = std::string(
                    std::istreambuf_iterator<char>(ifs.rdbuf()), {});
                json ttsConfig = json::parse(content)["tts"];
                if (ttsConfig["key"].is_null() ||
                    ttsConfig["key"].get<std::string>().empty())
                {
                    throw std::runtime_error(
                        "Cannot get TTS key from config file");
                }
                return convUri + "?key=" + ttsConfig["key"].get<std::string>();
            }(configfile)},
            voice{voice}
        {}

        std::string getaudio(const std::string& text)
        {
            const auto& [code, name, gender] = getmappedvoice();
            const std::string config =
                "{'input':{'text':'" + text + "'},'voice':{'languageCode':'" +
                code + "','name':'" + name + "','ssmlGender':'" + gender +
                "'},'audioConfig':{'audioEncoding':'MP3'}}";
            std::string audioData;
            helpers->uploadData(audiourl, config, audioData);
            auto rawaudio = json::parse(std::move(audioData))["audioContent"]
                                .get<std::string>();
            return decode(rawaudio);
        }

        std::string getaudio(const std::string& text, const voice_t& tmpvoice)
        {
            const auto mainVoice{voice};
            voice = tmpvoice;
            auto audio = getaudio(text);
            voice = mainVoice;
            return audio;
        }

        voice_t getvoice() const
        {
            return voice;
        }

        void setvoice(const voice_t& voice)
        {
            this->voice = voice;
        }

      private:
        std::shared_ptr<ttshelpers::HelpersIf> helpers;
        const std::string audiourl;
        voice_t voice;

        decltype(voiceMap)::mapped_type getmappedvoice() const
        {
            decltype(voice) defaultvoice = {std::get<language>(voice),
                                            std::get<gender>(voice), 1};
            return voiceMap.contains(voice) ? voiceMap.at(voice)
                                            : voiceMap.at(defaultvoice);
        }

        std::string decode(const std::string& encoded)
        {
            using namespace boost::beast::detail;
            std::string decoded(encoded.size(), '\0');
            // decoded.resize(base64::encoded_size(encoded.size()));
            base64::decode(&decoded[0], encoded.c_str(), decoded.size());
            return decoded;
        }
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

} // namespace tts::googleapi
