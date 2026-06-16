#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputPollOption.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputPollOption> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputPollOption>();
    parse(data, "text", &result->text);
    parse(data, "text_parse_mode", &result->textParseMode);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->media = parse<InputMedia>(data, "media");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputPollOption> &object) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("text_parse_mode", object->textParseMode);
        json.put("text_entities", object->textEntities);
        json.put("media", object->media);
    }
    return json;
}

} // namespace TgBot
