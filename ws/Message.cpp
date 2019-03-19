#include "Message.h"
#include "AppendResultToJson.h"

#include <string>

#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace ws {


Message::Message()
: result()
, status(200)
, error("")
{}

std::string MessageToJson(const Message& msg)
{
  rapidjson::Document j;
  j.SetObject();
  auto& alloc = j.GetAllocator();

  rapidjson::Value kStat("status", alloc);
  j.AddMember(kStat, msg.status, alloc);
  if( msg.status == 200 )
  {
    AppendResultToJson(msg.result, j);
  }
  else
  {
    rapidjson::Value kError("error", alloc);
    if( msg.error.empty() )
    {
      rapidjson::Value vError("Please try again later", alloc);
      j.AddMember(kError, vError, alloc);
    }
    else
    {
      rapidjson::Value vError(msg.error.c_str(), alloc);
      j.AddMember(kError, vError, alloc);
    }
  }

  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  j.Accept(writer);

  return std::string(s.GetString());
}


} // namespace ws

