#ifndef TGBOT_HTTPPARAMETER_H
#define TGBOT_HTTPPARAMETER_H

#ifdef HAVE_CURL
#include <curl/curl.h>
#endif /* HAVE_CURL */

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "tgbot/export.h"
#include "tgbot/types/InputFile.h"

namespace TgBot {

namespace detail {
struct CRLF_T {};
constexpr CRLF_T CRLF{};
}  // namespace detail

inline std::ostream& operator<<(std::ostream& stream,
                                const detail::CRLF_T& /*unused*/) {
    return stream << "\r\n";
}

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

#ifdef HAVE_CURL
    /**
     * @brief Constructs curl_mime* unique_ptr for this argument.
     */
    virtual void update_mime_part(curl_mimepart* part) const {
        curl_mime_data(part, value.c_str(), value.size());
        curl_mime_type(part, "text/plain");
        curl_mime_name(part, name.c_str());
    }
#endif

    [[nodiscard]] virtual std::string create_mime_part(
        const std::string_view boundary) const {
        std::stringstream stream;
        stream << "--" << boundary << detail::CRLF;
        stream << "Content-Disposition: form-data; name=" << std::quoted(name)
               << detail::CRLF << detail::CRLF;
        stream << value << detail::CRLF;
        return stream.str();
    }

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

#ifdef HAVE_CURL
    void update_mime_part(curl_mimepart* part) const override {
        curl_mime_data(part, value.c_str(), value.size());
        curl_mime_type(part, mimeType.c_str());
        curl_mime_name(part, name.c_str());
        curl_mime_filename(part, fileName.c_str());
    }
#endif

    [[nodiscard]] std::string create_mime_part(
        const std::string_view boundary) const override {
        std::stringstream stream;
        stream << "--" << boundary << detail::CRLF;
        stream << "Content-Disposition: form-data; name=" << std::quoted(name)
               << "; filename=" << std::quoted(fileName) << detail::CRLF;
        stream << "Content-Type: " << std::quoted(mimeType) << detail::CRLF
               << detail::CRLF;
        stream << value << detail::CRLF;
        return stream.str();
    }

    std::ostream& print(std::ostream& stream) const override {
        return stream << name << "= <file:" << fileName << ">";
    }

    [[nodiscard]] bool isFile() const noexcept override { return true; }
};
}  // namespace TgBot

#endif  // TGBOT_HTTPPARAMETER_H
