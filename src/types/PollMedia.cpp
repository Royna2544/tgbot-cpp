#include <tgbot/TgTypeParser.h>
#include <tgbot/types/PollMedia.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(PollMedia) {
    auto result = std::make_shared<PollMedia>();
    result->animation = parse<Animation>(data, "animation");
    result->audio = parse<Audio>(data, "audio");
    result->document = parse<Document>(data, "document");
    result->link = parse<Link>(data, "link");
    result->livePhoto = parse<LivePhoto>(data, "live_photo");
    result->location = parse<Location>(data, "location");
    result->photo = parseArray<PhotoSize>(data, "photo");
    result->sticker = parse<Sticker>(data, "sticker");
    result->venue = parse<Venue>(data, "venue");
    result->video = parse<Video>(data, "video");
    return result;
}

DECLARE_PARSER_TO_JSON(PollMedia) {
    JsonWrapper json;
    if (object) {
        json.put("animation", object->animation);
        json.put("audio", object->audio);
        json.put("document", object->document);
        json.put("link", object->link);
        json.put("live_photo", object->livePhoto);
        json.put("location", object->location);
        json.put("photo", object->photo);
        json.put("sticker", object->sticker);
        json.put("venue", object->venue);
        json.put("video", object->video);
    }
    return json;
}

} // namespace TgBot
