#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ChatLocation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ChatLocation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ChatLocation>();
    result->location = parseRequired<Location>(data, "location");
    parse(data, "address", &result->address);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ChatLocation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("location", object->location);
        json.put("address", object->address);
    }
    return json;
}

} // namespace TgBot
