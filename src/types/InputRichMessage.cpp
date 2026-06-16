#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichMessage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputRichMessage) {
    auto result = std::make_shared<InputRichMessage>();
    parse(data, "html", &result->html);
    parse(data, "markdown", &result->markdown);
    parse(data, "is_rtl", &result->isRtl);
    parse(data, "skip_entity_detection", &result->skipEntityDetection);
    return result;
}

DECLARE_PARSER_TO_JSON(InputRichMessage) {
    JsonWrapper json;
    if (object) {
        json.put("html", object->html);
        json.put("markdown", object->markdown);
        json.put("is_rtl", object->isRtl);
        json.put("skip_entity_detection", object->skipEntityDetection);
    }
    return json;
}

} // namespace TgBot
