#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(UserProfilePhotos) {
    auto result = std::make_shared<UserProfilePhotos>();
    parse(data, "total_count", &result->totalCount);
    result->photos = parseMatrix<PhotoSize>(data, "photos");
    return result;
}

DECLARE_PARSER_TO_JSON(UserProfilePhotos) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("photos", object->photos);
    }
    return json;
}

} // namespace TgBot
