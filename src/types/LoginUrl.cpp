#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(LoginUrl) {
    auto result = std::make_shared<LoginUrl>();
    parse(data, "url", &result->url);
    parse(data, "forward_text", &result->forwardText);
    parse(data, "bot_username", &result->botUsername);
    parse(data, "request_write_access", &result->requestWriteAccess);
    return result;
}

DECLARE_PARSER_TO_JSON(LoginUrl) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
        json.put("forward_text", object->forwardText);
        json.put("bot_username", object->botUsername);
        json.put("request_write_access", object->requestWriteAccess);
    }
    return json;
}

} // namespace TgBot
