#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ChatOwnerLeft) {
    auto result = std::make_shared<ChatOwnerLeft>();
    result->newOwner = parse<User>(data, "new_owner");
    return result;
}

DECLARE_PARSER_TO_JSON(ChatOwnerLeft) {
    JsonWrapper json;
    if (object) {
        json.put("new_owner", object->newOwner);
    }
    return json;
}

} // namespace TgBot
