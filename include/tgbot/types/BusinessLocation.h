#ifndef TGBOT_BUSINESSLOCATION_H
#define TGBOT_BUSINESSLOCATION_H

#include "tgbot/types/Location.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @ingroup types
 */
class BusinessLocation {

public:
    using Ptr = std::shared_ptr<BusinessLocation>;

    /**
     * @brief Address of the business
     */
    std::string address;

    /**
     * @brief Optional. Location of the business
     */
    Location::Ptr location;
};
}

#endif //TGBOT_BUSINESSLOCATION_H
