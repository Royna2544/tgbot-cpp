#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InlineQueryResultCachedSticker.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InlineQueryResultCachedSticker> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InlineQueryResultCachedSticker>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "sticker_file_id", &result->stickerFileId);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InlineQueryResultCachedSticker> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("sticker_file_id", object->stickerFileId);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
    }
    return json;
}

} // namespace TgBot
