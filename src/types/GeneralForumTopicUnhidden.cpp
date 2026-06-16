#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GeneralForumTopicUnhidden.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GeneralForumTopicUnhidden> parse(const nlohmann::json &data) {
    auto result = std::make_shared<GeneralForumTopicUnhidden>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<GeneralForumTopicUnhidden> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
