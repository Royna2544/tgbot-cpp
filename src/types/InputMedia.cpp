#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputMedia.h>
#include <tgbot/types/InputMediaAnimation.h>
#include <tgbot/types/InputMediaAudio.h>
#include <tgbot/types/InputMediaDocument.h>
#include <tgbot/types/InputMediaLink.h>
#include <tgbot/types/InputMediaLivePhoto.h>
#include <tgbot/types/InputMediaLocation.h>
#include <tgbot/types/InputMediaPhoto.h>
#include <tgbot/types/InputMediaSticker.h>
#include <tgbot/types/InputMediaVenue.h>
#include <tgbot/types/InputMediaVideo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputMedia> parse(const nlohmann::json &data) {
    std::string type;
    InputMedia::Ptr result;
    parse(data, "type", &type);
    if (type == "animation") {
        result = parse<InputMediaAnimation>(data);
    } else if (type == "audio") {
        result = parse<InputMediaAudio>(data);
    } else if (type == "document") {
        result = parse<InputMediaDocument>(data);
    } else if (type == "live_photo") {
        result = parse<InputMediaLivePhoto>(data);
    } else if (type == "photo") {
        result = parse<InputMediaPhoto>(data);
    } else if (type == "video") {
        result = parse<InputMediaVideo>(data);
    } else if (type == "location") {
        result = parse<InputMediaLocation>(data);
    } else if (type == "venue") {
        result = parse<InputMediaVenue>(data);
    } else if (type == "link") {
        result = parse<InputMediaLink>(data);
    } else if (type == "sticker") {
        result = parse<InputMediaSticker>(data);
    } else {
        throw invalidType("InputMedia", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputMedia> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "animation") {
            json += put<InputMediaAnimation>(object);
        } else if (object->type == "audio") {
            json += put<InputMediaAudio>(object);
        } else if (object->type == "document") {
            json += put<InputMediaDocument>(object);
        } else if (object->type == "live_photo") {
            json += put<InputMediaLivePhoto>(object);
        } else if (object->type == "photo") {
            json += put<InputMediaPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputMediaVideo>(object);
        } else if (object->type == "location") {
            json += put<InputMediaLocation>(object);
        } else if (object->type == "venue") {
            json += put<InputMediaVenue>(object);
        } else if (object->type == "link") {
            json += put<InputMediaLink>(object);
        } else if (object->type == "sticker") {
            json += put<InputMediaSticker>(object);
        } else {
            throw invalidType("InputMedia", object->type);
        }
    }
    return json;
}

} // namespace TgBot
