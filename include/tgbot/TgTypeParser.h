#ifndef TGBOT_TGTYPEPARSER_H
#define TGBOT_TGTYPEPARSER_H

#include <nlohmann/json.hpp>

#include "tgbot/TgException.h"

#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "tgbot/types/Animation.h"
#include "tgbot/types/Audio.h"
#include "tgbot/types/Birthdate.h"
#include "tgbot/types/BotCommand.h"
#include "tgbot/types/BotCommandScope.h"
#include "tgbot/types/BotCommandScopeAllChatAdministrators.h"
#include "tgbot/types/BotCommandScopeAllGroupChats.h"
#include "tgbot/types/BotCommandScopeAllPrivateChats.h"
#include "tgbot/types/BotCommandScopeChat.h"
#include "tgbot/types/BotCommandScopeChatAdministrators.h"
#include "tgbot/types/BotCommandScopeChatMember.h"
#include "tgbot/types/BotCommandScopeDefault.h"
#include "tgbot/types/BotDescription.h"
#include "tgbot/types/BotName.h"
#include "tgbot/types/BotShortDescription.h"
#include "tgbot/types/BusinessConnection.h"
#include "tgbot/types/BusinessIntro.h"
#include "tgbot/types/BusinessLocation.h"
#include "tgbot/types/BusinessMessagesDeleted.h"
#include "tgbot/types/BusinessOpeningHours.h"
#include "tgbot/types/BusinessOpeningHoursInterval.h"
#include "tgbot/types/CallbackGame.h"
#include "tgbot/types/CallbackQuery.h"
#include "tgbot/types/Chat.h"
#include "tgbot/types/ChatAdministratorRights.h"
#include "tgbot/types/ChatBoost.h"
#include "tgbot/types/ChatBoostAdded.h"
#include "tgbot/types/ChatBoostRemoved.h"
#include "tgbot/types/ChatBoostSource.h"
#include "tgbot/types/ChatBoostSourceGiftCode.h"
#include "tgbot/types/ChatBoostSourceGiveaway.h"
#include "tgbot/types/ChatBoostSourcePremium.h"
#include "tgbot/types/ChatBoostUpdated.h"
#include "tgbot/types/ChatInviteLink.h"
#include "tgbot/types/ChatJoinRequest.h"
#include "tgbot/types/ChatLocation.h"
#include "tgbot/types/ChatMember.h"
#include "tgbot/types/ChatMemberAdministrator.h"
#include "tgbot/types/ChatMemberBanned.h"
#include "tgbot/types/ChatMemberLeft.h"
#include "tgbot/types/ChatMemberMember.h"
#include "tgbot/types/ChatMemberOwner.h"
#include "tgbot/types/ChatMemberRestricted.h"
#include "tgbot/types/ChatMemberUpdated.h"
#include "tgbot/types/ChatPermissions.h"
#include "tgbot/types/ChatPhoto.h"
#include "tgbot/types/ChatShared.h"
#include "tgbot/types/ChosenInlineResult.h"
#include "tgbot/types/Contact.h"
#include "tgbot/types/Dice.h"
#include "tgbot/types/Document.h"
#include "tgbot/types/EncryptedCredentials.h"
#include "tgbot/types/EncryptedPassportElement.h"
#include "tgbot/types/ExternalReplyInfo.h"
#include "tgbot/types/File.h"
#include "tgbot/types/ForceReply.h"
#include "tgbot/types/ForumTopic.h"
#include "tgbot/types/ForumTopicClosed.h"
#include "tgbot/types/ForumTopicCreated.h"
#include "tgbot/types/ForumTopicEdited.h"
#include "tgbot/types/ForumTopicReopened.h"
#include "tgbot/types/Game.h"
#include "tgbot/types/GameHighScore.h"
#include "tgbot/types/GeneralForumTopicHidden.h"
#include "tgbot/types/GeneralForumTopicUnhidden.h"
#include "tgbot/types/GenericReply.h"
#include "tgbot/types/Giveaway.h"
#include "tgbot/types/GiveawayCompleted.h"
#include "tgbot/types/GiveawayCreated.h"
#include "tgbot/types/GiveawayWinners.h"
#include "tgbot/types/InaccessibleMessage.h"
#include "tgbot/types/InlineKeyboardButton.h"
#include "tgbot/types/InlineKeyboardMarkup.h"
#include "tgbot/types/InlineQuery.h"
#include "tgbot/types/InlineQueryResult.h"
#include "tgbot/types/InlineQueryResultArticle.h"
#include "tgbot/types/InlineQueryResultAudio.h"
#include "tgbot/types/InlineQueryResultCachedAudio.h"
#include "tgbot/types/InlineQueryResultCachedDocument.h"
#include "tgbot/types/InlineQueryResultCachedGif.h"
#include "tgbot/types/InlineQueryResultCachedMpeg4Gif.h"
#include "tgbot/types/InlineQueryResultCachedPhoto.h"
#include "tgbot/types/InlineQueryResultCachedSticker.h"
#include "tgbot/types/InlineQueryResultCachedVideo.h"
#include "tgbot/types/InlineQueryResultCachedVoice.h"
#include "tgbot/types/InlineQueryResultContact.h"
#include "tgbot/types/InlineQueryResultDocument.h"
#include "tgbot/types/InlineQueryResultGame.h"
#include "tgbot/types/InlineQueryResultGif.h"
#include "tgbot/types/InlineQueryResultLocation.h"
#include "tgbot/types/InlineQueryResultMpeg4Gif.h"
#include "tgbot/types/InlineQueryResultPhoto.h"
#include "tgbot/types/InlineQueryResultVenue.h"
#include "tgbot/types/InlineQueryResultVideo.h"
#include "tgbot/types/InlineQueryResultVoice.h"
#include "tgbot/types/InlineQueryResultsButton.h"
#include "tgbot/types/InputContactMessageContent.h"
#include "tgbot/types/InputInvoiceMessageContent.h"
#include "tgbot/types/InputLocationMessageContent.h"
#include "tgbot/types/InputMedia.h"
#include "tgbot/types/InputMediaAnimation.h"
#include "tgbot/types/InputMediaAudio.h"
#include "tgbot/types/InputMediaDocument.h"
#include "tgbot/types/InputMediaPhoto.h"
#include "tgbot/types/InputMediaVideo.h"
#include "tgbot/types/InputMessageContent.h"
#include "tgbot/types/InputSticker.h"
#include "tgbot/types/InputTextMessageContent.h"
#include "tgbot/types/InputVenueMessageContent.h"
#include "tgbot/types/Invoice.h"
#include "tgbot/types/KeyboardButton.h"
#include "tgbot/types/KeyboardButtonPollType.h"
#include "tgbot/types/KeyboardButtonRequestChat.h"
#include "tgbot/types/KeyboardButtonRequestUsers.h"
#include "tgbot/types/LabeledPrice.h"
#include "tgbot/types/LinkPreviewOptions.h"
#include "tgbot/types/Location.h"
#include "tgbot/types/LoginUrl.h"
#include "tgbot/types/MaskPosition.h"
#include "tgbot/types/MaybeInaccessibleMessage.h"
#include "tgbot/types/MenuButton.h"
#include "tgbot/types/MenuButtonCommands.h"
#include "tgbot/types/MenuButtonDefault.h"
#include "tgbot/types/MenuButtonWebApp.h"
#include "tgbot/types/Message.h"
#include "tgbot/types/MessageAutoDeleteTimerChanged.h"
#include "tgbot/types/MessageEntity.h"
#include "tgbot/types/MessageId.h"
#include "tgbot/types/MessageOrigin.h"
#include "tgbot/types/MessageOriginChannel.h"
#include "tgbot/types/MessageOriginChat.h"
#include "tgbot/types/MessageOriginHiddenUser.h"
#include "tgbot/types/MessageOriginUser.h"
#include "tgbot/types/MessageReactionCountUpdated.h"
#include "tgbot/types/MessageReactionUpdated.h"
#include "tgbot/types/OrderInfo.h"
#include "tgbot/types/PassportData.h"
#include "tgbot/types/PassportElementError.h"
#include "tgbot/types/PassportElementErrorDataField.h"
#include "tgbot/types/PassportElementErrorFile.h"
#include "tgbot/types/PassportElementErrorFiles.h"
#include "tgbot/types/PassportElementErrorFrontSide.h"
#include "tgbot/types/PassportElementErrorReverseSide.h"
#include "tgbot/types/PassportElementErrorSelfie.h"
#include "tgbot/types/PassportElementErrorTranslationFile.h"
#include "tgbot/types/PassportElementErrorTranslationFiles.h"
#include "tgbot/types/PassportElementErrorUnspecified.h"
#include "tgbot/types/PassportFile.h"
#include "tgbot/types/PhotoSize.h"
#include "tgbot/types/Poll.h"
#include "tgbot/types/PollAnswer.h"
#include "tgbot/types/PollOption.h"
#include "tgbot/types/PreCheckoutQuery.h"
#include "tgbot/types/ProximityAlertTriggered.h"
#include "tgbot/types/ReactionCount.h"
#include "tgbot/types/ReactionType.h"
#include "tgbot/types/ReactionTypeCustomEmoji.h"
#include "tgbot/types/ReactionTypeEmoji.h"
#include "tgbot/types/ReplyKeyboardMarkup.h"
#include "tgbot/types/ReplyKeyboardRemove.h"
#include "tgbot/types/ReplyParameters.h"
#include "tgbot/types/ResponseParameters.h"
#include "tgbot/types/SentWebAppMessage.h"
#include "tgbot/types/SharedUser.h"
#include "tgbot/types/ShippingAddress.h"
#include "tgbot/types/ShippingOption.h"
#include "tgbot/types/ShippingQuery.h"
#include "tgbot/types/Sticker.h"
#include "tgbot/types/StickerSet.h"
#include "tgbot/types/Story.h"
#include "tgbot/types/SuccessfulPayment.h"
#include "tgbot/types/SwitchInlineQueryChosenChat.h"
#include "tgbot/types/TextQuote.h"
#include "tgbot/types/Update.h"
#include "tgbot/types/User.h"
#include "tgbot/types/UserChatBoosts.h"
#include "tgbot/types/UserProfilePhotos.h"
#include "tgbot/types/UsersShared.h"
#include "tgbot/types/Venue.h"
#include "tgbot/types/Video.h"
#include "tgbot/types/VideoChatEnded.h"
#include "tgbot/types/VideoChatParticipantsInvited.h"
#include "tgbot/types/VideoChatScheduled.h"
#include "tgbot/types/VideoChatStarted.h"
#include "tgbot/types/VideoNote.h"
#include "tgbot/types/Voice.h"
#include "tgbot/types/WebAppData.h"
#include "tgbot/types/WebAppInfo.h"
#include "tgbot/types/WebhookInfo.h"
#include "tgbot/types/WriteAccessAllowed.h"
#include "tgbot/types/AcceptedGiftTypes.h"
#include "tgbot/types/BackgroundFillFreeformGradient.h"
#include "tgbot/types/BackgroundFillGradient.h"
#include "tgbot/types/BackgroundFillSolid.h"
#include "tgbot/types/BackgroundTypeChatTheme.h"
#include "tgbot/types/BackgroundTypeFill.h"
#include "tgbot/types/BackgroundTypePattern.h"
#include "tgbot/types/BackgroundTypeWallpaper.h"
#include "tgbot/types/BusinessBotRights.h"
#include "tgbot/types/ChatBackground.h"
#include "tgbot/types/ChatFullInfo.h"
#include "tgbot/types/ChatOwnerChanged.h"
#include "tgbot/types/ChatOwnerLeft.h"
#include "tgbot/types/Checklist.h"
#include "tgbot/types/ChecklistTask.h"
#include "tgbot/types/ChecklistTasksAdded.h"
#include "tgbot/types/ChecklistTasksDone.h"
#include "tgbot/types/CopyTextButton.h"
#include "tgbot/types/DirectMessagePriceChanged.h"
#include "tgbot/types/DirectMessagesTopic.h"
#include "tgbot/types/Gift.h"
#include "tgbot/types/GiftBackground.h"
#include "tgbot/types/GiftInfo.h"
#include "tgbot/types/Gifts.h"
#include "tgbot/types/InputChecklist.h"
#include "tgbot/types/InputChecklistTask.h"
#include "tgbot/types/InputPaidMediaPhoto.h"
#include "tgbot/types/InputPaidMediaVideo.h"
#include "tgbot/types/InputPollOption.h"
#include "tgbot/types/InputProfilePhoto.h"
#include "tgbot/types/InputProfilePhotoAnimated.h"
#include "tgbot/types/InputProfilePhotoStatic.h"
#include "tgbot/types/InputStoryContent.h"
#include "tgbot/types/InputStoryContentPhoto.h"
#include "tgbot/types/InputStoryContentVideo.h"
#include "tgbot/types/LocationAddress.h"
#include "tgbot/types/OwnedGift.h"
#include "tgbot/types/OwnedGiftRegular.h"
#include "tgbot/types/OwnedGifts.h"
#include "tgbot/types/OwnedGiftUnique.h"
#include "tgbot/types/PaidMedia.h"
#include "tgbot/types/PaidMediaInfo.h"
#include "tgbot/types/PaidMediaPhoto.h"
#include "tgbot/types/PaidMediaPreview.h"
#include "tgbot/types/PaidMediaPurchased.h"
#include "tgbot/types/PaidMediaVideo.h"
#include "tgbot/types/PaidMessagePriceChanged.h"
#include "tgbot/types/PreparedInlineMessage.h"
#include "tgbot/types/ReactionTypePaid.h"
#include "tgbot/types/RefundedPayment.h"
#include "tgbot/types/RevenueWithdrawalStateFailed.h"
#include "tgbot/types/RevenueWithdrawalStatePending.h"
#include "tgbot/types/RevenueWithdrawalStateSucceeded.h"
#include "tgbot/types/StarAmount.h"
#include "tgbot/types/StarTransactions.h"
#include "tgbot/types/StoryArea.h"
#include "tgbot/types/StoryAreaPosition.h"
#include "tgbot/types/StoryAreaType.h"
#include "tgbot/types/StoryAreaTypeLink.h"
#include "tgbot/types/StoryAreaTypeLocation.h"
#include "tgbot/types/StoryAreaTypeSuggestedReaction.h"
#include "tgbot/types/StoryAreaTypeUniqueGift.h"
#include "tgbot/types/StoryAreaTypeWeather.h"
#include "tgbot/types/SuggestedPostApprovalFailed.h"
#include "tgbot/types/SuggestedPostApproved.h"
#include "tgbot/types/SuggestedPostDeclined.h"
#include "tgbot/types/SuggestedPostInfo.h"
#include "tgbot/types/SuggestedPostPaid.h"
#include "tgbot/types/SuggestedPostParameters.h"
#include "tgbot/types/SuggestedPostPrice.h"
#include "tgbot/types/SuggestedPostRefunded.h"
#include "tgbot/types/TransactionPartnerAffiliateProgram.h"
#include "tgbot/types/TransactionPartnerChat.h"
#include "tgbot/types/TransactionPartnerFragment.h"
#include "tgbot/types/TransactionPartnerOther.h"
#include "tgbot/types/TransactionPartnerTelegramAds.h"
#include "tgbot/types/TransactionPartnerTelegramApi.h"
#include "tgbot/types/TransactionPartnerUser.h"
#include "tgbot/types/UniqueGift.h"
#include "tgbot/types/UniqueGiftBackdrop.h"
#include "tgbot/types/UniqueGiftBackdropColors.h"
#include "tgbot/types/UniqueGiftColors.h"
#include "tgbot/types/UniqueGiftInfo.h"
#include "tgbot/types/UniqueGiftModel.h"
#include "tgbot/types/UniqueGiftSymbol.h"
#include "tgbot/types/UserProfileAudios.h"
#include "tgbot/types/UserRating.h"
#include "tgbot/types/VideoQuality.h"


namespace TgBot {

template <typename T>
using Matrix = std::vector<std::vector<T>>;
namespace detail {  // shared_ptr
template <typename T>
struct is_shared_ptr : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {
    using type = T;
};

template <typename T>
constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

// vector
template <typename T>
struct is_vector : std::false_type {};
template <typename T>
struct is_vector<std::vector<T>> : std::true_type {
    using type = T;
};
template <typename T>
constexpr bool is_vector_v = is_vector<T>::value;

// optional
template <typename T>
struct is_optional : std::false_type {};
template <typename T>
struct is_optional<std::optional<T>> : std::true_type {
    using type = T;
};
template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

// primitive
template <typename T>
constexpr bool is_primitive_v =
    std::is_same_v<T, bool> || std::is_convertible_v<T, std::string> ||
    std::is_integral_v<T> || std::is_floating_point_v<T>;

// Matrix
template <typename T>
struct is_matrix : std::false_type {};
template <typename T>
struct is_matrix<Matrix<T>> : std::true_type {
    using type = T;
};
template <typename T>
constexpr bool is_matrix_v = is_matrix<T>::value;

}  // namespace detail

// Parse function for shared_ptr<T>
template <typename T>
std::shared_ptr<T> parse(const nlohmann::json &data) = delete;

#define DECLARE_PARSER_FROM_JSON(TYPE) \
    template <>                        \
    TYPE::Ptr parse(const nlohmann::json &data)

// Grab array of T from JSON array.
template <typename T>
std::vector<std::shared_ptr<T>> parseArray(const nlohmann::json &data) {
    std::vector<std::shared_ptr<T>> result;
    for (const auto &item : data) {
        result.emplace_back(parse<T>(item));
    }
    return result;
}

// Parse array from a key.
template <typename T>
std::vector<std::shared_ptr<T>> parseRequiredArray(const nlohmann::json &data,
                                           const std::string &key) {
    if (!data.contains(key)) {
        return {};
    }
    return parseArray<T>(data[key]);
}

template <typename T>
std::optional<std::vector<std::shared_ptr<T>>> parseArray(const nlohmann::json& data,
    const std::string& key) {
    if (!data.contains(key)) {
        return std::nullopt;
    }
    return parseArray<T>(data[key]);
}

// Parse 2D array of T from JSON.
template <typename T>
Matrix<std::shared_ptr<T>> parseMatrix(const nlohmann::json &data) {
    Matrix<std::shared_ptr<T>> result;
    for (const auto &item : data) {
        result.emplace_back(parseArray<T>(item));
    }
    return result;
}

template <typename T>
Matrix<std::shared_ptr<T>> parseMatrix(const nlohmann::json &data,
                                       const std::string &key) {
    if (!data.contains(key)) {
        return {};
    }
    return parseMatrix<T>(data[key]);
}

// Parse an array of primitive types.
template <typename T>
std::optional<std::vector<T>> parsePrimitiveArray(const nlohmann::json &data,
                                   const std::string &key) {
    if (!data.contains(key)) {
        return std::nullopt;
    }
    std::vector<T> result;
    for (const auto &item : data[key]) {
        result.emplace_back(item.get<T>());
    }
    return result;
}

template <typename T>
std::vector<T> parsePrimitiveRequiredArray(const nlohmann::json& data,
    const std::string& key) {
    if (!data.contains(key)) {
        return {};
    }
    std::vector<T> result;
    for (const auto& item : data[key]) {
        result.emplace_back(item.get<T>());
    }
    return result;
}

// Put function for objects to JSON
template <typename T>
nlohmann::json put(const T &value) = delete;

#define DECLARE_PARSER_TO_JSON(TYPE) \
    template <>                      \
    nlohmann::json put(const TYPE::Ptr &object)

// Helper to put base class shared_ptr to derived T.
template <typename T, typename V,
          std::enable_if_t<detail::is_shared_ptr_v<V> &&
                               !std::is_same_v<typename T::Ptr, V> &&
                               std::is_base_of_v<typename V::element_type, T>,
                           bool> = true>
nlohmann::json put(const V &data) {
    return put(std::static_pointer_cast<T>(data));
}

// Put vector to JSON.
template <typename T, std::enable_if_t<!detail::is_vector_v<T>, bool> = true>
nlohmann::json put(const std::vector<T> &vector) {
    nlohmann::json dataArray = nlohmann::json::array();
    for (const auto &item : vector) {
        if constexpr (detail::is_primitive_v<T>) {
            dataArray.push_back(item);
        } else if constexpr (std::is_same_v<std::string_view, T>) {
            dataArray.push_back(std::string(item));
        } else {
            // Recursively call put for non-primitives
            dataArray.push_back(put(item));
        }
    }
    return dataArray;
}

// Put 2D array (Matrix) to JSON.
template <typename T>
nlohmann::json put(const Matrix<T> &matrix) {
    nlohmann::json dataMatrix = nlohmann::json::array();
    for (const auto &row : matrix) {
        dataMatrix.push_back(put(row));  // Recursively call put for each row
    }
    return dataMatrix;
}

// Serialize object to JSON string.
template <typename T>
std::string putJSON(const T &object) {
    return put(object).dump();
}


// T should be instance of std::shared_ptr.
template <typename T>
std::optional<std::shared_ptr<T>> parse(const nlohmann::json& data, const std::string& key) {
    if (!data.contains(key)) {
        return std::nullopt;
    }
    return parse<T>(data[key]);
}

template <typename T>
std::shared_ptr<T> parseRequired(const nlohmann::json& data, const std::string& key) {
    if (!data.contains(key)) {
        return nullptr;
    }
    return parse<T>(data[key]);
}


inline TgException invalidType(const std::string_view name,
    const std::string_view type) {
    std::stringstream ss;
    ss << "Invalid type for " << name << ": " << type;
    return TgException(ss.str(), TgException::ErrorCode::Internal);
}

struct JsonWrapper {
    JsonWrapper() : data_(nlohmann::json::object()) {}

    // Overload for primitive types
    template <typename T,
        std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, T value) {
        data_[std::string(key)] = std::move(value);
    }
    // Required objects
    template <typename T,
        std::enable_if_t<!detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::shared_ptr<T> value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Support for vector of primitives and objects
    template <typename T>
    void put(const std::string_view key, std::vector<T> value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Overload for optional types
    template <typename T,
        std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = *value;
    }
    template <typename T,
        std::enable_if_t<!detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, std::optional<T> value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = TgBot::put(*value);
    }

    static void merge(nlohmann::json& thiz, const nlohmann::json& other) {
        if (!thiz.is_object() || !other.is_object()) {
            return;
        }

        for (auto it = other.begin(); it != other.end(); ++it) {
            if (thiz[it.key()].is_object()) {
                merge(thiz[it.key()], it.value());
            }
            else {
                thiz[it.key()] = it.value();
            }
        }
    }

    void operator+=(const nlohmann::json& other) { merge(data_, other); }

    JsonWrapper& operator=(nlohmann::json&& other) {
        data_ = std::move(other);
        return *this;
    }
    operator nlohmann::json() const { return data_; }

private:
    nlohmann::json data_;
};

template <typename T>
void parse(const nlohmann::json& data, const std::string& key, T* value) {
    using Type = std::conditional_t<detail::is_optional_v<T>,
        typename detail::is_optional<T>::type, T>;
    using FixedType =
        std::conditional_t<std::is_floating_point_v<Type>, double, Type>;
    using MoreFixedType =
        std::conditional_t<std::is_integral_v<Type>, int64_t, FixedType>;
    using FinalType =
        std::conditional_t<std::is_same_v<Type, bool>, bool, MoreFixedType>;
    if (data.contains(key) && !data[key].is_null()) {
        if constexpr (detail::is_primitive_v<Type>) {
            *value = static_cast<Type>(data[key].get<FinalType>());
        }
        else {
            *value = parse<typename detail::is_shared_ptr<Type>::type>(data[key]);
        }
    }
}


#define IMPLEMENT_PARSERS(type)     \
    DECLARE_PARSER_FROM_JSON(type); \
    DECLARE_PARSER_TO_JSON(type)
IMPLEMENT_PARSERS(Animation);
IMPLEMENT_PARSERS(Audio);
IMPLEMENT_PARSERS(Birthdate);
IMPLEMENT_PARSERS(BotCommand);
IMPLEMENT_PARSERS(BotCommandScope);
IMPLEMENT_PARSERS(BotCommandScopeAllChatAdministrators);
IMPLEMENT_PARSERS(BotCommandScopeAllGroupChats);
IMPLEMENT_PARSERS(BotCommandScopeAllPrivateChats);
IMPLEMENT_PARSERS(BotCommandScopeChat);
IMPLEMENT_PARSERS(BotCommandScopeChatAdministrators);
IMPLEMENT_PARSERS(BotCommandScopeChatMember);
IMPLEMENT_PARSERS(BotCommandScopeDefault);
IMPLEMENT_PARSERS(BotDescription);
IMPLEMENT_PARSERS(BotName);
IMPLEMENT_PARSERS(BotShortDescription);
IMPLEMENT_PARSERS(BusinessConnection);
IMPLEMENT_PARSERS(BusinessIntro);
IMPLEMENT_PARSERS(BusinessLocation);
IMPLEMENT_PARSERS(BusinessMessagesDeleted);
IMPLEMENT_PARSERS(BusinessOpeningHours);
IMPLEMENT_PARSERS(BusinessOpeningHoursInterval);
IMPLEMENT_PARSERS(CallbackGame);
IMPLEMENT_PARSERS(CallbackQuery);
IMPLEMENT_PARSERS(Chat);
IMPLEMENT_PARSERS(ChatAdministratorRights);
IMPLEMENT_PARSERS(ChatBoost);
IMPLEMENT_PARSERS(ChatBoostAdded);
IMPLEMENT_PARSERS(ChatBoostRemoved);
IMPLEMENT_PARSERS(ChatBoostSource);
IMPLEMENT_PARSERS(ChatBoostSourceGiftCode);
IMPLEMENT_PARSERS(ChatBoostSourceGiveaway);
IMPLEMENT_PARSERS(ChatBoostSourcePremium);
IMPLEMENT_PARSERS(ChatBoostUpdated);
IMPLEMENT_PARSERS(ChatInviteLink);
IMPLEMENT_PARSERS(ChatJoinRequest);
IMPLEMENT_PARSERS(ChatLocation);
IMPLEMENT_PARSERS(ChatMember);
IMPLEMENT_PARSERS(ChatMemberAdministrator);
IMPLEMENT_PARSERS(ChatMemberBanned);
IMPLEMENT_PARSERS(ChatMemberLeft);
IMPLEMENT_PARSERS(ChatMemberMember);
IMPLEMENT_PARSERS(ChatMemberOwner);
IMPLEMENT_PARSERS(ChatMemberRestricted);
IMPLEMENT_PARSERS(ChatMemberUpdated);
IMPLEMENT_PARSERS(ChatPermissions);
IMPLEMENT_PARSERS(ChatPhoto);
IMPLEMENT_PARSERS(ChatShared);
IMPLEMENT_PARSERS(ChosenInlineResult);
IMPLEMENT_PARSERS(Contact);
IMPLEMENT_PARSERS(Dice);
IMPLEMENT_PARSERS(Document);
IMPLEMENT_PARSERS(EncryptedCredentials);
IMPLEMENT_PARSERS(EncryptedPassportElement);
IMPLEMENT_PARSERS(ExternalReplyInfo);
IMPLEMENT_PARSERS(File);
IMPLEMENT_PARSERS(ForceReply);
IMPLEMENT_PARSERS(ForumTopic);
IMPLEMENT_PARSERS(ForumTopicClosed);
IMPLEMENT_PARSERS(ForumTopicCreated);
IMPLEMENT_PARSERS(ForumTopicEdited);
IMPLEMENT_PARSERS(ForumTopicReopened);
IMPLEMENT_PARSERS(Game);
IMPLEMENT_PARSERS(GameHighScore);
IMPLEMENT_PARSERS(GeneralForumTopicHidden);
IMPLEMENT_PARSERS(GeneralForumTopicUnhidden);
IMPLEMENT_PARSERS(GenericReply);
IMPLEMENT_PARSERS(Giveaway);
IMPLEMENT_PARSERS(GiveawayCompleted);
IMPLEMENT_PARSERS(GiveawayCreated);
IMPLEMENT_PARSERS(GiveawayWinners);
IMPLEMENT_PARSERS(InaccessibleMessage);
IMPLEMENT_PARSERS(InlineKeyboardButton);
IMPLEMENT_PARSERS(InlineKeyboardMarkup);
IMPLEMENT_PARSERS(InlineQuery);
IMPLEMENT_PARSERS(InlineQueryResult);
IMPLEMENT_PARSERS(InlineQueryResultArticle);
IMPLEMENT_PARSERS(InlineQueryResultAudio);
IMPLEMENT_PARSERS(InlineQueryResultCachedAudio);
IMPLEMENT_PARSERS(InlineQueryResultCachedDocument);
IMPLEMENT_PARSERS(InlineQueryResultCachedGif);
IMPLEMENT_PARSERS(InlineQueryResultCachedMpeg4Gif);
IMPLEMENT_PARSERS(InlineQueryResultCachedPhoto);
IMPLEMENT_PARSERS(InlineQueryResultCachedSticker);
IMPLEMENT_PARSERS(InlineQueryResultCachedVideo);
IMPLEMENT_PARSERS(InlineQueryResultCachedVoice);
IMPLEMENT_PARSERS(InlineQueryResultContact);
IMPLEMENT_PARSERS(InlineQueryResultDocument);
IMPLEMENT_PARSERS(InlineQueryResultGame);
IMPLEMENT_PARSERS(InlineQueryResultGif);
IMPLEMENT_PARSERS(InlineQueryResultLocation);
IMPLEMENT_PARSERS(InlineQueryResultMpeg4Gif);
IMPLEMENT_PARSERS(InlineQueryResultPhoto);
IMPLEMENT_PARSERS(InlineQueryResultVenue);
IMPLEMENT_PARSERS(InlineQueryResultVideo);
IMPLEMENT_PARSERS(InlineQueryResultVoice);
IMPLEMENT_PARSERS(InlineQueryResultsButton);
IMPLEMENT_PARSERS(InputContactMessageContent);
IMPLEMENT_PARSERS(InputInvoiceMessageContent);
IMPLEMENT_PARSERS(InputLocationMessageContent);
IMPLEMENT_PARSERS(InputMedia);
IMPLEMENT_PARSERS(InputMediaAnimation);
IMPLEMENT_PARSERS(InputMediaAudio);
IMPLEMENT_PARSERS(InputMediaDocument);
IMPLEMENT_PARSERS(InputMediaPhoto);
IMPLEMENT_PARSERS(InputMediaVideo);
IMPLEMENT_PARSERS(InputMessageContent);
IMPLEMENT_PARSERS(InputSticker);
IMPLEMENT_PARSERS(InputTextMessageContent);
IMPLEMENT_PARSERS(InputVenueMessageContent);
IMPLEMENT_PARSERS(Invoice);
IMPLEMENT_PARSERS(KeyboardButton);
IMPLEMENT_PARSERS(KeyboardButtonPollType);
IMPLEMENT_PARSERS(KeyboardButtonRequestChat);
IMPLEMENT_PARSERS(KeyboardButtonRequestUsers);
IMPLEMENT_PARSERS(LabeledPrice);
IMPLEMENT_PARSERS(LinkPreviewOptions);
IMPLEMENT_PARSERS(Location);
IMPLEMENT_PARSERS(LoginUrl);
IMPLEMENT_PARSERS(MaskPosition);
IMPLEMENT_PARSERS(MenuButton);
IMPLEMENT_PARSERS(MenuButtonCommands);
IMPLEMENT_PARSERS(MenuButtonDefault);
IMPLEMENT_PARSERS(MenuButtonWebApp);
IMPLEMENT_PARSERS(Message);
IMPLEMENT_PARSERS(MessageAutoDeleteTimerChanged);
IMPLEMENT_PARSERS(MessageEntity);
IMPLEMENT_PARSERS(MessageId);
IMPLEMENT_PARSERS(MessageOrigin);
IMPLEMENT_PARSERS(MessageOriginChannel);
IMPLEMENT_PARSERS(MessageOriginChat);
IMPLEMENT_PARSERS(MessageOriginHiddenUser);
IMPLEMENT_PARSERS(MessageOriginUser);
IMPLEMENT_PARSERS(MessageReactionCountUpdated);
IMPLEMENT_PARSERS(MessageReactionUpdated);
IMPLEMENT_PARSERS(OrderInfo);
IMPLEMENT_PARSERS(PassportData);
IMPLEMENT_PARSERS(PassportElementError);
IMPLEMENT_PARSERS(PassportElementErrorDataField);
IMPLEMENT_PARSERS(PassportElementErrorFile);
IMPLEMENT_PARSERS(PassportElementErrorFiles);
IMPLEMENT_PARSERS(PassportElementErrorFrontSide);
IMPLEMENT_PARSERS(PassportElementErrorReverseSide);
IMPLEMENT_PARSERS(PassportElementErrorSelfie);
IMPLEMENT_PARSERS(PassportElementErrorTranslationFile);
IMPLEMENT_PARSERS(PassportElementErrorTranslationFiles);
IMPLEMENT_PARSERS(PassportElementErrorUnspecified);
IMPLEMENT_PARSERS(PassportFile);
IMPLEMENT_PARSERS(PhotoSize);
IMPLEMENT_PARSERS(Poll);
IMPLEMENT_PARSERS(PollAnswer);
IMPLEMENT_PARSERS(PollOption);
IMPLEMENT_PARSERS(PreCheckoutQuery);
IMPLEMENT_PARSERS(ProximityAlertTriggered);
IMPLEMENT_PARSERS(ReactionCount);
IMPLEMENT_PARSERS(ReactionType);
IMPLEMENT_PARSERS(ReactionTypeCustomEmoji);
IMPLEMENT_PARSERS(ReactionTypeEmoji);
IMPLEMENT_PARSERS(ReplyKeyboardMarkup);
IMPLEMENT_PARSERS(ReplyKeyboardRemove);
IMPLEMENT_PARSERS(ReplyParameters);
IMPLEMENT_PARSERS(ResponseParameters);
IMPLEMENT_PARSERS(SentWebAppMessage);
IMPLEMENT_PARSERS(SharedUser);
IMPLEMENT_PARSERS(ShippingAddress);
IMPLEMENT_PARSERS(ShippingOption);
IMPLEMENT_PARSERS(ShippingQuery);
IMPLEMENT_PARSERS(StarTransaction);
IMPLEMENT_PARSERS(StarTransactions);
IMPLEMENT_PARSERS(Sticker);
IMPLEMENT_PARSERS(StickerSet);
IMPLEMENT_PARSERS(Story);
IMPLEMENT_PARSERS(SuccessfulPayment);
IMPLEMENT_PARSERS(SwitchInlineQueryChosenChat);
IMPLEMENT_PARSERS(TextQuote);
IMPLEMENT_PARSERS(Update);
IMPLEMENT_PARSERS(User);
IMPLEMENT_PARSERS(UserChatBoosts);
IMPLEMENT_PARSERS(UserProfilePhotos);
IMPLEMENT_PARSERS(UsersShared);
IMPLEMENT_PARSERS(Venue);
IMPLEMENT_PARSERS(Video);
IMPLEMENT_PARSERS(VideoChatEnded);
IMPLEMENT_PARSERS(VideoChatParticipantsInvited);
IMPLEMENT_PARSERS(VideoChatScheduled);
IMPLEMENT_PARSERS(VideoChatStarted);
IMPLEMENT_PARSERS(VideoNote);
IMPLEMENT_PARSERS(Voice);
IMPLEMENT_PARSERS(WebAppData);
IMPLEMENT_PARSERS(WebAppInfo);
IMPLEMENT_PARSERS(WebhookInfo);
IMPLEMENT_PARSERS(WriteAccessAllowed);
IMPLEMENT_PARSERS(AffiliateInfo);
IMPLEMENT_PARSERS(AcceptedGiftTypes);
IMPLEMENT_PARSERS(BackgroundFill);
IMPLEMENT_PARSERS(BackgroundFillFreeformGradient);
IMPLEMENT_PARSERS(BackgroundFillGradient);
IMPLEMENT_PARSERS(BackgroundFillSolid);
IMPLEMENT_PARSERS(BackgroundType);
IMPLEMENT_PARSERS(BackgroundTypeChatTheme);
IMPLEMENT_PARSERS(BackgroundTypeFill);
IMPLEMENT_PARSERS(BackgroundTypePattern);
IMPLEMENT_PARSERS(BackgroundTypeWallpaper);
IMPLEMENT_PARSERS(BusinessBotRights);
IMPLEMENT_PARSERS(ChatBackground);
IMPLEMENT_PARSERS(ChatFullInfo);
IMPLEMENT_PARSERS(ChatOwnerChanged);
IMPLEMENT_PARSERS(ChatOwnerLeft);
IMPLEMENT_PARSERS(Checklist);
IMPLEMENT_PARSERS(ChecklistTask);
IMPLEMENT_PARSERS(ChecklistTasksAdded);
IMPLEMENT_PARSERS(ChecklistTasksDone);
IMPLEMENT_PARSERS(CopyTextButton);
IMPLEMENT_PARSERS(DirectMessagePriceChanged);
IMPLEMENT_PARSERS(DirectMessagesTopic);
IMPLEMENT_PARSERS(Gift);
IMPLEMENT_PARSERS(GiftBackground);
IMPLEMENT_PARSERS(GiftInfo);
IMPLEMENT_PARSERS(Gifts);
IMPLEMENT_PARSERS(InputChecklist);
IMPLEMENT_PARSERS(InputChecklistTask);
IMPLEMENT_PARSERS(InputPaidMediaPhoto);
IMPLEMENT_PARSERS(InputPaidMediaVideo);
IMPLEMENT_PARSERS(InputPollOption);
IMPLEMENT_PARSERS(InputProfilePhoto);
IMPLEMENT_PARSERS(InputProfilePhotoAnimated);
IMPLEMENT_PARSERS(InputProfilePhotoStatic);
IMPLEMENT_PARSERS(InputStoryContent);
IMPLEMENT_PARSERS(InputStoryContentPhoto);
IMPLEMENT_PARSERS(InputStoryContentVideo);
IMPLEMENT_PARSERS(LocationAddress);
IMPLEMENT_PARSERS(OwnedGift);
IMPLEMENT_PARSERS(OwnedGiftRegular);
IMPLEMENT_PARSERS(OwnedGifts);
IMPLEMENT_PARSERS(OwnedGiftUnique);
IMPLEMENT_PARSERS(PaidMedia);
IMPLEMENT_PARSERS(PaidMediaInfo);
IMPLEMENT_PARSERS(PaidMediaPhoto);
IMPLEMENT_PARSERS(PaidMediaPreview);
IMPLEMENT_PARSERS(PaidMediaPurchased);
IMPLEMENT_PARSERS(PaidMediaVideo);
IMPLEMENT_PARSERS(PaidMessagePriceChanged);
IMPLEMENT_PARSERS(PreparedInlineMessage);
IMPLEMENT_PARSERS(ReactionTypePaid);
IMPLEMENT_PARSERS(RefundedPayment);
IMPLEMENT_PARSERS(RevenueWithdrawalState);
IMPLEMENT_PARSERS(RevenueWithdrawalStateFailed);
IMPLEMENT_PARSERS(RevenueWithdrawalStatePending);
IMPLEMENT_PARSERS(RevenueWithdrawalStateSucceeded);
IMPLEMENT_PARSERS(StarAmount);
IMPLEMENT_PARSERS(StarTransactions);
IMPLEMENT_PARSERS(StoryArea);
IMPLEMENT_PARSERS(StoryAreaPosition);
IMPLEMENT_PARSERS(StoryAreaType);
IMPLEMENT_PARSERS(StoryAreaTypeLink);
IMPLEMENT_PARSERS(StoryAreaTypeLocation);
IMPLEMENT_PARSERS(StoryAreaTypeSuggestedReaction);
IMPLEMENT_PARSERS(StoryAreaTypeUniqueGift);
IMPLEMENT_PARSERS(StoryAreaTypeWeather);
IMPLEMENT_PARSERS(SuggestedPostApprovalFailed);
IMPLEMENT_PARSERS(SuggestedPostApproved);
IMPLEMENT_PARSERS(SuggestedPostDeclined);
IMPLEMENT_PARSERS(SuggestedPostInfo);
IMPLEMENT_PARSERS(SuggestedPostPaid);
IMPLEMENT_PARSERS(SuggestedPostParameters);
IMPLEMENT_PARSERS(SuggestedPostPrice);
IMPLEMENT_PARSERS(SuggestedPostRefunded);
IMPLEMENT_PARSERS(TransactionPartner);
IMPLEMENT_PARSERS(TransactionPartnerAffiliateProgram);
IMPLEMENT_PARSERS(TransactionPartnerChat);
IMPLEMENT_PARSERS(TransactionPartnerFragment);
IMPLEMENT_PARSERS(TransactionPartnerOther);
IMPLEMENT_PARSERS(TransactionPartnerTelegramAds);
IMPLEMENT_PARSERS(TransactionPartnerTelegramApi);
IMPLEMENT_PARSERS(TransactionPartnerUser);
IMPLEMENT_PARSERS(UniqueGift);
IMPLEMENT_PARSERS(UniqueGiftBackdrop);
IMPLEMENT_PARSERS(UniqueGiftBackdropColors);
IMPLEMENT_PARSERS(UniqueGiftColors);
IMPLEMENT_PARSERS(UniqueGiftInfo);
IMPLEMENT_PARSERS(UniqueGiftModel);
IMPLEMENT_PARSERS(UniqueGiftSymbol);
IMPLEMENT_PARSERS(UserProfileAudios);
IMPLEMENT_PARSERS(UserRating);
IMPLEMENT_PARSERS(VideoQuality);

MaybeInaccessibleMessage parse(const nlohmann::json& data);
template <>
nlohmann::json put(const MaybeInaccessibleMessage& value);

}  // namespace TgBot

#endif  // TGBOT_TGTYPEPARSER_H
