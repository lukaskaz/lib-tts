#include "texttospeech.hpp"

#include <boost/beast/core/detail/base64.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <unordered_map>

namespace tts
{

static constexpr const char* audioDirectory = "audio/";
static constexpr const char* playbackName = "playback.mp3";

namespace simple
{
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
    audioFilePath = std::string{audioDirectory} + std::string{playbackName};
    voiceFromTextUrl = std::string(convUri) + "&tl=" + languageId + "&q=";
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

} // namespace simple

namespace extended
{
static constexpr const char* configFilePath = "../conf/init.json";
static constexpr const char* convUri =
    "https://texttospeech.googleapis.com/v1/text:synthesize";
static const std::unordered_map<
    language, std::tuple<std::string, std::string, std::string>>
    langMap = {{language::english, {"en-US", "en-US-Standard-C", "FEMALE"}},
               {language::german, {"de-DE", "de-DE-Standard-C", "FEMALE"}},
               {language::polish, {"pl-PL", "pl-PL-Standard-E", "FEMALE"}}};

using json = nlohmann::json;

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
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Cannot open config file for STT");
    }

    std::string fileData;
    configFile >> fileData;
    json sttConfig = json::parse(fileData)["tts"];

    if (sttConfig["key"].is_null() ||
        sttConfig["key"].get<std::string>().empty())
    {
        throw std::runtime_error("Cannot retrieve key for TTS");
    }
    usageKey = sttConfig["key"];

    boost::filesystem::create_directory(tts::audioDirectory);
    audioFilePath = std::string{audioDirectory} + std::string{playbackName};
    voiceFromTextUrl = std::string(convUri) + "?key=" + usageKey;
    playVoiceCmd = "play --no-show-progress " + audioFilePath + " --type alsa ";
}

inline std::string TextToVoice::decode(const std::string& encoded)
{
    using namespace boost::beast::detail;
    std::string decoded(encoded.size(), '\0');
    // decoded.resize(base64::encoded_size(encoded.size()));
    base64::decode(&decoded[0], encoded.c_str(), decoded.size());
    return decoded;
}

inline void TextToVoice::saveaudio(std::string&& audio)
{
    std::ofstream ofs(audioFilePath, std::ios::binary);
    ofs << audio;
}

inline void TextToVoice::run(const std::string& text)
{
    const auto& [code, name, gender] = languageId;
    const std::string config =
        "{'input':{'text':'" + text + "'},'voice':{'languageCode':'" + code +
        "','name':'" + name + "','ssmlGender':'" + gender +
        "'},'audioConfig':{'audioEncoding':'MP3'}}";
    std::string audioData;
    helpers->uploadData(voiceFromTextUrl, config, audioData);
    auto rawaudio =
        json::parse(std::move(audioData))["audioContent"].get<std::string>();
    saveaudio(decode(rawaudio));
    commandHandler->run(std::move(playVoiceCmd));
}

void TextToVoice::speak(const std::string& text)
{
    init();
    run(text);
}

void TextToVoice::speak(const std::string& texttospeak, language langOfText)
{
    auto prevLanguageId{languageId};
    languageId = langMap.at(langOfText);
    speak(texttospeak);
    languageId = prevLanguageId;
}

} // namespace extended

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
    return std::make_shared<extended::TextToVoice>(shell, helpers, lang);
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
    return std::make_shared<extended::TextToVoice>(shell, helpers, text, lang);
}

} // namespace tts
