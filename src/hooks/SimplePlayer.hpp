#pragma once
#include <Geode/modify/SimplePlayer.hpp>

class $modify(HookedSimplePlayer, SimplePlayer) {
    struct Fields {
        Fields();

        IconType m_iconType;
    };

    void updatePlayerFrame(int frame, IconType iconType);
};
