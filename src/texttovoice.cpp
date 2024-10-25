#include "tts/interfaces/texttovoice.hpp"

#include "shellcommand.hpp"

namespace tts
{

void tts::TextToVoiceIf::kill()
{
    shell::BashCommand().run("killall -s KILL play");
}

} // namespace tts
