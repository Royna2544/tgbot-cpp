#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichTextHashtag.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichTextHashtag> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichTextHashtag>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    parse(data, "hashtag", &result->hashtag);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichTextHashtag> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
        json.put("hashtag", object->hashtag);
    }
    return json;
}

} // namespace TgBot
