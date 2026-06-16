#ifndef TGBOT_RICHTEXTHASHTAG_H
#define TGBOT_RICHTEXTHASHTAG_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A hashtag.
 *
 * @ingroup types
 */
class RichTextHashtag : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextHashtag>;

    constexpr static const char* TYPE = "hashtag";

    RichTextHashtag() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The hashtag
     */
    std::string hashtag;

};
}

#endif //TGBOT_RICHTEXTHASHTAG_H
