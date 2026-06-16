#include <tgbot/TgTypeParser.h>
#include <tgbot/types/TransactionPartnerChat.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<TransactionPartnerChat> parse(const nlohmann::json &data) {
    auto result = std::make_shared<TransactionPartnerChat>();
    parse(data, "type", &result->type);
    result->chat = parseRequired<Chat>(data, "chat");
    result->gift = parse<Gift>(data, "gift");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<TransactionPartnerChat> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("chat", object->chat);
        json.put("gift", object->gift);
    }
    return json;
}

} // namespace TgBot
