#ifndef TGBOT_CPP_INPUTFILE_H
#define TGBOT_CPP_INPUTFILE_H

#include "tgbot/export.h"

#include <iterator>
#include <memory>
#include <string>
#include <filesystem>
#include <fstream>
#include <utility>

namespace TgBot {

/**
 * @brief This object represents the contents of a file to be uploaded.
 *
 * @ingroup types
 */
class TGBOT_API InputFile {

public:
    typedef std::shared_ptr<InputFile> Ptr;

    /**
     * @brief Contents of a file.
     */
    std::string data;

    /**
     * @brief Mime type of a file.
     */
    std::string mimeType;

    /**
     * @brief File name.
     */
    std::string fileName;

    /**
     * @brief Creates new InputFile::Ptr from an existing file.
     */
    static InputFile::Ptr fromFile(const std::filesystem::path& filePath, std::string mimeType) {
        auto result(std::make_shared<InputFile>());
        std::ifstream file(filePath);
        result->data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        result->mimeType = std::move(mimeType);
        result->fileName = filePath.filename().string();
        return result;
    }
};

}

#endif //TGBOT_CPP_INPUTFILE_H
