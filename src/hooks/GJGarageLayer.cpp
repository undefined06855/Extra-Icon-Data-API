#include "GJGarageLayer.hpp"
#include "../globals.hpp"
#include "events.hpp"

bool HookedGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;


    return true;
}
