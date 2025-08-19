#pragma once
#include <Geode/loader/Event.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "undefined0.extra_icon_data_api"

namespace icon_api {

struct ExtraIconData {
    std::optional<matjson::Value>
        m_cube, m_ship, m_ball, m_ufo, m_wave, m_robot, m_spider, m_swing, m_jetpack,
        m_shared;

    matjson::Value toJSON();
    static ExtraIconData fromJSON(matjson::Value json);
};

class PopulateSimplePlayerEvent : public geode::Event {
protected:
    SimplePlayer* m_player;
    IconType m_iconType;
    std::unordered_map<geode::Mod*, ExtraIconData> m_data;

public:
    PopulateSimplePlayerEvent(SimplePlayer* player, IconType type, std::unordered_map<geode::Mod*, ExtraIconData> data);
    
    geode::Result<SimplePlayer*> getPlayer()
        GEODE_EVENT_EXPORT(&PopulateSimplePlayerEvent::getPlayer, (this));
    geode::Result<IconType> getIconType()
        GEODE_EVENT_EXPORT(&PopulateSimplePlayerEvent::getIconType, (this));
    geode::Result<ExtraIconData> getDataForMod(geode::Mod* mod)
        GEODE_EVENT_EXPORT(&PopulateSimplePlayerEvent::getDataForMod, (this, mod));
};

class SupplyExtraIconDataEvent : public geode::Event {
protected:
    std::shared_ptr<std::unordered_map<geode::Mod*, ExtraIconData>> m_data;
public:
    SupplyExtraIconDataEvent(std::shared_ptr<std::unordered_map<geode::Mod*, ExtraIconData>> dataMap);

    geode::Result<> submitData(geode::Mod* mod, ExtraIconData data)
        GEODE_EVENT_EXPORT(&SupplyExtraIconDataEvent::submitData, (this, mod, data));
};

}
