#ifndef TGBOT_INPUTRICHBLOCKMAP_H
#define TGBOT_INPUTRICHBLOCKMAP_H

#include "tgbot/types/InputRichBlock.h"
#include "tgbot/types/Location.h"
#include "tgbot/types/RichBlockCaption.h"

#include <cstdint>
#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a map, corresponding to the custom HTML tag <tg-map>. The map's width and height must not exceed 10000 in total. The width and height ratio must be at most 20.
 *
 * @ingroup types
 */
class InputRichBlockMap : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockMap>;

    constexpr static const char* TYPE = "map";

    InputRichBlockMap() {
        type = TYPE;
    }

    /**
     * @brief Location of the center of the map
     */
    Location::Ptr location;

    /**
     * @brief Map zoom level; 0-24
     */
    std::int32_t zoom;

    /**
     * @brief Map width; 0-10000
     */
    std::int32_t width;

    /**
     * @brief Map height; 0-10000
     */
    std::int32_t height;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_INPUTRICHBLOCKMAP_H
