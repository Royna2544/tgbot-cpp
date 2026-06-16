#include <tgbot/TgTypeParser.h>
#include <tgbot/types/VideoChatStarted.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<VideoChatStarted> parse(const nlohmann::json &data) {
    auto result = std::make_shared<VideoChatStarted>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<VideoChatStarted> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
