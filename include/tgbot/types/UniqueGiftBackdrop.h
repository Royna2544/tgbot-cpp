#ifndef TGBOT_UNIQUEGIFTBACKDROP_H
#define TGBOT_UNIQUEGIFTBACKDROP_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/UniqueGiftBackdropColors.h"

namespace TgBot {

/**
 * @brief This object describes the backdrop of a unique gift.
 *
 * @ingroup types
 */
class UniqueGiftBackdrop {
public:
    using Ptr = std::shared_ptr<UniqueGiftBackdrop>;

    /**
     * @brief Name of the backdrop
     */
    std::string name;

    /**
     * @brief Colors of the backdrop
     */
    UniqueGiftBackdropColors::Ptr colors;

    /**
     * @brief The number of unique gifts that receive this backdrop for every 1000 gifts upgraded
     */
    std::int64_t rarityPerMille;

};
}

#endif //TGBOT_UNIQUEGIFTBACKDROP_H
