#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoChatStarted) {
    auto result = std::make_shared<VideoChatStarted>();
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatStarted) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
