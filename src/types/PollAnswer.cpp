#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PollAnswer.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PollAnswer> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PollAnswer>();
    parse(data, "poll_id", &result->pollId);
    result->voterChat = parse<Chat>(data, "voter_chat");
    result->user = parse<User>(data, "user");
    result->optionIds = parsePrimitiveRequiredArray<std::int32_t>(data, "option_ids");
    result->optionPersistentIds = parsePrimitiveRequiredArray<std::string>(data, "option_persistent_ids");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PollAnswer> &object) {
    JsonWrapper json;
    if (object) {
        json.put("poll_id", object->pollId);
        json.put("voter_chat", object->voterChat);
        json.put("user", object->user);
        json.put("option_ids", object->optionIds);
        json.put("option_persistent_ids", object->optionPersistentIds);
    }
    return json;
}

} // namespace TgBot
