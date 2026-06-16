#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Link.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Link) {
    auto result = std::make_shared<Link>();
    parse(data, "url", &result->url);
    return result;
}

DECLARE_PARSER_TO_JSON(Link) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
    }
    return json;
}

} // namespace TgBot
