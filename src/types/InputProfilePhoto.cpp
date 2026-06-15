#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputProfilePhoto.h>
#include <tgbot/types/InputProfilePhotoAnimated.h>
#include <tgbot/types/InputProfilePhotoStatic.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputProfilePhoto) {
    std::string type;
    InputProfilePhoto::Ptr result;
    parse(data, "type", &type);
    if (type == "static") {
        result = parse<InputProfilePhotoStatic>(data);
    } else if (type == "animated") {
        result = parse<InputProfilePhotoAnimated>(data);
    } else {
        throw invalidType("InputProfilePhoto", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "static") {
            json += put<InputProfilePhotoStatic>(object);
        } else if (object->type == "animated") {
            json += put<InputProfilePhotoAnimated>(object);
        } else {
            throw invalidType("InputProfilePhoto", object->type);
        }
    }
    return json;
}

} // namespace TgBot
