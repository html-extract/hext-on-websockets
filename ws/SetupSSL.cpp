#include "SetupSSL.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>

#include <string>


namespace ws {

  
void SetupSSL(boost::asio::ssl::context& ctx,
              const std::string& path_cert,
              const std::string& path_key,
              const std::string& path_dhparam)
{
  ctx.set_options(boost::asio::ssl::context::default_workarounds |
                  boost::asio::ssl::context::no_sslv2 |
                  boost::asio::ssl::context::single_dh_use);

  ctx.use_certificate_chain_file(path_cert);

  ctx.use_private_key_file(path_key,
                           boost::asio::ssl::context::file_format::pem);

  ctx.use_tmp_dh_file(path_dhparam);
}


} // namespace ws

