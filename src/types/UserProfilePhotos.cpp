#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UserProfilePhotos.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UserProfilePhotos> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UserProfilePhotos>();
    parse(data, "total_count", &result->totalCount);
    result->photos = parseMatrix<PhotoSize>(data, "photos");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UserProfilePhotos> &object) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("photos", object->photos);
    }
    return json;
}

} // namespace TgBot
