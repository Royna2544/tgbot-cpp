#ifndef TGBOT_RICHBLOCKLIST_H
#define TGBOT_RICHBLOCKLIST_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockListItem.h"

#include <memory>
#include <vector>

namespace TgBot {

/**
 * @brief A list of blocks, corresponding to the HTML tag <ul> or <ol> with multiple nested tags <li>.
 *
 * @ingroup types
 */
class RichBlockList : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockList>;

    constexpr static const char* TYPE = "list";

    RichBlockList() {
        type = TYPE;
    }

    /**
     * @brief Items of the list
     */
    std::vector<RichBlockListItem::Ptr> items;

};
}

#endif //TGBOT_RICHBLOCKLIST_H
