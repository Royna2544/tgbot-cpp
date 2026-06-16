#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockPullQuotation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockPullQuotation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockPullQuotation>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockPullQuotation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
