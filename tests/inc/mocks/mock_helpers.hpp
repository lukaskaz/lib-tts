#pragma once

#include "helpers.hpp"

#include <gmock/gmock.h>

class HelpersMock : public helpers::HelpersIf
{
  public:
    MOCK_METHOD(bool, downloadFile,
                (std::string&, const std::string&, const std::string&),
                (override));
};
