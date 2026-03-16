#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StickerSet) {
    auto result = std::make_shared<StickerSet>();
    parse(data, "name", &result->name);
    parse(data, "title", &result->title);
    std::string stickerType;
    parse(data, "sticker_type", &stickerType);
    if (stickerType == "regular") {
        result->stickerType = StickerSet::Type::Regular;
    } else if (stickerType == "mask") {
        result->stickerType = StickerSet::Type::Mask;
    } else if (stickerType == "custom_emoji") {
        result->stickerType = StickerSet::Type::CustomEmoji;
    }

    result->stickers = parseRequiredArray<Sticker>(data, "stickers");
    result->thumbnail = parse<PhotoSize>(data, "thumbnail");
    return result;
}

DECLARE_PARSER_TO_JSON(StickerSet) {
    JsonWrapper json;
    if (object) {
        json.put("name", object->name);
        json.put("title", object->title);
        switch (object->stickerType) {
            case StickerSet::Type::Regular: json.put("sticker_type", "regular"); break;
            case StickerSet::Type::Mask: json.put("sticker_type", "mask"); break;
            case StickerSet::Type::CustomEmoji: json.put("sticker_type", "custom_emoji"); break;
        }
        json.put("stickers", object->stickers);
        json.put("thumbnail", object->thumbnail);
    }
    return json;
}

} // namespace TgBot
