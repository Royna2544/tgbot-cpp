#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostApproved.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostApproved> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostApproved>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostApproved> &object) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
