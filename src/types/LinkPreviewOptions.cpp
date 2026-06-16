#include <tgbot/TgTypeParser.h>
#include <tgbot/types/LinkPreviewOptions.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<LinkPreviewOptions> parse(const nlohmann::json &data) {
    auto result = std::make_shared<LinkPreviewOptions>();
    parse(data, "is_disabled", &result->isDisabled);
    parse(data, "url", &result->url);
    parse(data, "prefer_small_media", &result->preferSmallMedia);
    parse(data, "prefer_large_media", &result->preferLargeMedia);
    parse(data, "show_above_text", &result->showAboveText);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<LinkPreviewOptions> &object) {
    JsonWrapper json;
    if (object) {
        json.put("is_disabled", object->isDisabled);
        json.put("url", object->url);
        json.put("prefer_small_media", object->preferSmallMedia);
        json.put("prefer_large_media", object->preferLargeMedia);
        json.put("show_above_text", object->showAboveText);
    }
    return json;
}

} // namespace TgBot
