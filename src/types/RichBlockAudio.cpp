#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlockAudio.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<RichBlockAudio> parse(const nlohmann::json &data) {
    auto result = std::make_shared<RichBlockAudio>();
    parse(data, "type", &result->type);
    result->audio = parseRequired<Audio>(data, "audio");
    result->caption = parse<RichBlockCaption>(data, "caption");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<RichBlockAudio> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        json.put("audio", object->audio);
        json.put("caption", object->caption);
    }
    return json;
}

} // namespace TgBot
