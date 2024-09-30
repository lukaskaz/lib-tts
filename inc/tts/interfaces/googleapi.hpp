#pragma once

#include "tts/factory.hpp"

namespace tts
{

namespace googleapi
{

class TextToVoice : public TextToVoiceIf
{
  public:
    ~TextToVoice();
    void speak(const std::string&) override;
    void speak(const std::string&, const voice_t&) override;

  private:
    friend class TextToVoiceFactory<TextToVoice>;
    TextToVoice(std::shared_ptr<shell::ShellCommand>,
                std::shared_ptr<ttshelpers::HelpersIf>, const voice_t&);
    std::shared_ptr<shell::ShellCommand> commandHandler;
    std::shared_ptr<ttshelpers::HelpersIf> helpers;
    std::tuple<std::string, std::string, std::string> voiceId;
    std::string audioFilePath;
    std::string playVoiceCmd;
    std::string voiceFromTextUrl;
    std::string usageKey;

    void init();
    std::string decode(const std::string&);
    void run(const std::string&);
    void setvoice(const voice_t&);
    void saveaudio(std::string&&);
};

} // namespace googleapi

} // namespace tts
