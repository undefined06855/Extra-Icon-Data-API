#pragma once
#include <Geode/ui/Popup.hpp>

class ArgonVerificationPopup : public geode::Popup<> {
public:
    static ArgonVerificationPopup* create();
    bool setup();

    cocos2d::CCLabelBMFont* m_content;
    cocos2d::CCLabelBMFont* m_subtitle;
    CCMenuItemSpriteExtra* m_button;
    bool m_done;
    geode::EventListener<geode::utils::web::WebTask> m_listener;

    void onClose(cocos2d::CCObject* sender);

    void verifyArgon();
    void verifyAPI(std::string argonToken);

    void finish();
};
