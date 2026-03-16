#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UniqueGiftColors) {
    auto result = std::make_shared<UniqueGiftColors>();
    parse(data, "model_custom_emoji_id", &result->modelCustomEmojiId);
    parse(data, "symbol_custom_emoji_id", &result->symbolCustomEmojiId);
    parse(data, "light_theme_main_color", &result->lightThemeMainColor);
    result->lightThemeOtherColors = parsePrimitiveRequiredArray<std::int64_t>(data, "light_theme_other_colors");
    parse(data, "dark_theme_main_color", &result->darkThemeMainColor);
    result->darkThemeOtherColors = parsePrimitiveRequiredArray<std::int64_t>(data, "dark_theme_other_colors");
    return result;
}

DECLARE_PARSER_TO_JSON(UniqueGiftColors) {
    JsonWrapper json;
    if (object) {
        json.put("model_custom_emoji_id", object->modelCustomEmojiId);
        json.put("symbol_custom_emoji_id", object->symbolCustomEmojiId);
        json.put("light_theme_main_color", object->lightThemeMainColor);
        json.put("light_theme_other_colors", object->lightThemeOtherColors);
        json.put("dark_theme_main_color", object->darkThemeMainColor);
        json.put("dark_theme_other_colors", object->darkThemeOtherColors);
    }
    return json;
}

} // namespace TgBot
