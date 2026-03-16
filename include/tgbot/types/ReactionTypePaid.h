#ifndef TGBOT_REACTIONTYPEPAID_H
#define TGBOT_REACTIONTYPEPAID_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/ReactionType.h"

namespace TgBot {

/**
 * @brief The reaction is paid.
 *
 * @ingroup types
 */
class ReactionTypePaid : public ReactionType {
public:
    using Ptr = std::shared_ptr<ReactionTypePaid>;
    constexpr static const char* TYPE = "paid";

    ReactionTypePaid() {
        type = TYPE;
    }
};
}

#endif //TGBOT_REACTIONTYPEPAID_H
