#pragma once
#include <Geode/modify/InfoLayer.hpp>

class $modify(HookedInfoLayer, InfoLayer) {
    static HookedInfoLayer* g_infoLayer;

    struct Fields {
        Fields();

        std::vector<CommentCell*> m_commentCells;
        geode::EventListener<geode::utils::web::WebTask> m_uploadListener;
    };

    void loadCommentsFinished(cocos2d::CCArray* comments, const char* p1);
    void onClose(cocos2d::CCObject* sender);

    void addCommentCell(CommentCell* cell);
};
