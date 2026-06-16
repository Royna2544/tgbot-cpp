#include <tgbot/TgTypeParser.h>
#include <tgbot/types/ForceReply.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<ForceReply> parse(const nlohmann::json &data) {
    auto result = std::make_shared<ForceReply>();
    parse(data, "force_reply", &result->forceReply);
    parse(data, "input_field_placeholder", &result->inputFieldPlaceholder);
    parse(data, "selective", &result->selective);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<ForceReply> &object) {
    JsonWrapper json;
    if (object) {
        json.put("force_reply", object->forceReply);
        json.put("input_field_placeholder", object->inputFieldPlaceholder);
        json.put("selective", object->selective);
    }
    return json;
}

} // namespace TgBot
