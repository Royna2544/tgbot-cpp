#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(OwnedGift) {
    std::string type;
    OwnedGift::Ptr result;
    parse(data, "type", &type);
    if (type == "regular") {
        result = parse<OwnedGiftRegular>(data);
    } else if (type == "unique") {
        result = parse<OwnedGiftUnique>(data);
    } else {
        throw invalidType("OwnedGift", type);
    }
    return result;
}

DECLARE_PARSER_TO_JSON(OwnedGift) {
    JsonWrapper json;
    if (object) {
        if (object->type == "regular") {
            json += put<OwnedGiftRegular>(object);
        } else if (object->type == "unique") {
            json += put<OwnedGiftUnique>(object);
        } else {
            throw invalidType("OwnedGift", object->type);
        }
    }
    return json;
}

} // namespace TgBot
