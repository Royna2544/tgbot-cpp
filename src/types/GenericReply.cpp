#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(GenericReply) {
    if (data.contains("inline_keyboard")) return parse<InlineKeyboardMarkup>(data);
    if (data.contains("keyboard")) return parse<ReplyKeyboardMarkup>(data);
    if (data.contains("remove_keyboard")) return parse<ReplyKeyboardRemove>(data);
    if (data.contains("force_reply")) return parse<ForceReply>(data);
    return nullptr;
}

DECLARE_PARSER_TO_JSON(GenericReply) {
    JsonWrapper json;
    if (object) {
        if (auto t = std::dynamic_pointer_cast<InlineKeyboardMarkup>(object)) return put(t);
        if (auto t = std::dynamic_pointer_cast<ReplyKeyboardMarkup>(object)) return put(t);
        if (auto t = std::dynamic_pointer_cast<ReplyKeyboardRemove>(object)) return put(t);
        if (auto t = std::dynamic_pointer_cast<ForceReply>(object)) return put(t);
        return JsonWrapper();
    }
    return json;
}

} // namespace TgBot
