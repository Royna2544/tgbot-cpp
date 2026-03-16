#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(VideoChatScheduled) {
    auto result = std::make_shared<VideoChatScheduled>();
    parse(data, "start_date", &result->startDate);
    return result;
}

DECLARE_PARSER_TO_JSON(VideoChatScheduled) {
    JsonWrapper json;
    if (object) {
        json.put("start_date", object->startDate);
    }
    return json;
}

} // namespace TgBot
