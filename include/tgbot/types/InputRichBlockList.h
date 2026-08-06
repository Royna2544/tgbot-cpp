#ifndef TGBOT_INPUTRICHBLOCKLIST_H
#define TGBOT_INPUTRICHBLOCKLIST_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/InputRichBlockListItem.h"

#include <memory>
#include <vector>

namespace TgBot {

/**
 * @brief A list of blocks, corresponding to the HTML tag <ul> or <ol> with multiple nested tags <li>.
 *
 * @ingroup types
 */
class InputRichBlockList : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockList>;

    constexpr static const char* TYPE = "list";

    InputRichBlockList() {
        type = TYPE;
    }

    /**
     * @brief Items of the list
     */
    std::vector<InputRichBlockListItem::Ptr> items;

};
}

#endif //TGBOT_INPUTRICHBLOCKLIST_H
