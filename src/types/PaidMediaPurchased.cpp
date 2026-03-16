#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaPurchased) {
    auto result = std::make_shared<PaidMediaPurchased>();
    result->from = parseRequired<User>(data, "from");
    parse(data, "paid_media_payload", &result->paidMediaPayload);
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPurchased) {
    JsonWrapper json;
    if (object) {
        json.put("from", object->from);
        json.put("paid_media_payload", object->paidMediaPayload);
    }
    return json;
}

} // namespace TgBot
