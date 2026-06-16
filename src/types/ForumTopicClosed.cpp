#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ForumTopicClosed.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ForumTopicClosed> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ForumTopicClosed>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ForumTopicClosed> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
