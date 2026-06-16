#include <tgbot/TgTypeParser.h>
#include <tgbot/types/StarAmount.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<StarAmount> parse(const nlohmann::json &data) {
    auto result = std::make_shared<StarAmount>();
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<StarAmount> &object) {
    JsonWrapper json;
    if (object) {
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
    }
    return json;
}

} // namespace TgBot
