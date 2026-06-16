#include <tgbot/TgTypeParser.h>
#include <tgbot/types/WebhookInfo.h>
#include <nlohmann/json.hpp>

namespace TgBot {

template <>
std::shared_ptr<WebhookInfo> parse(const nlohmann::json &data) {
    auto result = std::make_shared<WebhookInfo>();
    parse(data, "url", &result->url);
    parse(data, "has_custom_certificate", &result->hasCustomCertificate);
    parse(data, "pending_update_count", &result->pendingUpdateCount);
    parse(data, "ip_address", &result->ipAddress);
    parse(data, "last_error_date", &result->lastErrorDate);
    parse(data, "last_error_message", &result->lastErrorMessage);
    parse(data, "last_synchronization_error_date", &result->lastSynchronizationErrorDate);
    parse(data, "max_connections", &result->maxConnections);
    result->allowedUpdates = parsePrimitiveArray<std::string>(data, "allowed_updates");
    return result;
}

template <>
nlohmann::json put(const std::shared_ptr<WebhookInfo> &object) {
    JsonWrapper json;
    if (object) {
        json.put("url", object->url);
        json.put("has_custom_certificate", object->hasCustomCertificate);
        json.put("pending_update_count", object->pendingUpdateCount);
        json.put("ip_address", object->ipAddress);
        json.put("last_error_date", object->lastErrorDate);
        json.put("last_error_message", object->lastErrorMessage);
        json.put("last_synchronization_error_date", object->lastSynchronizationErrorDate);
        json.put("max_connections", object->maxConnections);
        json.put("allowed_updates", object->allowedUpdates);
    }
    return json;
}

} // namespace TgBot
