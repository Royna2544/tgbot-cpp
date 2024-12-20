#ifndef TGBOT_INLINEQUERYRESULTGAME_H
#define TGBOT_INLINEQUERYRESULTGAME_H

#include "tgbot/types/InlineQueryResult.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents a Game.
 *
 * @ingroup types
 */
class InlineQueryResultGame : public InlineQueryResult {

public:
    static constexpr std::string_view TYPE = "game";

    using Ptr = std::shared_ptr<InlineQueryResultGame>;

    InlineQueryResultGame() {
        this->type = TYPE;
    }

    /**
     * @brief Short name of the game
     */
    std::string gameShortName;
};
}

#endif //TGBOT_INLINEQUERYRESULTGAME_H
