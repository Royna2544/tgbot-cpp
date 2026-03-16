#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MenuButton) {
    std::string type;
    MenuButton::Ptr result;
    parse(data, "type", &type);
    if (type == "commands") {
        result = parse<MenuButtonCommands>(data);
    } else if (type == "web_app") {
        result = parse<MenuButtonWebApp>(data);
    } else if (type == "default") {
        result = parse<MenuButtonDefault>(data);
    } else {
        throw invalidType("MenuButton", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(MenuButton) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "commands") {
            json += put<MenuButtonCommands>(object);
        } else if (object->type == "web_app") {
            json += put<MenuButtonWebApp>(object);
        } else if (object->type == "default") {
            json += put<MenuButtonDefault>(object);
        } else {
            throw invalidType("MenuButton", object->type);
        }
    }
    return json;
}

} // namespace TgBot
