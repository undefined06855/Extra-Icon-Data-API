#pragma once

class ServerManager {
    ServerManager();
public:
    static ServerManager& get();

    std::string m_token;
    geode::EventListener<geode::utils::web::WebTask> m_listener;

    void runArgonAuth();
    void runAPIAuth(std::string argonToken);

    void checkSavedToken();

    void fetchAndUploadData();
    void requestData(SimplePlayer* player, IconType type, unsigned int accountID);
};
