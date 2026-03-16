#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostApproved) {
    auto result = std::make_shared<SuggestedPostApproved>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostApproved) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
