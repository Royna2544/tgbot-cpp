#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReplyKeyboardMarkup.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReplyKeyboardMarkup> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReplyKeyboardMarkup>();
    result->keyboard = parseMatrix<KeyboardButton>(data, "keyboard");
    parse(data, "is_persistent", &result->isPersistent);
    parse(data, "resize_keyboard", &result->resizeKeyboard);
    parse(data, "one_time_keyboard", &result->oneTimeKeyboard);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReplyKeyboardMarkup> &object) {
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
