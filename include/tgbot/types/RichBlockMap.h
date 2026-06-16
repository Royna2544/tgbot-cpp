#ifndef TGBOT_RICHBLOCKMAP_H
#define TGBOT_RICHBLOCKMAP_H

#include "tgbot/types/Location.h"
#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichBlockCaption.h"

#include <cstdint>
#include <memory>
#include <optional>

namespace TgBot {

/**
 * @brief A block with a map, corresponding to the custom HTML tag <tg-map>.
 *
 * @ingroup types
 */
class RichBlockMap : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockMap>;

    constexpr static const char* TYPE = "map";

    RichBlockMap() {
        type = TYPE;
    }

    /**
     * @brief Location of the center of the map
     */
    Location::Ptr location;

    /**
     * @brief Map zoom level; 13-20
     */
    std::int32_t zoom;

    /**
     * @brief Expected width of the map
     */
    std::int32_t width;

    /**
     * @brief Expected height of the map
     */
    std::int32_t height;

    /**
     * @brief Optional. Caption of the block
     */
    std::optional<RichBlockCaption::Ptr> caption;

};
}

#endif //TGBOT_RICHBLOCKMAP_H
