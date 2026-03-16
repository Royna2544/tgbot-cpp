#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputPaidMediaPhoto) {
    auto result = std::make_shared<InputPaidMediaPhoto>();
    parse(data, "type", &result->type);
    parse(data, "media", &result->media);
    return result;
}

DECLARE_PARSER_TO_JSON(InputPaidMediaPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("media", object->media);
    }
    return json;
}

} // namespace TgBot
