#ifndef TGBOT_INPUTPAIDMEDIA_H
#define TGBOT_INPUTPAIDMEDIA_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the paid media to be sent. Currently, it can be one of - InputPaidMediaPhoto - InputPaidMediaVideo
 *
 * @ingroup types
 */
class InputPaidMedia {
public:
    using Ptr = std::shared_ptr<InputPaidMedia>;

    std::string type;

};
}

#endif //TGBOT_INPUTPAIDMEDIA_H
