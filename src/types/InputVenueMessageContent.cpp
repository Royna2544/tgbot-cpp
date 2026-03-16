#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputVenueMessageContent) {
    auto result = std::make_shared<InputVenueMessageContent>();
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "address", &result->address);
    parse(data, "foursquare_id", &result->foursquareId);
    parse(data, "foursquare_type", &result->foursquareType);
    parse(data, "google_place_id", &result->googlePlaceId);
    parse(data, "google_place_type", &result->googlePlaceType);
    return result;
}

DECLARE_PARSER_TO_JSON(InputVenueMessageContent) {
    JsonWrapper json;
    if (object) {
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("title", object->title);
        json.put("address", object->address);
        json.put("foursquare_id", object->foursquareId);
        json.put("foursquare_type", object->foursquareType);
        json.put("google_place_id", object->googlePlaceId);
        json.put("google_place_type", object->googlePlaceType);
    }
    return json;
}

} // namespace TgBot
