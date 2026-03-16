#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineKeyboardMarkup) {
    auto result = std::make_shared<InlineKeyboardMarkup>();
    result->inlineKeyboard = parseMatrix<InlineKeyboardButton>(data, "inline_keyboard");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineKeyboardMarkup) {
    JsonWrapper json;
    if (object) {
        json.put("inline_keyboard", object->inlineKeyboard);
    }
    return json;
}

} // namespace TgBot
