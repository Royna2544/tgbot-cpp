#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MenuButtonDefault) {
    auto result = std::make_shared<MenuButtonDefault>();
    parse(data, "type", &result->type);
    return result;
}

DECLARE_PARSER_TO_JSON(MenuButtonDefault) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
