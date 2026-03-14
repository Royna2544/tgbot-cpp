#ifndef TGBOT_LOCATIONADDRESS_H
#define TGBOT_LOCATIONADDRESS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes the physical address of a location.
 *
 * @ingroup types
 */
class LocationAddress {
public:
    using Ptr = std::shared_ptr<LocationAddress>;

    /**
     * @brief The two-letter ISO 3166-1 alpha-2 country code of the country where the location is located
     */
    std::string countryCode;

    /**
     * @brief Optional. State of the location
     */
    std::optional<std::string> state;

    /**
     * @brief Optional. City of the location
     */
    std::optional<std::string> city;

    /**
     * @brief Optional. Street address of the location
     */
    std::optional<std::string> street;

};
}

#endif //TGBOT_LOCATIONADDRESS_H
