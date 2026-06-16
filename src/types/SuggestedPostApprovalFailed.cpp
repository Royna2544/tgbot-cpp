#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostApprovalFailed.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostApprovalFailed> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostApprovalFailed>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    result->price = parseRequired<SuggestedPostPrice>(data, "price");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostApprovalFailed> &object) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
    }
    return json;
}

} // namespace TgBot
