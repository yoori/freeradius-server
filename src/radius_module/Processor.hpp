#pragma once

#include <gears/Exception.hpp>

#include <dpi/UserStorage.hpp>
#include <dpi/UserSessionStorage.hpp>
#include <dpi/Logger.hpp>

#include <dpi/DiameterSession.hpp>

class Processor
{
public:
  DECLARE_EXCEPTION(Invalid, Gears::DescriptiveException);

  enum class AcctStatusType: int
  {
    START = 1,
    STOP = 2,
    UPDATE = 3
  };

public:
  Processor(
    dpi::UserStoragePtr user_storage,
    dpi::UserSessionStoragePtr user_session_storage,
    dpi::DiameterSessionPtr gx_diameter_session,
    dpi::DiameterSessionPtr gy_diameter_session);

  void load_config(std::string_view config_path);

  bool process_request(
    AcctStatusType acct_status_type,
    std::string_view called_station_id,
    std::string_view imsi,
    std::string_view imei,
    uint32_t framed_ip_address,
    uint32_t nas_ip_address,
    uint8_t rat_type,
    std::string_view mcc_mnc,
    uint8_t tz,
    uint32_t sgsn_address,
    uint32_t access_network_charging_address,
    uint32_t charging_id,
    const char* gprs_negotiated_qos_profile);

  dpi::LoggerPtr logger() const;

  dpi::LoggerPtr event_logger() const;

private:
  void init_gx_gy_session_(const dpi::UserSessionTraits& user_session_traits);

  void terminate_gx_gy_session_(const dpi::UserSession& user_session);

private:
  dpi::LoggerPtr logger_;
  dpi::LoggerPtr event_logger_;
  dpi::UserStoragePtr user_storage_;
  dpi::UserSessionStoragePtr user_session_storage_;
  std::string config_path_;
  std::string diameter_url_;
  dpi::DiameterSessionPtr gx_diameter_session_;
  dpi::DiameterSessionPtr gy_diameter_session_;
};

using ProcessorPtr = std::shared_ptr<Processor>;

inline dpi::LoggerPtr
Processor::logger() const
{
  return logger_;
}

inline dpi::LoggerPtr
Processor::event_logger() const
{
  return event_logger_;
}
