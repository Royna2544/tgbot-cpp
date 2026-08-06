#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockPullQuotation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockPullQuotation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockPullQuotation>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockPullQuotation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
