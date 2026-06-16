#include <tgbot/TgTypeParser.h>
#include <tgbot/types/TransactionPartner.h>
#include <tgbot/types/TransactionPartnerAffiliateProgram.h>
#include <tgbot/types/TransactionPartnerChat.h>
#include <tgbot/types/TransactionPartnerFragment.h>
#include <tgbot/types/TransactionPartnerOther.h>
#include <tgbot/types/TransactionPartnerTelegramAds.h>
#include <tgbot/types/TransactionPartnerTelegramApi.h>
#include <tgbot/types/TransactionPartnerUser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<TransactionPartner> parse(const nlohmann::json &data) {
    std::string type;
    TransactionPartner::Ptr result;
    parse(data, "type", &type);
    if (type == "user") {
        result = parse<TransactionPartnerUser>(data);
    } else if (type == "chat") {
        result = parse<TransactionPartnerChat>(data);
    } else if (type == "affiliate_program") {
        result = parse<TransactionPartnerAffiliateProgram>(data);
    } else if (type == "fragment") {
        result = parse<TransactionPartnerFragment>(data);
    } else if (type == "telegram_ads") {
        result = parse<TransactionPartnerTelegramAds>(data);
    } else if (type == "telegram_api") {
        result = parse<TransactionPartnerTelegramApi>(data);
    } else if (type == "other") {
        result = parse<TransactionPartnerOther>(data);
    } else {
        throw invalidType("TransactionPartner", type);
    }

    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<TransactionPartner> &object) {
    JsonWrapper json;
    if (object) {
        json.put("type", object->type);
        if (object->type == "user") {
            json += put<TransactionPartnerUser>(object);
        } else if (object->type == "chat") {
            json += put<TransactionPartnerChat>(object);
        } else if (object->type == "affiliate_program") {
            json += put<TransactionPartnerAffiliateProgram>(object);
        } else if (object->type == "fragment") {
            json += put<TransactionPartnerFragment>(object);
        } else if (object->type == "telegram_ads") {
            json += put<TransactionPartnerTelegramAds>(object);
        } else if (object->type == "telegram_api") {
            json += put<TransactionPartnerTelegramApi>(object);
        } else if (object->type == "other") {
            json += put<TransactionPartnerOther>(object);
        } else {
            throw invalidType("TransactionPartner", object->type);
        }
    }
    return json;
}

} // namespace TgBot
