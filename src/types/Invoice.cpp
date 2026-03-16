#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Invoice) {
    auto result = std::make_shared<Invoice>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "start_parameter", &result->startParameter);
    parse(data, "currency", &result->currency);
    parse(data, "total_amount", &result->totalAmount);
    return result;
}

DECLARE_PARSER_TO_JSON(Invoice) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("description", object->description);
        json.put("start_parameter", object->startParameter);
        json.put("currency", object->currency);
        json.put("total_amount", object->totalAmount);
    }
    return json;
}

} // namespace TgBot
