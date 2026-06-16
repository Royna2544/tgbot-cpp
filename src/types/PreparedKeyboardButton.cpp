#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PreparedKeyboardButton.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PreparedKeyboardButton) {
    auto result = std::make_shared<PreparedKeyboardButton>();
    parse(data, "id", &result->id);
    return result;
}

DECLARE_PARSER_TO_JSON(PreparedKeyboardButton) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
    }
    return json;
}

} // namespace TgBot
