#include "tts/interfaces/googlebasic.hpp"

#include <boost/filesystem.hpp>

#include <algorithm>
#include <map>

namespace tts
{

namespace googlebasic
{

static constexpr const char* audioDirectory = "audio/";
static constexpr const char* playbackName = "playback.mp3";

static constexpr const char* convUri =
    "https://translate.google.com/translate_tts?client=tw-ob";
static const std::map<voice_t, std::string> voiceMap = {
    {{language::polish, gender::male, 1}, "pl"},
    {{language::polish, gender::female, 1}, "pl"},
    {{language::english, gender::male, 1}, "en"},
    {{language::english, gender::female, 1}, "en"},
    {{language::german, gender::male, 1}, "de"},
    {{language::german, gender::female, 1}, "de"}};

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellCommand> commandHandler,
                         std::shared_ptr<ttshelpers::HelpersIf> helpers,
                         const voice_t& voice) :
    commandHandler{commandHandler},
    helpers{helpers}
{
    setvoice(voice);
}

TextToVoice::~TextToVoice()
{
    boost::filesystem::remove_all(audioDirectory);
}

inline void TextToVoice::init()
{
    boost::filesystem::create_directory(audioDirectory);
    audioFilePath = std::string{audioDirectory} + std::string{playbackName};
    voiceFromTextUrl = std::string(convUri) + "&tl=" + voiceId + "&q=";
    playVoiceCmd = "play --no-show-progress " + audioFilePath + " --type alsa";
}

inline void TextToVoice::run(const std::string& text)
{
    helpers->downloadFile(voiceFromTextUrl, text, audioFilePath);
    commandHandler->run(std::move(playVoiceCmd));
}

inline void TextToVoice::setvoice(const voice_t& voice)
{
    if (voiceMap.contains(voice))
    {
        voiceId = voiceMap.at(voice);
    }
    else
    {
        voice_t defaultvoice = {std::get<language>(voice),
                                std::get<gender>(voice), 1};
        voiceId = voiceMap.at(defaultvoice);
    }
}

void TextToVoice::speak(const std::string& text)
{
    init();
    run(text);
}

void TextToVoice::speak(const std::string& texttospeak, const voice_t& voice)
{
    std::string prevVoiceId{voiceId};
    setvoice(voice);
    speak(texttospeak);
    voiceId = prevVoiceId;
}

} // namespace googlebasic

} // namespace tts
