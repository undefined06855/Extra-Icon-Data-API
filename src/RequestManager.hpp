#pragma once

struct PlayerData {
    SimplePlayer* m_player;
    IconType m_type;
    unsigned int m_accountID;
};

struct FullPlayerData {
    SimplePlayer* m_cube;
    SimplePlayer* m_ship;
    SimplePlayer* m_ball;
    SimplePlayer* m_ufo;
    SimplePlayer* m_wave;
    SimplePlayer* m_robot;
    SimplePlayer* m_spider;
    SimplePlayer* m_swing;
    SimplePlayer* m_jetpack;
    unsigned int m_accountID;
};

class RequestManager {
    RequestManager();
public:
    static RequestManager& get();

    geode::EventListener<geode::utils::web::WebTask> m_uploadListener;
    geode::EventListener<geode::utils::web::WebTask> m_downloadListener;

    bool m_isBatching;
    std::vector<CommentCell*> m_batch;

    void fetchAndUploadData();
};
