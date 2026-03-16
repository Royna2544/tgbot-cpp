#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultCachedVoice) {
    auto result = std::make_shared<InlineQueryResultCachedVoice>();
    parse(data, "type", &result->type);
    parse(data, "id", &result->id);
    parse(data, "voice_file_id", &result->voiceFileId);
    parse(data, "title", &result->title);
    parse(data, "caption", &result->caption);
    parse(data, "parse_mode", &result->parseMode);
    result->captionEntities = parseArray<MessageEntity>(data, "caption_entities");
    result->replyMarkup = parse<InlineKeyboardMarkup>(data, "reply_markup");
    result->inputMessageContent = parse<InputMessageContent>(data, "input_message_content");
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultCachedVoice) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("id", object->id);
        json.put("voice_file_id", object->voiceFileId);
        json.put("title", object->title);
        json.put("caption", object->caption);
        json.put("parse_mode", object->parseMode);
        json.put("caption_entities", object->captionEntities);
        json.put("reply_markup", object->replyMarkup);
        json.put("input_message_content", object->inputMessageContent);
    }
    return json;
}

} // namespace TgBot
