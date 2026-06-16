#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StarTransactions.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StarTransactions> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StarTransactions>();
    result->transactions = parseRequiredArray<StarTransaction>(data, "transactions");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StarTransactions> &object) {
    JsonWrapper json;
    if (object) {
        json.put("transactions", object->transactions);
    }
    return json;
}

} // namespace TgBot
