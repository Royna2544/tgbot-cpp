#ifndef TGBOT_LINK_H
#define TGBOT_LINK_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief Represents an HTTP link.
 *
 * @ingroup types
 */
class Link {
public:
    using Ptr = std::shared_ptr<Link>;

    /**
     * @brief URL of the link
     */
    std::string url;

};
}

#endif //TGBOT_LINK_H
