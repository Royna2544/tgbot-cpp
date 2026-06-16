#ifndef TGBOT_RICHBLOCKLISTITEM_H
#define TGBOT_RICHBLOCKLISTITEM_H

#include "tgbot/types/RichBlock.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TgBot {

/**
 * @brief An item of a list.
 *
 * @ingroup types
 */
class RichBlockListItem {
public:
    using Ptr = std::shared_ptr<RichBlockListItem>;

    /**
     * @brief Label of the item
     */
    std::string label;

    /**
     * @brief The content of the item
     */
    std::vector<RichBlock::Ptr> blocks;

    /**
     * @brief Optional. True, if the item has a checkbox
     */
    std::optional<bool> hasCheckbox;

    /**
     * @brief Optional. True, if the item has a checked checkbox
     */
    std::optional<bool> isChecked;

    /**
     * @brief Optional. For ordered lists, the numeric value of the item label
     */
    std::optional<std::int32_t> value;

    /**
     * @brief Optional. For ordered lists, the type of the item label; must be one of "a" for lowercase letters, "A" for uppercase letters, "i" for lowercase Roman numerals, "I" for uppercase Roman numerals, or "1" for decimal numbers
     */
    std::optional<std::string> type;

};
}

#endif //TGBOT_RICHBLOCKLISTITEM_H
