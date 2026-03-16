#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(StarTransaction) {
    auto result = std::make_shared<StarTransaction>();
    parse(data, "id", &result->id);
    parse(data, "amount", &result->amount);
    parse(data, "nanostar_amount", &result->nanostarAmount);
    parse(data, "date", &result->date);
    result->source = parse<TransactionPartner>(data, "source");
    result->receiver = parse<TransactionPartner>(data, "receiver");
    return result;
}

DECLARE_PARSER_TO_JSON(StarTransaction) {
    JsonWrapper json;
    if (object) {
        json.put("id", object->id);
        json.put("amount", object->amount);
        json.put("nanostar_amount", object->nanostarAmount);
        json.put("date", object->date);
        json.put("source", object->source);
        json.put("receiver", object->receiver);
    }
    return json;
}

} // namespace TgBot
