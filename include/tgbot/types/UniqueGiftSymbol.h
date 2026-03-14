#ifndef TGBOT_UNIQUEGIFTSYMBOL_H
#define TGBOT_UNIQUEGIFTSYMBOL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Sticker.h"

namespace TgBot {

/**
 * @brief This object describes the symbol shown on the pattern of a unique gift.
 *
 * @ingroup types
 */
class UniqueGiftSymbol {
public:
    using Ptr = std::shared_ptr<UniqueGiftSymbol>;

    /**
     * @brief Name of the symbol
     */
    std::string name;

    /**
     * @brief The sticker that represents the unique gift
     */
    Sticker::Ptr sticker;

    /**
     * @brief The number of unique gifts that receive this model for every 1000 gifts upgraded
     */
    std::int64_t rarityPerMille;

};
}

#endif //TGBOT_UNIQUEGIFTSYMBOL_H
