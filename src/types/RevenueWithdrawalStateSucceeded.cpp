#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RevenueWithdrawalStateSucceeded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RevenueWithdrawalStateSucceeded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RevenueWithdrawalStateSucceeded>();
    parse(data, "type", &result->type);
    parse(data, "date", &result->date);
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RevenueWithdrawalStateSucceeded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("date", object->date);
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
