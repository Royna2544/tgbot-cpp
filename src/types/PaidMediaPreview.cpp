#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMediaPreview.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMediaPreview> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PaidMediaPreview>();
    parse(data, "type", &result->type);
    parse(data, "width", &result->width);
    parse(data, "height", &result->height);
    parse(data, "duration", &result->duration);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMediaPreview> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("width", object->width);
        json.put("height", object->height);
        json.put("duration", object->duration);
    }
    return json;
}

} // namespace TgBot
