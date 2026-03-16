#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GeneralForumTopicUnhidden) {
    auto result = std::make_shared<GeneralForumTopicUnhidden>();
    return result;
}

DECLARE_PARSER_TO_JSON(GeneralForumTopicUnhidden) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
