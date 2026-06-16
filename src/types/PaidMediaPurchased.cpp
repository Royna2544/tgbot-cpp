#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaPurchased.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaPurchased> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaPurchased>();
    result->from = parseRequired<User>(data, "from");
    parse(data, "paid_media_payload", &result->paidMediaPayload);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaPurchased> &object) {
    JsonWrapper json;
    if (object) {
        json.put("from", object->from);
        json.put("paid_media_payload", object->paidMediaPayload);
    }
    return json;
}

} // namespace TgBot
