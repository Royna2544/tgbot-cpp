#include <tgbot/TgTypeParser.h>
#include <tgbot/types/CommunityChatAdded.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<CommunityChatAdded> parse(const nlohmann::json &data) {
    auto result = std::make_shared<CommunityChatAdded>();
    result->community = parseRequired<Community>(data, "community");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<CommunityChatAdded> &object) {
    JsonWrapper json;
    if (object) {
        json.put("community", object->community);
    }
    return json;
}

} // namespace TgBot
