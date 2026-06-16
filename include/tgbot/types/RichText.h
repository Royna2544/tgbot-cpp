#ifndef TGBOT_RICHTEXT_H
#define TGBOT_RICHTEXT_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object describes a rich text. It is recursive: a RichText is one
 * of a plain string (RichTextString), a sequence of RichText (RichTextArray),
 * or a styled node such as RichTextBold whose contents are themselves RichText.
 *
 * Inspect kind to tell the three forms apart; for Kind::Object use type to tell
 * the concrete styled node and downcast accordingly.
 *
 * @ingroup types
 */
class RichText {
public:
    using Ptr = std::shared_ptr<RichText>;

    virtual ~RichText() = default;

    /**
     * @brief Which of the three RichText forms this value takes.
     */
    enum class Kind { String, Array, Object };

    /**
     * @brief The form this RichText takes.
     */
    Kind kind = Kind::Object;

    /**
     * @brief For Kind::Object: the styled node type, e.g. "bold", "italic", "url".
     */
    std::string type;
};
}

#endif //TGBOT_RICHTEXT_H
