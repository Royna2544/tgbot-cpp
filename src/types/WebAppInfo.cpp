#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(WebAppInfo) {
    auto result = std::make_shared<WebAppInfo>();
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(WebAppInfo) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
