#ifndef TGBOT_RICHBLOCKPREFORMATTED_H
#define TGBOT_RICHBLOCKPREFORMATTED_H

#include "tgbot/types/RichBlock.h"
#include "tgbot/types/RichText.h"

#include <memory>
#include <optional>
#include <string>

namespace TgBot {

/**
 * @brief A preformatted text block, corresponding to the nested HTML tags <pre> and <code>.
 *
 * @ingroup types
 */
class RichBlockPreformatted : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockPreformatted>;

    constexpr static const char* TYPE = "pre";

    RichBlockPreformatted() {
        type = TYPE;
    }

    /**
     * @brief Text of the block
     */
    RichText::Ptr text;

    /**
     * @brief Optional. The programming language of the text
     */
    std::optional<std::string> language;

};
}

#endif //TGBOT_RICHBLOCKPREFORMATTED_H
