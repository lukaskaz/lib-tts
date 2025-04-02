#pragma once

#include "shell/interfaces/linux/bash/shell.hpp"
#include "tts/helpers.hpp"
#include "tts/interfaces/texttovoice.hpp"

#include <memory>

namespace tts
{

class TextToVoiceFactory
{
  public:
    template <typename T>
    static std::shared_ptr<TextToVoiceIf> create(const voice_t& voice)
    {
        auto shell = shell::Factory::create<shell::lnx::bash::Shell>();
        return create<T>(shell, voice);
    }

    template <typename T>
    static std::shared_ptr<TextToVoiceIf>
        create(std::shared_ptr<shell::ShellIf> shell, const voice_t& voice)
    {
        auto helpers = ttshelpers::HelpersFactory::create();
        return create<T>(shell, helpers, voice);
    }

    template <typename T>
    static std::shared_ptr<TextToVoiceIf>
        create(std::shared_ptr<shell::ShellIf> shell,
               std::shared_ptr<ttshelpers::HelpersIf> helpers,
               const voice_t& voice)
    {
        return std::shared_ptr<T>(new T(shell, helpers, voice));
    }
};

} // namespace tts
