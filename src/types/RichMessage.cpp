#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichMessage> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichMessage>();
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    parse(data, "is_rtl", &result->isRtl);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichMessage> &object) {
    JsonWrapper json;
    if (object) {
        json.put("blocks", object->blocks);
        json.put("is_rtl", object->isRtl);
    }
    return json;
}

} // namespace TgBot
