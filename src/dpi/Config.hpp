#pragma once

#include <string>
#include <optional>
#include <vector>
#include <set>

#include "DiameterSession.hpp"

namespace dpi
{
  struct DiameterUrl
  {
    struct RemoteEndpoint
    {
      std::string host;
      unsigned int port = 0;
    };

    std::vector<SCTPConnection::Endpoint> local_endpoints;
    std::vector<SCTPConnection::Endpoint> connect_endpoints;
    std::string origin_host;
    std::string origin_realm;
    std::string destination_host;
    std::string destination_realm;
  };

  struct Config
  {
    struct Radius
    {
      unsigned int listen_port = 0;
      std::string secret;
      std::string dictionary;
    };

    struct RadiusAttributeSource
    {
      std::string name;
      std::string vendor;
    };

    struct Diameter
    {
      struct PassAttribute
      {
        std::string avp_path;
        RadiusAttributeSource source;
      };

      std::optional<DiameterUrl> diameter_url;
      std::vector<PassAttribute> pass_attributes;
    };

    std::string pcap_file;
    std::string interface;
    std::string interface2;
    std::string dump_stat_root;
    std::string ip_rules_root;
    unsigned int http_port = 0;
    std::string pcc_config_file;

    std::optional<Radius> radius;
    std::optional<Diameter> gx;
    std::optional<Diameter> gy;
    std::string diameter_dictionary;

    static Config read(const std::string_view& file);
  };
}
