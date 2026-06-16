#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockBlockQuotation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockBlockQuotation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockBlockQuotation>();
    parse(data, "type", &result->type);
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockBlockQuotation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("blocks", object->blocks);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
