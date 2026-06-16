#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaInfo>();
    parse(data, "star_count", &result->starCount);
    result->paidMedia = parseRequiredArray<PaidMedia>(data, "paid_media");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaInfo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("star_count", object->starCount);
        json.put("paid_media", object->paidMedia);
    }
    return json;
}

} // namespace TgBot
