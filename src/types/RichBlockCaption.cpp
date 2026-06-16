#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockCaption.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockCaption) {
    auto result = std::make_shared<RichBlockCaption>();
    result->text = parseRequired<RichText>(data, "text");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockCaption) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
