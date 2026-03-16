#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMedia) {
    std::string type;
    PaidMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "preview") {
        result = parse<PaidMediaPreview>(data);
    } else if (type == "photo") {
        result = parse<PaidMediaPhoto>(data);
    } else if (type == "video") {
        result = parse<PaidMediaVideo>(data);
    } else {
        throw invalidType("PaidMedia", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(PaidMedia) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "preview") {
            json += put<PaidMediaPreview>(object);
        } else if (object->type == "photo") {
            json += put<PaidMediaPhoto>(object);
        } else if (object->type == "video") {
            json += put<PaidMediaVideo>(object);
        } else {
            throw invalidType("PaidMedia", object->type);
        }
    }
    return json;
}

} // namespace TgBot
