#pragma once

#include "shellcommand.hpp"

#include <memory>
#include <string>

namespace tts
{
enum class language
{
    english,
    german,
    polish
};

class TextToVoiceIf
{
  public:
    virtual ~TextToVoiceIf(){};
    virtual void speak(const std::string&) = 0;
};

class TextToVoice : public TextToVoiceIf
{
  public:
    TextToVoice(std::shared_ptr<ShellCommand>, language);
    TextToVoice(std::shared_ptr<ShellCommand>, const std::string&, language);
    ~TextToVoice();

    void speak(const std::string&) override;

  private:
    std::shared_ptr<ShellCommand> commandHandler;
    std::string text;
    const std::string languageId;
    std::string audioFilePath;
    std::string playVoiceCmd;
    std::string getVoiceFromTextCmd;

    void init();
    void run();
};

class TextToVoiceFactory
{
  public:
    TextToVoiceFactory() = delete;
    TextToVoiceFactory(const TextToVoiceFactory&) = delete;
    TextToVoiceFactory(TextToVoiceFactory&&) = delete;
    TextToVoiceFactory& operator=(const TextToVoiceFactory&) = delete;
    TextToVoiceFactory& operator=(TextToVoiceFactory&&) = delete;

    static std::shared_ptr<TextToVoiceIf> create(language);
    static std::shared_ptr<TextToVoiceIf> create(std::shared_ptr<ShellCommand>,
                                                 language);
    static std::shared_ptr<TextToVoiceIf> create(const std::string&, language);
    static std::shared_ptr<TextToVoiceIf> create(std::shared_ptr<ShellCommand>,
                                                 const std::string&, language);
};

} // namespace tts
