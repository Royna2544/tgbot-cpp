#include <tgbot/TgTypeParser.h>
#include <tgbot/types/InputRichBlock.h>
#include <tgbot/types/InputRichBlockAnchor.h>
#include <tgbot/types/InputRichBlockAnimation.h>
#include <tgbot/types/InputRichBlockAudio.h>
#include <tgbot/types/InputRichBlockBlockQuotation.h>
#include <tgbot/types/InputRichBlockCollage.h>
#include <tgbot/types/InputRichBlockDetails.h>
#include <tgbot/types/InputRichBlockDivider.h>
#include <tgbot/types/InputRichBlockFooter.h>
#include <tgbot/types/InputRichBlockList.h>
#include <tgbot/types/InputRichBlockMap.h>
#include <tgbot/types/InputRichBlockMathematicalExpression.h>
#include <tgbot/types/InputRichBlockParagraph.h>
#include <tgbot/types/InputRichBlockPhoto.h>
#include <tgbot/types/InputRichBlockPreformatted.h>
#include <tgbot/types/InputRichBlockPullQuotation.h>
#include <tgbot/types/InputRichBlockSectionHeading.h>
#include <tgbot/types/InputRichBlockSlideshow.h>
#include <tgbot/types/InputRichBlockTable.h>
#include <tgbot/types/InputRichBlockThinking.h>
#include <tgbot/types/InputRichBlockVideo.h>
#include <tgbot/types/InputRichBlockVoiceNote.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<InputRichBlock> parse(const nlohmann::json &data) {
    std::string type;
    InputRichBlock::Ptr result;
    parse(data, "type", &type);
    if (type == "paragraph") {
        result = parse<InputRichBlockParagraph>(data);
    } else if (type == "heading") {
        result = parse<InputRichBlockSectionHeading>(data);
    } else if (type == "pre") {
        result = parse<InputRichBlockPreformatted>(data);
    } else if (type == "footer") {
        result = parse<InputRichBlockFooter>(data);
    } else if (type == "divider") {
        result = parse<InputRichBlockDivider>(data);
    } else if (type == "mathematical_expression") {
        result = parse<InputRichBlockMathematicalExpression>(data);
    } else if (type == "anchor") {
        result = parse<InputRichBlockAnchor>(data);
    } else if (type == "list") {
        result = parse<InputRichBlockList>(data);
    } else if (type == "blockquote") {
        result = parse<InputRichBlockBlockQuotation>(data);
    } else if (type == "pullquote") {
        result = parse<InputRichBlockPullQuotation>(data);
    } else if (type == "collage") {
        result = parse<InputRichBlockCollage>(data);
    } else if (type == "slideshow") {
        result = parse<InputRichBlockSlideshow>(data);
    } else if (type == "table") {
        result = parse<InputRichBlockTable>(data);
    } else if (type == "details") {
        result = parse<InputRichBlockDetails>(data);
    } else if (type == "map") {
        result = parse<InputRichBlockMap>(data);
    } else if (type == "animation") {
        result = parse<InputRichBlockAnimation>(data);
    } else if (type == "audio") {
        result = parse<InputRichBlockAudio>(data);
    } else if (type == "photo") {
        result = parse<InputRichBlockPhoto>(data);
    } else if (type == "video") {
        result = parse<InputRichBlockVideo>(data);
    } else if (type == "voice_note") {
        result = parse<InputRichBlockVoiceNote>(data);
    } else if (type == "thinking") {
        result = parse<InputRichBlockThinking>(data);
    } else {
        throw invalidType("InputRichBlock", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<InputRichBlock> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "paragraph") {
            json += put<InputRichBlockParagraph>(object);
        } else if (object->type == "heading") {
            json += put<InputRichBlockSectionHeading>(object);
        } else if (object->type == "pre") {
            json += put<InputRichBlockPreformatted>(object);
        } else if (object->type == "footer") {
            json += put<InputRichBlockFooter>(object);
        } else if (object->type == "divider") {
            json += put<InputRichBlockDivider>(object);
        } else if (object->type == "mathematical_expression") {
            json += put<InputRichBlockMathematicalExpression>(object);
        } else if (object->type == "anchor") {
            json += put<InputRichBlockAnchor>(object);
        } else if (object->type == "list") {
            json += put<InputRichBlockList>(object);
        } else if (object->type == "blockquote") {
            json += put<InputRichBlockBlockQuotation>(object);
        } else if (object->type == "pullquote") {
            json += put<InputRichBlockPullQuotation>(object);
        } else if (object->type == "collage") {
            json += put<InputRichBlockCollage>(object);
        } else if (object->type == "slideshow") {
            json += put<InputRichBlockSlideshow>(object);
        } else if (object->type == "table") {
            json += put<InputRichBlockTable>(object);
        } else if (object->type == "details") {
            json += put<InputRichBlockDetails>(object);
        } else if (object->type == "map") {
            json += put<InputRichBlockMap>(object);
        } else if (object->type == "animation") {
            json += put<InputRichBlockAnimation>(object);
        } else if (object->type == "audio") {
            json += put<InputRichBlockAudio>(object);
        } else if (object->type == "photo") {
            json += put<InputRichBlockPhoto>(object);
        } else if (object->type == "video") {
            json += put<InputRichBlockVideo>(object);
        } else if (object->type == "voice_note") {
            json += put<InputRichBlockVoiceNote>(object);
        } else if (object->type == "thinking") {
            json += put<InputRichBlockThinking>(object);
        } else {
            throw invalidType("InputRichBlock", object->type);
        }
    }
    return json;
}

} // namespace TgBot
