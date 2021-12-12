#pragma once

#include <optional>
#include <string>
#include <utility>

#include <hext/Result.h>


namespace ws {


struct Message
{
  Message();

  std::optional<hext::Result> result;
  int status;
  std::string error;
};


std::string MessageToJson(const Message& msg);


} // namespace ws

