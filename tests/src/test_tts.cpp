#include "mocks/mock_shell.hpp"
#include "texttospeech.hpp"

using namespace testing;

class TestTts : public Test
{
  public:
    std::shared_ptr<NiceMock<ShellMock>> shellMock =
        std::make_shared<NiceMock<ShellMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestTts, testTtsByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(2);
    auto tts = std::make_unique<tts::TextToVoice>(shellMock, "Test meesage one",
                                                  tts::language::english);
}

TEST_F(TestTts, testTtsBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(2);
    auto tts =
        std::make_unique<tts::TextToVoice>(shellMock, tts::language::english);
    tts->speak("Test message two");
}

TEST_F(TestTts, testTtsFactoryByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(2);
    auto tts = tts::TextToVoiceFactory::create(shellMock, "Test meesage one",
                                               tts::language::english);
}

TEST_F(TestTts, testTtsFactoryBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(2);
    auto tts =
        tts::TextToVoiceFactory::create(shellMock, tts::language::english);
    tts->speak("Test message two");
}
