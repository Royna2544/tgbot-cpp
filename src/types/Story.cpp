#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Story.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Story> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Story>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "id", &result->id);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Story> &object) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("id", object->id);
    }
    return json;
}

} // namespace TgBot
