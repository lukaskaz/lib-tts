#pragma once

#include "shellcommand.hpp"
#include "tts/helpers.hpp"

#include <cstdint>
#include <string>
#include <tuple>

namespace tts
{

enum class language
{
    polish,
    english,
    german
};

enum class gender
{
    male,
    female
};

using index = uint8_t;
using voice_t = std::tuple<language, gender, index>;

class TextToVoiceIf
{
  public:
    virtual ~TextToVoiceIf() = default;
    virtual void speak(const std::string&) = 0;
    virtual void speak(const std::string&, const voice_t&) = 0;
};

template <typename T>
class TextToVoiceFactory
{
  public:
    static std::shared_ptr<TextToVoiceIf> create(const voice_t& voice)
    {
        auto shell = std::make_shared<shell::BashCommand>();
        return create(shell, voice);
    }
    static std::shared_ptr<TextToVoiceIf>
        create(std::shared_ptr<shell::ShellCommand> shell, const voice_t& voice)
    {
        auto helpers = ttshelpers::HelpersFactory::create();
        return create(shell, helpers, voice);
    }
    static std::shared_ptr<TextToVoiceIf>
        create(std::shared_ptr<shell::ShellCommand> shell,
               std::shared_ptr<ttshelpers::HelpersIf> helpers,
               const voice_t& voice)
    {
        return std::shared_ptr<T>(new T(shell, helpers, voice));
    }
};

} // namespace tts
