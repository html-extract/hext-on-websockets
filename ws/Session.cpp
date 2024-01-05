#include "Session.h"
#include "HandleRequest.h"
#include "WsBoostAsioHandlerLocation.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <chrono>
#include <cstdlib>
#ifdef HEXTWS_DEBUG
#include <iostream>
#endif
#include <memory>
#include <string>
#include <utility>
#include <vector>


namespace ws {


Session::Session(boost::asio::ip::tcp::socket&& socket,
                 boost::asio::ssl::context& ctx,
                 std::uint64_t max_searches)
: max_searches_(max_searches)
, ws_(std::move(socket), ctx)
, buffer_()
{
}

void Session::run()
{
  boost::beast::get_lowest_layer(this->ws_)
    .expires_after(std::chrono::seconds(30));

  this->ws_.next_layer().async_handshake(
      boost::asio::ssl::stream_base::server,
      boost::beast::bind_front_handler(
        &Session::onHandshake,
        this->shared_from_this()));
}

void Session::onHandshake(boost::beast::error_code ec)
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  if( ec )
  {
    this->onFail(ec, "handshake");
    return;
  }

  boost::beast::get_lowest_layer(this->ws_).expires_never();

  this->ws_.set_option(boost::beast::websocket::stream_base::timeout::suggested(
                         boost::beast::role_type::server));

  this->ws_.async_accept(
      boost::beast::bind_front_handler(&Session::onAccept,
                                       this->shared_from_this()));
}

void Session::onAccept(boost::beast::error_code ec)
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  if( ec )
  {
    this->onFail(ec, "accept");
    return;
  }

  this->doRead();
}

void Session::doRead()
{
  this->ws_.async_read(
      this->buffer_,
      boost::beast::bind_front_handler(&Session::onRead,
                                       this->shared_from_this()));
}

void Session::onRead(boost::beast::error_code ec,
                     std::size_t /* bytes_transferred */)
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  if( ec == boost::beast::websocket::error::closed )
    return;

  if( ec )
    this->onFail(ec, "read");

  this->ws_.text(true);

  std::vector<char> data(boost::asio::buffer_size(this->buffer_.data()));
  boost::asio::buffer_copy(boost::asio::buffer(data), this->buffer_.data());

  auto response = std::make_shared<std::string>(
      ws::HandleRequest(
        const_cast<const char*>(data.data()),
        data.size(),
        this->max_searches_));

  this->ws_.async_write(boost::asio::buffer(*response),
                        boost::beast::bind_front_handler(
                            [response, self=this->shared_from_this()] (
                                boost::beast::error_code errc, std::size_t bytes_xfer)
                            {
                              self->onWrite(errc, bytes_xfer);
                            }));
}

void Session::onWrite(boost::beast::error_code ec,
                      std::size_t /* bytes_transferred */)
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  if( ec )
  {
    this->onFail(ec, "write");
    return;
  }

  this->buffer_.consume(this->buffer_.size());
  this->doRead();
}

void Session::onFail([[maybe_unused]] boost::beast::error_code ec,
                     [[maybe_unused]] const char * what) const
{
#ifdef HEXTWS_DEBUG
  std::cerr << (std::string(what) + ": " + ec.message() + "\n");
#endif
}


} // namespace ws

