#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BackgroundTypeChatTheme) {
    auto result = std::make_shared<BackgroundTypeChatTheme>();
    parse(data, "type", &result->type);
    parse(data, "theme_name", &result->themeName);
    return result;
}

DECLARE_PARSER_TO_JSON(BackgroundTypeChatTheme) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("theme_name", object->themeName);
    }
    return json;
}

} // namespace TgBot
