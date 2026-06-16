#include <tgbot/TgTypeParser.h>
#include <tgbot/types/DirectMessagesTopic.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<DirectMessagesTopic> parse(const nlohmann::json &data) {
    auto result = std::make_shared<DirectMessagesTopic>();
    parse(data, "topic_id", &result->topicId);
    result->user = parse<User>(data, "user");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<DirectMessagesTopic> &object) {
    JsonWrapper json;
    if (object) {
        json.put("topic_id", object->topicId);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
