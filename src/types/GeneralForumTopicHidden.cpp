#include <tgbot/TgTypeParser.h>
#include <tgbot/types/GeneralForumTopicHidden.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<GeneralForumTopicHidden> parse(const nlohmann::json &data) {
    auto result = std::make_shared<GeneralForumTopicHidden>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<GeneralForumTopicHidden> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
