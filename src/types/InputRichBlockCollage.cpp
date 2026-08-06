#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockCollage.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockCollage> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockCollage>();
    parse(data, "type", &result->type);
    result->blocks = parseRequiredArray<InputRichBlock>(data, "blocks");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockCollage> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("blocks", object->blocks);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
