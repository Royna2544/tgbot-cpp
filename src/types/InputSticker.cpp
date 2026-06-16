#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputSticker.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputSticker> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputSticker>();
    parse(data, "sticker", &result->sticker);
    parse(data, "format", &result->format);
    result->emojiList = parsePrimitiveRequiredArray<std::string>(data, "emoji_list");
    result->maskPosition = parse<MaskPosition>(data, "mask_position");
    result->keywords = parsePrimitiveArray<std::string>(data, "keywords");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputSticker> &object) {
    JsonWrapper json;
    if (object) {
        json.put("sticker", object->sticker);
        json.put("format", object->format);
        json.put("emoji_list", object->emojiList);
        json.put("mask_position", object->maskPosition);
        json.put("keywords", object->keywords);
    }
    return json;
}

} // namespace TgBot
