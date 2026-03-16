#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostDeclined) {
    auto result = std::make_shared<SuggestedPostDeclined>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    parse(data, "comment", &result->comment);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostDeclined) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("comment", object->comment);
    }
    return json;
}

} // namespace TgBot
