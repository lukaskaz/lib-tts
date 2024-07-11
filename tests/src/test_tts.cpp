#include "mocks/mock_helpers.hpp"
#include "mocks/mock_shell.hpp"
#include "texttospeech.hpp"

using namespace testing;

class TestTts : public Test
{
  public:
    std::shared_ptr<NiceMock<ShellMock>> shellMock =
        std::make_shared<NiceMock<ShellMock>>();
    std::shared_ptr<NiceMock<HelpersMock>> helpersMock =
        std::make_shared<NiceMock<HelpersMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestTts, testTtsByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, downloadFile(_, _, _)).Times(1);
    auto tts = std::make_unique<tts::TextToVoice>(
        shellMock, helpersMock, "Test message one", tts::language::english);
}

TEST_F(TestTts, testTtsBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, downloadFile(_, _, _)).Times(1);
    auto tts = std::make_unique<tts::TextToVoice>(shellMock, helpersMock,
                                                  tts::language::english);
    tts->speak("Test message two");
}

TEST_F(TestTts, testTtsFactoryByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto tts = tts::TextToVoiceFactory::create(shellMock, "Test message one",
                                               tts::language::english);
}

TEST_F(TestTts, testTtsFactoryBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto tts =
        tts::TextToVoiceFactory::create(shellMock, tts::language::english);
    tts->speak("Test message two");
}
