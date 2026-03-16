#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(MenuButtonWebApp) {
    auto result = std::make_shared<MenuButtonWebApp>();
    parse(data, "type", &result->type);
    parse(data, "text", &result->text);
    result->webApp = parseRequired<WebAppInfo>(data, "web_app");
    return result;
}

DECLARE_PARSER_TO_JSON(MenuButtonWebApp) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("web_app", object->webApp);
    }
    return json;
}

} // namespace TgBot
