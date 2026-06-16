#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextCashtag.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextCashtag> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextCashtag>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "cashtag", &result->cashtag);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextCashtag> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("cashtag", object->cashtag);
    }
    return json;
}

} // namespace TgBot
