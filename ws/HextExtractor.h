#pragma once

#include <hext/Result.h>
#include <hext/Rule.h>

#include <cstdint>


namespace ws {


class HextExtractor
{
public:
  explicit HextExtractor(const char * hext_str);
  hext::Result extract(const char * html_str, std::uint64_t max_searches) const;

private:
  hext::Rule rule_;
};


} // namespace ws

