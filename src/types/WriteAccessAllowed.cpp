#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(WriteAccessAllowed) {
    auto result = std::make_shared<WriteAccessAllowed>();
    parse(data, "from_request", &result->fromRequest);
    parse(data, "web_app_name", &result->webAppName);
    parse(data, "from_attachment_menu", &result->fromAttachmentMenu);
    return result;
}

DECLARE_PARSER_TO_JSON(WriteAccessAllowed) {
    JsonWrapper json;
    if (object) {
        json.put("from_request", object->fromRequest);
        json.put("web_app_name", object->webAppName);
        json.put("from_attachment_menu", object->fromAttachmentMenu);
    }
    return json;
}

} // namespace TgBot
