#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InlineQueryResultAudio.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InlineQueryResultAudio> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InlineQueryResultAudio>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "audio_url", &result->audioUrl);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    parse(data, "performer", &result->performer);
    parse(data, "audio_duration", &result->audioDuration);
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InlineQueryResultAudio> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("audio_url", object->audioUrl);
        json.put("title", object->title);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("performer", object->performer);
        json.put("audio_duration", object->audioDuration);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
    }
    return json;
}

} // namespace TgBot
