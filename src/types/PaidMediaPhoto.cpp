#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PaidMediaPhoto) {
    auto result = std::make_shared<PaidMediaPhoto>();
    parse(data, "type", &result->type);
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
    return result;
}

DECLARE_PARSER_TO_JSON(PaidMediaPhoto) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
