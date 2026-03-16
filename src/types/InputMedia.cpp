#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMedia) {
    std::string type;
    InputMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "photo") {
        result = parse<InputMediaPhoto>(data);
    } else if (type == "video") {
        result = parse<InputMediaVideo>(data);
    } else if (type == "animation") {
        result = parse<InputMediaAnimation>(data);
    } else if (type == "audio") {
        result = parse<InputMediaAudio>(data);
    } else if (type == "document") {
        result = parse<InputMediaDocument>(data);
    } else {
        throw invalidType("InputMedia", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InputMedia) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "photo") {
            json += put<InputMediaPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputMediaVideo>(object);
        } else if (object->type == "animation") {
            json += put<InputMediaAnimation>(object);
        } else if (object->type == "audio") {
            json += put<InputMediaAudio>(object);
        } else if (object->type == "document") {
            json += put<InputMediaDocument>(object);
        } else {
            throw invalidType("InputMedia", object->type);
        }
    }
    return json;
}

} // namespace TgBot
