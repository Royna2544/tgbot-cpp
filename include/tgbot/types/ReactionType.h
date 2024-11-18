#ifndef TGBOT_REACTIONTYPE_H
#define TGBOT_REACTIONTYPE_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object describes the type of a reaction.
 *
 * Currently, it can be one of
 * - ReactionTypeEmoji
 * - ReactionTypeCustomEmoji
 *
 * @ingroup types
 */
class ReactionType {

public:
    using Ptr = std::shared_ptr<ReactionType>;

    ReactionType() = default;
    virtual ~ReactionType() = default;

    /**
     * @brief Type of the reaction
     */
    std::string type;
};
}

#endif //TGBOT_REACTIONTYPE_H
