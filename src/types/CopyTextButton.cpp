#include <tgbot/TgTypeParser.h>
#include <tgbot/types/CopyTextButton.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<CopyTextButton> parse(const nlohmann::json &data) {
    auto result = std::make_shared<CopyTextButton>();
    parse(data, "text", &result->text);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<CopyTextButton> &object) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
    }
    return json;
}

} // namespace TgBot
