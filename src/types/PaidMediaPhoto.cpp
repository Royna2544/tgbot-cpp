#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaPhoto>();
    parse(data, "type", &result->type);
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
