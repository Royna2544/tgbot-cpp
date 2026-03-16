#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ForumTopicReopened) {
    auto result = std::make_shared<ForumTopicReopened>();
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicReopened) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
