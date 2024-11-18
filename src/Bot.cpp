#include "tgbot/ApiImpl.h"
#include "tgbot/net/BoostHttpOnlySslClient.h"
#include "tgbot/Bot.h"

#include "tgbot/EventBroadcaster.h"

#include <memory>
#include <string>

namespace TgBot {

Bot::Bot(std::string token, std::unique_ptr<HttpClient> httpClient, std::string url)
    : _token(std::move(token))
    , _httpClient(std::move(httpClient))
    , _api(std::make_unique<ApiImpl>(_token, _httpClient.get(), std::move(url)))
    , _eventBroadcaster(std::make_unique<EventBroadcaster>())
    , _eventHandler(std::make_unique<EventHandler>(_eventBroadcaster.get())) {
}

std::unique_ptr<HttpClient> Bot::_getDefaultHttpClient() {
    return std::make_unique<BoostHttpOnlySslClient>();
}

}
