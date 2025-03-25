#pragma once

#include <mutex>
#include <fstream>
#include <gears/Time.hpp>
#include <gears/HashTable.hpp>
#include <gears/HashTableAdapters.hpp>
#include <gears/CompositeActiveObject.hpp>
#include <gears/TaskRunner.hpp>
#include <gears/Planner.hpp>

#include "StatCollector.hpp"
#include "UserSessionPacketProcessor.hpp"

namespace dpi
{
  class StatUserSessionPacketProcessor:
    public UserSessionPacketProcessor,
    public Gears::CompositeActiveObject
  {
  public:
    StatUserSessionPacketProcessor(
      std::string ch_dump_path,
      const Gears::Time& dump_period = Gears::Time::ONE_MINUTE);

    virtual PacketProcessingState process_user_session_packet(
      const Gears::Time& time,
      const UserPtr& user,
      uint32_t src_ip,
      uint32_t dst_ip,
      Direction direction,
      const SessionKey& session_key,
      uint64_t packet_size,
      const void* packet) override;

    void
    dump();

  protected:
    struct StatKey
    {
      StatKey(
        const Gears::Time& date_val,
        std::string msisdn_val,
        std::string traffic_type_val,
        std::string traffic_category_val,
        uint32_t src_ip_val,
        uint32_t dst_ip_val,
        UserSessionPacketProcessor::Direction direction_val
        );

      bool
      operator==(const StatKey& right) const;

      unsigned long
      hash() const;

      const Gears::Time date;
      const std::string traffic_type;
      const std::string traffic_category;
      const std::string msisdn;
      const uint32_t src_ip;
      const uint32_t dst_ip;
      const UserSessionPacketProcessor::Direction direction;

    protected:
      void calc_hash_();

    protected:
      unsigned long hash_;
    };

    friend std::ostream&
    operator<<(std::ostream& out, const StatKey& dump_value);

    struct StatValue
    {
      StatValue();

      StatValue(int64_t packets_val, int64_t bytes_val);

      StatValue&
      operator+=(const StatValue& right) noexcept;

      int64_t packets;
      int64_t bytes;
    };

    friend std::ostream&
    operator<<(std::ostream& out, const StatValue& dump_value);

    typedef StatCollector<StatKey, StatValue> DetailedStatCollector;

    class StatsDumpTask;

  protected:
    static std::pair<std::string, std::string>
    generate_file_name_(const std::string& prefix);

    Gears::Time
    dump_stats_() noexcept;

  protected:
    const std::string ch_dump_path_;
    const Gears::Time dump_period_;
    Gears::TaskRunner_var task_runner_;
    Gears::Planner_var planner_;

    DetailedStatCollector detailed_stat_collector_;
  };

  typedef std::shared_ptr<StatUserSessionPacketProcessor> StatUserSessionPacketProcessorPtr;

  std::ostream&
  operator<<(std::ostream& out, const StatUserSessionPacketProcessor::StatKey& dump_value);

  std::ostream&
  operator<<(std::ostream& out, const StatUserSessionPacketProcessor::StatValue& dump_value);
}

namespace dpi
{
  // StatUserSessionPacketProcessor::StatValue
  inline
  StatUserSessionPacketProcessor::StatValue::StatValue()
    : packets(0),
      bytes(0)
  {}

  inline
  StatUserSessionPacketProcessor::StatValue::StatValue(
    int64_t packets_val, int64_t bytes_val)
    : packets(packets_val),
      bytes(bytes_val)
  {}

  inline StatUserSessionPacketProcessor::StatValue&
  StatUserSessionPacketProcessor::StatValue::operator+=(
    const StatUserSessionPacketProcessor::StatValue& right) noexcept
  {
    packets += right.packets;
    bytes += right.bytes;
    return *this;
  }

  // StatUserSessionPacketProcessor::StatKey
  inline
  StatUserSessionPacketProcessor::StatKey::StatKey(
    const Gears::Time& date_val,
    std::string msisdn_val,
    std::string traffic_type_val,
    std::string traffic_category_val,
    uint32_t src_ip_val,
    uint32_t dst_ip_val,
    UserSessionPacketProcessor::Direction direction_val
    )
    : date(date_val),
      msisdn(std::move(msisdn_val)),
      traffic_type(std::move(traffic_type_val)),
      traffic_category(std::move(traffic_category_val)),
      src_ip(src_ip_val),
      dst_ip(dst_ip_val),
      direction(direction_val),
      hash_(0)
  {
    calc_hash_();
  }

  inline bool
  StatUserSessionPacketProcessor::StatKey::operator==(const StatKey& right) const
  {
    return date == right.date &&
      traffic_type == right.traffic_type &&
      msisdn == right.msisdn &&
      src_ip == right.src_ip &&
      dst_ip == right.dst_ip &&
      direction == right.direction;
  }

  inline unsigned long
  StatUserSessionPacketProcessor::StatKey::hash() const
  {
    return hash_;
  }

  inline void
  StatUserSessionPacketProcessor::StatKey::calc_hash_()
  {
    Gears::Murmur64Hash hasher(hash_);
    hash_add(hasher, date.tv_sec);
    hash_add(hasher, traffic_type);
    hash_add(hasher, msisdn);
    hash_add(hasher, src_ip);
    hash_add(hasher, dst_ip);
    hash_add(hasher, static_cast<unsigned char>(direction));
  }

  inline std::ostream&
  operator<<(std::ostream& out, const StatUserSessionPacketProcessor::StatValue& dump_value)
  {
    out << dump_value.packets << "," << dump_value.bytes;
    return out;
  }

}
