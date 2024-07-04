#include "mocks/mock_command.hpp"
#include "texttospeech.hpp"

using namespace testing;

class TestCommand : public Test
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
