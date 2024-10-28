#ifndef TGBOT_HTTPPARAMETER_H
#define TGBOT_HTTPPARAMETER_H

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

#include "tgbot/export.h"

namespace TgBot {

/**
 * @brief This class represents argument in POST http requests.
 *
 * @ingroup net
 */
class TGBOT_API HttpReqArg {
   public:
    template <typename IntT,
              typename std::enable_if_t<std::is_integral_v<IntT> ||
                                            std::is_floating_point_v<IntT>,
                                        bool> = true>
    HttpReqArg(std::string name,
               const std::variant<IntT, std::string>& value,
               bool isFile = false, std::string mimeType = "text/plain",
               std::string fileName = "")
        : name(std::move(name)),
          value(std::visit(
            [](const auto& v) -> std::string {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_integral_v<T>) {
                    return std::to_string(v);
                } else if constexpr (std::is_same_v<std::string, T>) {
                    return v;
                }
            },
            value)), isFile(isFile),
          mimeType(std::move(mimeType)),
          fileName(std::move(fileName)) {
        
    }

    template <typename IntT,
              typename std::enable_if_t<std::is_integral_v<IntT> ||
                                            std::is_floating_point_v<IntT>,
                                        bool> = true>
    HttpReqArg(std::string name, IntT value, bool isFile = false,
               std::string mimeType = "text/plain", std::string fileName = "")
        : name(std::move(name)),
          value(std::to_string(value)),
          isFile(isFile),
          mimeType(std::move(mimeType)),
          fileName(std::move(fileName)) {}

    template <typename StrT,
              typename std::enable_if_t<
                  std::is_constructible_v<std::string, StrT>, bool> = true>
    HttpReqArg(std::string name, StrT value, bool isFile = false,
               std::string mimeType = "text/plain", std::string fileName = "")
        : name(std::move(name)),
          value(value),
          isFile(isFile),
          mimeType(std::move(mimeType)),
          fileName(std::move(fileName)) {}

    /**
     * @brief Name of an argument.
     */
    std::string name;

    /**
     * @brief Value of an argument.
     */
    std::string value;

    /**
     * @brief Should be true if an argument value hold some file contents
     */
    bool isFile = false;

    /**
     * @brief Mime type of an argument value. This field makes sense only if
     * isFile is true.
     */
    std::string mimeType = "text/plain";

    /**
     * @brief Should be set if an argument value hold some file contents
     */
    std::string fileName;
};

}  // namespace TgBot

#endif  // TGBOT_HTTPPARAMETER_H
