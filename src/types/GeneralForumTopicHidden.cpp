#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GeneralForumTopicHidden) {
    auto result = std::make_shared<GeneralForumTopicHidden>();
    return result;
}

DECLARE_PARSER_TO_JSON(GeneralForumTopicHidden) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
