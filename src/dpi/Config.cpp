#include <fstream>
#include <rapidjson/document.h>

#include "Config.hpp"

namespace dpi
{
  Config Config::read(const std::string_view& file)
  {
    Config result;

    std::string config_text;
    std::ifstream config_file_istr(std::string(file).c_str());
    std::string line;
    while(std::getline(config_file_istr, line))
    {
      config_text += line;
    }

    rapidjson::Document document;
    document.Parse(config_text.c_str());
    if (document.HasMember("pcap_file"))
    {
      result.pcap_file = document["pcap_file"].GetString();
    }

    if (document.HasMember("dpi_interface"))
    {
      result.interface = document["dpi_interface"].GetString();
    }

    if (document.HasMember("dpi_interface2"))
    {
      result.interface2 = document["dpi_interface2"].GetString();
    }

    if (document.HasMember("dump_stat_root"))
    {
      result.dump_stat_root = document["dump_stat_root"].GetString();
    }

    if (document.HasMember("ip_rules_root"))
    {
      result.ip_rules_root = document["ip_rules_root"].GetString();
    }

    if (document.HasMember("http_port"))
    {
      result.http_port = document["http_port"].GetInt();
    }

    if (document.HasMember("diameter_url"))
    {
      DiameterUrl result_diameter_url;

      const auto& diameter_url_obj = document["diameter_url"];

      if (diameter_url_obj.HasMember("local_endpoints"))
      {
	for (const auto& local_endpoint_json : diameter_url_obj["local_endpoints"].GetArray())
	{
	  result_diameter_url.local_endpoints.emplace_back(DiameterSession::Endpoint(
	    local_endpoint_json["host"].GetString(),
	    local_endpoint_json.HasMember("port") ? local_endpoint_json["port"].GetInt() : 0
	    ));
	}
      }

      std::vector<DiameterSession::Endpoint> connect_endpoints;
      if (diameter_url_obj.HasMember("connect_endpoints"))
      {
	for (const auto& endpoint_json : diameter_url_obj["connect_endpoints"].GetArray())
	{
	  result_diameter_url.connect_endpoints.emplace_back(DiameterSession::Endpoint(
	    endpoint_json["host"].GetString(),
	    endpoint_json["port"].GetInt()
	    ));
	}
      }

      if (diameter_url_obj.HasMember("origin-host"))
      {
        result_diameter_url.origin_host = diameter_url_obj["origin-host"].GetString();
      }

      if (diameter_url_obj.HasMember("origin-realm"))
      {
        result_diameter_url.origin_realm = diameter_url_obj["origin-realm"].GetString();
      }

      if (diameter_url_obj.HasMember("destination-host"))
      {
        result_diameter_url.destination_host = diameter_url_obj["destination-host"].GetString();
      }

      result.diameter_url = result_diameter_url;
    }

    return result;
  }
}
