#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResultsButton) {
    auto result = std::make_shared<InlineQueryResultsButton>();
    parse(data, "text", &result->text);
    result->webApp = parse<WebAppInfo>(data, "web_app");
    parse(data, "start_parameter", &result->startParameter);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResultsButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("web_app", object->webApp);
        json.put("start_parameter", object->startParameter);
    }
    return json;
}

} // namespace TgBot
