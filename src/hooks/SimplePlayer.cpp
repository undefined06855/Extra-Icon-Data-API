#include "SimplePlayer.hpp"

HookedSimplePlayer::Fields::Fields()
    : m_iconType(IconType::Cube) {}

void HookedSimplePlayer::updatePlayerFrame(int frame, IconType iconType) {
    SimplePlayer::updatePlayerFrame(frame, iconType);
    m_fields->m_iconType = iconType;
}
