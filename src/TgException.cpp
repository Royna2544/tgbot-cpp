#include "tgbot/TgException.h"

#include <sstream>
#include <string>

namespace TgBot {

TgException::TgException(const std::string& description, ErrorCode errorCode)
    : runtime_error(description), errorCode(errorCode) {}

TgException TgException::invalidType(const std::string_view name,
                                     const std::string_view type) {
    std::stringstream ss;
    ss << "Invalid type for " << name << ": " << type;
    return TgException(ss.str(), ErrorCode::Internal);
}

}  // namespace TgBot
