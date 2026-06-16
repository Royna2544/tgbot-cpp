#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextDateTime.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichTextDateTime) {
    auto result = std::make_shared<RichTextDateTime>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "unix_time", &result->unixTime);
    parse(data, "date_time_format", &result->dateTimeFormat);
    return result;
}

DECLARE_PARSER_TO_JSON(RichTextDateTime) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("unix_time", object->unixTime);
        json.put("date_time_format", object->dateTimeFormat);
    }
    return json;
}

} // namespace TgBot
