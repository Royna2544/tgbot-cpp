#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessLocation.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessLocation> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessLocation>();
    parse(data, "address", &result->address);
    result->location = parse<Location>(data, "location");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessLocation> &object) {
    JsonWrapper json;
    if (object) {
        json.put("address", object->address);
        json.put("location", object->location);
    }
    return json;
}

} // namespace TgBot
