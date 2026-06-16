#ifndef TGBOT_POLLMEDIA_H
#define TGBOT_POLLMEDIA_H

#include "tgbot/types/Animation.h"
#include "tgbot/types/Audio.h"
#include "tgbot/types/Document.h"
#include "tgbot/types/Link.h"
#include "tgbot/types/LivePhoto.h"
#include "tgbot/types/Location.h"
#include "tgbot/types/PhotoSize.h"
#include "tgbot/types/Sticker.h"
#include "tgbot/types/Venue.h"
#include "tgbot/types/Video.h"

#include <memory>
#include <optional>
#include <vector>

namespace TgBot {

/**
 * @brief At most one of the optional fields can be present in any given object.
 *
 * @ingroup types
 */
class PollMedia {
public:
    using Ptr = std::shared_ptr<PollMedia>;

    /**
     * @brief Optional. Media is an animation, information about the animation
     */
    std::optional<Animation::Ptr> animation;

    /**
     * @brief Optional. Media is an audio file, information about the file; currently, can't be received in a poll option
     */
    std::optional<Audio::Ptr> audio;

    /**
     * @brief Optional. Media is a general file, information about the file; currently, can't be received in a poll option
     */
    std::optional<Document::Ptr> document;

    /**
     * @brief Optional. The HTTP link attached to the poll option
     */
    std::optional<Link::Ptr> link;

    /**
     * @brief Optional. Media is a live photo, information about the live photo
     */
    std::optional<LivePhoto::Ptr> livePhoto;

    /**
     * @brief Optional. Media is a shared location, information about the location
     */
    std::optional<Location::Ptr> location;

    /**
     * @brief Optional. Media is a photo, available sizes of the photo
     */
    std::optional<std::vector<PhotoSize::Ptr>> photo;

    /**
     * @brief Optional. Media is a sticker, information about the sticker; currently, for poll options only
     */
    std::optional<Sticker::Ptr> sticker;

    /**
     * @brief Optional. Media is a venue, information about the venue
     */
    std::optional<Venue::Ptr> venue;

    /**
     * @brief Optional. Media is a video, information about the video
     */
    std::optional<Video::Ptr> video;

};
}

#endif //TGBOT_POLLMEDIA_H
