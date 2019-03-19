#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <cstdlib>
#include <memory>


namespace ws {


class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx);
  void run();
  void onHandshake(boost::beast::error_code ec);
  void onAccept(boost::beast::error_code ec);
  void doRead();
  void onRead(boost::beast::error_code ec, std::size_t /* bytes_transferred */);
  void onWrite(boost::beast::error_code ec, std::size_t /* bytes_transferred */);

private:
  void onFail(boost::beast::error_code ec, const char * what) const;

  boost::beast::websocket::stream<
    boost::beast::ssl_stream<boost::beast::tcp_stream>> ws_;
  boost::beast::flat_buffer buffer_;
};


} // namespace ws

