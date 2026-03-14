#ifndef TGBOT_UNIQUEGIFTMODEL_H
#define TGBOT_UNIQUEGIFTMODEL_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the model of a unique gift.
 *
 * @ingroup types
 */
class UniqueGiftModel {
public:
    using Ptr = std::shared_ptr<UniqueGiftModel>;

    /**
     * @brief Name of the model
     */
    std::string name;

    /**
     * @brief The sticker that represents the unique gift
     */
    Sticker::Ptr sticker;

    /**
     * @brief The number of unique gifts that receive this model for every 1000 gift upgrades. Always 0 for crafted gifts.
     */
    std::int64_t rarityPerMille;

    /**
     * @brief Optional. Rarity of the model if it is a crafted model. Currently, can be "uncommon", "rare", "epic", or "legendary".
     */
    std::optional<std::string> rarity;

};
}

#endif //TGBOT_UNIQUEGIFTMODEL_H
