#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(Poll) {
    auto result = std::make_shared<Poll>();
    parse(data, "id", &result->id);
    parse(data, "question", &result->question);
    result->questionEntities = parseArray<MessageEntity>(data, "question_entities");
    result->options = parseRequiredArray<PollOption>(data, "options");
    parse(data, "total_voter_count", &result->totalVoterCount);
    parse(data, "is_closed", &result->isClosed);
    parse(data, "is_anonymous", &result->isAnonymous);
    parse(data, "type", &result->type);
    parse(data, "allows_multiple_answers", &result->allowsMultipleAnswers);
    parse(data, "correct_option_id", &result->correctOptionId);
    parse(data, "explanation", &result->explanation);
    result->explanationEntities = parseArray<MessageEntity>(data, "explanation_entities");
    parse(data, "open_period", &result->openPeriod);
    parse(data, "close_date", &result->closeDate);
    return result;
}

DECLARE_PARSER_TO_JSON(Poll) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("question", object->question);
        json.put("question_entities", object->questionEntities);
        json.put("options", object->options);
        json.put("total_voter_count", object->totalVoterCount);
        json.put("is_closed", object->isClosed);
        json.put("is_anonymous", object->isAnonymous);
        json.put("type", object->type);
        json.put("allows_multiple_answers", object->allowsMultipleAnswers);
        json.put("correct_option_id", object->correctOptionId);
        json.put("explanation", object->explanation);
        json.put("explanation_entities", object->explanationEntities);
        json.put("open_period", object->openPeriod);
        json.put("close_date", object->closeDate);
    }
    return json;
}

} // namespace TgBot
