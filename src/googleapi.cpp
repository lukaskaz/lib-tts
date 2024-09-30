#include "tts/interfaces/googleapi.hpp"

#include <boost/beast/core/detail/base64.hpp>
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <map>

namespace tts
{

namespace googleapi
{

using json = nlohmann::json;

static constexpr const char* audioDirectory = "audio/";
static constexpr const char* playbackName = "playback.mp3";
static constexpr const char* configFilePath = "../conf/init.json";
static constexpr const char* convUri =
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
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Cannot open config file for TTS");
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

    boost::filesystem::create_directory(audioDirectory);
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

inline void TextToVoice::run(const std::string& text)
{
    const auto& [code, name, gender] = voiceId;
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

inline void TextToVoice::saveaudio(std::string&& audio)
{
    std::ofstream ofs(audioFilePath, std::ios::binary);
    ofs << audio;
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
    auto prevVoiceId{voiceId};
    setvoice(voice);
    speak(texttospeak);
    voiceId = prevVoiceId;
}

} // namespace googleapi

} // namespace tts
