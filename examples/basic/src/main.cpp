#include "shellcommand.hpp"
#include "texttospeech.hpp"

#include <iostream>
#include <memory>

int main()
{
    try
    {
        auto shellCommand = std::make_shared<BashCommand>();
        auto tts = std::make_unique<tts::TextToVoice>(shellCommand,
                                                      tts::language::polish);
        tts->speak("Jestem twoim asystentem, co mam zrobić?");
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
