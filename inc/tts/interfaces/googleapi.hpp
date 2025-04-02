#pragma once

#include "logs/interfaces/logs.hpp"
#include "tts/factory.hpp"

namespace tts::googleapi
{

class TextToVoice : public TextToVoiceIf
{
  public:
    ~TextToVoice();
    void speak(const std::string&) override;
    void speak(const std::string&, const voice_t&) override;
    voice_t getvoice() override;
    void setvoice(const voice_t&) override;

  private:
    friend class tts::TextToVoiceFactory;
    TextToVoice(std::shared_ptr<shell::ShellIf>,
                std::shared_ptr<ttshelpers::HelpersIf>, const voice_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace tts::googleapi
