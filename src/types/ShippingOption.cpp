#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ShippingOption.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ShippingOption> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ShippingOption>();
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    result->prices = parseRequiredArray<LabeledPrice>(data, "prices");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ShippingOption> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("title", object->title);
        json.put("prices", object->prices);
    }
    return json;
}

} // namespace TgBot
