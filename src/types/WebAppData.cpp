#include <tgbot/TgTypeParser.h>
#include <tgbot/types/WebAppData.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<WebAppData> parse(const nlohmann::json &data) {
    auto result = std::make_shared<WebAppData>();
    parse(data, "data", &result->data);
    parse(data, "button_text", &result->buttonText);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<WebAppData> &object) {
    JsonWrapper json;
    if (object) {
        json.put("data", object->data);
        json.put("button_text", object->buttonText);
    }
    return json;
}

} // namespace TgBot
