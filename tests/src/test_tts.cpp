#include "mocks/mock_helpers.hpp"
#include "mocks/mock_shell.hpp"
#include "tts/interfaces/googleapi.hpp"
#include "tts/interfaces/googlebasic.hpp"
#include "tts/interfaces/googlecloud.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

using namespace testing;
using namespace tts;

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

    auto tts = TextToVoiceFactory::create<googlebasic::TextToVoice,
                                          googlebasic::configall_t>(
        {initialvoice, shellMock, helpersMock, {}});
    tts->speak(testmessage);
    tts->speak(testmessage, changedvoice);
    tts->speak(testmessage);

    EXPECT_NE(urlfirst, urlsecond);
    EXPECT_EQ(urlfirst, urlthird);
}

TEST_F(TestTts, googleBasicCreated_checkedVoiceIsCorrect)
{
    const voice_t usedvoice{language::english, gender::male, 1};

    auto tts = TextToVoiceFactory::create<googlebasic::TextToVoice,
                                          googlebasic::configall_t>(
        {usedvoice, shellMock, helpersMock, {}});
    EXPECT_EQ(usedvoice, tts->getvoice());
}

TEST_F(TestTts, googleBasicCreatedAndVoiceChanged_voiceChangedProperly)
{
    const voice_t initialvoice{language::english, gender::male, 1},
        newvoice{language::german, gender::female, 1};

    auto tts = TextToVoiceFactory::create<googlebasic::TextToVoice,
                                          googlebasic::configall_t>(
        {initialvoice, shellMock, helpersMock, {}});
    ASSERT_NE(newvoice, tts->getvoice());

    tts->setvoice(newvoice);
    EXPECT_EQ(newvoice, tts->getvoice());
}

TEST_F(TestTts, googleApiCreatedAndUsed_voiceIsChangedAndRestored)
{
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

    auto tts = TextToVoiceFactory::create<googleapi::TextToVoice,
                                          googleapi::configall_t>(
        {initialvoice, shellMock, helpersMock, {}});
    tts->speak(testmessage);
    tts->speak(testmessage, changedvoice);
    tts->speak(testmessage);

    EXPECT_NE(configfirst, configsecond);
    EXPECT_EQ(configfirst, configthird);
}

TEST_F(TestTts, googleApiCreated_checkedVoiceIsCorrect)
{
    const voice_t usedvoice{language::english, gender::male, 1};

    auto tts = TextToVoiceFactory::create<googleapi::TextToVoice,
                                          googleapi::configall_t>(
        {usedvoice, shellMock, helpersMock, {}});
    EXPECT_EQ(usedvoice, tts->getvoice());
}

TEST_F(TestTts, googleApiCreatedAndVoiceChanged_voiceChangedProperly)
{
    const voice_t initialvoice{language::english, gender::male, 1},
        newvoice{language::german, gender::female, 1};

    auto tts = TextToVoiceFactory::create<googleapi::TextToVoice,
                                          googleapi::configall_t>(
        {initialvoice, shellMock, helpersMock, {}});
    ASSERT_NE(newvoice, tts->getvoice());

    tts->setvoice(newvoice);
    EXPECT_EQ(newvoice, tts->getvoice());
}

TEST_F(TestTts, googleCloudCreatedAndUsed_speakThrows)
{
    const std::string testmessage = "Test message googlecloud!";
    const voice_t voice{language::english, gender::male, 1};
    EXPECT_THROW((TextToVoiceFactory::create<googlecloud::TextToVoice,
                                             googlecloud::configall_t>(
                      {voice, shellMock, {}})
                      ->speak(testmessage)),
                 std::runtime_error);
}

TEST_F(TestTts, googleCloudCreatedAndUsed_speakWithChangedVoiceThrows)
{
    const std::string testmessage = "Test message googlecloud!";
    const voice_t initialvoice{language::english, gender::male, 1},
        changedvoice = {language::polish, gender::male, 1};
    EXPECT_THROW((TextToVoiceFactory::create<googlecloud::TextToVoice,
                                             googlecloud::configall_t>(
                      {initialvoice, shellMock, {}})
                      ->speak(testmessage, changedvoice)),
                 std::runtime_error);
}

class ConfigDummy
{
  public:
    ConfigDummy()
    {
        std::filesystem::create_directories(dir);
        std::ofstream{file};
    }
    ~ConfigDummy()
    {
        std::filesystem::remove(file);
    }

  private:
    const std::string dir{"../conf/"};
    const std::string file{dir + "key.json"};
};

TEST_F(TestTts, googleCloudCreated_checkedVoiceIsCorrect)
{
    ConfigDummy config;
    const voice_t usedvoice{language::english, gender::male, 1};

    auto tts = TextToVoiceFactory::create<googlecloud::TextToVoice,
                                          googlecloud::configall_t>(
        {usedvoice, shellMock, {}});
    EXPECT_EQ(usedvoice, tts->getvoice());
}

TEST_F(TestTts, googleCloudCreatedAndVoiceChanged_voiceChangedProperly)
{
    ConfigDummy config;
    const voice_t initialvoice{language::english, gender::male, 1},
        newvoice{language::german, gender::female, 1};

    auto tts = TextToVoiceFactory::create<googlecloud::TextToVoice,
                                          googlecloud::configall_t>(
        {initialvoice, shellMock, {}});
    ASSERT_NE(newvoice, tts->getvoice());

    tts->setvoice(newvoice);
    EXPECT_EQ(newvoice, tts->getvoice());
}

class TestTtsParams : public TestWithParam<voice_t>
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
    TextToVoiceFactory::create<googlebasic::TextToVoice,
                               googlebasic::configall_t>(
        {voice, shellMock, helpersMock, {}})
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
    TextToVoiceFactory::create<googleapi::TextToVoice, googleapi::configall_t>(
        {voice, shellMock, helpersMock, {}})
        ->speak(testmessage);
}

auto getVoices()
{
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
