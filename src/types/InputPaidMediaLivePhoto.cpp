#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPaidMediaLivePhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputPaidMediaLivePhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputPaidMediaLivePhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "photo", &result->photo);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputPaidMediaLivePhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
