#ifndef TGBOT_INPUTPROFILEPHOTO_H
#define TGBOT_INPUTPROFILEPHOTO_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes a profile photo to set. Currently, it can be one of - InputProfilePhotoStatic - InputProfilePhotoAnimated
 *
 * @ingroup types
 */
class InputProfilePhoto {
public:
    using Ptr = std::shared_ptr<InputProfilePhoto>;

    std::string type;

};
}

#endif //TGBOT_INPUTPROFILEPHOTO_H
