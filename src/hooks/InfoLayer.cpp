#include "InfoLayer.hpp"
#include "SimplePlayer.hpp"
#include "utils.hpp"

HookedInfoLayer* HookedInfoLayer::g_infoLayer = nullptr;
HookedInfoLayer::Fields::Fields()
    : m_commentCells({})
    , m_uploadListener() {}

void HookedInfoLayer::loadPage(int page, bool p1) {
    m_fields->m_commentCells.clear();
    InfoLayer::loadPage(page, p1);
}

void HookedInfoLayer::loadCommentsFinished(cocos2d::CCArray* comments, const char* p1) {
    g_infoLayer = this;
    InfoLayer::loadCommentsFinished(comments, p1);
    g_infoLayer = nullptr;

    auto fields = m_fields.self();

    auto req = geode::utils::web::WebRequest();

    auto playersObject = matjson::makeObject({});

    for (auto comment : m_fields->m_commentCells) {
        auto playerIcon = static_cast<HookedSimplePlayer*>(comment->getChildByIDRecursive("player-icon"));
        if (!playerIcon) continue;
        playersObject[fmt::to_string(comment->m_comment->m_accountID)] = matjson::Value(std::vector{ icon_api::utils::iconTypeToString(playerIcon->m_fields->m_iconType) });
    }

    geode::log::debug("{}", playersObject.dump());

    req.bodyJSON(matjson::makeObject({
        { "players", playersObject }
    }));
    req.timeout(std::chrono::seconds(10));

    auto task = req.post("https://icon-data-api.undefined0.dev/icons/get");
    
    fields->m_uploadListener.setFilter(task);
    fields->m_uploadListener.bind([](geode::utils::web::WebTask::Event* event) {
        if (auto res = event->getValue()) {
            geode::log::debug("{}", res->json().unwrapOrDefault().dump());
        }

        if (event->isCancelled()) {
            geode::log::error("Extra icon data request was cancelled!");
        }
    });
}

void HookedInfoLayer::addCommentCell(CommentCell* cell) {
    m_fields->m_commentCells.push_back(cell);
}
