#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(CallbackGame) {
    auto result = std::make_shared<CallbackGame>();
    return result;
}

DECLARE_PARSER_TO_JSON(CallbackGame) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
