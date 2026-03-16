#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Sticker) {
    auto result = std::make_shared<Sticker>();
    parse(data, "file_id", &result->fileId);
    parse(data, "file_unique_id", &result->fileUniqueId);
    std::string type;
    parse(data, "type", &type);
    if (type == "regular") {
        result->type = Sticker::Type::Regular;
    } else if (type == "mask") {
        result->type = Sticker::Type::Mask;
    } else if (type == "custom_emoji") {
        result->type = Sticker::Type::CustomEmoji;
    }

    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "is_animated", &result->isAnimated);
    parse(data, "is_video", &result->isVideo);
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    parse(data, "emoji", &result->emoji);
    parse(data, "set_name", &result->setName);
    result->premiumAnimation = parse<File>(data, "premium_animation");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    parse(data, "custom_emoji_id", &result->customEmojiId);
    parse(data, "needs_repainting", &result->needsRepainting);
    parse(data, "file_size", &result->fileSize);
    return result;
}

DECLARE_PARSER_TO_JSON(Sticker) {
    JsonWrapper json;
    if (object) {
        json.put("file_id", object->fileId);
        json.put("file_unique_id", object->fileUniqueId);
        switch (object->type) {
            case Sticker::Type::Regular: json.put("type", "regular"); break;
            case Sticker::Type::Mask: json.put("type", "mask"); break;
            case Sticker::Type::CustomEmoji: json.put("type", "custom_emoji"); break;
        }
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("is_animated", object->isAnimated);
        json.put("is_video", object->isVideo);
        json.put("thumbnail", object->thumbnail);
        json.put("emoji", object->emoji);
        json.put("set_name", object->setName);
        json.put("premium_animation", object->premiumAnimation);
        json.put("mask_position", object->maskPosition);
        json.put("custom_emoji_id", object->customEmojiId);
        json.put("needs_repainting", object->needsRepainting);
        json.put("file_size", object->fileSize);
    }
    return json;
}

} // namespace TgBot
