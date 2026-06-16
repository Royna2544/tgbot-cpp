#include <tgbot/TgTypeParser.h>
#include <tgbot/types/LoginUrl.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<LoginUrl> parse(const nlohmann::json &data) {
    auto result = std::make_shared<LoginUrl>();
    parse(data, "url", &result->url);
    parse(data, "forward_text", &result->forwardText);
    parse(data, "bot_username", &result->botUsername);
    parse(data, "request_write_access", &result->requestWriteAccess);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<LoginUrl> &object) {
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
