#pragma once

#include <boost/asio/ssl/context.hpp>

#include <string>


namespace ws {


void SetupSSL(boost::asio::ssl::context& ctx,
              const std::string& path_cert,
              const std::string& path_key,
              const std::string& path_dhparam);


} // namespace ws

