#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReplyKeyboardMarkup) {
    auto result = std::make_shared<ReplyKeyboardMarkup>();
    result->keyboard = parseMatrix<KeyboardButton>(data, "keyboard");
    parse(data, "is_persistent", &result->isPersistent);
    parse(data, "resize_keyboard", &result->resizeKeyboard);
    parse(data, "one_time_keyboard", &result->oneTimeKeyboard);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyKeyboardMarkup) {
    JsonWrapper json;
    if (object) {
        json.put("keyboard", object->keyboard);
        json.put("is_persistent", object->isPersistent);
        json.put("resize_keyboard", object->resizeKeyboard);
        json.put("one_time_keyboard", object->oneTimeKeyboard);
        json.put("input_field_placeholder", object->inputFieldPlaceholder);
        json.put("selective", object->selective);
    }
    return json;
}

} // namespace TgBot
