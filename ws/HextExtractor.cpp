#include "HextExtractor.h"

#include <hext/Html.h>
#include <hext/ParseHext.h>
#include <hext/Result.h>

#include <cstdint>


namespace ws {


HextExtractor::HextExtractor(const char * hext_str)
: rule_(hext::ParseHext(hext_str))
{
}

hext::Result HextExtractor::extract(
    const char * html_str,
    std::uint64_t max_searches) const
{
  hext::Html html(html_str);
  return this->rule_.extract(html, max_searches);
}


} // namespace ws

