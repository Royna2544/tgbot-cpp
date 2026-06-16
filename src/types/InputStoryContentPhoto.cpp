#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputStoryContentPhoto.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputStoryContentPhoto> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputStoryContentPhoto>();
    parse(data, "type", &result->type);
    parse(data, "photo", &result->photo);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputStoryContentPhoto> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("photo", object->photo);
    }
    return json;
}

} // namespace TgBot
