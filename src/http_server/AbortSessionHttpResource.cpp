#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <jsoncons/json.hpp>

#include <dpi/NetworkUtils.hpp>

#include "Utils.hpp"
#include "AbortSessionHttpResource.hpp"

namespace dpi
{
  AbortSessionHttpResource::AbortSessionHttpResource(ManagerPtr manager)
    : manager_(std::move(manager))
  {}

  std::shared_ptr<httpserver::http_response>
  AbortSessionHttpResource::render(const httpserver::http_request& request)
  {
    if (request.get_method() == "POST")
    {
      auto body = request.get_content();
      jsoncons::json request_json = jsoncons::json::parse(body);

      if (request_json.contains("session_id"))
      {
        bool abort_gx = !request_json.contains("gx") || request_json["gx"].as_bool();
        bool abort_gy = !request_json.contains("gy") || request_json["gy"].as_bool();
        const std::string session_id = request_json["session_id"].as_string();
        manager_->abort_session(session_id, true, abort_gx, abort_gy, "Abort over http endpoint");
        return generate_json_response(request, "");
      }
      else
      {
        return generate_error_response(request, 400, "No session_id defined", "ERROR");
      }
    }

    return generate_uri_not_found_response(request);
  }
}
