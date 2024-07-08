#include "mocks/mock_command.hpp"
#include "texttospeech.hpp"

using namespace testing;

class TestTts : public Test
{
  public:
    std::shared_ptr<NiceMock<CommandsMock>> commandMock =
        std::make_shared<NiceMock<CommandsMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestTts, testTtsByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts = std::make_unique<tts::TextToVoice>(
        commandMock, "Test meesage one", tts::language::english);
}

TEST_F(TestTts, testTtsBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts =
        std::make_unique<tts::TextToVoice>(commandMock, tts::language::english);
    tts->speak("Test message two");
}

TEST_F(TestTts, testTtsFactoryByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts = tts::TextToVoiceFactory::create(commandMock, "Test meesage one",
                                               tts::language::english);
}

TEST_F(TestTts, testTtsFactoryBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts =
        tts::TextToVoiceFactory::create(commandMock, tts::language::english);
    tts->speak("Test message two");
}
