#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaLivePhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaLivePhoto) {
    auto result = std::make_shared<PaidMediaLivePhoto>();
    parse(data, "type", &result->type);
    result->livePhoto = parseRequired<LivePhoto>(data, "live_photo");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaLivePhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("live_photo", object->livePhoto);
    }
    return json;
}

} // namespace TgBot
