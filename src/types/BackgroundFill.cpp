#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundFill) {
    std::string type;
    BackgroundFill::Ptr result;
    parse(data, "type", &type);
    if (type == "solid") {
        result = parse<BackgroundFillSolid>(data);
    } else if (type == "gradient") {
        result = parse<BackgroundFillGradient>(data);
    } else if (type == "freeform_gradient") {
        result = parse<BackgroundFillFreeformGradient>(data);
    } else {
        throw invalidType("BackgroundFill", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundFill) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "solid") {
            json += put<BackgroundFillSolid>(object);
        } else if (object->type == "gradient") {
            json += put<BackgroundFillGradient>(object);
        } else if (object->type == "freeform_gradient") {
            json += put<BackgroundFillFreeformGradient>(object);
        } else {
            throw invalidType("BackgroundFill", object->type);
        }
    }
    return json;
}

} // namespace TgBot
