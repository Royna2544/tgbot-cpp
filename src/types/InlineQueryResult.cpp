#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineQueryResult) {
    std::string type;
    InlineQueryResult::Ptr result;
    parse(data, "type", &type);
    if (type == "article") {
        result = parse<InlineQueryResultArticle>(data);
    } else if (type == "photo") {
        result = parse<InlineQueryResultPhoto>(data);
    } else if (type == "gif") {
        result = parse<InlineQueryResultGif>(data);
    } else if (type == "mpeg4_gif") {
        result = parse<InlineQueryResultMpeg4Gif>(data);
    } else if (type == "video") {
        result = parse<InlineQueryResultVideo>(data);
    } else if (type == "audio") {
        result = parse<InlineQueryResultAudio>(data);
    } else if (type == "voice") {
        result = parse<InlineQueryResultVoice>(data);
    } else if (type == "document") {
        result = parse<InlineQueryResultDocument>(data);
    } else if (type == "location") {
        result = parse<InlineQueryResultLocation>(data);
    } else if (type == "venue") {
        result = parse<InlineQueryResultVenue>(data);
    } else if (type == "contact") {
        result = parse<InlineQueryResultContact>(data);
    } else if (type == "game") {
        result = parse<InlineQueryResultGame>(data);
    } else {
        throw invalidType("InlineQueryResult", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(InlineQueryResult) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "article") {
            json += put<InlineQueryResultArticle>(object);
        } else if (object->type == "photo") {
            json += put<InlineQueryResultPhoto>(object);
        } else if (object->type == "gif") {
            json += put<InlineQueryResultGif>(object);
        } else if (object->type == "mpeg4_gif") {
            json += put<InlineQueryResultMpeg4Gif>(object);
        } else if (object->type == "video") {
            json += put<InlineQueryResultVideo>(object);
        } else if (object->type == "audio") {
            json += put<InlineQueryResultAudio>(object);
        } else if (object->type == "voice") {
            json += put<InlineQueryResultVoice>(object);
        } else if (object->type == "document") {
            json += put<InlineQueryResultDocument>(object);
        } else if (object->type == "location") {
            json += put<InlineQueryResultLocation>(object);
        } else if (object->type == "venue") {
            json += put<InlineQueryResultVenue>(object);
        } else if (object->type == "contact") {
            json += put<InlineQueryResultContact>(object);
        } else if (object->type == "game") {
            json += put<InlineQueryResultGame>(object);
        } else {
            throw invalidType("InlineQueryResult", object->type);
        }
    }
    return json;
}

} // namespace TgBot
