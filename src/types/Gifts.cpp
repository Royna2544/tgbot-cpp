#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Gifts.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Gifts> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Gifts>();
    result->gifts = parseRequiredArray<Gift>(data, "gifts");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Gifts> &object) {
    JsonWrapper json;
    if (object) {
        json.put("gifts", object->gifts);
    }
    return json;
}

} // namespace TgBot
