#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockTableCell.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockTableCell> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockTableCell>();
    result->text = parse<RichText>(data, "text");
    parse(data, "is_header", &result->isHeader);
    parse(data, "colspan", &result->colspan);
    parse(data, "rowspan", &result->rowspan);
    parse(data, "align", &result->align);
    parse(data, "valign", &result->valign);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockTableCell> &object) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("is_header", object->isHeader);
        json.put("colspan", object->colspan);
        json.put("rowspan", object->rowspan);
        json.put("align", object->align);
        json.put("valign", object->valign);
    }
    return json;
}

} // namespace TgBot
