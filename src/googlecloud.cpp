#include "tts/interfaces/googlecloud.hpp"

#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/texttospeech/v1/text_to_speech_client.h"

#include "shell/interfaces/linux/bash/shell.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <source_location>

namespace tts::googlecloud
{

namespace texttospeech = google::cloud::texttospeech::v1;
namespace texttospeech_type = google::cloud::texttospeech_v1;

using namespace std::string_literals;
using ssmlgender = texttospeech::SsmlVoiceGender;

static const std::string audioDirectory = "audio/";
static const std::string playbackName = "playback.mp3";
static const std::string playAudioCmd =
    "play --no-show-progress " + audioDirectory + playbackName + " --type alsa";
// static constexpr const char* keyEnvVar = "GOOGLE_APPLICATION_CREDENTIALS";
static const std::string keyFile = "../conf/key.json";

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
                 {"en-US", "en-US-Standard-D", ssmlgender::MALE}},
                {{language::german, gender::female, 1},
                 {"de-DE", "de-DE-Standard-C", ssmlgender::FEMALE}},
                {{language::german, gender::male, 1},
                 {"de-DE", "de-DE-Standard-B", ssmlgender::MALE}}};

struct TextToVoice::Handler
{
  public:
    explicit Handler(const configmin_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{shell::Factory::create<shell::lnx::bash::Shell>()},
        filesystem{audioDirectory, playbackName}, google{
                                                      keyFile,
                                                      std::get<voice_t>(config)}
    {}

    explicit Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        filesystem{audioDirectory, playbackName}, google{
                                                      keyFile,
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

} // namespace tts::googlecloud
