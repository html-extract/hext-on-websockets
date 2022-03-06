#pragma once

#include <boost/asio/detail/handler_tracking.hpp>
#include <boost/current_function.hpp>

#define WS_BOOST_ASIO_HANDLER_LOCATION \
  BOOST_ASIO_HANDLER_LOCATION((__FILE__, __LINE__, BOOST_CURRENT_FUNCTION))

