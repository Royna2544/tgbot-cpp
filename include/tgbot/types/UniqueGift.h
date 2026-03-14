#ifndef TGBOT_UNIQUEGIFT_H
#define TGBOT_UNIQUEGIFT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Chat.h"
#include "tgbot/types/UniqueGiftBackdrop.h"
#include "tgbot/types/UniqueGiftColors.h"
#include "tgbot/types/UniqueGiftModel.h"
#include "tgbot/types/UniqueGiftSymbol.h"

namespace TgBot {

/**
 * @brief This object describes a unique gift that was upgraded from a regular gift.
 *
 * @ingroup types
 */
class UniqueGift {
public:
    using Ptr = std::shared_ptr<UniqueGift>;

    /**
     * @brief Identifier of the regular gift from which the gift was upgraded
     */
    std::string giftId;

    /**
     * @brief Human-readable name of the regular gift from which this unique gift was upgraded
     */
    std::string baseName;

    /**
     * @brief Unique name of the gift. This name can be used in https://t.me/nft/... links and story areas
     */
    std::string name;

    /**
     * @brief Unique number of the upgraded gift among gifts upgraded from the same regular gift
     */
    std::int64_t number;

    /**
     * @brief Model of the gift
     */
    UniqueGiftModel::Ptr model;

    /**
     * @brief Symbol of the gift
     */
    UniqueGiftSymbol::Ptr symbol;

    /**
     * @brief Backdrop of the gift
     */
    UniqueGiftBackdrop::Ptr backdrop;

    /**
     * @brief Optional. True, if the original regular gift was exclusively purchaseable by Telegram Premium subscribers
     */
    std::optional<bool> isPremium;

    /**
     * @brief Optional. True, if the gift was used to craft another gift and isn't available anymore
     */
    std::optional<bool> isBurned;

    /**
     * @brief Optional. True, if the gift is assigned from the TON blockchain and can't be resold or transferred in Telegram
     */
    std::optional<bool> isFromBlockchain;

    /**
     * @brief Optional. The color scheme that can be used by the gift's owner for the chat's name, replies to messages and link previews; for business account gifts and gifts that are currently on sale only
     */
    std::optional<UniqueGiftColors::Ptr> colors;

    /**
     * @brief Optional. Information about the chat that published the gift
     */
    std::optional<Chat::Ptr> publisherChat;

};
}

#endif //TGBOT_UNIQUEGIFT_H
