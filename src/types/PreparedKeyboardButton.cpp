#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PreparedKeyboardButton.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PreparedKeyboardButton> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PreparedKeyboardButton>();
    parse(data, "id", &result->id);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PreparedKeyboardButton> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
    }
    return json;
}

} // namespace TgBot
