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

#include "tgbot/types/fwd.h"
#include "tgbot/types/MaybeInaccessibleMessage.h"


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

// Always-false helper so a fallback template can static_assert only when it is
// actually instantiated (i.e. for an unregistered type).
template <typename>
inline constexpr bool always_false_v = false;

}  // namespace detail

// Parse function for shared_ptr<T>. This primary template is only selected when
// no IMPLEMENT_PARSERS(T) specialization exists for T; emit a readable error
// instead of the cryptic "use of deleted function".
template <typename T>
std::shared_ptr<T> parse(const nlohmann::json & /*data*/) {
    static_assert(detail::always_false_v<T>,
                  "No JSON parser registered for this type. Declare it with "
                  "IMPLEMENT_PARSERS(T) (and define the parser in src/types).");
    return nullptr;
}

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

// Put function for objects to JSON. This primary template is only selected when
// no IMPLEMENT_PARSERS(T) specialization exists for T; emit a readable error
// instead of the cryptic "use of deleted function".
template <typename T>
nlohmann::json put(const T & /*value*/) {
    static_assert(detail::always_false_v<T>,
                  "No JSON serializer registered for this type. Declare it with "
                  "IMPLEMENT_PARSERS(T) (and define the serializer in "
                  "src/types).");
    return {};
}

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
    void put(const std::string_view key, const std::shared_ptr<T>& value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Support for vector of primitives and objects
    template <typename T>
    void put(const std::string_view key, const std::vector<T>& value) {
        data_[std::string(key)] = TgBot::put(value);
    }

    // Overload for optional types
    template <typename T,
        std::enable_if_t<detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, const std::optional<T>& value) {
        if (!value) {
            return;  // Skip empty optional
        }
        data_[std::string(key)] = *value;
    }
    template <typename T,
        std::enable_if_t<!detail::is_primitive_v<T>, bool> = true>
    void put(const std::string_view key, const std::optional<T>& value) {
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


// Declares the parse/put specializations for a registered type. Spelled as
// std::shared_ptr<type> rather than type::Ptr so the declaration only needs a
// forward declaration of the type (the two spellings denote the same type).
#define IMPLEMENT_PARSERS(type)                              \
    template <>                                              \
    std::shared_ptr<type> parse(const nlohmann::json &data); \
    template <>                                              \
    nlohmann::json put(const std::shared_ptr<type> &object)
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
IMPLEMENT_PARSERS(InputPaidMedia);
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

IMPLEMENT_PARSERS(RichText);
IMPLEMENT_PARSERS(RichTextBold);
IMPLEMENT_PARSERS(RichTextItalic);
IMPLEMENT_PARSERS(RichTextUnderline);
IMPLEMENT_PARSERS(RichTextStrikethrough);
IMPLEMENT_PARSERS(RichTextSpoiler);
IMPLEMENT_PARSERS(RichTextDateTime);
IMPLEMENT_PARSERS(RichTextTextMention);
IMPLEMENT_PARSERS(RichTextSubscript);
IMPLEMENT_PARSERS(RichTextSuperscript);
IMPLEMENT_PARSERS(RichTextMarked);
IMPLEMENT_PARSERS(RichTextCode);
IMPLEMENT_PARSERS(RichTextCustomEmoji);
IMPLEMENT_PARSERS(RichTextMathematicalExpression);
IMPLEMENT_PARSERS(RichTextUrl);
IMPLEMENT_PARSERS(RichTextEmailAddress);
IMPLEMENT_PARSERS(RichTextPhoneNumber);
IMPLEMENT_PARSERS(RichTextBankCardNumber);
IMPLEMENT_PARSERS(RichTextMention);
IMPLEMENT_PARSERS(RichTextHashtag);
IMPLEMENT_PARSERS(RichTextCashtag);
IMPLEMENT_PARSERS(RichTextBotCommand);
IMPLEMENT_PARSERS(RichTextAnchor);
IMPLEMENT_PARSERS(RichTextAnchorLink);
IMPLEMENT_PARSERS(RichTextReference);
IMPLEMENT_PARSERS(RichTextReferenceLink);

IMPLEMENT_PARSERS(RichBlock);
IMPLEMENT_PARSERS(RichBlockParagraph);
IMPLEMENT_PARSERS(RichBlockSectionHeading);
IMPLEMENT_PARSERS(RichBlockPreformatted);
IMPLEMENT_PARSERS(RichBlockFooter);
IMPLEMENT_PARSERS(RichBlockDivider);
IMPLEMENT_PARSERS(RichBlockMathematicalExpression);
IMPLEMENT_PARSERS(RichBlockAnchor);
IMPLEMENT_PARSERS(RichBlockList);
IMPLEMENT_PARSERS(RichBlockBlockQuotation);
IMPLEMENT_PARSERS(RichBlockPullQuotation);
IMPLEMENT_PARSERS(RichBlockCollage);
IMPLEMENT_PARSERS(RichBlockSlideshow);
IMPLEMENT_PARSERS(RichBlockTable);
IMPLEMENT_PARSERS(RichBlockDetails);
IMPLEMENT_PARSERS(RichBlockMap);
IMPLEMENT_PARSERS(RichBlockAnimation);
IMPLEMENT_PARSERS(RichBlockAudio);
IMPLEMENT_PARSERS(RichBlockPhoto);
IMPLEMENT_PARSERS(RichBlockVideo);
IMPLEMENT_PARSERS(RichBlockVoiceNote);
IMPLEMENT_PARSERS(RichBlockThinking);
IMPLEMENT_PARSERS(RichBlockCaption);
IMPLEMENT_PARSERS(RichBlockTableCell);
IMPLEMENT_PARSERS(RichBlockListItem);

IMPLEMENT_PARSERS(RichMessage);
IMPLEMENT_PARSERS(InputRichMessage);
IMPLEMENT_PARSERS(InputRichMessageContent);

IMPLEMENT_PARSERS(LivePhoto);
IMPLEMENT_PARSERS(PaidMediaLivePhoto);
IMPLEMENT_PARSERS(Link);
IMPLEMENT_PARSERS(PollMedia);
IMPLEMENT_PARSERS(ManagedBotCreated);
IMPLEMENT_PARSERS(ManagedBotUpdated);
IMPLEMENT_PARSERS(PollOptionAdded);
IMPLEMENT_PARSERS(PollOptionDeleted);
IMPLEMENT_PARSERS(KeyboardButtonRequestManagedBot);
IMPLEMENT_PARSERS(BotAccessSettings);
IMPLEMENT_PARSERS(SentGuestMessage);
IMPLEMENT_PARSERS(PreparedKeyboardButton);
IMPLEMENT_PARSERS(InputPaidMediaLivePhoto);

IMPLEMENT_PARSERS(InputMediaLink);
IMPLEMENT_PARSERS(InputMediaSticker);
IMPLEMENT_PARSERS(InputMediaLocation);
IMPLEMENT_PARSERS(InputMediaVenue);
IMPLEMENT_PARSERS(InputMediaLivePhoto);

MaybeInaccessibleMessage parse(const nlohmann::json& data);
template <>
nlohmann::json put(const MaybeInaccessibleMessage& value);

}  // namespace TgBot

#endif  // TGBOT_TGTYPEPARSER_H
