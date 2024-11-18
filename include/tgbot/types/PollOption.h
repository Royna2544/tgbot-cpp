#ifndef TGBOT_POLLOPTION_H
#define TGBOT_POLLOPTION_H

#include <cstdint>
#include <memory>
#include <string>

namespace TgBot {

class PollOption {

public:
    using Ptr = std::shared_ptr<PollOption>;

    /**
     * @brief Option text, 1-100 characters.
     */
    std::string text;

    /**
     * @brief Number of users that voted for this option.
     */
    std::int64_t voterCount;
};

}

#endif //TGBOT_POLLOPTION_H
