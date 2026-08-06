#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlockAudio.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlockAudio> parse(const nlohmann::json &data) {
    auto result = std::make_shared<InputRichBlockAudio>();
    parse(data, "type", &result->type);
    result->audio = parseRequired<InputMediaAudio>(data, "audio");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlockAudio> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("audio", object->audio);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
