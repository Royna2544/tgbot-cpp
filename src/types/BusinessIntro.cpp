#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(BusinessIntro) {
    auto result = std::make_shared<BusinessIntro>();
    parse(data, "title", &result->title);
    parse(data, "message", &result->message);
    result->sticker = parse<Sticker>(data, "sticker");
    return result;
}

DECLARE_PARSER_TO_JSON(BusinessIntro) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("message", object->message);
        json.put("sticker", object->sticker);
    }
    return json;
}

} // namespace TgBot
