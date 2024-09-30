#include "mocks/mock_helpers.hpp"
#include "mocks/mock_shell.hpp"
#include "tts/interfaces/googleapi.hpp"
#include "tts/interfaces/googlebasic.hpp"

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

TEST_F(TestTts, googleBasicCreatedAndUsed_voiceIsChangedAndRestored)
{
    using namespace tts;
    const std::string testmessage = "Test message googlebasic!";
    const voice_t initialvoice{language::english, gender::male, 1},
        changedvoice = {language::polish, gender::male, 1};
    std::string urlfirst, urlsecond, urlthird;

    InSequence seq;
    EXPECT_CALL(*helpersMock, downloadFile(HasSubstr("tl=en"), testmessage, _))
        .WillOnce(DoAll(SaveArg<0>(&urlfirst), Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, downloadFile(HasSubstr("tl=pl"), testmessage, _))
        .WillOnce(DoAll(SaveArg<0>(&urlsecond), Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, downloadFile(HasSubstr("tl=en"), testmessage, _))
        .WillOnce(DoAll(SaveArg<0>(&urlthird), Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);

    auto tts = TextToVoiceFactory<googlebasic::TextToVoice>::create(
        shellMock, helpersMock, initialvoice);
    tts->speak(testmessage);
    tts->speak(testmessage, changedvoice);
    tts->speak(testmessage);

    EXPECT_NE(urlfirst, urlsecond);
    EXPECT_EQ(urlfirst, urlthird);
}

TEST_F(TestTts, googleApiCreatedAndUsed_voiceIsChangedAndRestored)
{
    using namespace tts;
    const std::string testmessage = "Test message googleapi!";
    const voice_t initialvoice{language::english, gender::male, 1},
        changedvoice = {language::polish, gender::male, 1};
    std::string resultjson = {"{\"audioContent\":\"BASE64ENCODEDAUDIO\"}"};
    std::string configfirst, configsecond, configthird;

    InSequence seq;
    EXPECT_CALL(*helpersMock, uploadData(_, HasSubstr("en-US"), _))
        .WillOnce(DoAll(SaveArg<1>(&configfirst), SetArgReferee<2>(resultjson),
                        Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadData(_, HasSubstr("pl-PL"), _))
        .WillOnce(DoAll(SaveArg<1>(&configsecond), SetArgReferee<2>(resultjson),
                        Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadData(_, HasSubstr("en-US"), _))
        .WillOnce(DoAll(SaveArg<1>(&configthird), SetArgReferee<2>(resultjson),
                        Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);

    auto tts = tts::TextToVoiceFactory<googleapi::TextToVoice>::create(
        shellMock, helpersMock, initialvoice);
    tts->speak(testmessage);
    tts->speak(testmessage, changedvoice);
    tts->speak(testmessage);

    EXPECT_NE(configfirst, configsecond);
    EXPECT_EQ(configfirst, configthird);
}

class TestTtsParams : public TestWithParam<tts::voice_t>
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

TEST_P(TestTtsParams, gooleBasicCreatedAndUsed_shellAndCurlCalledOnce)
{
    const std::string testmessage = "Test message googlebasic!";
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, downloadFile(_, testmessage, _)).Times(1);

    const auto& voice = GetParam();
    tts::TextToVoiceFactory<tts::googlebasic::TextToVoice>::create(
        shellMock, helpersMock, voice)
        ->speak(testmessage);
}

TEST_P(TestTtsParams, gooleApiCreatedAndUsed_shellAndCurlCalledOnce)
{
    const std::string testmessage = "Test message googleapi!";
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    std::string resultjson = {"{\"audioContent\":\"BASE64ENCODEDAUDIO\"}"};
    EXPECT_CALL(*helpersMock, uploadData(_, HasSubstr(testmessage), _))
        .WillOnce(DoAll(SetArgReferee<2>(resultjson), Return(true)));

    const auto& voice = GetParam();
    tts::TextToVoiceFactory<tts::googleapi::TextToVoice>::create(
        shellMock, helpersMock, voice)
        ->speak(testmessage);
}

auto getVoices()
{
    using namespace tts;
    static constexpr auto languages = {language::english, language::german,
                                       language::polish};
    static constexpr auto genders = {gender::male, gender::female};
    static constexpr uint8_t maxindex = 5;
    std::vector<voice_t> values;

    for (const auto& language : languages)
    {
        for (const auto& gender : genders)
        {
            for (uint8_t idx{0}; idx <= maxindex; idx++)
            {
                values.emplace_back(language, gender, idx);
            }
        }
    }
    return values;
}

INSTANTIATE_TEST_SUITE_P(_, TestTtsParams, ::testing::ValuesIn(getVoices()));
