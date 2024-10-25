#include "tts/interfaces/googlecloud.hpp"

#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/texttospeech/v1/text_to_speech_client.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>

namespace tts::googlecloud
{

namespace texttospeech = google::cloud::texttospeech::v1;
namespace texttospeech_type = google::cloud::texttospeech_v1;

static const std::string audioDirectory = "audio/";
static const std::string playbackName = "playback.mp3";
static const std::string playAudioCmd =
    "play --no-show-progress " + audioDirectory + playbackName + " --type alsa";
// static constexpr const char* keyEnvVar = "GOOGLE_APPLICATION_CREDENTIALS";
static const std::string keyFile = "../conf/key.json";

using ssmlgender = texttospeech::SsmlVoiceGender;
static const std::map<voice_t, std::tuple<std::string, std::string, ssmlgender>>
    voiceMap = {{{language::polish, gender::female, 1},
                 {"pl-PL", "pl-PL-Standard-E", ssmlgender::FEMALE}},
                {{language::polish, gender::female, 2},
                 {"pl-PL", "pl-PL-Standard-A", ssmlgender::FEMALE}},
                {{language::polish, gender::female, 3},
                 {"pl-PL", "pl-PL-Standard-D", ssmlgender::FEMALE}},
                {{language::polish, gender::male, 1},
                 {"pl-PL", "pl-PL-Standard-B", ssmlgender::MALE}},
                {{language::polish, gender::male, 2},
                 {"pl-PL", "pl-PL-Standard-C", ssmlgender::MALE}},
                {{language::english, gender::female, 1},
                 {"en-US", "en-US-Standard-C", ssmlgender::FEMALE}},
                {{language::english, gender::male, 1},
                 {"en-US", "en-US-Standard-A", ssmlgender::MALE}},
                {{language::german, gender::female, 1},
                 {"de-DE", "de-DE-Standard-C", ssmlgender::FEMALE}},
                {{language::german, gender::male, 1},
                 {"de-DE", "de-DE-Standard-B", ssmlgender::MALE}}};

struct TextToVoice::Handler
{
    explicit Handler(std::shared_ptr<shell::ShellCommand> shell,
                     const voice_t& voice) :
        shell{shell},
        filesystem{audioDirectory, playbackName}, google{keyFile, voice}
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
        Google(const std::string& keyfile, const voice_t& voice) :
            client{texttospeech_type::MakeTextToSpeechConnection(
                google::cloud::Options{}
                    .set<google::cloud::UnifiedCredentialsOption>(
                        google::cloud::MakeServiceAccountCredentials(
                            [](const std::string& file) {
                                std::ifstream ifs(file);
                                if (!ifs.is_open())
                                {
                                    throw std::runtime_error(
                                        "Cannot open key file for TTS");
                                }
                                return std::string(
                                    std::istreambuf_iterator<char>(ifs.rdbuf()),
                                    {});
                            }(keyfile))))}
        {
            setvoice(voice);
            audio.set_audio_encoding(texttospeech::LINEAR16);
        }

        std::string getaudio(const std::string& text)
        {
            input.set_text(text);
            return client.SynthesizeSpeech(input, params, audio)
                ->audio_content();
        }

        std::string getaudio(const std::string& text, const voice_t& tmpvoice)
        {
            const auto mainVoice{voice};
            setvoice(tmpvoice);
            auto response = getaudio(text);
            setvoice(mainVoice);
            return response;
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
            const auto& [code, name, gender] = voiceMap.contains(voice)
                                                   ? voiceMap.at(voice)
                                                   : voiceMap.at(defaultvoice);

            params.set_language_code(code);
            params.set_name(name);
            params.set_ssml_gender(gender);
        }

      private:
        texttospeech_type::TextToSpeechClient client;
        texttospeech::SynthesisInput input;
        texttospeech::VoiceSelectionParams params;
        texttospeech::AudioConfig audio;
        voice_t voice;
    } google;
};

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellCommand> shell,
                         std::shared_ptr<ttshelpers::HelpersIf>,
                         const voice_t& voice) :
    handler{std::make_unique<Handler>(shell, voice)}
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

voice_t TextToVoice::getvoice()
{
    return handler->google.getvoice();
}

void TextToVoice::setvoice(const voice_t& voice)
{
    handler->google.setvoice(voice);
}

} // namespace tts::googlecloud
