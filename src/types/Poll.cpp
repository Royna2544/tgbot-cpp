#include <tgbot/TgTypeParser.h>
#include <tgbot/types/Poll.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<Poll> parse(const nlohmann::json &data) {
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
    parse(data, "allows_revoting", &result->allowsRevoting);
    parse(data, "members_only", &result->membersOnly);
    result->countryCodes = parsePrimitiveArray<std::string>(data, "country_codes");
    result->correctOptionIds = parsePrimitiveArray<std::int64_t>(data, "correct_option_ids");
    parse(data, "explanation", &result->explanation);
    result->explanationEntities = parseArray<MessageEntity>(data, "explanation_entities");
    result->explanationMedia = parse<PollMedia>(data, "explanation_media");
    parse(data, "open_period", &result->openPeriod);
    parse(data, "close_date", &result->closeDate);
    parse(data, "description", &result->description);
    result->descriptionEntities = parseArray<MessageEntity>(data, "description_entities");
    result->media = parse<PollMedia>(data, "media");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<Poll> &object) {
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
        json.put("allows_revoting", object->allowsRevoting);
        json.put("members_only", object->membersOnly);
        json.put("country_codes", object->countryCodes);
        json.put("correct_option_ids", object->correctOptionIds);
        json.put("explanation", object->explanation);
        json.put("explanation_entities", object->explanationEntities);
        json.put("explanation_media", object->explanationMedia);
        json.put("open_period", object->openPeriod);
        json.put("close_date", object->closeDate);
        json.put("description", object->description);
        json.put("description_entities", object->descriptionEntities);
        json.put("media", object->media);
    }
    return json;
}

} // namespace TgBot
