#ifndef TGBOT_RICHBLOCKTABLECELL_H
#define TGBOT_RICHBLOCKTABLECELL_H

#include "tgbot/types/RichText.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief Cell in a table.
 *
 * @ingroup types
 */
class RichBlockTableCell {
public:
    using Ptr = std::shared_ptr<RichBlockTableCell>;

    /**
     * @brief Optional. Text in the cell. If omitted, then the cell is invisible.
     */
    std::optional<RichText::Ptr> text;

    /**
     * @brief Optional. True, if the cell is a header cell
     */
    std::optional<bool> isHeader;

    /**
     * @brief Optional. The number of columns the cell spans if it is bigger than 1
     */
    std::optional<std::int32_t> colspan;

    /**
     * @brief Optional. The number of rows the cell spans if it is bigger than 1
     */
    std::optional<std::int32_t> rowspan;

    /**
     * @brief Horizontal cell content alignment. Currently, must be one of "left", "center", or "right".
     */
    std::string align;

    /**
     * @brief Vertical cell content alignment. Currently, must be one of "top", "middle", or "bottom".
     */
    std::string valign;

};
}

#endif //TGBOT_RICHBLOCKTABLECELL_H
