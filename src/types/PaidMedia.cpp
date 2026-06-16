#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PaidMedia.h>
#include <tgbot/types/PaidMediaLivePhoto.h>
#include <tgbot/types/PaidMediaPhoto.h>
#include <tgbot/types/PaidMediaPreview.h>
#include <tgbot/types/PaidMediaVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<PaidMedia> parse(const nlohmann::json &data) {
    std::string type;
    PaidMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "live_photo") {
        result = parse<PaidMediaLivePhoto>(data);
    } else if (type == "photo") {
        result = parse<PaidMediaPhoto>(data);
    } else if (type == "preview") {
        result = parse<PaidMediaPreview>(data);
    } else if (type == "video") {
        result = parse<PaidMediaVideo>(data);
    } else {
        throw invalidType("PaidMedia", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<PaidMedia> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "live_photo") {
            json += put<PaidMediaLivePhoto>(object);
        } else if (object->type == "photo") {
            json += put<PaidMediaPhoto>(object);
        } else if (object->type == "preview") {
            json += put<PaidMediaPreview>(object);
        } else if (object->type == "video") {
            json += put<PaidMediaVideo>(object);
        } else {
            throw invalidType("PaidMedia", object->type);
        }
    }
    return json;
}

} // namespace TgBot
