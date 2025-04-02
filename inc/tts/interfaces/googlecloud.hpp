#pragma once

#include "logs/interfaces/logs.hpp"
#include "shell/interfaces/shell.hpp"
#include "tts/factory.hpp"
#include "tts/helpers.hpp"

#include <tuple>
#include <variant>

namespace tts::googlecloud
{

using configmin_t = std::tuple<voice_t, std::shared_ptr<logs::LogIf>>;
using configall_t = std::tuple<voice_t, std::shared_ptr<shell::ShellIf>,
                               std::shared_ptr<ttshelpers::HelpersIf>,
                               std::shared_ptr<logs::LogIf>>;
using config_t = std::variant<std::monostate, configmin_t, configall_t>;

class TextToVoice : public TextToVoiceIf
{
  public:
    ~TextToVoice();
    void speak(const std::string&) override;
    void speak(const std::string&, const voice_t&) override;
    void speakasync(const std::string&) override;
    void speakasync(const std::string&, const voice_t&) override;
    voice_t getvoice() override;
    void setvoice(const voice_t&) override;

  private:
    friend class tts::TextToVoiceFactory;
    TextToVoice(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace tts::googlecloud
