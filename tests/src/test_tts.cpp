#include "mocks/mock_command.hpp"
#include "texttospeech.hpp"

using namespace testing;

class TestCommand : public Test
{
  public:
    std::shared_ptr<NiceMock<CommandsMock>> commandMock;

  protected:
    void SetUp() override
    {
        commandMock = std::make_shared<NiceMock<CommandsMock>>();
    }

    void TearDown() override
    {
        commandMock.reset();
    }
};

TEST_F(TestCommand, testTtsByCtorShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts = std::make_unique<tts::TextToVoice>(
        commandMock, "Test meesage one", tts::language::english);
}

TEST_F(TestCommand, testTtsBySpeakShellCmdCalledTwice)
{
    EXPECT_CALL(*commandMock, run(_)).Times(2);
    auto tts =
        std::make_unique<tts::TextToVoice>(commandMock, tts::language::english);
    tts->speak("Test message two");
}
