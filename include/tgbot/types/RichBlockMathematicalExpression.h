#ifndef TGBOT_RICHBLOCKMATHEMATICALEXPRESSION_H
#define TGBOT_RICHBLOCKMATHEMATICALEXPRESSION_H

#include "tgbot/types/RichBlock.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A block with a mathematical expression in LaTeX format, corresponding to the custom HTML tag <tg-math-block>.
 *
 * @ingroup types
 */
class RichBlockMathematicalExpression : public RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlockMathematicalExpression>;

    constexpr static const char* TYPE = "mathematical_expression";

    RichBlockMathematicalExpression() {
        type = TYPE;
    }

    /**
     * @brief The mathematical expression in LaTeX format
     */
    std::string expression;

};
}

#endif //TGBOT_RICHBLOCKMATHEMATICALEXPRESSION_H
