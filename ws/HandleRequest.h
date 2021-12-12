#pragma once

#include <cstddef>
#include <cstdint>
#include <string>


namespace ws {


std::string HandleRequest(
    const char * str,
    std::size_t size,
    std::uint64_t max_searches);


} // namespace ws

