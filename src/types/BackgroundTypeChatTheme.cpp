#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BackgroundTypeChatTheme.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BackgroundTypeChatTheme> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BackgroundTypeChatTheme>();
    parse(data, "type", &result->type);
    parse(data, "theme_name", &result->themeName);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BackgroundTypeChatTheme> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("theme_name", object->themeName);
    }
    return json;
}

} // namespace TgBot
