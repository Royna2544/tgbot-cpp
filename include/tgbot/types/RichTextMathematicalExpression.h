#ifndef TGBOT_RICHTEXTMATHEMATICALEXPRESSION_H
#define TGBOT_RICHTEXTMATHEMATICALEXPRESSION_H

#include "tgbot/types/RichText.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A mathematical expression.
 *
 * @ingroup types
 */
class RichTextMathematicalExpression : public RichText {
public:
    using Ptr = std::shared_ptr<RichTextMathematicalExpression>;

    constexpr static const char* TYPE = "mathematical_expression";

    RichTextMathematicalExpression() {
        type = TYPE;
    }

    /**
     * @brief The expression in LaTeX format
     */
    std::string expression;

};
}

#endif //TGBOT_RICHTEXTMATHEMATICALEXPRESSION_H
