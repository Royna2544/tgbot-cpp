#ifndef TGBOT_CPP_GENERICREPLY_H
#define TGBOT_CPP_GENERICREPLY_H

#include <memory>

namespace TgBot {

/**
 * @brief This abstract class is base of all keyboard related events.
 * @ingroup types
 */
class GenericReply {

public:
    using Ptr = std::shared_ptr<GenericReply>;

    virtual ~GenericReply() = default;
};

}

#endif //TGBOT_CPP_GENERICREPLY_H
