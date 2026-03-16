#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(KeyboardButtonRequestUsers) {
    auto result = std::make_shared<KeyboardButtonRequestUsers>();
    parse(data, "request_id", &result->requestId);
    parse(data, "user_is_bot", &result->userIsBot);
    parse(data, "user_is_premium", &result->userIsPremium);
    parse(data, "max_quantity", &result->maxQuantity);
    parse(data, "request_name", &result->requestName);
    parse(data, "request_username", &result->requestUsername);
    parse(data, "request_photo", &result->requestPhoto);
    return result;
}

DECLARE_PARSER_TO_JSON(KeyboardButtonRequestUsers) {
    JsonWrapper json;
    if (object) {
        json.put("request_id", object->requestId);
        json.put("user_is_bot", object->userIsBot);
        json.put("user_is_premium", object->userIsPremium);
        json.put("max_quantity", object->maxQuantity);
        json.put("request_name", object->requestName);
        json.put("request_username", object->requestUsername);
        json.put("request_photo", object->requestPhoto);
    }
    return json;
}

} // namespace TgBot
