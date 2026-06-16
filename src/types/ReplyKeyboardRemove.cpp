#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ReplyKeyboardRemove.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ReplyKeyboardRemove> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ReplyKeyboardRemove>();
    parse(data, "remove_keyboard", &result->removeKeyboard);
    parse(data, "selective", &result->selective);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ReplyKeyboardRemove> &object) {
    JsonWrapper json;
    if (object) {
        json.put("remove_keyboard", object->removeKeyboard);
        json.put("selective", object->selective);
    }
    return json;
}

} // namespace TgBot
