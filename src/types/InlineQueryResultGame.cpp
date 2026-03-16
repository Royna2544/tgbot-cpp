#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultGame) {
    auto result = std::make_shared<InlineQueryResultGame>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "game_short_name", &result->gameShortName);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultGame) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("game_short_name", object->gameShortName);
        json.put("reply_markup", object->replyMarkup);
    }
    return json;
}

} // namespace TgBot
