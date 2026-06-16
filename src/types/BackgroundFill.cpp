#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundFill.h>
#include <tgbot/types/BackgroundFillFreeformGradient.h>
#include <tgbot/types/BackgroundFillGradient.h>
#include <tgbot/types/BackgroundFillSolid.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundFill> parse(const nlohmann::json &data) {
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

template <>
nlohmann::json put(const std::shared_ptr<BackgroundFill> &object) {
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
