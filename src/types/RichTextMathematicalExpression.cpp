#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextMathematicalExpression.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextMathematicalExpression) {
    auto result = std::make_shared<RichTextMathematicalExpression>();
    parse(data, "type", &result->type);
    parse(data, "expression", &result->expression);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextMathematicalExpression) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("expression", object->expression);
    }
    return json;
}

} // namespace TgBot
