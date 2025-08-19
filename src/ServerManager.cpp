#include "ServerManager.hpp"
#include "events.hpp"
#include <argon/argon.hpp>

ServerManager::ServerManager()
    : m_token(geode::Mod::get()->getSavedValue<std::string>("TOKEN_DO_NOT_SHARE", ""))
    , m_listener() {}

ServerManager& ServerManager::get() {
    static ServerManager instance;
    return instance;
}

void ServerManager::runArgonAuth() {
    // "its argon time" - me, 2025

    auto auth = argon::startAuth([this](geode::Result<std::string> res) {
        if (!res) {
            geode::log::warn("Auth failed: {}", res.unwrapErr());
            return;
        }

        auto token = std::move(res).unwrap();

        runAPIAuth(token);
    }, [](argon::AuthProgress progress) {
        geode::log::debug("Auth progress: {}", argon::authProgressToString(progress));
    });

    if (!auth.isOk()) {
        geode::log::warn("Failed to start auth attempt: {}", auth.unwrapErr());
    }
}

void ServerManager::runAPIAuth(std::string argonToken) {
    // "its extra icon data api time" - me, 2025

    auto req = geode::utils::web::WebRequest();

    req.bodyJSON(matjson::makeObject({
        { "accountID", GJAccountManager::get()->m_accountID },
        { "token", argonToken } 
    }));

    req.timeout(std::chrono::seconds(20));

    auto task = req.post("https://icon-data-api.undefined0.dev/token");

    m_listener.setFilter(task);
    m_listener.bind([this] (geode::utils::web::WebTask::Event* event) {
        auto value = event->getValue();
        if (!value) return;

        auto json = value->json().unwrapOrDefault();

        if (!json["success"].asBool().unwrapOrDefault()) {
            // fail :(
            geode::log::warn("Icon Data API returned unsucessfully when getting token!");
            return;
        }

        if (!json["token"].asString().isOk()) {
            // not a string?
            geode::log::warn("Icon Data API returned invalid response when getting token!");
            return;
        }

        m_token = json["token"].asString().unwrap();
    });
}

void ServerManager::fetchAndUploadData() {
    if (m_token.empty()) {
        geode::log::info("no token yet, cannot send");
        return;
    }

    auto map = std::make_shared<std::unordered_map<geode::Mod*, icon_api::ExtraIconData>>(std::unordered_map<geode::Mod*, icon_api::ExtraIconData>{});

    icon_api::SupplyExtraIconDataEvent(map).post();

    // now map should be populated
    // convert to format for server
    auto jsonData = matjson::makeObject({});

    for (auto [mod, data] : *map) {
        auto thisModData = matjson::makeObject({});

        if (data.m_cube) thisModData["cube"] = data.m_cube;
        if (data.m_ship) thisModData["ship"] = data.m_ship;
        if (data.m_ball) thisModData["ball"] = data.m_ball;
        if (data.m_ufo) thisModData["ufo"] = data.m_ufo;
        if (data.m_wave) thisModData["wave"] = data.m_wave;
        if (data.m_robot) thisModData["robot"] = data.m_robot;
        if (data.m_spider) thisModData["spider"] = data.m_spider;
        if (data.m_swing) thisModData["swing"] = data.m_swing;
        if (data.m_jetpack) thisModData["jetpack"] = data.m_jetpack;
        if (data.m_shared) thisModData["shared"] = data.m_shared;

        jsonData[mod->getID()] = thisModData;
    }

    // then send to server
    auto req = geode::utils::web::WebRequest();

    req.bodyJSON(matjson::makeObject({
        { "accountID", GJAccountManager::get()->m_accountID },
        { "data", jsonData }
    }));
    req.timeout(std::chrono::seconds(10));

    auto task = req.post("https://icon-data.undefined0.dev/icons");
    
    m_listener.setFilter(task);
    m_listener.bind([](geode::utils::web::WebTask::Event* event) {
        
    });
}

void ServerManager::requestData(SimplePlayer* player, IconType type, unsigned int accountID) {
    
    icon_api::PopulateSimplePlayerEvent(player, type)
}
