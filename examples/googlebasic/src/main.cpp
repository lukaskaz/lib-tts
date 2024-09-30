#include "tts/interfaces/googlebasic.hpp"

#include <iostream>

int main()
{
    try
    {
        using namespace tts::googlebasic;
        auto tts = tts::TextToVoiceFactory<TextToVoice>::create(
            {tts::language::polish, tts::gender::male, 1});
        tts->speak("Jestem twoim asystentem, co mam zrobić?");
        tts::TextToVoiceFactory<TextToVoice>::create(
            {tts::language::english, tts::gender::male, 1})
            ->speak("Hi, this is second speech!");
        tts->speak("Tschüss, wie gehts du?",
                   {tts::language::german, tts::gender::male, 1});
        tts->speak("To wszystko, dzięki :)");
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
