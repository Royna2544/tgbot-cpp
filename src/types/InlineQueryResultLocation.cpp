#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultLocation) {
    auto result = std::make_shared<InlineQueryResultLocation>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "latitude", &result->latitude);
    parse(data, "longitude", &result->longitude);
    parse(data, "title", &result->title);
    parse(data, "horizontal_accuracy", &result->horizontalAccuracy);
    parse(data, "live_period", &result->livePeriod);
    parse(data, "heading", &result->heading);
    parse(data, "proximity_alert_radius", &result->proximityAlertRadius);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    parse(data, "thumbnail_url", &result->thumbnailUrl);
    parse(data, "thumbnail_width", &result->thumbnailWidth);
    parse(data, "thumbnail_height", &result->thumbnailHeight);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultLocation) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("latitude", object->latitude);
        json.put("longitude", object->longitude);
        json.put("title", object->title);
        json.put("horizontal_accuracy", object->horizontalAccuracy);
        json.put("live_period", object->livePeriod);
        json.put("heading", object->heading);
        json.put("proximity_alert_radius", object->proximityAlertRadius);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
        json.put("thumbnail_url", object->thumbnailUrl);
        json.put("thumbnail_width", object->thumbnailWidth);
        json.put("thumbnail_height", object->thumbnailHeight);
    }
    return json;
}

} // namespace TgBot
