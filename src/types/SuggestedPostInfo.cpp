#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostInfo>();
    parse(data, "state", &result->state);
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostInfo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("state", object->state);
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
