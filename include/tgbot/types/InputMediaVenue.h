#ifndef TGBOT_INPUTMEDIAVENUE_H
#define TGBOT_INPUTMEDIAVENUE_H

#include "tgbot/types/InputMedia.h"

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief Represents a venue to be sent.
 *
 * @ingroup types
 */
class InputMediaVenue : public InputMedia {
public:
    using Ptr = std::shared_ptr<InputMediaVenue>;

    constexpr static const char* TYPE = "venue";

    InputMediaVenue() {
        type = TYPE;
    }

    /**
     * @brief Latitude of the location
     */
    float latitude;

    /**
     * @brief Longitude of the location
     */
    float longitude;

    /**
     * @brief Name of the venue
     */
    std::string title;

    /**
     * @brief Address of the venue
     */
    std::string address;

    /**
     * @brief Optional. Foursquare identifier of the venue
     */
    std::optional<std::string> foursquareId;

    /**
     * @brief Optional. Foursquare type of the venue, if known. (For example, "arts_entertainment/default", "arts_entertainment/aquarium" or "food/icecream".)
     */
    std::optional<std::string> foursquareType;

    /**
     * @brief Optional. Google Places identifier of the venue
     */
    std::optional<std::string> googlePlaceId;

    /**
     * @brief Optional. Google Places type of the venue. (See supported types.)
     */
    std::optional<std::string> googlePlaceType;

};
}

#endif //TGBOT_INPUTMEDIAVENUE_H
