#include "Listener.h"
#include "Session.h"
#include "WsBoostAsioHandlerLocation.h"

#include <boost/beast/core.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/strand.hpp>

#include <cstdint>
#ifdef HEXTWS_DEBUG
#include <iostream>
#endif
#include <memory>
#include <string>
#include <utility>


namespace ws {


Listener::Listener(boost::asio::io_context& ioc,
                   boost::asio::ssl::context& ctx,
                   boost::asio::ip::tcp::endpoint endpoint,
                   std::uint64_t max_searches)
: max_searches_(max_searches)
, ioc_(ioc)
, ctx_(ctx)
, acceptor_(boost::asio::make_strand(ioc))
{
  boost::beast::error_code ec;

  this->acceptor_.open(endpoint.protocol(), ec);
  if( ec )
  {
    this->onFail(ec, "open");
    return;
  }

  // SO_REUSEADDR
  this->acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if( ec )
  {
    this->onFail(ec, "set_option");
    return;
  }

  this->acceptor_.bind(endpoint, ec);
  if( ec )
  {
    this->onFail(ec, "bind");
    return;
  }

  this->acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if( ec )
  {
    this->onFail(ec, "listen");
    return;
  }
}

void Listener::run() { this->doAccept(); }

void Listener::doAccept()
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  this->acceptor_.async_accept(boost::asio::make_strand(this->ioc_),
                               boost::beast::bind_front_handler(
                                 &Listener::onAccept,
                                 this->shared_from_this()));
}

void Listener::onAccept(boost::beast::error_code ec,
                        boost::asio::ip::tcp::socket socket)
{
  WS_BOOST_ASIO_HANDLER_LOCATION;
  if( ec )
    this->onFail(ec, "accept");
  else
    std::make_shared<Session>(std::move(socket), this->ctx_, this->max_searches_)->run();

  this->doAccept();
}

void Listener::onFail([[maybe_unused]] boost::beast::error_code ec,
                      [[maybe_unused]] const char * what) const
{
#ifdef HEXTWS_DEBUG
  std::cerr << (std::string(what) + ": " + ec.message() + "\n");
#endif
}


} // namespace ws

