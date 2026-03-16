#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

MaybeInaccessibleMessage parse(const nlohmann::json& data) {
    if (data.is_null()) {
        return Message::Ptr{};
    }
    if (data.contains("date") && data["date"] == 0) return parse<InaccessibleMessage>(data);
    return parse<Message>(data);
}

nlohmann::json put(const MaybeInaccessibleMessage& object) {
    JsonWrapper json;
    std::visit([&json](const auto& value) { json += TgBot::put(value); }, object);
    return json;
}

} // namespace TgBot
