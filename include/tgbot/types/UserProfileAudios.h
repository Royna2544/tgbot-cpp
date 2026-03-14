#ifndef TGBOT_USERPROFILEAUDIOS_H
#define TGBOT_USERPROFILEAUDIOS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/Audio.h"

namespace TgBot {

/**
 * @brief This object represents the audios displayed on a user's profile.
 *
 * @ingroup types
 */
class UserProfileAudios {
public:
    using Ptr = std::shared_ptr<UserProfileAudios>;

    /**
     * @brief Total number of profile audios for the target user
     */
    std::int64_t totalCount;

    /**
     * @brief Requested profile audios
     */
    std::vector<Audio::Ptr> audios;

};
}

#endif //TGBOT_USERPROFILEAUDIOS_H
