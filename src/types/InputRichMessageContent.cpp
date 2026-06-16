#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichMessageContent.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichMessageContent> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichMessageContent>();
    result->richMessage = parseRequired<InputRichMessage>(data, "rich_message");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichMessageContent> &object) {
    JsonWrapper json;
    if (object) {
        json.put("rich_message", object->richMessage);
    }
    return json;
}

} // namespace TgBot
