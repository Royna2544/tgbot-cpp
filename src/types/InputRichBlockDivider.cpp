#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockDivider.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockDivider> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockDivider>();
    parse(data, "type", &result->type);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockDivider> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
    }
    return json;
}

} // namespace TgBot
