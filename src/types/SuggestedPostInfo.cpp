#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(SuggestedPostInfo) {
    auto result = std::make_shared<SuggestedPostInfo>();
    parse(data, "state", &result->state);
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

DECLARE_PARSER_TO_JSON(SuggestedPostInfo) {
    JsonWrapper json;
    if (object) {
        json.put("state", object->state);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
