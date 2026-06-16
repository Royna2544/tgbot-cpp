#include <tgbot/TgTypeParser.h>
#include <tgbot/types/UniqueGiftBackdropColors.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<UniqueGiftBackdropColors> parse(const nlohmann::json &data) {
    auto result = std::make_shared<UniqueGiftBackdropColors>();
    parse(data, "center_color", &result->centerColor);
    parse(data, "edge_color", &result->edgeColor);
    parse(data, "symbol_color", &result->symbolColor);
    parse(data, "text_color", &result->textColor);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<UniqueGiftBackdropColors> &object) {
    JsonWrapper json;
    if (object) {
        json.put("center_color", object->centerColor);
        json.put("edge_color", object->edgeColor);
        json.put("symbol_color", object->symbolColor);
        json.put("text_color", object->textColor);
    }
    return json;
}

} // namespace TgBot
