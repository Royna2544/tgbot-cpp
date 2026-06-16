#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputChecklistTask.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputChecklistTask> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputChecklistTask>();
    parse(data, "id", &result->id);
    parse(data, "text", &result->text);
    parse(data, "parse_mode", &result->parseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputChecklistTask> &object) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("text", object->text);
        json.put("parse_mode", object->parseMode);
        json.put("text_entities", object->textEntities);
    }
    return json;
}

} // namespace TgBot
