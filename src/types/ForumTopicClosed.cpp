#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ForumTopicClosed) {
    auto result = std::make_shared<ForumTopicClosed>();
    return result;
}

DECLARE_PARSER_TO_JSON(ForumTopicClosed) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
