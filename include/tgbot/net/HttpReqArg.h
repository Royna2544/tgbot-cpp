#ifndef TGBOT_HTTPPARAMETER_H
#define TGBOT_HTTPPARAMETER_H

#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "tgbot/export.h"
#include "tgbot/types/InputFile.h"

namespace TgBot {

/**
 * @brief This class represents argument in POST http requests.
 *
 * @ingroup net
 */
class TGBOT_API HttpReqArg {
   public:
    virtual ~HttpReqArg() = default;
    using Vec = std::vector<std::unique_ptr<HttpReqArg>>;

    /**
     * @brief Constructs an argument from a integer or floating point value.
     */
    template <typename IntT,
              typename std::enable_if_t<std::is_integral_v<IntT> ||
                                            std::is_floating_point_v<IntT>,
                                        bool> = true>
    HttpReqArg(std::string name, IntT value)
        : name(std::move(name)), value(std::to_string(value)) {}

    /**
     * @brief Constructs an argument from a stringlike value.
     */
    HttpReqArg(std::string name, std::string_view value)
        : name(std::move(name)), value(value) {}

    /**
     * @brief Name of an argument.
     */
    std::string name;

    /**
     * @brief Value of an argument.
     */
    std::string value;

    virtual std::ostream& print(std::ostream& stream) const {
        return stream << name << "=" << value;
    }

    [[nodiscard]] virtual bool isFile() const noexcept { return false; }
};

/**
 * @brief This class represents argument in POST http requests. Where it is a
 * file.
 *
 * @ingroup net
 */
class TGBOT_API HttpReqArgFile : public HttpReqArg {
   public:
    // Take InputFile::Ptr
    HttpReqArgFile(std::string name, const InputFile::Ptr& file)
        : HttpReqArg(std::move(name), file->data),
          mimeType(file->mimeType),
          fileName(file->fileName) {}

    // Take all sperate arguments
    HttpReqArgFile(std::string name, std::string data, std::string mimeType,
                   std::string fileName)
        : HttpReqArg(std::move(name), std::move(data)),
          mimeType(std::move(mimeType)),
          fileName(std::move(fileName)) {}

    ~HttpReqArgFile() override = default;

    /**
     * @brief Mime type of an argument value.
     */
    std::string mimeType;

    /**
     * @brief Should be set if an argument value hold some file contents
     */
    std::string fileName;

    std::ostream& print(std::ostream& stream) const override {
        return stream << name << "= <file:" << fileName << ">";
    }

    [[nodiscard]] bool isFile() const noexcept override { return true; }
};
}  // namespace TgBot

#endif  // TGBOT_HTTPPARAMETER_H
