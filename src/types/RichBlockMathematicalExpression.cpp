#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockMathematicalExpression.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockMathematicalExpression> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockMathematicalExpression>();
    parse(data, "type", &result->type);
    parse(data, "expression", &result->expression);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockMathematicalExpression> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("expression", object->expression);
    }
    return json;
}

} // namespace TgBot
