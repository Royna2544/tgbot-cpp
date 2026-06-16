#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBackground.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBackground> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBackground>();
    result->type = parseRequired<BackgroundType>(data, "type");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBackground> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
