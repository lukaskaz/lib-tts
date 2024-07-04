#include "texttospeech.hpp"

#include <boost/filesystem.hpp>

#include <unordered_map>

namespace tts
{
static constexpr const char* audioDirectory = "audio/";
static constexpr const char* playbackName = "playback.mp3";
static constexpr const char* convUri =
    "https://translate.google.com/translate_tts?client=tw-ob";

static const std::unordered_map<language, std::string> langMap = {
    {language::english, "en"},
    {language::german, "de"},
    {language::polish, "pl"}};

TextToVoice::TextToVoice(std::shared_ptr<ShellCommand> commandHandler,
                         language langOfText) :
    commandHandler{commandHandler},
    languageId{langMap.at(langOfText)}
{}

TextToVoice::TextToVoice(std::shared_ptr<ShellCommand> commandHandler,
                         const std::string& text, language langOfText) :
    commandHandler{commandHandler},
    text{text}, languageId{langMap.at(langOfText)}
{
    init();
    run();
}

TextToVoice::~TextToVoice()
{
    boost::filesystem::remove_all(tts::audioDirectory);
}

inline void TextToVoice::init()
{
    boost::filesystem::create_directory(tts::audioDirectory);
    audioFilePath =
        std::string{tts::audioDirectory} + std::string{tts::playbackName};
    getVoiceFromTextCmd = "wget -q -U Mozilla -O " + audioFilePath + " \"" +
                          tts::convUri + "&tl=" + languageId + "&q=" + text +
                          "\"";
    playVoiceCmd = "play --no-show-progress " + audioFilePath + " --type alsa";
}

inline void TextToVoice::run()
{
    commandHandler->run(std::move(getVoiceFromTextCmd));
    commandHandler->run(std::move(playVoiceCmd));
}

void TextToVoice::speak(const std::string& speak)
{
    text = speak;
    init();
    run();
}

std::shared_ptr<TextToVoice> TextToVoiceFactory::create(language lang)
{
    auto shell = std::make_shared<BashCommand>();
    return std::make_shared<TextToVoice>(shell, lang);
}

std::shared_ptr<TextToVoice> TextToVoiceFactory::create(const std::string& text,
                                                        language lang)
{
    auto shell = std::make_shared<BashCommand>();
    return std::make_shared<TextToVoice>(shell, text, lang);
}

} // namespace tts
