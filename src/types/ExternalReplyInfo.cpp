#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(ExternalReplyInfo) {
    auto result = std::make_shared<ExternalReplyInfo>();
    result->origin = parseRequired<MessageOrigin>(data, "origin");
    result->chat = parse<Chat>(data, "chat");
    parse(data, "message_id", &result->messageId);
    result->linkPreviewOptions = parse<LinkPreviewOptions>(data, "link_preview_options");
    result->animation = parse<Animation>(data, "animation");
    result->audio = parse<Audio>(data, "audio");
    result->document = parse<Document>(data, "document");
    result->paidMedia = parse<PaidMediaInfo>(data, "paid_media");
    result->photo = parseArray<PhotoSize>(data, "photo");
    result->sticker = parse<Sticker>(data, "sticker");
    result->story = parse<Story>(data, "story");
    result->video = parse<Video>(data, "video");
    result->videoNote = parse<VideoNote>(data, "video_note");
    result->voice = parse<Voice>(data, "voice");
    parse(data, "has_media_spoiler", &result->hasMediaSpoiler);
    result->checklist = parse<Checklist>(data, "checklist");
    result->contact = parse<Contact>(data, "contact");
    result->dice = parse<Dice>(data, "dice");
    result->game = parse<Game>(data, "game");
    result->giveaway = parse<Giveaway>(data, "giveaway");
    result->giveawayWinners = parse<GiveawayWinners>(data, "giveaway_winners");
    result->invoice = parse<Invoice>(data, "invoice");
    result->location = parse<Location>(data, "location");
    result->poll = parse<Poll>(data, "poll");
    result->venue = parse<Venue>(data, "venue");
    return result;
}

DECLARE_PARSER_TO_JSON(ExternalReplyInfo) {
    JsonWrapper json;
    if (object) {
        json.put("origin", object->origin);
        json.put("chat", object->chat);
        json.put("message_id", object->messageId);
        json.put("link_preview_options", object->linkPreviewOptions);
        json.put("animation", object->animation);
        json.put("audio", object->audio);
        json.put("document", object->document);
        json.put("paid_media", object->paidMedia);
        json.put("photo", object->photo);
        json.put("sticker", object->sticker);
        json.put("story", object->story);
        json.put("video", object->video);
        json.put("video_note", object->videoNote);
        json.put("voice", object->voice);
        json.put("has_media_spoiler", object->hasMediaSpoiler);
        json.put("checklist", object->checklist);
        json.put("contact", object->contact);
        json.put("dice", object->dice);
        json.put("game", object->game);
        json.put("giveaway", object->giveaway);
        json.put("giveaway_winners", object->giveawayWinners);
        json.put("invoice", object->invoice);
        json.put("location", object->location);
        json.put("poll", object->poll);
        json.put("venue", object->venue);
    }
    return json;
}

} // namespace TgBot
