#include <tgbot/TgException.h>
#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputFile.h>

namespace TgBot {

template <>
std::shared_ptr<InputFile> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputFile>();
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputFile> &object) {
    JsonWrapper json;
    if (object) {
    }
    return json;
}

} // namespace TgBot
