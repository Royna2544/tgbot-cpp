#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputStoryContent.h>
#include <tgbot/types/InputStoryContentPhoto.h>
#include <tgbot/types/InputStoryContentVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputStoryContent> parse(const nlohmann::json &data) {
    std::string type;
    InputStoryContent::Ptr result;
    parse(data, "type", &type);
    if (type == "photo") {
        result = parse<InputStoryContentPhoto>(data);
    } else if (type == "video") {
        result = parse<InputStoryContentVideo>(data);
    } else {
        throw invalidType("InputStoryContent", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputStoryContent> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "photo") {
            json += put<InputStoryContentPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputStoryContentVideo>(object);
        } else {
            throw invalidType("InputStoryContent", object->type);
        }
    }
    return json;
}

} // namespace TgBot
