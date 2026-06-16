#ifndef TGBOT_RICHBLOCKTABLE_H
#define TGBOT_RICHBLOCKTABLE_H

#include "tgbot/types/RichBlock.h"
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
class RichBlockTable : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockTable>;

    constexpr static const char* TYPE = "table";

    RichBlockTable() {
        type = TYPE;
    }

    /**
     * @brief Cells of the table
     */
    std::vector<std::vector<RichBlockTableCell::Ptr>> cells;

    /**
     * @brief Optional. True, if the table has borders
     */
    std::optional<bool> isBordered;

    /**
     * @brief Optional. True, if the table is striped
     */
    std::optional<bool> isStriped;

    /**
     * @brief Optional. Caption of the table
     */
    std::optional<RichText::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKTABLE_H
