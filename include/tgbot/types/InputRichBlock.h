#ifndef TGBOT_INPUTRICHBLOCK_H
#define TGBOT_INPUTRICHBLOCK_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object represents a block in a rich formatted message to be sent. Currently, it can be any of the following types: - InputRichBlockParagraph - InputRichBlockSectionHeading - InputRichBlockPreformatted - InputRichBlockFooter - InputRichBlockDivider - InputRichBlockMathematicalExpression - InputRichBlockAnchor - InputRichBlockList - InputRichBlockBlockQuotation - InputRichBlockPullQuotation - InputRichBlockCollage - InputRichBlockSlideshow - InputRichBlockTable - InputRichBlockDetails - InputRichBlockMap - InputRichBlockAnimation - InputRichBlockAudio - InputRichBlockPhoto - InputRichBlockVideo - InputRichBlockVoiceNote - InputRichBlockThinking
 *
 * @ingroup types
 */
class InputRichBlock {
public:
    using Ptr = std::shared_ptr<InputRichBlock>;

    std::string type;

};
}

#endif //TGBOT_INPUTRICHBLOCK_H
