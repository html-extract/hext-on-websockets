#include "Listener.h"
#include "SetupSSL.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>


int main(int argc, char ** argv)
{
  if( argc != 8 )
  {
    std::cerr << "Usage: " << argv[0]
              << " <address> <port> <num_threads> <max-searches> <path-to-cert> <path-to-key> <path-to-dhparam>\n";
    return EXIT_FAILURE;
  }

  const auto num_threads = std::max<int>(1, std::atoi(argv[3]));
  boost::asio::io_context ioc(num_threads);
  boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);

  {
    const auto path_to_cert = std::string(argv[5]);
    const auto path_to_key = std::string(argv[6]);
    const auto path_to_dhparam = std::string(argv[7]);
    ws::SetupSSL(ctx, path_to_cert, path_to_key, path_to_dhparam);
  }

  const auto address = boost::asio::ip::make_address(argv[1]);
  const auto port = static_cast<unsigned short>(std::atoi(argv[2]));
  std::uint64_t max_searches = std::stoull(argv[4]);
  std::make_shared<ws::Listener>(ioc,
                                 ctx,
                                 boost::asio::ip::tcp::endpoint(address, port),
                                 max_searches)
    ->run();

  std::vector<std::thread> threads;
  assert(num_threads >= 1);
  threads.reserve(static_cast<std::size_t>(num_threads - 1));
  for(auto i = num_threads - 1; i > 0; --i)
    threads.emplace_back([&ioc]{ ioc.run(); });
  ioc.run();

  return EXIT_SUCCESS;
}

