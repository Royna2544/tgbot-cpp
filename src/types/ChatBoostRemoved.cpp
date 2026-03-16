#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatBoostRemoved) {
    auto result = std::make_shared<ChatBoostRemoved>();
    result->chat = parseRequired<Chat>(data, "chat");
    parse(data, "boost_id", &result->boostId);
    parse(data, "remove_date", &result->removeDate);
    result->source = parseRequired<ChatBoostSource>(data, "source");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatBoostRemoved) {
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
