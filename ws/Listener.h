#pragma once

#include <boost/beast/core.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

#include <memory>


namespace ws {


class Listener : public std::enable_shared_from_this<Listener>
{
public:
  Listener(boost::asio::io_context& ioc,
           boost::asio::ssl::context& ctx,
           boost::asio::ip::tcp::endpoint endpoint);
  void run();

private:
  void doAccept();
  void onAccept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);
  void onFail(boost::beast::error_code ec, const char * what) const;

  boost::asio::io_context& ioc_;
  boost::asio::ssl::context& ctx_;
  boost::asio::ip::tcp::acceptor acceptor_;
};


} // namespace ws

