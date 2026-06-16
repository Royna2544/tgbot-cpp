#!/usr/bin/env python3
"""Generate type headers (include/tgbot/types/<Type>.h) from api.json.

Companion to gen_parsers.py (which generates the .cpp). Used to bring in new
spec types without hand-writing headers. The spec doesn't express C++ int width
or which String fields are enums, so:
  - Integer -> std::int64_t for id-like / "more than 32 bits" fields, else int32,
  - enum modelling is out of scope (those few types stay hand-written).

    python scripts/gen_types.py --only LivePhoto,Link        # print to stdout
    python scripts/gen_types.py --new --write                # write new types
"""
import argparse
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HDR_DIR = os.path.join(ROOT, "include", "tgbot", "types")


# The spec splits input media across three overlapping "bases" (InputMedia,
# InputPollMedia, InputPollOptionMedia) that share leaves -- the same leaf
# (e.g. InputMediaPhoto) belongs to several. Since these are put-only (never
# parsed) and serve only as a method-parameter type, we collapse the two poll
# bases into InputMedia (runtime-validated): every leaf inherits InputMedia and
# any field/param typed as a poll base is spelled InputMedia. See InputMedia.cpp.
MEDIA_COLLAPSE = {"InputPollMedia": "InputMedia",
                  "InputPollOptionMedia": "InputMedia"}


def snake_to_camel(s):
    p = s.split("_")
    return p[0] + "".join(x.title() for x in p[1:])


def _desc(field):
    d = field.get("description", "")
    return " ".join(d) if isinstance(d, list) else d


def is_int64(field):
    # The spec doesn't give int width; the big-int fields (user/chat id,
    # file_size, ...) all carry a "significant bits" note. message_id and dates
    # do not, so they stay int32 -- matching the hand-written headers.
    d = _desc(field)
    return any(s in d for s in ("32 significant bits", "52 significant bits",
                                "64-bit"))


def elem_type(t, spec):
    """C++ element type for a spec type token (object/primitive/array)."""
    if t == "String":
        return "std::string"
    if t == "Boolean" or t == "True":
        return "bool"
    if t in ("Float", "Float number"):
        return "float"
    if t == "Integer":
        return "std::int64_t"
    if t.startswith("Array of "):
        return f"std::vector<{elem_type(t[len('Array of '):], spec)}>"
    if t == "MaybeInaccessibleMessage":
        return "MaybeInaccessibleMessage"  # a std::variant alias, not a ::Ptr
    t = MEDIA_COLLAPSE.get(t, t)  # poll media bases collapse into InputMedia
    return f"{t}::Ptr"  # object type


def field_cpp_type(field, spec):
    t = field["types"][0]
    if t == "Integer":
        cpp = "std::int64_t" if is_int64(field) else "std::int32_t"
    else:
        cpp = elem_type(t, spec)
    if not field.get("required", False):
        cpp = f"std::optional<{cpp}>"
    return cpp


def referenced_types(cpp_type):
    refs = set(re.findall(r"([A-Z]\w*)::Ptr", cpp_type))
    if "MaybeInaccessibleMessage" in cpp_type:
        refs.add("MaybeInaccessibleMessage")
    return refs


def discriminator(spec, name):
    """(field_name, value) for a subtype, from its own type field description."""
    for f in spec["types"][name].get("fields") or []:
        m = re.search(r'(?:always|must be)\s+"?([\w-]+)"?', _desc(f))
        if m:
            return f["name"], m.group(1)
    return None, None


def gen_header(spec, name):
    info = spec["types"][name]
    fields = info.get("fields") or []
    base = (info.get("subtype_of") or [None])[0]
    base = MEDIA_COLLAPSE.get(base, base)  # leaves of poll bases inherit InputMedia
    is_base = bool(info.get("subtypes"))

    disc_field = None
    if base:
        disc_field, disc_val = discriminator(spec, name)

    guard = f"TGBOT_{name.upper()}_H"
    std_inc, type_inc = set(), set()
    members = []  # (cpp_type, camel, doc, required)
    for f in fields:
        if base and f["name"] == disc_field:
            continue  # discriminator lives on the base
        cpp = field_cpp_type(f, spec)
        std_inc.add("memory")
        if "std::int" in cpp:
            std_inc.add("cstdint")
        if "std::string" in cpp:
            std_inc.add("string")
        if "std::optional" in cpp:
            std_inc.add("optional")
        if "std::vector" in cpp:
            std_inc.add("vector")
        for r in referenced_types(cpp):
            type_inc.add(r)
        members.append((cpp, snake_to_camel(f["name"]), _desc(f),
                        f.get("required", False)))

    std_inc.add("memory")
    if is_base:
        std_inc.add("string")  # discriminator member
    if base:
        type_inc.add(base)

    out = [f"#ifndef {guard}", f"#define {guard}", ""]
    for h in sorted(type_inc):
        out.append(f'#include "tgbot/types/{h}.h"')
    if type_inc:
        out.append("")
    for h in sorted(std_inc):
        out.append(f"#include <{h}>")
    out += ["", "namespace TgBot {", "", "/**",
            f" * @brief {' '.join(info.get('description', []))}", " *",
            " * @ingroup types", " */"]
    decl = f"class {name}"
    if base:
        decl += f" : public {base}"
    out += [decl + " {", "public:",
            f"    using Ptr = std::shared_ptr<{name}>;", ""]
    if base:
        out += [f'    constexpr static const char* TYPE = "{disc_val}";', "",
                f"    {name}() {{", f"        {disc_field} = TYPE;", "    }", ""]
    if is_base:
        # base carries only the discriminator field (a String)
        df, _ = None, None
        for sub in info["subtypes"]:
            df, _ = discriminator(spec, sub)
            if df:
                break
        out += [f"    std::string {df};", ""]
    for cpp, camel, doc, req in members:
        # The spec already prefixes optional fields' descriptions with "Optional."
        out += ["    /**", f"     * @brief {doc}", "     */",
                f"    {cpp} {camel};", ""]
    out += ["};", "}", "", f"#endif //{guard}", ""]
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--spec", default=os.path.join(ROOT, "api.json"))
    ap.add_argument("--only", help="comma-separated type names")
    ap.add_argument("--new", action="store_true",
                    help="all spec types that have no header yet")
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    spec = json.load(open(args.spec, encoding="utf-8"))

    if args.only:
        names = args.only.split(",")
    elif args.new:
        have = {f[:-2] for f in os.listdir(HDR_DIR) if f.endswith(".h")}
        names = [t for t in spec["types"] if t not in have]
    else:
        names = []

    for name in names:
        h = gen_header(spec, name)
        if args.write:
            open(os.path.join(HDR_DIR, f"{name}.h"), "w", encoding="utf-8",
                 newline="\n").write(h)
            print(f"wrote {name}.h")
        else:
            print(h)


if __name__ == "__main__":
    main()
