#include <tgbot/TgTypeParser.h>
#include <nlohmann/json.hpp>

namespace TgBot {

DECLARE_PARSER_FROM_JSON(InlineKeyboardButton) {
    auto result = std::make_shared<InlineKeyboardButton>();
    parse(data, "text", &result->text);
    parse(data, "icon_custom_emoji_id", &result->iconCustomEmojiId);
    parse(data, "style", &result->style);
    parse(data, "url", &result->url);
    parse(data, "callback_data", &result->callbackData);
    result->webApp = parse<WebAppInfo>(data, "web_app");
    result->loginUrl = parse<LoginUrl>(data, "login_url");
    parse(data, "switch_inline_query", &result->switchInlineQuery);
    parse(data, "switch_inline_query_current_chat", &result->switchInlineQueryCurrentChat);
    result->switchInlineQueryChosenChat = parse<SwitchInlineQueryChosenChat>(data, "switch_inline_query_chosen_chat");
    result->copyText = parse<CopyTextButton>(data, "copy_text");
    result->callbackGame = parse<CallbackGame>(data, "callback_game");
    parse(data, "pay", &result->pay);
    return result;
}

DECLARE_PARSER_TO_JSON(InlineKeyboardButton) {
    JsonWrapper json;
    if (object) {
        json.put("text", object->text);
        json.put("icon_custom_emoji_id", object->iconCustomEmojiId);
        json.put("style", object->style);
        json.put("url", object->url);
        json.put("callback_data", object->callbackData);
        json.put("web_app", object->webApp);
        json.put("login_url", object->loginUrl);
        json.put("switch_inline_query", object->switchInlineQuery);
        json.put("switch_inline_query_current_chat", object->switchInlineQueryCurrentChat);
        json.put("switch_inline_query_chosen_chat", object->switchInlineQueryChosenChat);
        json.put("copy_text", object->copyText);
        json.put("callback_game", object->callbackGame);
        json.put("pay", object->pay);
    }
    return json;
}

} // namespace TgBot
