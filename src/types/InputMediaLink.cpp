#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMediaLink.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMediaLink) {
    auto result = std::make_shared<InputMediaLink>();
    parse(data, "type", &result->type);
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(InputMediaLink) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
