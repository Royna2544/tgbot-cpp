#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockFooter.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockFooter> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockFooter>();
    parse(data, "type", &result->type);
    result->text = parseRequired<RichText>(data, "text");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockFooter> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
