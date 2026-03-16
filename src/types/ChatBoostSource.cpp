#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoostSource) {
    std::string type;
    ChatBoostSource::Ptr result;
    parse(data, "source", &type);
    if (type == "premium") {
        result = parse<ChatBoostSourcePremium>(data);
    } else if (type == "gift_code") {
        result = parse<ChatBoostSourceGiftCode>(data);
    } else if (type == "giveaway") {
        result = parse<ChatBoostSourceGiveaway>(data);
    } else {
        throw invalidType("ChatBoostSource", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostSource) {
    JsonWrapper json;
    if (object) {
        json.put("source", object->source);
        if (object->source == "premium") {
            json += put<ChatBoostSourcePremium>(object);
        } else if (object->source == "gift_code") {
            json += put<ChatBoostSourceGiftCode>(object);
        } else if (object->source == "giveaway") {
            json += put<ChatBoostSourceGiveaway>(object);
        } else {
            throw invalidType("ChatBoostSource", object->source);
        }
    }
    return json;
}

} // namespace TgBot
