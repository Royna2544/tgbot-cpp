#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageAutoDeleteTimerChanged) {
    auto result = std::make_shared<MessageAutoDeleteTimerChanged>();
    parse(data, "message_auto_delete_time", &result->messageAutoDeleteTime);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageAutoDeleteTimerChanged) {
    JsonWrapper json;
    if (object) {
        json.put("message_auto_delete_time", object->messageAutoDeleteTime);
    }
    return json;
}

} // namespace TgBot
