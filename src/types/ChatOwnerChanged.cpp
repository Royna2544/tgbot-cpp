#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatOwnerChanged) {
    auto result = std::make_shared<ChatOwnerChanged>();
    result->newOwner = parseRequired<User>(data, "new_owner");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatOwnerChanged) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

} // namespace TgBot
