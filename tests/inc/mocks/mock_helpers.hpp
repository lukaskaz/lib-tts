#pragma once

#include "tts/helpers.hpp"

#include <gmock/gmock.h>

class HelpersMock : public ttshelpers::HelpersIf
{
  public:
    MOCK_METHOD(bool, downloadFile,
                (std::string&, const std::string&, const std::string&),
                (override));
    MOCK_METHOD(bool, uploadData,
                (const std::string&, const std::string&, std::string&),
                (override));
};
