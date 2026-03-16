#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MessageEntity) {
    auto result = std::make_shared<MessageEntity>();
    std::string type;
    parse(data, "type", &type);
    if (type == "mention") {
        result->type = MessageEntity::Type::Mention;
    } else if (type == "hashtag") {
        result->type = MessageEntity::Type::Hashtag;
    } else if (type == "cashtag") {
        result->type = MessageEntity::Type::Cashtag;
    } else if (type == "bot_command") {
        result->type = MessageEntity::Type::BotCommand;
    } else if (type == "url") {
        result->type = MessageEntity::Type::Url;
    } else if (type == "email") {
        result->type = MessageEntity::Type::Email;
    } else if (type == "phone_number") {
        result->type = MessageEntity::Type::PhoneNumber;
    } else if (type == "bold") {
        result->type = MessageEntity::Type::Bold;
    } else if (type == "italic") {
        result->type = MessageEntity::Type::Italic;
    } else if (type == "underline") {
        result->type = MessageEntity::Type::Underline;
    } else if (type == "strikethrough") {
        result->type = MessageEntity::Type::Strikethrough;
    } else if (type == "spoiler") {
        result->type = MessageEntity::Type::Spoiler;
    } else if (type == "blockquote") {
        result->type = MessageEntity::Type::Blockquote;
    } else if (type == "expandable_blockquote") {
        result->type = MessageEntity::Type::ExpandableBlockquote;
    } else if (type == "code") {
        result->type = MessageEntity::Type::Code;
    } else if (type == "pre") {
        result->type = MessageEntity::Type::Pre;
    } else if (type == "text_link") {
        result->type = MessageEntity::Type::TextLink;
    } else if (type == "text_mention") {
        result->type = MessageEntity::Type::TextMention;
    } else if (type == "custom_emoji") {
        result->type = MessageEntity::Type::CustomEmoji;
    }

    parse(data, "offset", &result->offset);
    parse(data, "length", &result->length);
    parse(data, "url", &result->url);
    result->user = parse<User>(data, "user");
    parse(data, "language", &result->language);
    parse(data, "custom_emoji_id", &result->customEmojiId);
    parse(data, "unix_time", &result->unixTime);
    parse(data, "date_time_format", &result->dateTimeFormat);
    return result;
}

DECLARE_PARSER_TO_JSON(MessageEntity) {
    JsonWrapper json;
    if (object) {
        switch (object->type) {
            case MessageEntity::Type::Mention: json.put("type", "mention"); break;
            case MessageEntity::Type::Hashtag: json.put("type", "hashtag"); break;
            case MessageEntity::Type::Cashtag: json.put("type", "cashtag"); break;
            case MessageEntity::Type::BotCommand: json.put("type", "bot_command"); break;
            case MessageEntity::Type::Url: json.put("type", "url"); break;
            case MessageEntity::Type::Email: json.put("type", "email"); break;
            case MessageEntity::Type::PhoneNumber: json.put("type", "phone_number"); break;
            case MessageEntity::Type::Bold: json.put("type", "bold"); break;
            case MessageEntity::Type::Italic: json.put("type", "italic"); break;
            case MessageEntity::Type::Underline: json.put("type", "underline"); break;
            case MessageEntity::Type::Strikethrough: json.put("type", "strikethrough"); break;
            case MessageEntity::Type::Spoiler: json.put("type", "spoiler"); break;
            case MessageEntity::Type::Blockquote: json.put("type", "blockquote"); break;
            case MessageEntity::Type::ExpandableBlockquote: json.put("type", "expandable_blockquote"); break;
            case MessageEntity::Type::Code: json.put("type", "code"); break;
            case MessageEntity::Type::Pre: json.put("type", "pre"); break;
            case MessageEntity::Type::TextLink: json.put("type", "text_link"); break;
            case MessageEntity::Type::TextMention: json.put("type", "text_mention"); break;
            case MessageEntity::Type::CustomEmoji: json.put("type", "custom_emoji"); break;
        }
        json.put("offset", object->offset);
        json.put("length", object->length);
        json.put("url", object->url);
        json.put("user", object->user);
        json.put("language", object->language);
        json.put("custom_emoji_id", object->customEmojiId);
        json.put("unix_time", object->unixTime);
        json.put("date_time_format", object->dateTimeFormat);
    }
    return json;
}

} // namespace TgBot
