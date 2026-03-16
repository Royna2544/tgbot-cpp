#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundType) {
    std::string type;
    BackgroundType::Ptr result;
    parse(data, "type", &type);
    if (type == "fill") {
        result = parse<BackgroundTypeFill>(data);
    } else if (type == "wallpaper") {
        result = parse<BackgroundTypeWallpaper>(data);
    } else if (type == "pattern") {
        result = parse<BackgroundTypePattern>(data);
    } else if (type == "chat_theme") {
        result = parse<BackgroundTypeChatTheme>(data);
    } else {
        throw invalidType("BackgroundType", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundType) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "fill") {
            json += put<BackgroundTypeFill>(object);
        } else if (object->type == "wallpaper") {
            json += put<BackgroundTypeWallpaper>(object);
        } else if (object->type == "pattern") {
            json += put<BackgroundTypePattern>(object);
        } else if (object->type == "chat_theme") {
            json += put<BackgroundTypeChatTheme>(object);
        } else {
            throw invalidType("BackgroundType", object->type);
        }
    }
    return json;
}

} // namespace TgBot
