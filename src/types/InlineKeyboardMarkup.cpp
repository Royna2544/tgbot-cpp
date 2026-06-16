#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InlineKeyboardMarkup.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InlineKeyboardMarkup> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InlineKeyboardMarkup>();
    result->inlineKeyboard = parseMatrix<InlineKeyboardButton>(data, "inline_keyboard");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InlineKeyboardMarkup> &object) {
    JsonWrapper json;
    if (object) {
        json.put("inline_keyboard", object->inlineKeyboard);
    }
    return json;
}

} // namespace TgBot
