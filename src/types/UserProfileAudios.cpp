#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UserProfileAudios) {
    auto result = std::make_shared<UserProfileAudios>();
    parse(data, "total_count", &result->totalCount);
    result->audios = parseRequiredArray<Audio>(data, "audios");
    return result;
}

DECLARE_PARSER_TO_JSON(UserProfileAudios) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("audios", object->audios);
    }
    return json;
}

} // namespace TgBot
