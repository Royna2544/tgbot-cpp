#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoostAdded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoostAdded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoostAdded>();
    parse(data, "boost_count", &result->boostCount);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoostAdded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("boost_count", object->boostCount);
    }
    return json;
}

} // namespace TgBot
