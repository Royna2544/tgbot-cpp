#ifndef TGBOT_INPUTRICHBLOCKMATHEMATICALEXPRESSION_H
#define TGBOT_INPUTRICHBLOCKMATHEMATICALEXPRESSION_H

#include "tgbot/types/InputRichBlock.h"

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief A block with a mathematical expression in LaTeX format, corresponding to the custom HTML tag <tg-math-block>.
 *
 * @ingroup types
 */
class InputRichBlockMathematicalExpression : public InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlockMathematicalExpression>;

    constexpr static const char* TYPE = "mathematical_expression";

    InputRichBlockMathematicalExpression() {
        type = TYPE;
    }

    /**
     * @brief The mathematical expression in LaTeX format
     */
    std::string expression;

};
}

#endif //TGBOT_INPUTRICHBLOCKMATHEMATICALEXPRESSION_H
