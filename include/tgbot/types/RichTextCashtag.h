#ifndef TGBOT_RICHTEXTCASHTAG_H
#define TGBOT_RICHTEXTCASHTAG_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A cashtag.
 *
 * @ingroup types
 */
class RichTextCashtag : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextCashtag>;

    constexpr static const char* TYPE = "cashtag";

    RichTextCashtag() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The cashtag
     */
    std::string cashtag;

};
}

#endif //TGBOT_RICHTEXTCASHTAG_H
