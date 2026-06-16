#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GenericReply.h>
#include <tgbot/types/ForceReply.h>
#include <tgbot/types/InlineKeyboardMarkup.h>
#include <tgbot/types/ReplyKeyboardMarkup.h>
#include <tgbot/types/ReplyKeyboardRemove.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GenericReply> parse(const nlohmann::json &data) {
    if (data.contains("inline_keyboard")) return parse<InlineKeyboardMarkup>(data);
    if (data.contains("keyboard")) return parse<ReplyKeyboardMarkup>(data);
    if (data.contains("remove_keyboard")) return parse<ReplyKeyboardRemove>(data);
    if (data.contains("force_reply")) return parse<ForceReply>(data);
    return nullptr;
}

template <>
nlohmann::json put(const std::shared_ptr<GenericReply> &object) {
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
