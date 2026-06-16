#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockDetails.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockDetails> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockDetails>();
    parse(data, "type", &result->type);
    result->summary = parseRequired<RichText>(data, "summary");
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    parse(data, "is_open", &result->isOpen);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockDetails> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("summary", object->summary);
        json.put("blocks", object->blocks);
        json.put("is_open", object->isOpen);
    }
    return json;
}

} // namespace TgBot
