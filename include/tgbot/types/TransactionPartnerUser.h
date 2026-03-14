#ifndef TGBOT_TRANSACTIONPARTNERUSER_H
#define TGBOT_TRANSACTIONPARTNERUSER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include "tgbot/types/TransactionPartner.h"
#include "tgbot/types/User.h"
#include "tgbot/types/AffiliateInfo.h"
#include "tgbot/types/PaidMedia.h"
#include "tgbot/types/Gift.h"

namespace TgBot {

/**
 * @brief Describes a transaction with a user.
 *
 * @ingroup types
 */
class TransactionPartnerUser : public TransactionPartner {
public:
    using Ptr = std::shared_ptr<TransactionPartnerUser>;
    constexpr static const char* TYPE = "user";

    TransactionPartnerUser() {
        type = TYPE;
    }

    /**
     * @brief Type of the transaction, currently one of "invoice_payment" for payments via invoices, "paid_media_payment" for payments for paid media, "gift_purchase" for gifts sent by the bot, "premium_purchase" for Telegram Premium subscriptions gifted by the bot, "business_account_transfer" for direct transfers from managed business accounts
     */
    std::string transactionType;

    /**
     * @brief Information about the user
     */
    User::Ptr user;

    /**
     * @brief Optional. Information about the affiliate that received a commission via this transaction. Can be available only for "invoice_payment" and "paid_media_payment" transactions.
     */
    std::optional<AffiliateInfo::Ptr> affiliate;

    /**
     * @brief Optional. Bot-specified invoice payload. Can be available only for "invoice_payment" transactions.
     */
    std::optional<std::string> invoicePayload;

    /**
     * @brief Optional. The duration of the paid subscription. Can be available only for "invoice_payment" transactions.
     */
    std::optional<std::int64_t> subscriptionPeriod;

    /**
     * @brief Optional. Information about the paid media bought by the user; for "paid_media_payment" transactions only
     */
    std::optional<std::vector<PaidMedia::Ptr>> paidMedia;

    /**
     * @brief Optional. Bot-specified paid media payload. Can be available only for "paid_media_payment" transactions.
     */
    std::optional<std::string> paidMediaPayload;

    /**
     * @brief Optional. The gift sent to the user by the bot; for "gift_purchase" transactions only
     */
    std::optional<Gift::Ptr> gift;

    /**
     * @brief Optional. Number of months the gifted Telegram Premium subscription will be active for; for "premium_purchase" transactions only
     */
    std::optional<std::int64_t> premiumSubscriptionDuration;

};
}

#endif //TGBOT_TRANSACTIONPARTNERUSER_H
