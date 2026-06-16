#include <tgbot/TgTypeParser.h>
#include <tgbot/types/SuggestedPostDeclined.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<SuggestedPostDeclined> parse(const nlohmann::json &data) {
    auto result = std::make_shared<SuggestedPostDeclined>();
    result->suggestedPostMessage = parse<Message>(data, "suggested_post_message");
    parse(data, "comment", &result->comment);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<SuggestedPostDeclined> &object) {
    JsonWrapper json;
    if (object) {
        json.put("suggested_post_message", object->suggestedPostMessage);
        json.put("comment", object->comment);
    }
    return json;
}

} // namespace TgBot
