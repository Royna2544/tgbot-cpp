#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(CopyTextButton) {
    auto result = std::make_shared<CopyTextButton>();
    parse(data, "text", &result->text);
    return result;
}

DECLARE_PARSER_TO_JSON(CopyTextButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
