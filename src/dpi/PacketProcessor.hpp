#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include <pcap.h>

#include <gears/Time.hpp>

#include "Logger.hpp"
#include "UserStorage.hpp"
#include "ReaderUtil.hpp"
#include "UserSessionPacketProcessor.hpp"
#include "FlowTraits.hpp"
#include "NetInterfaceProcessor.hpp"
#include "ShapingManager.hpp"

namespace dpi
{
  class PacketProcessor
  {
  public:
    PacketProcessor(
      UserStoragePtr user_storage,
      UserSessionPacketProcessorPtr user_session_packet_processor,
      LoggerPtr event_logger,
      std::string_view ip_rules_path);

    PacketProcessingState
    process_packet(
      const FlowTraits& flow_traits,
      unsigned long packet_size,
      const void* packet,
      UserSessionPacketProcessor::Direction direction,
      NetInterfacePtr send_interface);

    const UserSessionPacketProcessorPtr&
    user_session_packet_processor()
    {
      return user_session_packet_processor_;
    }
    
  private:
    struct ClientState
    {
      Gears::Time telegram_call_packet_start_timestamp;
      Gears::Time telegram_call_packet_last_timestamp;

      Gears::Time telegram_call_with_sber_open_start_timestamp;

      Gears::Time sber_packet_last_timestamp;
    };

    PacketProcessingState process_packet_(
      u_int16_t proto,
      uint32_t src_ip,
      uint32_t dst_ip,
      uint64_t packet_size,
      UserSessionPacketProcessor::Direction direction,
      const void* packet);

    bool process_session_packet_(
      uint32_t src_ip,
      uint32_t dst_ip,
      const SessionKey& session_key,
      uint64_t packet_size);

    UserPtr get_user_(
      uint32_t& src_ip,
      uint32_t& dst_ip,
      const Gears::Time& now) const;

    const SessionKey& proto_to_session_key_(u_int16_t proto) const;

  private:
    const Gears::Time TELEGRAM_CALL_MAX_PERIOD_ = Gears::Time(30);
    const Gears::Time SBER_OPEN_MAX_PERIOD_ = Gears::Time(60);
    const UserStoragePtr user_storage_;
    const LoggerPtr event_logger_;
    const SessionKey unknown_session_key_;
    const UserSessionPacketProcessorPtr user_session_packet_processor_;

    //SessionRuleConfig session_rule_config_;

    int packet_i_ = 0;
    std::unordered_map<uint32_t, std::string> ip_categories_;
    std::unordered_map<uint32_t, SessionKey> protocol_session_keys_;

    std::mutex client_states_lock_;
    std::unordered_map<uint32_t, ClientState> client_states_;
  };

  using PacketProcessorPtr = std::shared_ptr<PacketProcessor>;
}
