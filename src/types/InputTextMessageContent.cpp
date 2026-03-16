#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputTextMessageContent) {
    auto result = std::make_shared<InputTextMessageContent>();
    parse(data, "message_text", &result->messageText);
    parse(data, "parse_mode", &result->parseMode);
    result->entities = parseArray<MessageEntity>(data, "entities");
    result->linkPreviewOptions = parse<LinkPreviewOptions>(data, "link_preview_options");
    return result;
}

DECLARE_PARSER_TO_JSON(InputTextMessageContent) {
    JsonWrapper json;
    if (object) {
        json.put("message_text", object->messageText);
        json.put("parse_mode", object->parseMode);
        json.put("entities", object->entities);
        json.put("link_preview_options", object->linkPreviewOptions);
    }
    return json;
}

} // namespace TgBot
