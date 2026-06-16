#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoost.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoost> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoost>();
    parse(data, "boost_id", &result->boostId);
    parse(data, "add_date", &result->addDate);
    parse(data, "expiration_date", &result->expirationDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoost> &object) {
    JsonWrapper json;
    if (object) {
        json.put("boost_id", object->boostId);
        json.put("add_date", object->addDate);
        json.put("expiration_date", object->expirationDate);
        json.put("source", object->source);
    }
    return json;
}

} // namespace TgBot
