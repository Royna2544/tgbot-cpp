#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ForumTopicReopened.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ForumTopicReopened> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ForumTopicReopened>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ForumTopicReopened> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
