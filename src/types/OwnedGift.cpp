#include <tgbot/TgTypeParser.h>
#include <tgbot/types/OwnedGift.h>
#include <tgbot/types/OwnedGiftRegular.h>
#include <tgbot/types/OwnedGiftUnique.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<OwnedGift> parse(const nlohmann::json &data) {
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

template <>
nlohmann::json put(const std::shared_ptr<OwnedGift> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
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
