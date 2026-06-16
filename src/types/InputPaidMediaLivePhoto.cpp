#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPaidMediaLivePhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputPaidMediaLivePhoto) {
    auto result = std::make_shared<InputPaidMediaLivePhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    parse(data, "photo", &result->photo);
    return result;
}

DECLARE_PARSER_TO_JSON(InputPaidMediaLivePhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
