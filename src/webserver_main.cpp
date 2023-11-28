#include "../http/app.hpp"
#include "../include/webassets.hpp"

#include <boost/asio/io_context.hpp>

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    auto io = std::make_shared<boost::asio::io_context>();

    crow::App app(io);

#ifdef BMCWEB_ENABLE_STATIC_HOSTING
    crow::webassets::requestRoutes(app);
#endif

    return 0;
}
