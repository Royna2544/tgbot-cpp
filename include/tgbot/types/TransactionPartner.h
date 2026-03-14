#ifndef TGBOT_TRANSACTIONPARTNER_H
#define TGBOT_TRANSACTIONPARTNER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief This object describes the source of a transaction, or its recipient for outgoing transactions. Currently, it can be one of - TransactionPartnerUser - TransactionPartnerChat - TransactionPartnerAffiliateProgram - TransactionPartnerFragment - TransactionPartnerTelegramAds - TransactionPartnerTelegramApi - TransactionPartnerOther
 *
 * @ingroup types
 */
class TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartner>;

    std::string type;

};
}

#endif //TGBOT_TRANSACTIONPARTNER_H
