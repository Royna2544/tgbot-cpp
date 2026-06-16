#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPaidMedia.h>
#include <tgbot/types/InputPaidMediaLivePhoto.h>
#include <tgbot/types/InputPaidMediaPhoto.h>
#include <tgbot/types/InputPaidMediaVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputPaidMedia) {
    std::string type;
    InputPaidMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "live_photo") {
        result = parse<InputPaidMediaLivePhoto>(data);
    } else if (type == "photo") {
        result = parse<InputPaidMediaPhoto>(data);
    } else if (type == "video") {
        result = parse<InputPaidMediaVideo>(data);
    } else {
        throw invalidType("InputPaidMedia", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InputPaidMedia) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "live_photo") {
            json += put<InputPaidMediaLivePhoto>(object);
        } else if (object->type == "photo") {
            json += put<InputPaidMediaPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputPaidMediaVideo>(object);
        } else {
            throw invalidType("InputPaidMedia", object->type);
        }
    }
    return json;
}

} // namespace TgBot
