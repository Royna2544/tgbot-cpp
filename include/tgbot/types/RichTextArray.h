#ifndef TGBOT_RICHTEXTARRAY_H
#define TGBOT_RICHTEXTARRAY_H

#include <memory>
#include <vector>

#include "tgbot/types/RichText.h"

namespace TgBot {

/**
 * @brief A sequence of RichText (the "Array of RichText" form of RichText).
 *
 * @ingroup types
 */
class RichTextArray : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextArray>;

    RichTextArray() {
        kind = Kind::Array;
    }

    /**
     * @brief The contained rich-text items.
     */
    std::vector<RichText::Ptr> items;
};
}

#endif //TGBOT_RICHTEXTARRAY_H
