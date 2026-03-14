#ifndef TGBOT_INPUTSTORYCONTENT_H
#define TGBOT_INPUTSTORYCONTENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the content of a story to post. Currently, it can be one of - InputStoryContentPhoto - InputStoryContentVideo
 *
 * @ingroup types
 */
class InputStoryContent {
public:
    using Ptr = std::shared_ptr<InputStoryContent>;

    /**
     * @brief Type of the content
     */
    std::string type;
};
}

#endif //TGBOT_INPUTSTORYCONTENT_H
