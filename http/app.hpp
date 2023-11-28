#pragma once

#include <boost/asio/io_context.hpp>

#include <memory>

namespace crow
{
class App
{
  public:
    explicit App(std::shared_ptr<boost::asio::io_context> ioIn) :
        io(std::move(ioIn))
    {}
    ~App()
    {
        // this->close();
    }

  private:
    std::shared_ptr<boost::asio::io_context> io;
};
} // namespace crow
