#include "CommentCell.hpp"
#include "InfoLayer.hpp"

void HookedCommentCell::loadFromComment(GJComment* comment) {
    CommentCell::loadFromComment(comment);
 
    if (!HookedInfoLayer::g_infoLayer) return;
    HookedInfoLayer::g_infoLayer->addCommentCell(this);
}
