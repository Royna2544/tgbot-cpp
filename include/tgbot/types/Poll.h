#ifndef TGBOT_POLL_H
#define TGBOT_POLL_H

#include "tgbot/types/PollOption.h"
#include "tgbot/types/PollMedia.h"
#include "tgbot/types/MessageEntity.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object contains information about a poll.
 *
 * @ingroup types
 */
class Poll {

public:
    using Ptr = std::shared_ptr<Poll>;

    /**
     * @brief Unique poll identifier
     */
    std::string id;

    /**
     * @brief Poll question, 1-255 characters
     */
    std::string question;

    /**
     * @brief List of poll options
     */
    std::vector<PollOption::Ptr> options;

    /**
     * @brief Total number of users that voted in the poll
     */
    std::int32_t totalVoterCount;

    /**
     * @brief True, if the poll is closed
     */
    bool isClosed;

    /**
     * @brief True, if the poll is anonymous
     */
    bool isAnonymous;

    /**
     * @brief Poll type, currently can be “regular” or “quiz”
     */
    std::string type;

    /**
     * @brief True, if the poll allows multiple answers
     */
    bool allowsMultipleAnswers;

    /**
     * @brief Optional. 0-based identifier of the correct answer option.
     * Available only for polls in the quiz mode, which are closed, or was sent (not forwarded) by the bot or to the private chat with the bot.
     */
    std::optional<std::int32_t> correctOptionId;

    /**
     * @brief Optional. Text that is shown when a user chooses an incorrect answer or taps on the lamp icon in a quiz-style poll, 0-200 characters
     */
    std::optional<std::string> explanation;

    /**
     * @brief Optional. Special entities like usernames, URLs, bot commands, etc. that appear in the explanation
     */
    std::optional<std::vector<MessageEntity::Ptr>> explanationEntities;

    /**
     * @brief Optional. Amount of time in seconds the poll will be active after creation
     */
    std::optional<std::int32_t> openPeriod;

    /**
     * @brief Optional. Point in time (Unix timestamp) when the poll will be automatically closed
     */
    std::optional<std::int64_t> closeDate;

    /**
     * @brief Optional. Special entities that appear in the question. Currently, only custom emoji entities are allowed in poll questions
     */
    std::optional<std::vector<MessageEntity::Ptr>> questionEntities;
    /**
     * @brief True, if the poll allows to change the chosen answer options
     */
    bool allowsRevoting;

    /**
     * @brief True if voting is limited to users who have been members of the chat where the poll was originally sent for more than 24 hours
     */
    bool membersOnly;

    /**
     * @brief Optional. A list of two-letter ISO 3166-1 alpha-2 country codes indicating the countries from which users can vote in the poll. The country code "FT" is used for users with anonymous numbers. If omitted, then users from any country can participate in the poll.
     */
    std::optional<std::vector<std::string>> countryCodes;

    /**
     * @brief Optional. Array of 0-based identifiers of the correct answer options. Available only for polls in quiz mode which are closed or were sent (not forwarded) by the bot or to the private chat with the bot.
     */
    std::optional<std::vector<std::int64_t>> correctOptionIds;

    /**
     * @brief Optional. Media added to the quiz explanation
     */
    std::optional<PollMedia::Ptr> explanationMedia;

    /**
     * @brief Optional. Description of the poll; for polls inside the Message object only
     */
    std::optional<std::string> description;

    /**
     * @brief Optional. Special entities like usernames, URLs, bot commands, etc. that appear in the description
     */
    std::optional<std::vector<MessageEntity::Ptr>> descriptionEntities;

    /**
     * @brief Optional. Media added to the poll description; for polls inside the Message object only
     */
    std::optional<PollMedia::Ptr> media;

};
}

#endif //TGBOT_POLL_H
