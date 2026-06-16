#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichMessageContent.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputRichMessageContent) {
    auto result = std::make_shared<InputRichMessageContent>();
    result->richMessage = parseRequired<InputRichMessage>(data, "rich_message");
    return result;
}

DECLARE_PARSER_TO_JSON(InputRichMessageContent) {
    JsonWrapper json;
    if (object) {
        json.put("rich_message", object->richMessage);
    }
    return json;
}

} // namespace TgBot
