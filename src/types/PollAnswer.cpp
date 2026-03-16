#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PollAnswer) {
    auto result = std::make_shared<PollAnswer>();
    parse(data, "poll_id", &result->pollId);
    result->voterChat = parse<Chat>(data, "voter_chat");
    result->user = parse<User>(data, "user");
    result->optionIds = parsePrimitiveRequiredArray<std::int32_t>(data, "option_ids");
    return result;
}

DECLARE_PARSER_TO_JSON(PollAnswer) {
    JsonWrapper json;
    if (object) {
        json.put("poll_id", object->pollId);
        json.put("voter_chat", object->voterChat);
        json.put("user", object->user);
        json.put("option_ids", object->optionIds);
    }
    return json;
}

} // namespace TgBot
