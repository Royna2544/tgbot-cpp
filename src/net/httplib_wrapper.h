#ifndef TGBOT_INTERNAL_HTTPLIB_WRAPPER_H
#define TGBOT_INTERNAL_HTTPLIB_WRAPPER_H

// Single inclusion point for the vendored cpp-httplib.
//
// cpp-httplib is header-only and its functions have external linkage (weak /
// COMDAT symbols). If another component in the final program also vendors
// cpp-httplib -- possibly a *different* version, e.g. llama.cpp -- the linker
// merges the two `httplib::...` definitions into one, and code built against
// the other version then constructs/destructs objects with a mismatched
// layout, corrupting the heap.
//
// To stay a good citizen we rename the namespace of *our* copy to a
// tgbot-private one via the preprocessor, so its mangled symbols can never
// collide with anyone else's httplib. The token `httplib` only appears as the
// namespace name and qualified uses inside the header (never inside string
// literals such as the "cpp-httplib" user agent), so a plain macro rename is
// safe.

#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#ifndef CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#endif

#define httplib tgbot_vendored_httplib
#include <httplib.h>
#undef httplib

// Let the rest of the library keep using the familiar `httplib::` spelling.
namespace httplib = tgbot_vendored_httplib;

#endif  // TGBOT_INTERNAL_HTTPLIB_WRAPPER_H
