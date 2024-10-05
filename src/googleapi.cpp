#include "tts/interfaces/googleapi.hpp"

#include <boost/beast/core/detail/base64.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>

namespace tts
{

namespace googleapi
{

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
    Handler(std::shared_ptr<shell::ShellCommand> shell,
            std::shared_ptr<ttshelpers::HelpersIf> helpers,
            const voice_t& voice) :
        shell{shell},
        filesystem{audioDirectory, playbackName},
        google{configFile, helpers, voice}
    {}

    std::shared_ptr<shell::ShellCommand> shell;
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
                json sttConfig = json::parse(content)["tts"];
                if (sttConfig["key"].is_null() ||
                    sttConfig["key"].get<std::string>().empty())
                {
                    throw std::runtime_error(
                        "Cannot get TTS key from config file");
                }
                return convUri + "?key=" + sttConfig["key"].get<std::string>();
            }(configfile)},
            voice{voice}
        {}

        std::string getaudio(const std::string& text)
        {
            const auto& [code, name, gender] = getvoice();
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

      private:
        std::shared_ptr<ttshelpers::HelpersIf> helpers;
        const std::string audiourl;
        voice_t voice;

        decltype(voiceMap)::mapped_type getvoice() const
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
};

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellCommand> shell,
                         std::shared_ptr<ttshelpers::HelpersIf> helpers,
                         const voice_t& voice) :
    handler{std::make_unique<Handler>(shell, helpers, voice)}
{}

TextToVoice::~TextToVoice() = default;

void TextToVoice::speak(const std::string& text)
{
    auto audio = handler->google.getaudio(text);
    handler->filesystem.savetofile(audio);
    handler->shell->run(playAudioCmd);
}

void TextToVoice::speak(const std::string& text, const voice_t& voice)
{
    auto audio = handler->google.getaudio(text, voice);
    handler->filesystem.savetofile(audio);
    handler->shell->run(playAudioCmd);
}

} // namespace googleapi

} // namespace tts
