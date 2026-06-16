#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PollOption.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PollOption> parse(const nlohmann::json &data) {
    auto result = std::make_shared<PollOption>();
    parse(data, "persistent_id", &result->persistentId);
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->media = parse<PollMedia>(data, "media");
    parse(data, "voter_count", &result->voterCount);
    result->addedByUser = parse<User>(data, "added_by_user");
    result->addedByChat = parse<Chat>(data, "added_by_chat");
    parse(data, "addition_date", &result->additionDate);
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PollOption> &object) {
    JsonWrapper json;
    if (object) {
        json.put("persistent_id", object->persistentId);
        json.put("text", object->text);
        json.put("text_entities", object->textEntities);
        json.put("media", object->media);
        json.put("voter_count", object->voterCount);
        json.put("added_by_user", object->addedByUser);
        json.put("added_by_chat", object->addedByChat);
        json.put("addition_date", object->additionDate);
    }
    return json;
}

} // namespace TgBot
