#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaLivePhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaLivePhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaLivePhoto>();
    parse(data, "type", &result->type);
    result->livePhoto = parseRequired<LivePhoto>(data, "live_photo");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaLivePhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("live_photo", object->livePhoto);
    }
    return json;
}

} // namespace TgBot
