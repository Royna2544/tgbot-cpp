#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostParameters.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostParameters> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostParameters>();
    result->price = parse<SuggestedPostPrice>(data, "price");
    parse(data, "send_date", &result->sendDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostParameters> &object) {
    JsonWrapper json;
    if (object) {
        json.put("price", object->price);
        json.put("send_date", object->sendDate);
    }
    return json;
}

} // namespace TgBot
