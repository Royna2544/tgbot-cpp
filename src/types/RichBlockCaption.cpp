#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockCaption.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockCaption> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockCaption>();
    result->text = parseRequired<RichText>(data, "text");
    result->credit = parse<RichText>(data, "credit");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockCaption> &object) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("credit", object->credit);
    }
    return json;
}

} // namespace TgBot
