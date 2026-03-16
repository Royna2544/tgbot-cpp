#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostParameters) {
    auto result = std::make_shared<SuggestedPostParameters>();
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostParameters) {
    JsonWrapper json;
    if (object) {
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
