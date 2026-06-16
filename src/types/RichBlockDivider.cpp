#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockDivider.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockDivider> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockDivider>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockDivider> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
