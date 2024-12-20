#ifndef TGBOT_SHIPPINGADDRESS_H
#define TGBOT_SHIPPINGADDRESS_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object represents a shipping address.
 *
 * @ingroup types
 */
class ShippingAddress {

public:
    using Ptr = std::shared_ptr<ShippingAddress>;

    /**
     * @brief Two-letter [ISO 3166-1 alpha-2](https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2) country code
     */
    std::string countryCode;

    /**
     * @brief State, if applicable
     */
    std::string state;

    /**
     * @brief City
     */
    std::string city;

    /**
     * @brief First line for the address
     */
    std::string streetLine1;

    /**
     * @brief Second line for the address
     */
    std::string streetLine2;

    /**
     * @brief Address post code
     */
    std::string postCode;
};
}

#endif //TGBOT_SHIPPINGADDRESS_H
