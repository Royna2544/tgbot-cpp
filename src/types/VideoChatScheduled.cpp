#include <tgbot/TgTypeParser.h>
#include <tgbot/types/VideoChatScheduled.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<VideoChatScheduled> parse(const nlohmann::json &data) {
    auto result = std::make_shared<VideoChatScheduled>();
    parse(data, "start_date", &result->startDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<VideoChatScheduled> &object) {
    JsonWrapper json;
    if (object) {
        json.put("start_date", object->startDate);
    }
    return json;
}

} // namespace TgBot
