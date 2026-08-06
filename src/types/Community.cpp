#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Community.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Community> parse(const nlohmann::json &data) {
    auto result = std::make_shared<Community>();
    parse(data, "id", &result->id);
    parse(data, "name", &result->name);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Community> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("name", object->name);
    }
    return json;
}

} // namespace TgBot
