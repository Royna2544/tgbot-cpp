#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(User) {
    auto result = std::make_shared<User>();
    parse(data, "id", &result->id);
    parse(data, "is_bot", &result->isBot);
    parse(data, "first_name", &result->firstName);
    parse(data, "last_name", &result->lastName);
    parse(data, "username", &result->username);
    parse(data, "language_code", &result->languageCode);
    parse(data, "is_premium", &result->isPremium);
    parse(data, "added_to_attachment_menu", &result->addedToAttachmentMenu);
    parse(data, "can_join_groups", &result->canJoinGroups);
    parse(data, "can_read_all_group_messages", &result->canReadAllGroupMessages);
    parse(data, "supports_inline_queries", &result->supportsInlineQueries);
    parse(data, "can_connect_to_business", &result->canConnectToBusiness);
    parse(data, "has_main_web_app", &result->hasMainWebApp);
    parse(data, "has_topics_enabled", &result->hasTopicsEnabled);
    parse(data, "allows_users_to_create_topics", &result->allowsUsersToCreateTopics);
    return result;
}

DECLARE_PARSER_TO_JSON(User) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("is_bot", object->isBot);
        json.put("first_name", object->firstName);
        json.put("last_name", object->lastName);
        json.put("username", object->username);
        json.put("language_code", object->languageCode);
        json.put("is_premium", object->isPremium);
        json.put("added_to_attachment_menu", object->addedToAttachmentMenu);
        json.put("can_join_groups", object->canJoinGroups);
        json.put("can_read_all_group_messages", object->canReadAllGroupMessages);
        json.put("supports_inline_queries", object->supportsInlineQueries);
        json.put("can_connect_to_business", object->canConnectToBusiness);
        json.put("has_main_web_app", object->hasMainWebApp);
        json.put("has_topics_enabled", object->hasTopicsEnabled);
        json.put("allows_users_to_create_topics", object->allowsUsersToCreateTopics);
    }
    return json;
}

} // namespace TgBot
