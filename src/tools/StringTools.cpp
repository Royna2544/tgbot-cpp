#include "tgbot/tools/StringTools.h"

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iomanip>
#include <cstdio>
#include <random>
#include <string>

namespace StringTools {

bool startsWith(const std::string& str1, const std::string& str2) {
    return boost::starts_with(str1, str2);
}

bool endsWith(const std::string& str1, const std::string& str2) {
    return boost::ends_with(str1, str2);
}

void split(const std::string& str, char delimiter, std::vector<std::string>& dest) {
    boost::split(dest, str, [delimiter](char c) { return c == delimiter; });
}

std::string generateRandomString(std::size_t length) {
    static constexpr std::string_view chars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890-=[]\\',./!@#$%^&*()_+{}|:\"<>?`~";
    std::string result;

    result.reserve(length);
    std::generate_n(std::back_inserter(result), length, [&] {
        std::mt19937 gen(std::random_device{}());
        return chars[std::uniform_int_distribution<std::size_t>(0, chars.size() - 1)(gen)];
    });
    return result;
}


std::string urlEncode(const std::string& value, const std::string& additionalLegitChars) {
    static constexpr std::string_view legitPunctuation = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.-~:";
    std::stringstream ss;
    for (auto const &c : value) {
        if ((legitPunctuation.find(c) == std::string::npos) && (additionalLegitChars.find(c) == std::string::npos)) {
            ss << '%' << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)(unsigned char)c;
        } else {
            ss << c;
        }
    }

    return ss.str();
}

std::string urlDecode(const std::string& value) {
    std::string result;
    for (std::size_t i = 0, count = value.length(); i < count; ++i) {
        const char c = value[i];
        if (c == '%') {
            int t = stoi(value.substr(i + 1, 2), nullptr, 16);
            result += (char) t;
            i += 2;
        } else {
            result += c;
        }
    }
    return result;
}

}
