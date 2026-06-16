#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MessageAutoDeleteTimerChanged.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MessageAutoDeleteTimerChanged> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MessageAutoDeleteTimerChanged>();
    parse(data, "message_auto_delete_time", &result->messageAutoDeleteTime);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MessageAutoDeleteTimerChanged> &object) {
    JsonWrapper json;
    if (object) {
        json.put("message_auto_delete_time", object->messageAutoDeleteTime);
    }
    return json;
}

} // namespace TgBot
