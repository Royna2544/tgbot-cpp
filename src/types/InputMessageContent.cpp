#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InputMessageContent) {
    if (data.contains("message_text")) {
        return parse<InputTextMessageContent>(data);
    }
    if (data.contains("payload") && data.contains("currency") && data.contains("prices")) {
        return parse<InputInvoiceMessageContent>(data);
    }
    if (data.contains("phone_number") && data.contains("first_name")) {
        return parse<InputContactMessageContent>(data);
    }
    if (data.contains("latitude") && data.contains("longitude")) {
        if (data.contains("title") && data.contains("address")) {
            return parse<InputVenueMessageContent>(data);
        }
        return parse<InputLocationMessageContent>(data);
    }
    throw invalidType("InputMessageContent", "unknown");
}

DECLARE_PARSER_TO_JSON(InputMessageContent) {
    JsonWrapper json;
    if (object) {
        if (std::dynamic_pointer_cast<InputTextMessageContent>(object)) {
            return put(std::dynamic_pointer_cast<InputTextMessageContent>(object));
        }
        if (std::dynamic_pointer_cast<InputLocationMessageContent>(object)) {
            return put(std::dynamic_pointer_cast<InputLocationMessageContent>(object));
        }
        if (std::dynamic_pointer_cast<InputVenueMessageContent>(object)) {
            return put(std::dynamic_pointer_cast<InputVenueMessageContent>(object));
        }
        if (std::dynamic_pointer_cast<InputContactMessageContent>(object)) {
            return put(std::dynamic_pointer_cast<InputContactMessageContent>(object));
        }
        if (std::dynamic_pointer_cast<InputInvoiceMessageContent>(object)) {
            return put(std::dynamic_pointer_cast<InputInvoiceMessageContent>(object));
        }
        throw invalidType("InputMessageContent", "unknown");
    }
    return json;
}

} // namespace TgBot
