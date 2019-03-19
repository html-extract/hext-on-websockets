#include "AppendResultToJson.h"

#include <utility>

#include <rapidjson/allocators.h>
#include <rapidjson/document.h>

#include <hext/Result.h>


namespace ws {


void AppendResultToJson(const std::optional<hext::Result>& result,
                        rapidjson::Document& j)
{
  rapidjson::Value set(rapidjson::kArrayType);
  auto& alloc = j.GetAllocator();

  if( result )
  {
    for(const auto& group : *result)
    {
      rapidjson::Value obj(rapidjson::kObjectType);

      for(const auto& p : group)
      {
        rapidjson::Value name(p.first.c_str(), alloc);
        rapidjson::Value value(p.second.c_str(), alloc);
        // If the key is already taken, transform the value into an array
        if( obj.HasMember(name) )
        {
          if( obj[name].IsArray() )
          {
            obj[name].PushBack(value, alloc);
          }
          else
          {
            rapidjson::Value array(rapidjson::kArrayType);
            array.PushBack(obj[name], alloc);
            array.PushBack(value, alloc);
            obj[name] = array;
          }
        }
        else
        {
          obj.AddMember(name, value, alloc);
        }
      }

      set.PushBack(obj, alloc);
    }
  }

  rapidjson::Value kResult("result", alloc);
  j.AddMember(kResult, set, alloc);
}


} // namespace ws

