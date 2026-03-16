#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputPaidMedia) {
    std::string type;
    InputPaidMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "photo") {
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
        if (object->type == "photo") {
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
