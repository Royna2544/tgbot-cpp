#ifndef TGBOT_PAIDMEDIA_H
#define TGBOT_PAIDMEDIA_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes paid media. Currently, it can be one of - PaidMediaPreview - PaidMediaPhoto - PaidMediaVideo
 *
 * @ingroup types
 */
class PaidMedia {
public:
    using Ptr = std::shared_ptr<PaidMedia>;

    std::string type;

};
}

#endif //TGBOT_PAIDMEDIA_H
