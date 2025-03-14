#include <string_view>
#include <gears/Tokenizer.hpp>
#include <gears/StringManip.hpp>
#include <httpserver.hpp>

#include "HttpServer.hpp"
#include "UserGetHttpResource.hpp"
#include "UserAddHttpResource.hpp"

namespace dpi
{
  class HttpServer::WebServerHolder
  {
  public:
    WebServerHolder(
      LoggerPtr logger,
      UserStoragePtr user_storage,
      unsigned int port,
      std::string url_prefix,
      unsigned long max_threads = 32)
      : user_get_http_resource_(std::make_shared<UserGetHttpResource>(user_storage)),
        user_add_http_resource_(std::make_shared<UserAddHttpResource>(user_storage)),
        ws(httpserver::create_webserver(port)
          .max_threads(max_threads)
          .put_processed_data_to_content()
          .file_upload_target(httpserver::FILE_UPLOAD_MEMORY_AND_DISK)
          .generate_random_filename_on_upload()
          .content_size_limit(200 * 1024 * 1024)
          )
    {
      ws.register_resource("/api/get_user", user_get_http_resource_.get());
      ws.register_resource("/api/add_user", user_add_http_resource_.get());
    }

  private:
    std::shared_ptr<httpserver::http_resource> user_get_http_resource_;
    std::shared_ptr<httpserver::http_resource> user_add_http_resource_;

  public:
    httpserver::webserver ws;
  };

  // HttpServer impl
  HttpServer::HttpServer(
    LoggerPtr logger,
    UserStoragePtr user_storage,
    unsigned int port,
    std::string url_prefix
    )
    : web_server_(std::make_shared<WebServerHolder>(
        logger,
        user_storage,
        port,
        url_prefix,
        16 //< threads
        ))
  {
  }

  void
  HttpServer::activate_object()
  {
    web_server_->ws.start(false);
    Gears::CompositeActiveObject::activate_object();
  }

  void
  HttpServer::deactivate_object()
  {
    Gears::CompositeActiveObject::deactivate_object();
  }

  void
  HttpServer::wait_object()
  {
    web_server_->ws.stop();
    Gears::CompositeActiveObject::wait_object();
  }
}
