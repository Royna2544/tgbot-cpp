#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockCollage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlockCollage) {
    auto result = std::make_shared<RichBlockCollage>();
    parse(data, "type", &result->type);
    result->blocks = parseRequiredArray<RichBlock>(data, "blocks");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

DECLARE_PARSER_TO_JSON(RichBlockCollage) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("blocks", object->blocks);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
