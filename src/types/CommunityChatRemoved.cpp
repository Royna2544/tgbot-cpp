#include <tgbot/TgTypeParser.h>
#include <tgbot/types/CommunityChatRemoved.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<CommunityChatRemoved> parse(const nlohmann::json &data) {
    auto result = std::make_shared<CommunityChatRemoved>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<CommunityChatRemoved> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
