#pragma once

#include <functional>
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

    bool createasync(std::function<void()>&&);

  private:
    friend class HelpersFactory;
    Helpers() = default;
};

class HelpersFactory
{
  public:
    static std::shared_ptr<HelpersIf> create()
    {
        return std::shared_ptr<Helpers>(new Helpers());
    }
};

} // namespace ttshelpers
