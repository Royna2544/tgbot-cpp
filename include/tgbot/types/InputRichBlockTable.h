#ifndef TGBOT_INPUTRICHBLOCKTABLE_H
#define TGBOT_INPUTRICHBLOCKTABLE_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/RichBlockTableCell.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief A table, corresponding to the HTML tag <table>.
 *
 * @ingroup types
 */
class InputRichBlockTable : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockTable>;

    constexpr static const char* TYPE = "table";

    InputRichBlockTable() {
        type = TYPE;
    }

    /**
     * @brief Cells of the table
     */
    std::vector<std::vector<RichBlockTableCell::Ptr>> cells;

    /**
     * @brief Optional. Pass True if the table has borders
     */
    std::optional<bool> isBordered;

    /**
     * @brief Optional. Pass True if the table is striped
     */
    std::optional<bool> isStriped;

    /**
     * @brief Optional. Caption of the table
     */
    std::optional<RichText::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKTABLE_H
