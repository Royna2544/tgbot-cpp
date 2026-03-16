#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostApprovalFailed) {
    auto result = std::make_shared<SuggestedPostApprovalFailed>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    result->price = parseRequired<SuggestedPostPrice>(data, "price");
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostApprovalFailed) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
    }
    return json;
}

} // namespace TgBot
