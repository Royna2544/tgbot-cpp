#include <tgbot/TgTypeParser.h>
#include <tgbot/types/VideoChatEnded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<VideoChatEnded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<VideoChatEnded>();
    parse(data, "duration", &result->duration);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<VideoChatEnded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("duration", object->duration);
    }
    return json;
}

} // namespace TgBot
