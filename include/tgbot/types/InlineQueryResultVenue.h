#ifndef TGBOT_INLINEQUERYRESULTVENUE_H
#define TGBOT_INLINEQUERYRESULTVENUE_H

#include "tgbot/types/InlineQueryResult.h"
#include "tgbot/types/InputMessageContent.h"

#include <cstdint>
#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents a venue.
 *
 * By default, the venue will be sent by the user.
 * Alternatively, you can use inputMessageContent to send a message with the specified content instead of the venue.
 *
 * @ingroup types
 */
class InlineQueryResultVenue : public InlineQueryResult {

public:
    static constexpr std::string_view TYPE = "venue";

    using Ptr = std::shared_ptr<InlineQueryResultVenue>;

    InlineQueryResultVenue() {
        this->type = TYPE;
    }

    /**
     * @brief Latitude of the venue location in degrees
     */
    float latitude{};

    /**
     * @brief Longitude of the venue location in degrees
     */
    float longitude{};

    /**
     * @brief Title of the venue
     */
    std::string title;

    /**
     * @brief Address of the venue
     */
    std::string address;

    /**
     * @brief Optional. Foursquare identifier of the venue if known
     */
    std::optional<std::string> foursquareId;

    /**
     * @brief Optional. Foursquare type of the venue, if known.
     *
     * (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
     */
    std::optional<std::string> foursquareType;

    /**
     * @brief Optional. Google Places identifier of the venue
     */
    std::optional<std::string> googlePlaceId;

    /**
     * @brief Optional. Google Places type of the venue.
     *
     * (See [supported types](https://developers.google.com/places/web-service/supported_types).)
     */
    std::optional<std::string> googlePlaceType;

    /**
     * @brief Optional. Content of the message to be sent instead of the venue
     */
    InputMessageContent::Ptr inputMessageContent;

    /**
     * @brief Optional. Url of the thumbnail for the result
     */
    std::optional<std::string> thumbnailUrl;

    /**
     * @brief Optional. Thumbnail width
     */
    std::optional<std::int32_t> thumbnailWidth;

    /**
     * @brief Optional. Thumbnail height
     */
    std::optional<std::int32_t> thumbnailHeight;
};
}

#endif //TGBOT_INLINEQUERYRESULTVENUE_H
