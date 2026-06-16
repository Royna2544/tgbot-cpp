#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostPrice.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostPrice> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostPrice>();
    parse(data, "currency", &result->currency);
    parse(data, "amount", &result->amount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostPrice> &object) {
    JsonWrapper json;
    if (object) {
        json.put("currency", object->currency);
        json.put("amount", object->amount);
    }
    return json;
}

} // namespace TgBot
