#ifndef TGBOT_INLINEQUERYRESULTARTICLE_H
#define TGBOT_INLINEQUERYRESULTARTICLE_H

#include "tgbot/types/InlineQueryResult.h"
#include "tgbot/types/InputMessageContent.h"

#include <cstdint>
#include <string>
#include <memory>
#include <string_view>

namespace TgBot {

/**
 * @brief Represents a link to an article of web page.
 *
 * @ingroup types
 */
class InlineQueryResultArticle : public InlineQueryResult {
public:
    static constexpr std::string_view TYPE = "article";

    using Ptr = std::shared_ptr<InlineQueryResultArticle>;

    InlineQueryResultArticle() {
        this->type = TYPE;
    }

    /**
     * @brief Title of the result
     */
    std::string title;

    /**
     * @brief Content of the message to be sent
     */
    InputMessageContent::Ptr inputMessageContent;

    /**
     * @brief Optional. URL of the result
     */
    std::optional<std::string> url;

    /**
     * @brief Optional. Pass True if you don't want the URL to be shown in the message.
     */
    std::optional<bool> hideUrl;

    /**
     * @brief Optional. Short description of the result
     */
    std::optional<std::string> description;

    /**
     * @brief Optional. Url of the thumbnail for the result
     */
    std::optional<std::string> thumbnailUrl;

    /**
     * @brief Optional. Thumbnail width
     */
    std::optional<std::int32_t> thumbnailWidth;

    /**
     * @brief Optional. Thumbnail height
     */
    std::optional<std::int32_t> thumbnailHeight;
};
}

#endif //TGBOT_INLINEQUERYRESULTARTICLE_H
