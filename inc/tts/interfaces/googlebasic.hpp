#pragma once

#include "tts/factory.hpp"

namespace tts
{

namespace googlebasic
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
    std::string voiceId;
    std::string audioFilePath;
    std::string playVoiceCmd;
    std::string voiceFromTextUrl;

    void init();
    void run(const std::string&);
    void setvoice(const voice_t&);
};

} // namespace googlebasic

} // namespace tts
