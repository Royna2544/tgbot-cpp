#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPaidMediaPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputPaidMediaPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputPaidMediaPhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputPaidMediaPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
    }
    return json;
}

} // namespace TgBot
