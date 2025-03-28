#ifndef TGBOT_REACTIONTYPEEMOJI_H
#define TGBOT_REACTIONTYPEEMOJI_H

#include "tgbot/types/ReactionType.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief The reaction is based on an emoji.
 *
 * @ingroup types
 */
class ReactionTypeEmoji : public ReactionType {

public:
    static constexpr std::string_view TYPE = "emoji";

    using Ptr = std::shared_ptr<ReactionTypeEmoji>;

    ReactionTypeEmoji() {
        this->type = TYPE;
    }

    /**
     * @brief Reaction emoji.
     *
     * Currently, it can be one of "👍", "👎", "❤", "🔥", "🥰", "👏", "😁", "🤔", "🤯", "😱", "🤬", "😢", "🎉", "🤩", "🤮", "💩", "🙏", "👌", "🕊", "🤡", "🥱", "🥴", "😍", "🐳", "❤‍🔥", "🌚", "🌭", "💯", "🤣", "⚡", "🍌", "🏆", "💔", "🤨", "😐", "🍓", "🍾", "💋", "🖕", "😈", "😴", "😭", "🤓", "👻", "👨‍💻", "👀", "🎃", "🙈", "😇", "😨", "🤝", "✍", "🤗", "🫡", "🎅", "🎄", "☃", "💅", "🤪", "🗿", "🆒", "💘", "🙉", "🦄", "😘", "💊", "🙊", "😎", "👾", "🤷‍♂", "🤷", "🤷‍♀", "😡"
     * 
     * See https://core.telegram.org/bots/api#reactiontypeemoji
     */
    std::string emoji;
};
}

#endif //TGBOT_REACTIONTYPEEMOJI_H
