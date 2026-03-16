#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ShippingOption) {
    auto result = std::make_shared<ShippingOption>();
    parse(data, "id", &result->id);
    parse(data, "title", &result->title);
    result->prices = parseRequiredArray<LabeledPrice>(data, "prices");
    return result;
}

DECLARE_PARSER_TO_JSON(ShippingOption) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("title", object->title);
        json.put("prices", object->prices);
    }
    return json;
}

} // namespace TgBot
