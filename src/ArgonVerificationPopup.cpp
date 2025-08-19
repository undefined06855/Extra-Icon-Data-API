#include "ArgonVerificationPopup.hpp"
#include <argon/argon.hpp>

ArgonVerificationPopup* ArgonVerificationPopup::create() {
    auto ret = new ArgonVerificationPopup;
    if (ret->initAnchored(300.f, 170.f)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ArgonVerificationPopup::setup() {
    setTitle("Verifying...");
    setID("ArgonVerificationPopup"_spr);
    m_closeBtn->removeFromParent();

    m_content = cocos2d::CCLabelBMFont::create("", "bigFont.fnt");
    m_content->setID("main-label");
    m_content->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_content, geode::Anchor::Center, { 0.f, 8.f });

    m_subtitle = cocos2d::CCLabelBMFont::create("", "chatFont.fnt");
    m_subtitle->setID("subtitle");
    m_subtitle->setZOrder(1);
    m_subtitle->setOpacity(155);
    m_mainLayer->addChildAtPosition(m_subtitle, geode::Anchor::Bottom, { 0.f, 25.f });

    m_button = geode::cocos::CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create("ok"),
        [this](auto) {
            if (m_done) {
                onClose(nullptr);
            }
        }
    );
    m_button->setID("button");
    m_button->setVisible(false);
    m_buttonMenu->addChildAtPosition(m_button, geode::Anchor::Bottom, { 0.f, 25.f });

    m_done = false;

    auto bg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    bg->setID("background");
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(75);
    bg->setContentSize({ 260.f, 84.f });
    m_mainLayer->addChildAtPosition(bg, geode::Anchor::Center, { 0.f, 8.f });

    verifyArgon();

    return true;
}

void ArgonVerificationPopup::onClose(cocos2d::CCObject* sender) {
    if (!m_done) return;
    Popup::onClose(sender);
}

void ArgonVerificationPopup::verifyArgon() {
    // "its argon time" - me, 2025
    m_content->setString("Verifying with Argon...");
    m_content->setScale(.6f);
    m_subtitle->setString("Waiting...");

    auto auth = argon::startAuth([this](geode::Result<std::string> res) {
        if (!res) {
            geode::log::warn("Auth failed: {}", res.unwrapErr());
            return;
        }

        auto token = std::move(res).unwrap();

        verifyAPI(token);
    }, [this](argon::AuthProgress progress) {
        m_subtitle->setString(argon::authProgressToString(progress).c_str());
    });

    if (!auth.isOk()) {
        geode::log::warn("Failed to start auth attempt: {}", auth.unwrapErr());
    }
}

void ArgonVerificationPopup::verifyAPI(std::string argonToken) {
    // "its extra icon data api time" - me, 2025
    m_content->setString("Verifying with API...");

    auto req = geode::utils::web::WebRequest();

    req.bodyJSON(matjson::makeObject({
        { "accountID", GJAccountManager::get()->m_accountID },
        { "token", argonToken } 
    }));

    req.timeout(std::chrono::seconds(20));

    auto task = req.post("https://icon-data-api.undefined0.dev/token/get");

    m_listener.setFilter(task);
    m_listener.bind([this] (geode::utils::web::WebTask::Event* event) {
        auto value = event->getValue();
        if (!value) return;

        auto json = value->json().unwrapOrDefault();

        if (!json["success"].asBool().unwrapOr(false)) {
            m_title->setString("API returned invalid response!");
            m_content->setString(json["error"].dump().c_str());
            finish();
            return;
        }

        setTitle("Done!");
        geode::Mod::get()->setSavedValue("TOKEN_DO_NOT_SHARE", json["token"].asString().unwrapOr(""));
        if (std::rand() / (float)RAND_MAX < 0.05f) {
            // "yay ur verified <cg>gg</c>" - cvolton 2025
            m_content->setString("yay ur verified <cg>gg</c>");
        } else {
            m_content->setString("Verified with Argon!");
        }

        finish();
    });
}

void ArgonVerificationPopup::finish() {
    m_done = true;
    m_subtitle->setString("");
    m_subtitle->setVisible(false);
    m_button->setVisible(true);
}
