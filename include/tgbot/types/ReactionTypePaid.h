#ifndef TGBOT_REACTIONTYPEPAID_H
#define TGBOT_REACTIONTYPEPAID_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief The reaction is paid.
 *
 * @ingroup types
 */
class ReactionTypePaid {
public:
    using Ptr = std::shared_ptr<ReactionTypePaid>;

    /**
     * @brief Type of the reaction, always "paid"
     */
    std::string type;

};
}

#endif //TGBOT_REACTIONTYPEPAID_H
