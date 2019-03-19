#pragma once

#include <hext/Result.h>
#include <hext/Rule.h>


namespace ws {


class HextExtractor
{
public:
  explicit HextExtractor(const char * hext_str);
  hext::Result extract(const char * html_str) const;

private:
  hext::Rule rule_;
};


} // namespace ws

