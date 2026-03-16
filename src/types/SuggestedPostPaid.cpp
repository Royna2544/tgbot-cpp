#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostPaid) {
    auto result = std::make_shared<SuggestedPostPaid>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    parse(data, "currency", &result->currency);
    parse(data, "amount", &result->amount);
    result->starAmount = parse<StarAmount>(data, "star_amount");
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostPaid) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("currency", object->currency);
        json.put("amount", object->amount);
        json.put("star_amount", object->starAmount);
    }
    return json;
}

} // namespace TgBot
