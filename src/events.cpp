#define GEODE_DEFINE_EVENT_EXPORTS
#include "events.hpp"

using namespace icon_api;

PopulateSimplePlayerEvent::PopulateSimplePlayerEvent(SimplePlayer* player, IconType type, std::unordered_map<geode::Mod*, ExtraIconData> data)
    : m_player(player)
    , m_iconType(type)
    , m_data(data) {}

geode::Result<SimplePlayer*> PopulateSimplePlayerEvent::getPlayer() { return geode::Ok(m_player); }
geode::Result<IconType> PopulateSimplePlayerEvent::getIconType() { return geode::Ok(m_iconType); }
geode::Result<ExtraIconData> PopulateSimplePlayerEvent::getDataForMod(geode::Mod* mod) { return geode::Ok(m_data[mod]); }


SupplyExtraIconDataEvent::SupplyExtraIconDataEvent(std::shared_ptr<std::unordered_map<geode::Mod*, ExtraIconData>> dataMap)
    : m_data(dataMap) {}

geode::Result<> SupplyExtraIconDataEvent::submitData(geode::Mod* mod, ExtraIconData data) { m_data->try_emplace(mod, data); return geode::Ok(); }
