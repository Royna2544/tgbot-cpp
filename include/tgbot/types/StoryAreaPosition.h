#ifndef TGBOT_STORYAREAPOSITION_H
#define TGBOT_STORYAREAPOSITION_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TgBot {

/**
 * @brief Describes the position of a clickable area within a story.
 *
 * @ingroup types
 */
class StoryAreaPosition {
public:
    using Ptr = std::shared_ptr<StoryAreaPosition>;

    /**
     * @brief The abscissa of the area's center, as a percentage of the media width
     */
    float xPercentage;

    /**
     * @brief The ordinate of the area's center, as a percentage of the media height
     */
    float yPercentage;

    /**
     * @brief The width of the area's rectangle, as a percentage of the media width
     */
    float widthPercentage;

    /**
     * @brief The height of the area's rectangle, as a percentage of the media height
     */
    float heightPercentage;

    /**
     * @brief The clockwise rotation angle of the rectangle, in degrees; 0-360
     */
    float rotationAngle;

    /**
     * @brief The radius of the rectangle corner rounding, as a percentage of the media width
     */
    float cornerRadiusPercentage;

};
}

#endif //TGBOT_STORYAREAPOSITION_H
