#include "AVPUtils.hpp"

#include "DiameterSession.hpp"
#include "InputDiameterRequestProcessor.hpp"

namespace dpi
{
  InputDiameterRequestProcessor::InputDiameterRequestProcessor(
    std::string origin_host,
    std::string origin_realm,
    std::shared_ptr<DiameterSession> diameter_session,
    ManagerPtr manager)
    : gx_application_id_(16777238),
      origin_host_(std::move(origin_host)),
      origin_realm_(std::move(origin_realm)),
      diameter_session_(diameter_session),
      manager_(manager)
  {}

  void
  InputDiameterRequestProcessor::process(const Diameter::Packet& request)
  {
    if (request.header().commandCode() == 258 || // RAR
      request.header().commandCode() == 274 || // ASR
      request.header().commandCode() == 275 // STR
    )
    {
      std::string session_id;

      // find Session-Id(263)
      for (int i = 0; i < request.numberOfAVPs(); ++i)
      {
        const auto& avp = request.avp(i);
        if (avp.header().avpCode() == 263)
        {
          ByteArray ba = avp.data().toOctetString();
          session_id = std::string(reinterpret_cast<const char*>(&ba[0]), ba.size());
          break;
        }
      }

      if (request.header().commandCode() == 258)
      {
        //std::cout << "[DIAMETER] Send response for RAR request" << std::endl;

        // check termination
        bool terminate = false;

        for (int i = 0; i < request.numberOfAVPs(); ++i)
        {
          const auto& avp = request.avp(i);
          if (avp.header().avpCode() == 1045) // AVP: Session-Release-Cause(1045)
          {
            terminate = true;
            break;
          }
        }

        std::unordered_set<std::string> install_charging_rule_names;
        std::unordered_set<std::string> remove_charging_rule_names;
        SCTPDiameterSession::get_charging_rules(
          install_charging_rule_names,
          remove_charging_rule_names,
          request);

        // Make update/terminate before RAR response
        auto manager = manager_.lock();

        std::cout << "[" << Gears::Time::get_time_of_day().gm_ft() << "] DIAMETER: PROCESS RAR for session_id = " <<
          session_id << std::endl;

        unsigned int result_code = 5012; //< DIAMETER_UNABLE_TO_COMPLY

        if (manager)
        {
          result_code = 2001;

          try
          {
            if (terminate)
            {
              manager->abort_session(session_id, true, true, true, "RAR terminate");
            }
            else
            {
              manager->update_session(
                session_id,
                true,
                true,
                "RAR request",
                install_charging_rule_names,
                remove_charging_rule_names);
            }
          }
          catch (const Manager::UnknownSession&)
          {
            result_code = 5002; //< DIAMETER_UNKNOWN_SESSION_ID
          }
        }

        auto diameter_session = diameter_session_.lock();
        if (diameter_session)
        {
          std::cout << "[" << Gears::Time::get_time_of_day().gm_ft() << "] DIAMETER: RESPONSE FOR RAR, result-code = " <<
            result_code << std::endl;
          auto rar_response_packet = generate_rar_response_packet_(
            session_id,
            request.header().hbhIdentifier(),
            request.header().eteIdentifier(),
            result_code);
          diameter_session->send_packet(rar_response_packet);
        }
      }
      else if (request.header().commandCode() == 274 || request.header().commandCode() == 275)
      {
        //std::cout << "[DIAMETER] Send response for ASR/STR request" << std::endl;

        auto manager = manager_.lock();

        std::cout << "[" << Gears::Time::get_time_of_day().gm_ft() << "] DIAMETER: PROCESS ASR/STR for session_id = " <<
          session_id << std::endl;

        unsigned int result_code = 5012; //< DIAMETER_UNABLE_TO_COMPLY

        if (manager)
        {
          result_code = 2001;

          try
          {
            manager->abort_session(session_id, true, true, true, "ASR terminate");
          }
          catch (const Manager::UnknownSession&)
          {
            result_code = 5002; //< DIAMETER_UNKNOWN_SESSION_ID
          }
        }

        auto asr_response_packet = generate_asr_response_packet_(
          session_id,
          request.header().commandCode(),
          request.header().hbhIdentifier(),
          result_code);
        auto diameter_session = diameter_session_.lock();
        if (diameter_session)
        {
          diameter_session->send_packet(asr_response_packet);
        }
      }
    }
    else
    {
      std::cout << "[" << Gears::Time::get_time_of_day().gm_ft() << "] DIAMETER, ERROR Input request (Command-Code = " <<
        request.header().commandCode() <<
        ") that can't be processed" << std::endl;
    }
  }

  ByteArray
  InputDiameterRequestProcessor::generate_rar_response_packet_(
    const std::string& session_id,
    uint32_t hbh_identifier,
    uint32_t ete_identifier,
    unsigned int result_code) const
  {
    auto packet = Diameter::Packet()
      .setHeader(
        Diameter::Packet::Header()
          .setCommandFlags(Diameter::Packet::Header::Flags())
          .setCommandCode(258)
          .setApplicationId(gx_application_id_)
          .setHBHIdentifier(hbh_identifier)
          .setETEIdentifier(ete_identifier)
      );

    packet
      .addAVP(create_string_avp(263, session_id, std::nullopt, true)) // Session-Id(263)
      .addAVP(create_string_avp(264, origin_host_, std::nullopt, true)) // Origin-Host(264)
      .addAVP(create_string_avp(296, origin_realm_, std::nullopt, true)) // Origin-Realm(296)
      .addAVP(create_uint32_avp(268, result_code, std::nullopt, true)) // Result-Code(268)
      /*
      .addAVP(create_ipv4_avp(
        501,
        request.access_network_charging_ip_address,
        10415,
        true)) //< Access-Network-Charging-Address
      .addAVP(create_avp( //< Access-Network-Charging-Identifier-Gx(1022)
        1022,
        Diameter::AVP::Data()
        .addAVP(create_octets_avp(503, uint32_to_buf_(request.charging_id), 10415, true))
        //< Access-Network-Charging-Identifier-Value(503)
        ,
        10415,
        true
        ))
      */
      ;

    return packet.updateLength().deploy();
  }

  ByteArray
  InputDiameterRequestProcessor::generate_asr_response_packet_(
    const std::string& session_id,
    unsigned long command_code,
    uint32_t hbh_identifier,
    unsigned int result_code) const
  {
    auto packet = Diameter::Packet()
      .setHeader(
        Diameter::Packet::Header()
          .setCommandFlags(Diameter::Packet::Header::Flags())
          .setCommandCode(command_code)
          .setApplicationId(gx_application_id_)
          .setHBHIdentifier(hbh_identifier)
          .setETEIdentifier(0xc15ecb12)
      );

    packet
      .addAVP(create_string_avp(263, session_id, std::nullopt, true)) // Session-Id(263)
      .addAVP(create_string_avp(264, origin_host_, std::nullopt, true)) // Origin-Host(264)
      .addAVP(create_string_avp(296, origin_realm_, std::nullopt, true)) // Origin-Realm(296)
      .addAVP(create_uint32_avp(268, 2001, std::nullopt, true)) // Result-Code(268)
      ;

    return packet.updateLength().deploy();
  }
}
