#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(WebAppData) {
    auto result = std::make_shared<WebAppData>();
    parse(data, "data", &result->data);
    parse(data, "button_text", &result->buttonText);
    return result;
}

DECLARE_PARSER_TO_JSON(WebAppData) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("button_text", object->buttonText);
    }
    return json;
}

} // namespace TgBot
