#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UserProfileAudios.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UserProfileAudios> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UserProfileAudios>();
    parse(data, "total_count", &result->totalCount);
    result->audios = parseRequiredArray<Audio>(data, "audios");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UserProfileAudios> &object) {
    JsonWrapper json;
    if (object) {
        json.put("total_count", object->totalCount);
        json.put("audios", object->audios);
    }
    return json;
}

} // namespace TgBot
