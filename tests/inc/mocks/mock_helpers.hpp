#pragma once

#include "tts/helpers.hpp"

#include <gmock/gmock.h>

class HelpersMock : public helpers::HelpersIf
{
  public:
    MOCK_METHOD(bool, downloadFile,
                (const std::string&, const std::string&, const std::string&),
                (override));
    MOCK_METHOD(bool, uploadData,
                (const std::string&, const std::string&, std::string&),
                (override));
    MOCK_METHOD(bool, createasync, (std::function<void()> &&), (override));
    MOCK_METHOD(bool, waitasync, (), (override));
    MOCK_METHOD(bool, killasync, (), (override));
};
