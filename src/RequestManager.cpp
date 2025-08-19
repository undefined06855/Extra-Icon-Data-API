#include "RequestManager.hpp"
#include "events.hpp"

RequestManager::RequestManager()
    : m_uploadListener()
    , m_downloadListener()
    , m_isBatching(false)
    , m_batch({}) {}

RequestManager& RequestManager::get() {
    static RequestManager instance;
    return instance;
}

void RequestManager::fetchAndUploadData() {
    auto token = geode::Mod::get()->getSavedValue<std::string>("TOKEN_DO_NOT_SHARE", "");
    if (token.empty()) {
        geode::log::info("no token yet, cannot send");
        return;
    }

    auto map = std::make_shared<std::unordered_map<geode::Mod*, icon_api::ExtraIconData>>(std::unordered_map<geode::Mod*, icon_api::ExtraIconData>{});

    icon_api::SupplyExtraIconDataEvent(map).post();

    // now map should be populated
    // convert to format for server
    auto jsonData = matjson::makeObject({});

    jsonData["cube"] = matjson::makeObject({});
    jsonData["ship"] = matjson::makeObject({});
    jsonData["ball"] = matjson::makeObject({});
    jsonData["ufo"] = matjson::makeObject({});
    jsonData["wave"] = matjson::makeObject({});
    jsonData["robot"] = matjson::makeObject({});
    jsonData["spider"] = matjson::makeObject({});
    jsonData["swing"] = matjson::makeObject({});
    jsonData["jetpack"] = matjson::makeObject({});
    jsonData["shared"] = matjson::makeObject({});

    for (auto [mod, data] : *map) {
        if (data.m_cube) jsonData["cube"][mod->getID()] = data.m_cube;
        if (data.m_ship) jsonData["ship"][mod->getID()] = data.m_ship;
        if (data.m_ball) jsonData["ball"][mod->getID()] = data.m_ball;
        if (data.m_ufo) jsonData["ufo"][mod->getID()] = data.m_ufo;
        if (data.m_wave) jsonData["wave"][mod->getID()] = data.m_wave;
        if (data.m_robot) jsonData["robot"][mod->getID()] = data.m_robot;
        if (data.m_spider) jsonData["spider"][mod->getID()] = data.m_spider;
        if (data.m_swing) jsonData["swing"][mod->getID()] = data.m_swing;
        if (data.m_jetpack) jsonData["jetpack"][mod->getID()] = data.m_jetpack;
        if (data.m_shared) jsonData["shared"][mod->getID()] = data.m_shared;
    }

    geode::log::debug("jsonData: {}", jsonData.dump());

    // then send to server
    auto req = geode::utils::web::WebRequest();

    req.bodyJSON(matjson::makeObject({
        { "accountID", GJAccountManager::get()->m_accountID },
        { "token", token },
        { "data", jsonData }
    }));
    req.timeout(std::chrono::seconds(10));

    auto task = req.post("https://icon-data-api.undefined0.dev/icons/set");
    
    m_uploadListener.setFilter(task);
    m_uploadListener.bind([](geode::utils::web::WebTask::Event* event) {
        if (auto res = event->getValue()) {
            auto json = res->json().unwrapOrDefault();
            if (!json["success"].asBool().unwrapOr(false)) {
                geode::log::warn("{}", json["error"].asString().unwrapOrDefault());
                return;
            }

            geode::log::info("Uploaded extra icon data!");
        }

        if (event->isCancelled()) {
            geode::log::error("Upload request was cancelled!");
        }
    });
}
