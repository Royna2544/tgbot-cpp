#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ReplyKeyboardRemove) {
    auto result = std::make_shared<ReplyKeyboardRemove>();
    parse(data, "remove_keyboard", &result->removeKeyboard);
    parse(data, "selective", &result->selective);
    return result;
}

DECLARE_PARSER_TO_JSON(ReplyKeyboardRemove) {
    JsonWrapper json;
    if (object) {
        json.put("remove_keyboard", object->removeKeyboard);
        json.put("selective", object->selective);
    }
    return json;
}

} // namespace TgBot
