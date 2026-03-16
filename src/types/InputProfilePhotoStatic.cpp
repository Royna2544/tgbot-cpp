#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputProfilePhotoStatic) {
    auto result = std::make_shared<InputProfilePhotoStatic>();
    parse(data, "type", &result->type);
    parse(data, "photo", &result->photo);
    return result;
}

DECLARE_PARSER_TO_JSON(InputProfilePhotoStatic) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
