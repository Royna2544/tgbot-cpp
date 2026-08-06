#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichMessageMedia.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichMessageMedia> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichMessageMedia>();
    parse(data, "id", &result->id);
    result->media = parseRequired<InputMedia>(data, "media");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichMessageMedia> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("media", object->media);
    }
    return json;
}

} // namespace TgBot
