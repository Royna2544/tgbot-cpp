#ifndef TGBOT_PASSPORTELEMENTERRORSELFIE_H
#define TGBOT_PASSPORTELEMENTERRORSELFIE_H

#include "tgbot/types/PassportElementError.h"

#include <string>
#include <memory>

namespace TgBot {

/**
 * @brief Represents an issue with the selfie with a document.
 * The error is considered resolved when the file with the selfie changes.
 *
 * @ingroup types
 */
class PassportElementErrorSelfie : public PassportElementError {
public:
    static constexpr std::string_view SOURCE = "selfie";

    using Ptr = std::shared_ptr<PassportElementErrorSelfie>;

    PassportElementErrorSelfie() {
        this->source = SOURCE;
    }

    /**
     * @brief Base64-encoded hash of the file with the selfie
     */
    std::string fileHash;
};
}

#endif //TGBOT_PASSPORTELEMENTERRORSELFIE_H
