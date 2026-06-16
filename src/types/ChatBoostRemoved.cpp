#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatBoostRemoved.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatBoostRemoved> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatBoostRemoved>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "boost_id", &result->boostId);
    parse(data, "remove_date", &result->removeDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatBoostRemoved> &object) {
    JsonWrapper json;
    if (object) {
        json.put("chat", object->chat);
        json.put("boost_id", object->boostId);
        json.put("remove_date", object->removeDate);
        json.put("source", object->source);
    }
    return json;
}

} // namespace TgBot
