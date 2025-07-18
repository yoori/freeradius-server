#include "TrafficRules.hpp"

namespace dpi
{
  /*
  Internet : MVNO_SBT_UNLIM / RG32 MK64
  Telegram : MVNO_SBT_TELEGRAM_UNLIM / RG61 MK161
  WhatsApp : MVNO_SBT_WHATSAPP_UNLIM / RG62 MK162
  IMO : MVNO_SBT_IMO_UNLIM / RG64 MK164
  YouTube : MVNO_SBT_YOUTUBE_UNLIM / RG75 MK175
  RuTube : MVNO_SBT_RUTUBE_UNLIM / RG76 MK176

  Okko : MVNO_SBT_OKKO_UNLIM / RG80 MK180
  Zvuk : MVNO_SBT_ZVUK_UNLIM / RG34 MK67
  */
  DiameterTrafficTypeProvider::DiameterTrafficTypeProvider()
  {
    default_traffic_types_.emplace_back(DiameterTrafficType(32, 64));

    diameter_traffic_type_by_session_key_[SessionKey("telegram_voip", "")].emplace_back(
      DiameterTrafficType(61, 161));
    diameter_traffic_type_by_session_key_[SessionKey("telegram", "")].emplace_back(
      DiameterTrafficType(61, 161));

    diameter_traffic_type_by_session_key_[SessionKey("whatsapp", "")].emplace_back(
      DiameterTrafficType(62, 162));
    diameter_traffic_type_by_session_key_[SessionKey("whatsapp_call", "")].emplace_back(
      DiameterTrafficType(62, 162));

    diameter_traffic_type_by_session_key_[SessionKey("youtube", "")].emplace_back(
      DiameterTrafficType(75, 175));
    diameter_traffic_type_by_session_key_[SessionKey("rutube", "")].emplace_back(
      DiameterTrafficType(76, 176));
    diameter_traffic_type_by_session_key_[SessionKey("imo", "")].emplace_back(
      DiameterTrafficType(64, 164));

    // add internet to all rules
    for (auto it = diameter_traffic_type_by_session_key_.begin();
      it != diameter_traffic_type_by_session_key_.end(); ++it)
    {
      std::copy(default_traffic_types_.begin(), default_traffic_types_.end(),
        std::back_inserter(it->second));
    }
  }

  const DiameterTrafficTypeProvider::DiameterTrafficTypeArray&
  DiameterTrafficTypeProvider::get_diameter_traffic_type(const SessionKey& session_key) const
  {
    auto it = diameter_traffic_type_by_session_key_.find(session_key);
    if (it != diameter_traffic_type_by_session_key_.end())
    {
      return it->second;
    }

    return default_traffic_types_;
  }
}
