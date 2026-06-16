#include <tgbot/TgTypeParser.h>
#include <tgbot/types/BusinessIntro.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<BusinessIntro> parse(const nlohmann::json &data) {
    auto result = std::make_shared<BusinessIntro>();
    parse(data, "title", &result->title);
    parse(data, "message", &result->message);
    result->sticker = parse<Sticker>(data, "sticker");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<BusinessIntro> &object) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("message", object->message);
        json.put("sticker", object->sticker);
    }
    return json;
}

} // namespace TgBot
