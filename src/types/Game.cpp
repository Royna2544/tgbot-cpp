#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Game) {
    auto result = std::make_shared<Game>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    result->photo = parseRequiredArray<PhotoSize>(data, "photo");
    parse(data, "text", &result->text);
    result->textEntities = parseArray<MessageEntity>(data, "text_entities");
    result->animation = parse<Animation>(data, "animation");
    return result;
}

DECLARE_PARSER_TO_JSON(Game) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("description", object->description);
        json.put("photo", object->photo);
        json.put("text", object->text);
        json.put("text_entities", object->textEntities);
        json.put("animation", object->animation);
    }
    return json;
}

} // namespace TgBot
