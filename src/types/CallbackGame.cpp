#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/types/CallbackGame.h>

namespace TgBot {

template <>
std::shared_ptr<CallbackGame> parse(const nlohmann::json &data) {
    auto result = std::make_shared<CallbackGame>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<CallbackGame> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
