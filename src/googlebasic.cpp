#include "tts/interfaces/googlebasic.hpp"

#include <algorithm>
#include <filesystem>
#include <map>

namespace tts::googlebasic
{

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
    Handler(std::shared_ptr<shell::ShellIf> shell,
            std::shared_ptr<ttshelpers::HelpersIf> helpers,
            const voice_t& voice) :
        shell{shell},
        filesystem{audioDirectory}, google{helpers, voice,
                                           audioDirectory + playbackName}
    {}

    std::shared_ptr<shell::ShellIf> shell;
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
};

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellIf> shell,
                         std::shared_ptr<ttshelpers::HelpersIf> helpers,
                         const voice_t& voice) :
    handler{std::make_unique<Handler>(shell, helpers, voice)}
{}

TextToVoice::~TextToVoice() = default;

void TextToVoice::speak(const std::string& text)
{
    handler->google.getaudio(text);
    handler->shell->run(playAudioCmd);
}

void TextToVoice::speak(const std::string& text, const voice_t& voice)
{
    handler->google.getaudio(text, voice);
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

} // namespace tts::googlebasic
