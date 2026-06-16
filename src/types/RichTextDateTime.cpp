#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextDateTime.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextDateTime> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextDateTime>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "unix_time", &result->unixTime);
    parse(data, "date_time_format", &result->dateTimeFormat);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextDateTime> &object) {
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
