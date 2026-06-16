#include <tgbot/TgTypeParser.h>
#include <tgbot/types/RichBlock.h>
#include <tgbot/types/RichBlockAnchor.h>
#include <tgbot/types/RichBlockAnimation.h>
#include <tgbot/types/RichBlockAudio.h>
#include <tgbot/types/RichBlockBlockQuotation.h>
#include <tgbot/types/RichBlockCollage.h>
#include <tgbot/types/RichBlockDetails.h>
#include <tgbot/types/RichBlockDivider.h>
#include <tgbot/types/RichBlockFooter.h>
#include <tgbot/types/RichBlockList.h>
#include <tgbot/types/RichBlockMap.h>
#include <tgbot/types/RichBlockMathematicalExpression.h>
#include <tgbot/types/RichBlockParagraph.h>
#include <tgbot/types/RichBlockPhoto.h>
#include <tgbot/types/RichBlockPreformatted.h>
#include <tgbot/types/RichBlockPullQuotation.h>
#include <tgbot/types/RichBlockSectionHeading.h>
#include <tgbot/types/RichBlockSlideshow.h>
#include <tgbot/types/RichBlockTable.h>
#include <tgbot/types/RichBlockThinking.h>
#include <tgbot/types/RichBlockVideo.h>
#include <tgbot/types/RichBlockVoiceNote.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(RichBlock) {
    std::string type;
    RichBlock::Ptr result;
    parse(data, "type", &type);
    if (type == "paragraph") {
        result = parse<RichBlockParagraph>(data);
    } else if (type == "heading") {
        result = parse<RichBlockSectionHeading>(data);
    } else if (type == "pre") {
        result = parse<RichBlockPreformatted>(data);
    } else if (type == "footer") {
        result = parse<RichBlockFooter>(data);
    } else if (type == "divider") {
        result = parse<RichBlockDivider>(data);
    } else if (type == "mathematical_expression") {
        result = parse<RichBlockMathematicalExpression>(data);
    } else if (type == "anchor") {
        result = parse<RichBlockAnchor>(data);
    } else if (type == "list") {
        result = parse<RichBlockList>(data);
    } else if (type == "blockquote") {
        result = parse<RichBlockBlockQuotation>(data);
    } else if (type == "pullquote") {
        result = parse<RichBlockPullQuotation>(data);
    } else if (type == "collage") {
        result = parse<RichBlockCollage>(data);
    } else if (type == "slideshow") {
        result = parse<RichBlockSlideshow>(data);
    } else if (type == "table") {
        result = parse<RichBlockTable>(data);
    } else if (type == "details") {
        result = parse<RichBlockDetails>(data);
    } else if (type == "map") {
        result = parse<RichBlockMap>(data);
    } else if (type == "animation") {
        result = parse<RichBlockAnimation>(data);
    } else if (type == "audio") {
        result = parse<RichBlockAudio>(data);
    } else if (type == "photo") {
        result = parse<RichBlockPhoto>(data);
    } else if (type == "video") {
        result = parse<RichBlockVideo>(data);
    } else if (type == "voice_note") {
        result = parse<RichBlockVoiceNote>(data);
    } else if (type == "thinking") {
        result = parse<RichBlockThinking>(data);
    } else {
        throw invalidType("RichBlock", type);
    }

    return result;
}

DECLARE_PARSER_TO_JSON(RichBlock) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "paragraph") {
            json += put<RichBlockParagraph>(object);
        } else if (object->type == "heading") {
            json += put<RichBlockSectionHeading>(object);
        } else if (object->type == "pre") {
            json += put<RichBlockPreformatted>(object);
        } else if (object->type == "footer") {
            json += put<RichBlockFooter>(object);
        } else if (object->type == "divider") {
            json += put<RichBlockDivider>(object);
        } else if (object->type == "mathematical_expression") {
            json += put<RichBlockMathematicalExpression>(object);
        } else if (object->type == "anchor") {
            json += put<RichBlockAnchor>(object);
        } else if (object->type == "list") {
            json += put<RichBlockList>(object);
        } else if (object->type == "blockquote") {
            json += put<RichBlockBlockQuotation>(object);
        } else if (object->type == "pullquote") {
            json += put<RichBlockPullQuotation>(object);
        } else if (object->type == "collage") {
            json += put<RichBlockCollage>(object);
        } else if (object->type == "slideshow") {
            json += put<RichBlockSlideshow>(object);
        } else if (object->type == "table") {
            json += put<RichBlockTable>(object);
        } else if (object->type == "details") {
            json += put<RichBlockDetails>(object);
        } else if (object->type == "map") {
            json += put<RichBlockMap>(object);
        } else if (object->type == "animation") {
            json += put<RichBlockAnimation>(object);
        } else if (object->type == "audio") {
            json += put<RichBlockAudio>(object);
        } else if (object->type == "photo") {
            json += put<RichBlockPhoto>(object);
        } else if (object->type == "video") {
            json += put<RichBlockVideo>(object);
        } else if (object->type == "voice_note") {
            json += put<RichBlockVoiceNote>(object);
        } else if (object->type == "thinking") {
            json += put<RichBlockThinking>(object);
        } else {
            throw invalidType("RichBlock", object->type);
        }
    }
    return json;
}

} // namespace TgBot
