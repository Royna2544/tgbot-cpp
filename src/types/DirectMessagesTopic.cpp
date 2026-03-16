#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(DirectMessagesTopic) {
    auto result = std::make_shared<DirectMessagesTopic>();
    parse(data, "topic_id", &result->topicId);
    result->user = parse<User>(data, "user");
    return result;
}

DECLARE_PARSER_TO_JSON(DirectMessagesTopic) {
    JsonWrapper json;
    if (object) {
        json.put("topic_id", object->topicId);
        json.put("user", object->user);
    }
    return json;
}

} // namespace TgBot
