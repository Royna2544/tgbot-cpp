#ifndef TGBOT_RICHTEXTURL_H
#define TGBOT_RICHTEXTURL_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A text with a link.
 *
 * @ingroup types
 */
class RichTextUrl : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextUrl>;

    constexpr static const char* TYPE = "url";

    RichTextUrl() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief URL of the link
     */
    std::string url;

};
}

#endif //TGBOT_RICHTEXTURL_H
