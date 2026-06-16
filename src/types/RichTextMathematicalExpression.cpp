#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextMathematicalExpression.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextMathematicalExpression> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextMathematicalExpression>();
    parse(data, "type", &result->type);
    parse(data, "expression", &result->expression);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextMathematicalExpression> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("expression", object->expression);
    }
    return json;
}

} // namespace TgBot
