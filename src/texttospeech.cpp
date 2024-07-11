#include "texttospeech.hpp"

#include <curl/curl.h>
#include <curl/easy.h>

#include <boost/filesystem.hpp>

#include <algorithm>
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

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellCommand> commandHandler,
                         std::shared_ptr<ttshelpers::HelpersIf> helpers,
                         language langOfText) :
    commandHandler{commandHandler},
    helpers{helpers}, languageId{langMap.at(langOfText)}
{}

TextToVoice::TextToVoice(std::shared_ptr<shell::ShellCommand> commandHandler,
                         std::shared_ptr<ttshelpers::HelpersIf> helpers,
                         const std::string& text, language langOfText) :
    commandHandler{commandHandler},
    helpers{helpers}, languageId{langMap.at(langOfText)}
{
    init();
    run(text);
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
    voiceFromTextUrl = std::string(tts::convUri) + "&tl=" + languageId + "&q=";
    playVoiceCmd = "play --no-show-progress " + audioFilePath + " --type alsa ";
}

inline void TextToVoice::run(const std::string& text)
{
    helpers->downloadFile(voiceFromTextUrl, text, audioFilePath);
    commandHandler->run(std::move(playVoiceCmd));
}

void TextToVoice::speak(const std::string& text)
{
    init();
    run(text);
}

void TextToVoice::speak(const std::string& texttospeak, language langOfText)
{
    std::string prevLanguageId{languageId};
    languageId = langMap.at(langOfText);
    speak(texttospeak);
    languageId = prevLanguageId;
}

std::shared_ptr<TextToVoiceIf> TextToVoiceFactory::create(language lang)
{
    auto shell = std::make_shared<shell::BashCommand>();
    return create(shell, lang);
}

std::shared_ptr<TextToVoiceIf>
    TextToVoiceFactory::create(std::shared_ptr<shell::ShellCommand> shell,
                               language lang)
{
    auto helpers = ttshelpers::HelpersFactory::create();
    return std::make_shared<TextToVoice>(shell, helpers, lang);
}

std::shared_ptr<TextToVoiceIf>
    TextToVoiceFactory::create(const std::string& text, language lang)
{
    auto shell = std::make_shared<shell::BashCommand>();
    return create(shell, text, lang);
}

std::shared_ptr<TextToVoiceIf>
    TextToVoiceFactory::create(std::shared_ptr<shell::ShellCommand> shell,
                               const std::string& text, language lang)
{
    auto helpers = ttshelpers::HelpersFactory::create();
    return std::make_shared<TextToVoice>(shell, helpers, text, lang);
}

} // namespace tts
