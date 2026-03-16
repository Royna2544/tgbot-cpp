#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GiftBackground) {
    auto result = std::make_shared<GiftBackground>();
    parse(data, "center_color", &result->centerColor);
    parse(data, "edge_color", &result->edgeColor);
    parse(data, "text_color", &result->textColor);
    return result;
}

DECLARE_PARSER_TO_JSON(GiftBackground) {
    JsonWrapper json;
    if (object) {
        json.put("center_color", object->centerColor);
        json.put("edge_color", object->edgeColor);
        json.put("text_color", object->textColor);
    }
    return json;
}

} // namespace TgBot
