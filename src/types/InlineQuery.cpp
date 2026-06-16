#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InlineQuery.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InlineQuery> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InlineQuery>();
    parse(data, "id", &result->id);
    result->from = parseRequired<User>(data, "from");
    parse(data, "query", &result->query);
    parse(data, "offset", &result->offset);
    parse(data, "chat_type", &result->chatType);
    result->location = parse<Location>(data, "location");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InlineQuery> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("from", object->from);
        json.put("query", object->query);
        json.put("offset", object->offset);
        json.put("chat_type", object->chatType);
        json.put("location", object->location);
    }
    return json;
}

} // namespace TgBot
