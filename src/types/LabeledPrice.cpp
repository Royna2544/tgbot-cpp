#include <tgbot/TgTypeParser.h>
#include <tgbot/types/LabeledPrice.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<LabeledPrice> parse(const nlohmann::json &data) {
    auto result = std::make_shared<LabeledPrice>();
    parse(data, "label", &result->label);
    parse(data, "amount", &result->amount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<LabeledPrice> &object) {
    JsonWrapper json;
    if (object) {
        json.put("label", object->label);
        json.put("amount", object->amount);
    }
    return json;
}

} // namespace TgBot
