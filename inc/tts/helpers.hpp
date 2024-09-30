#pragma once

#include <memory>
#include <string>

namespace ttshelpers
{

class HelpersIf
{
  public:
    virtual ~HelpersIf()
    {}
    virtual bool uploadData(const std::string&, const std::string&,
                            std::string&) = 0;
    virtual bool downloadFile(std::string&, const std::string&,
                              const std::string&) = 0;
};

class Helpers : public HelpersIf
{
  public:
    bool uploadData(const std::string&, const std::string&,
                    std::string&) override;
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

} // namespace ttshelpers
