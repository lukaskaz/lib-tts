#pragma once

#include <memory>
#include <string>

namespace helpers
{

class HelpersIf
{
  public:
    virtual ~HelpersIf()
    {}
    virtual bool downloadFile(std::string&, const std::string&,
                              const std::string&) = 0;
};

class Helpers : public HelpersIf
{
  public:
    bool downloadFile(std::string&, const std::string&,
                      const std::string&) override;
};

class HelpersFactory
{
  public:
  public:
    HelpersFactory() = delete;
    HelpersFactory(const HelpersFactory&) = delete;
    HelpersFactory(HelpersFactory&&) = delete;
    HelpersFactory& operator=(const HelpersFactory&) = delete;
    HelpersFactory& operator=(HelpersFactory&&) = delete;

    static std::shared_ptr<HelpersIf> create();
};

} // namespace helpers
