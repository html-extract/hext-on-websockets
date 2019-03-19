#include "HextExtractor.h"

#include <hext/Html.h>
#include <hext/ParseHext.h>
#include <hext/Result.h>


namespace ws {


HextExtractor::HextExtractor(const char * hext_str)
: rule_(hext::ParseHext(hext_str))
{
}

hext::Result HextExtractor::extract(const char * html_str) const
{
  hext::Html html(html_str);
  return this->rule_.extract(html);
}


} // namespace ws

