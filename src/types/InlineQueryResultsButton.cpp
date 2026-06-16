#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InlineQueryResultsButton.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InlineQueryResultsButton> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InlineQueryResultsButton>();
    parse(data, "text", &result->text);
    result->webApp = parse<WebAppInfo>(data, "web_app");
    parse(data, "start_parameter", &result->startParameter);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InlineQueryResultsButton> &object) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("web_app", object->webApp);
        json.put("start_parameter", object->startParameter);
    }
    return json;
}

} // namespace TgBot
