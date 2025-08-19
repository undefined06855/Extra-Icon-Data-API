#include "MenuLayer.hpp"
#include <Geode/ui/Popup.hpp>
#include "../ServerManager.hpp"

bool HookedMenuLayer::init() {
    if (!MenuLayer::init()) return false;
    
    if (GJAccountManager::get()->m_accountID < 0) {
        auto pop = FLAlertLayer::create(
            "Extra Icon Data API",
            "For other players to see your <co>modded icon data</c>, you must "
            "be <cj>signed in</c> to an account!",
            "ok"
        );
        pop->m_scene = this;
        pop->show();
        return true;
    }

    // argon
    auto pop = geode::createQuickPopup(
        "Icon Data API",
        "For mods to <cj>function properly</c> and to let other players see "
        "your modded icons, your account needs to be verified with "
        "<co>Argon</c>. This won't take long and requires access to a message "
        "sent to your account by a bot.",
        "Cancel", "Ok",
        [this](auto, bool ok){
            if (ok) ServerManager::get().runArgonAuth();
        },
        false
    );
    pop->m_scene = this;
    pop->show();

    return true;
}

void HookedMenuLayer::runArgonAuth() {

}
