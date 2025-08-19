#include "GJGarageLayer.hpp"
#include "../RequestManager.hpp"

void HookedGJGarageLayer::onBack(cocos2d::CCObject* sender) {
    GJGarageLayer::onBack(sender);
    RequestManager::get().fetchAndUploadData();
}
