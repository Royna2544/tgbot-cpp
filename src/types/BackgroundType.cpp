#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundType.h>
#include <tgbot/types/BackgroundTypeChatTheme.h>
#include <tgbot/types/BackgroundTypeFill.h>
#include <tgbot/types/BackgroundTypePattern.h>
#include <tgbot/types/BackgroundTypeWallpaper.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundType> parse(const nlohmann::json &data) {
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

template <>
nlohmann::json put(const std::shared_ptr<BackgroundType> &object) {
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
