#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultVenue) {
    auto result = std::make_shared<InlineQueryResultVenue>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "address", &result->address);
    parse(data, "foursquare_id", &result->foursquareId);
    parse(data, "foursquare_type", &result->foursquareType);
    parse(data, "google_place_id", &result->googlePlaceId);
    parse(data, "google_place_type", &result->googlePlaceType);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultVenue) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("title", object->title);
        json.put("address", object->address);
        json.put("foursquare_id", object->foursquareId);
        json.put("foursquare_type", object->foursquareType);
        json.put("google_place_id", object->googlePlaceId);
        json.put("google_place_type", object->googlePlaceType);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_width", object->thumbnailWidth);
        json.put("thumbnail_height", object->thumbnailHeight);
    }
    return json;
}

} // namespace TgBot
