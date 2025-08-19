#pragma once
#include <Geode/Enums.hpp>

namespace icon_api::utils {

inline std::string_view iconTypeToString(IconType type) {
    switch (type) {
        case IconType::Cube:        return "cube";
        case IconType::Ship:        return "ship";
        case IconType::Ball:        return "ball";
        case IconType::Ufo:         return "ufo";
        case IconType::Wave:        return "wave";
        case IconType::Robot:       return "robot";
        case IconType::Spider:      return "spider";
        case IconType::Swing:       return "swing";
        case IconType::Jetpack:     return "jetpack";
        
        // rest are unecessary
        case IconType::DeathEffect:
        case IconType::Special:
        case IconType::Item:
        case IconType::ShipFire:
            return "cube";
    }
}

}
