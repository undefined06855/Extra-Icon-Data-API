#pragma once
#include <Geode/modify/CommentCell.hpp>

class $modify(HookedCommentCell, CommentCell) {
    void loadFromComment(GJComment* comment);
};
