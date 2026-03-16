#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputFile) {
    auto result = std::make_shared<InputFile>();
    return result;
}

DECLARE_PARSER_TO_JSON(InputFile) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
