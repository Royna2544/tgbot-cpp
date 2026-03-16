#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputInvoiceMessageContent) {
    auto result = std::make_shared<InputInvoiceMessageContent>();
    parse(data, "title", &result->title);
    parse(data, "description", &result->description);
    parse(data, "payload", &result->payload);
    parse(data, "provider_token", &result->providerToken);
    parse(data, "currency", &result->currency);
    result->prices = parseRequiredArray<LabeledPrice>(data, "prices");
    parse(data, "max_tip_amount", &result->maxTipAmount);
    result->suggestedTipAmounts = parsePrimitiveArray<std::int32_t>(data, "suggested_tip_amounts");
    parse(data, "provider_data", &result->providerData);
    parse(data, "photo_url", &result->photoUrl);
    parse(data, "photo_size", &result->photoSize);
    parse(data, "photo_width", &result->photoWidth);
    parse(data, "photo_height", &result->photoHeight);
    parse(data, "need_name", &result->needName);
    parse(data, "need_phone_number", &result->needPhoneNumber);
    parse(data, "need_email", &result->needEmail);
    parse(data, "need_shipping_address", &result->needShippingAddress);
    parse(data, "send_phone_number_to_provider", &result->sendPhoneNumberToProvider);
    parse(data, "send_email_to_provider", &result->sendEmailToProvider);
    parse(data, "is_flexible", &result->isFlexible);
    return result;
}

DECLARE_PARSER_TO_JSON(InputInvoiceMessageContent) {
    JsonWrapper json;
    if (object) {
        json.put("title", object->title);
        json.put("description", object->description);
        json.put("payload", object->payload);
        json.put("provider_token", object->providerToken);
        json.put("currency", object->currency);
        json.put("prices", object->prices);
        json.put("max_tip_amount", object->maxTipAmount);
        json.put("suggested_tip_amounts", object->suggestedTipAmounts);
        json.put("provider_data", object->providerData);
        json.put("photo_url", object->photoUrl);
        json.put("photo_size", object->photoSize);
        json.put("photo_width", object->photoWidth);
        json.put("photo_height", object->photoHeight);
        json.put("need_name", object->needName);
        json.put("need_phone_number", object->needPhoneNumber);
        json.put("need_email", object->needEmail);
        json.put("need_shipping_address", object->needShippingAddress);
        json.put("send_phone_number_to_provider", object->sendPhoneNumberToProvider);
        json.put("send_email_to_provider", object->sendEmailToProvider);
        json.put("is_flexible", object->isFlexible);
    }
    return json;
}

} // namespace TgBot
