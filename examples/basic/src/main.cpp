#include "shellcommand.hpp"
#include "texttospeech.hpp"

#include <iostream>
#include <memory>

int main()
{
    try
    {
        auto tts = tts::TextToVoiceFactory::create(tts::language::polish);
        tts->speak("Jestem twoim asystentem, co mam zrobić?");
        tts::TextToVoiceFactory::create("Hi, this is second speech!",
                                        tts::language::english);
        tts->speak("Tschüss, wie gehts du?", tts::language::german);
        tts->speak("To wszystko, dzięki :)");
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
