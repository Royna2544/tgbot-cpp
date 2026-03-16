#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ForceReply) {
    auto result = std::make_shared<ForceReply>();
    parse(data, "force_reply", &result->forceReply);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
    return result;
}

DECLARE_PARSER_TO_JSON(ForceReply) {
    JsonWrapper json;
    if (object) {
        json.put("force_reply", object->forceReply);
        json.put("input_field_placeholder", object->inputFieldPlaceholder);
        json.put("selective", object->selective);
    }
    return json;
}

} // namespace TgBot
