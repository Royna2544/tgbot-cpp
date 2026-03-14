#ifndef TGBOT_OWNEDGIFT_H
#define TGBOT_OWNEDGIFT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes a gift received and owned by a user or a chat. Currently, it can be one of - OwnedGiftRegular - OwnedGiftUnique
 *
 * @ingroup types
 */
class OwnedGift {
public:
    using Ptr = std::shared_ptr<OwnedGift>;

    std::string type;

};
}

#endif //TGBOT_OWNEDGIFT_H
