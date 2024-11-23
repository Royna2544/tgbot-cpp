#ifndef _CRLF_HELPER_H
#define _CRLF_HELPER_H

#ifndef INCLUDING_CRLF
#error "Do not directly include this header"
#endif

#include <ostream>

namespace detail {
struct CRLF_T {};
constexpr CRLF_T CRLF{};
}  // namespace detail

inline std::ostream& operator<<(std::ostream& stream,
                                const detail::CRLF_T& /*unused*/) {
    return stream << "\r\n";
}

#endif // _CRLF_HELPER_H