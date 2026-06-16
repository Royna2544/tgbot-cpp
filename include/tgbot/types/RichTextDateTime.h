#ifndef TGBOT_RICHTEXTDATETIME_H
#define TGBOT_RICHTEXTDATETIME_H

#include "tgbot/types/RichText.h"

#include <cstdint>
#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Formatted date and time.
 *
 * @ingroup types
 */
class RichTextDateTime : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextDateTime>;

    constexpr static const char* TYPE = "date_time";

    RichTextDateTime() {
        type = TYPE;
    }

    /**
     * @brief The text
     */
    RichText::Ptr text;

    /**
     * @brief The Unix time associated with the entity
     */
    std::int32_t unixTime;

    /**
     * @brief The string that defines the formatting of the date and time. See date-time entity formatting for more details.
     */
    std::string dateTimeFormat;

};
}

#endif //TGBOT_RICHTEXTDATETIME_H
