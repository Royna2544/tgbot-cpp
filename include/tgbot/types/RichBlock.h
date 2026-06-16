#ifndef TGBOT_RICHBLOCK_H
#define TGBOT_RICHBLOCK_H

#include <memory>
#include <string>

namespace TgBot {

/**
 * @brief This object represents a block in a rich formatted message. Currently, it can be any of the following types: - RichBlockParagraph - RichBlockSectionHeading - RichBlockPreformatted - RichBlockFooter - RichBlockDivider - RichBlockMathematicalExpression - RichBlockAnchor - RichBlockList - RichBlockBlockQuotation - RichBlockPullQuotation - RichBlockCollage - RichBlockSlideshow - RichBlockTable - RichBlockDetails - RichBlockMap - RichBlockAnimation - RichBlockAudio - RichBlockPhoto - RichBlockVideo - RichBlockVoiceNote - RichBlockThinking
 *
 * @ingroup types
 */
class RichBlock {
public:
    using Ptr = std::shared_ptr<RichBlock>;

    std::string type;

};
}

#endif //TGBOT_RICHBLOCK_H
