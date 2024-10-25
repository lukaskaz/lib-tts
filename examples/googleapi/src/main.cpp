#include "tts/interfaces/googleapi.hpp"

#include <iostream>

int main()
{
    try
    {
        using namespace tts::googleapi;
        auto tts = tts::TextToVoiceFactory::create<TextToVoice>(
            {tts::language::polish, tts::gender::female, 1});
        tts->speak("Jestem twoim asystentem, co mam zrobić?");
        tts->speak("Jestem twoim asystentem, co mam zrobić?",
                   {tts::language::polish, tts::gender::female, 1});
        tts->speak("Jestem twoim asystentem, co mam zrobić?",
                   {tts::language::polish, tts::gender::female, 2});
        tts->speak("Jestem twoim asystentem, co mam zrobić?",
                   {tts::language::polish, tts::gender::female, 3});
        tts->speak("Jestem twoim asystentem, co mam zrobić?",
                   {tts::language::polish, tts::gender::male, 1});
        tts->speak("Jestem twoim asystentem, co mam zrobić?",
                   {tts::language::polish, tts::gender::male, 2});
        tts::TextToVoiceFactory::create<TextToVoice>(
            {tts::language::english, tts::gender::female, 1})
            ->speak("Hi, this is second speech!");
        tts->speak("Tschüss, wie gehts du?",
                   {tts::language::german, tts::gender::female, 1});
        tts->speak("To wszystko, dzięki :)");
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
