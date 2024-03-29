#include "HandleRequest.h"
#include "HextExtractor.h"
#include "Message.h"

#include <cstddef>
#include <cstdint>
#include <string>
#ifdef HEXTWS_SECCOMP_TEST
#include <cstdlib>
#include <iostream>
#endif

#include <rapidjson/document.h>

#include <hext/MaxSearchError.h>
#include <hext/SyntaxError.h>



namespace ws {


std::string HandleRequest(
    const char * str,
    std::size_t size,
    std::uint64_t max_searches)
{
  rapidjson::Document request;
  request.Parse(str, size);

#ifdef HEXTWS_SECCOMP_TEST
  std::string_view input(str, size);
  if( input.find("seccomp") != std::string_view::npos )
  {
    // write to stderr and flush the buffer to force an actual `write`
    // system call.
    std::cerr << "ERROR: seccomp inactive or this thread is allowed "
                 "to write to stderr"
              << std::endl;
    std::abort();
  }
#endif

  Message msg;
  if( request.HasParseError() || !request.IsArray() || request.Size() != 2 ||
      !request[0].IsString() || !request[1].IsString() )
  {
    msg.status = 400;
    msg.error = "Invalid request format";
    return MessageToJson(msg);
  }

  try
  {
    // request[0] contains Hext
    ws::HextExtractor extractor(request[0].GetString());
    msg.status = 200;
    // request[1] contains Html
    msg.result = extractor.extract(request[1].GetString(), max_searches);
  }
  catch( const hext::SyntaxError& e )
  {
    msg.status = 400;
    msg.error = e.what();
  }
  catch( const hext::MaxSearchError& e )
  {
    msg.status = 400;
    msg.error = e.what();
  }
  catch( ... )
  {
    msg.status = 400;
    msg.error = "Please try again later";
  }

  return MessageToJson(msg);
}


} // namespace ws

