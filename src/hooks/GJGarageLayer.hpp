#pragma once
#include <Geode/modify/GJGarageLayer.hpp>

class $modify(HookedGJGarageLayer, GJGarageLayer) {
    void onBack(cocos2d::CCObject* sender);
};
