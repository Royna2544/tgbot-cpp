#ifndef TGBOT_STORYAREATYPESUGGESTEDREACTION_H
#define TGBOT_STORYAREATYPESUGGESTEDREACTION_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/ReactionType.h"
#include "tgbot/types/StoryAreaType.h"

namespace TgBot {

/**
 * @brief Describes a story area pointing to a suggested reaction. Currently, a story can have up to 5 suggested reaction areas.
 *
 * @ingroup types
 */
class StoryAreaTypeSuggestedReaction : public StoryAreaType {
public:
    using Ptr = std::shared_ptr<StoryAreaTypeSuggestedReaction>;
    constexpr static const char* TYPE = "suggested_reaction";

    StoryAreaTypeSuggestedReaction() {
        type = TYPE;
    }

    /**
     * @brief Type of the reaction
     */
    ReactionType::Ptr reactionType;

    /**
     * @brief Optional. Pass True if the reaction area has a dark background
     */
    std::optional<bool> isDark;

    /**
     * @brief Optional. Pass True if reaction area corner is flipped
     */
    std::optional<bool> isFlipped;

};
}

#endif //TGBOT_STORYAREATYPESUGGESTEDREACTION_H
