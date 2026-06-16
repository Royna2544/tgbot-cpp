#include <tgbot/TgTypeParser.h>
#include <tgbot/types/WebAppInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<WebAppInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<WebAppInfo>();
    parse(data, "url", &result->url);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<WebAppInfo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
