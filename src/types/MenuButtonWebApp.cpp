#include <tgbot/TgTypeParser.h>
#include <tgbot/types/MenuButtonWebApp.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<MenuButtonWebApp> parse(const nlohmann::json &data) {
    auto result = std::make_shared<MenuButtonWebApp>();
    parse(data, "type", &result->type);
    parse(data, "text", &result->text);
    result->webApp = parseRequired<WebAppInfo>(data, "web_app");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<MenuButtonWebApp> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("web_app", object->webApp);
    }
    return json;
}

} // namespace TgBot
