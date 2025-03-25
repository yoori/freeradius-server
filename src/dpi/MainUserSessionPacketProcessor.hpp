#pragma once

#include "UserSessionPacketProcessor.hpp"
#include "UserStorage.hpp"
#include "EventProcessor.hpp"
#include "ShapingManager.hpp"

namespace dpi
{
  class MainUserSessionPacketProcessor:
    public UserSessionPacketProcessor,
    public Gears::CompositeActiveObject
  {
  public:
    MainUserSessionPacketProcessor(
      UserStoragePtr user_storage,
      EventProcessorPtr event_processor);

    void
    set_session_rule_config(const SessionRuleConfig& session_rule_config);

    virtual PacketProcessingState
    process_user_session_packet(
      const Gears::Time& time,
      const UserPtr& user,
      uint32_t src_ip,
      uint32_t dst_ip,
      Direction direction,
      const SessionKey& session_key,
      uint64_t packet_size,
      const void* packet) override;

  private:
    // return true if need to send packet
    bool check_user_state_(
      User& user,
      const SessionKey& trigger_session_key,
      uint32_t src_ip,
      uint32_t dst_ip,
      const Gears::Time& now);

    bool process_event_(
      const std::string& event,
      const Gears::Time& now,
      uint32_t src_ip,
      uint32_t dst_ip);

  private:
    const UserStoragePtr user_storage_;
    const EventProcessorPtr event_processor_;
    const ShapingManagerPtr shaping_manager_;

    Gears::GnuHashSet<SessionKey> recheck_state_session_keys_;
    SessionRuleConfig session_rule_config_;
  };
}
