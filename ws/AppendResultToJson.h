#pragma once

#include <utility>

#include <rapidjson/document.h>

#include <hext/Result.h>


namespace ws {


void AppendResultToJson(const std::optional<hext::Result>& result,
                        rapidjson::Document& j);


} // namespace ws

