#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGiftBackdropColors) {
    auto result = std::make_shared<UniqueGiftBackdropColors>();
    parse(data, "center_color", &result->centerColor);
    parse(data, "edge_color", &result->edgeColor);
    parse(data, "symbol_color", &result->symbolColor);
    parse(data, "text_color", &result->textColor);
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftBackdropColors) {
    JsonWrapper json;
    if (object) {
        json.put("center_color", object->centerColor);
        json.put("edge_color", object->edgeColor);
        json.put("symbol_color", object->symbolColor);
        json.put("text_color", object->textColor);
    }
    return json;
}

} // namespace TgBot
