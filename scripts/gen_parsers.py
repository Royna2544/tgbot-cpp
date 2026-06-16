#!/usr/bin/env python3
"""Phase-1 parser generator.

Generates src/types/<Type>.cpp (the parse/put bodies) from the Telegram Bot API
spec (api.json) plus the hand-written headers (which carry the C++ field types,
i.e. the int-width / optional / enum decisions the spec doesn't express).

This is a prototype: by default it writes nothing, it regenerates each type into
memory and diffs against the committed .cpp, reporting how many reproduce
byte-for-byte. Types whose shape the generator does not yet model (enums, a few
specials) are reported as skipped.

    python scripts/gen_parsers.py            # parity report (no writes)
    python scripts/gen_parsers.py --write    # overwrite the .cpp files
"""
import argparse
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HDR_DIR = os.path.join(ROOT, "include", "tgbot", "types")
SRC_DIR = os.path.join(ROOT, "src", "types")

# Types with bespoke (non-mechanical) parsers; left hand-written and never
# generated. Everything else must match the generator (enforced in CI).
HAND_WRITTEN = {
    # enum-valued fields (need a value<->string mapping)
    "Chat", "MessageEntity", "Sticker", "StickerSet",
    # std::variant (MaybeInaccessibleMessage) members / bespoke logic
    "MaybeInaccessibleMessage", "Message", "CallbackQuery",
    # polymorphic base without a discriminator field (structural dispatch)
    "InputMessageContent",
    # polymorphic base whose discriminator value is ambiguous (cached vs not)
    "InlineQueryResult",
    # recursive variant AST (String | Array of RichText | styled nodes); the
    # base + the two synthetic wrappers are hand-written, the styled subtypes
    # (RichTextBold, ...) are generated normally.
    "RichText",
    # empty / upload-only types
    "InputFile", "CallbackGame",
}


def snake_to_camel(s):
    parts = s.split("_")
    return parts[0] + "".join(p.title() for p in parts[1:])


def strip_comments(code):
    code = re.sub(r"/\*.*?\*/", "", code, flags=re.DOTALL)
    code = re.sub(r"//.*", "", code)
    return code


def header_member_types(type_name, _seen=None):
    """name -> C++ type for the data members of <Type>.h, including inherited
    ones (e.g. the discriminator 'type' declared on a polymorphic base)."""
    if _seen is None:
        _seen = set()
    if type_name in _seen:
        return {}
    _seen.add(type_name)
    path = os.path.join(HDR_DIR, f"{type_name}.h")
    if not os.path.exists(path):
        return {}
    body = strip_comments(open(path, encoding="utf-8").read())
    members = {}
    # inherited members first (own declarations override below)
    decl = re.search(rf"class\s+(?:TGBOT_API\s+)?{type_name}\b\s*:\s*([^{{]+)\{{",
                     body)
    if decl:
        for base in re.findall(r"public\s+([A-Za-z_]\w*)", decl.group(1)):
            for k, v in header_member_types(base, _seen).items():
                members.setdefault(k, v)
    # match: <type> <name>[ = default];   (type may contain ::, <>, spaces)
    for m in re.finditer(r"([A-Za-z_][\w:<>,\s]*?)\s+([A-Za-z_]\w*)\s*(?:=[^;]+)?;",
                         body):
        typ, name = m.group(1).strip(), m.group(2)
        if typ in ("using", "return", "class", "struct", "enum"):
            continue
        members[name] = re.sub(r"\s+", " ", typ)
    return members


def classify(cpp_type):
    """(kind, inner) for a member's C++ type."""
    # Normalise std::shared_ptr<X> to the equivalent X::Ptr spelling.
    t = re.sub(r"std::shared_ptr<\s*(\w+)\s*>", r"\1::Ptr", cpp_type.strip())
    opt = t.startswith("std::optional<")
    inner = t[len("std::optional<"):-1].strip() if opt else t
    m = (re.fullmatch(r"Matrix<\s*(\w+)::Ptr\s*>", inner) or
         re.fullmatch(r"std::vector<\s*std::vector<\s*(\w+)::Ptr\s*>\s*>", inner))
    if m:
        return "matrix", m.group(1)
    m = re.fullmatch(r"std::vector<\s*(\w+)::Ptr\s*>", inner)
    if m:
        return ("array" if opt else "array_req"), m.group(1)
    m = re.fullmatch(r"std::vector<\s*(.+?)\s*>", inner)
    if m:
        return ("prim_array" if opt else "prim_array_req"), m.group(1)
    if inner == "MaybeInaccessibleMessage":
        # A std::variant alias parsed via the non-template parse(json) overload.
        return ("maybe_msg_opt" if opt else "maybe_msg_req"), inner
    m = re.fullmatch(r"(\w+)::Ptr", inner)
    if m:
        return ("ptr_opt" if opt else "ptr_req"), m.group(1)
    if re.fullmatch(r"[A-Z]\w*::\w+", inner) or re.fullmatch(r"[A-Z]\w*", inner):
        return "enum", inner             # TgBot enum (Chat::Type / nested Type); not modelled
    return "primitive", inner            # bool / std::string / std::int*_t / ...


def gen_field(field, members):
    snake = field["name"]
    camel = snake_to_camel(snake)
    if camel not in members:
        return None  # member name doesn't match spec field -> bail
    kind, inner = classify(members[camel])
    if kind == "primitive":
        return (f'    parse(data, "{snake}", &result->{camel});',
                f'        json.put("{snake}", object->{camel});')
    if kind == "ptr_req":
        return (f'    result->{camel} = parseRequired<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "ptr_opt":
        return (f'    result->{camel} = parse<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "array":
        return (f'    result->{camel} = parseArray<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "array_req":
        return (f'    result->{camel} = parseRequiredArray<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "matrix":
        return (f'    result->{camel} = parseMatrix<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "prim_array":
        return (f'    result->{camel} = parsePrimitiveArray<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "prim_array_req":
        return (f'    result->{camel} = parsePrimitiveRequiredArray<{inner}>(data, "{snake}");',
                f'        json.put("{snake}", object->{camel});')
    if kind == "maybe_msg_opt":
        return (f'    if (data.contains("{snake}") && !data["{snake}"].is_null()) {{\n'
                f'        result->{camel} = parse(data["{snake}"]);\n    }}',
                f'        json.put("{snake}", object->{camel});')
    if kind == "maybe_msg_req":
        return (f'    result->{camel} = parse(data["{snake}"]);',
                f'        json.put("{snake}", object->{camel});')
    return None  # enum -> not modelled (kept hand-written)


def discriminator_values(spec, base):
    """(field_name, {subtype C++ name -> discriminator string}) from the spec."""
    out = {}
    field_name = None
    for sub in spec["types"][base].get("subtypes", []):
        for f in spec["types"].get(sub, {}).get("fields", []):
            mm = re.search(r'(?:always|must be)\s+"?([\w-]+)"?',
                           f.get("description", ""))
            if mm:
                out[sub] = mm.group(1)
                field_name = f["name"]
                break
    return field_name, out


def gen_type(spec, name):
    info = spec["types"][name]
    fields = info.get("fields", [])
    subtypes = info.get("subtypes", [])

    inc = ["#include <tgbot/TgTypeParser.h>",
           f"#include <tgbot/types/{name}.h>"]
    body_parse, body_put = [], []

    if subtypes:
        # Polymorphic base: dispatch on the discriminator field.
        df, disc = discriminator_values(spec, name)
        if df is None or len(disc) != len(subtypes):
            return None, "base:missing-discriminator"
        if len(set(disc.values())) != len(disc):
            # e.g. InlineQueryResult: cached/non-cached share a "type" value, so
            # the discriminator alone can't pick the subtype.
            return None, "base:ambiguous-discriminator"
        for sub in sorted(subtypes):
            inc.append(f"#include <tgbot/types/{sub}.h>")
        dcamel = snake_to_camel(df)
        body_parse.append("    std::string type;")
        body_parse.append(f"    {name}::Ptr result;")
        body_parse.append(f'    parse(data, "{df}", &type);')
        first = True
        for sub in subtypes:
            kw = "if" if first else "} else if"
            body_parse.append(f'    {kw} (type == "{disc[sub]}") {{')
            body_parse.append(f"        result = parse<{sub}>(data);")
            first = False
        body_parse.append("    } else {")
        body_parse.append(f'        throw invalidType("{name}", type);')
        body_parse.append("    }")
        body_put.append(f'        json.put("{df}", object->{dcamel});')
        first = True
        for sub in subtypes:
            kw = "if" if first else "} else if"
            body_put.append(f'        {kw} (object->{dcamel} == "{disc[sub]}") {{')
            body_put.append(f"            json += put<{sub}>(object);")
            first = False
        body_put.append("        } else {")
        body_put.append(f'            throw invalidType("{name}", object->{dcamel});')
        body_put.append("        }")
        ret_extra = True
    else:
        members = header_member_types(name)
        for f in fields:
            g = gen_field(f, members)
            if g is None:
                return None, f"field:{f['name']}"
            body_parse.append(g[0])
            body_put.append(g[1])
        ret_extra = False

    inc.append("#include <nlohmann/json.hpp>")

    out = []
    out += inc
    out += ["", "namespace TgBot {", ""]
    out.append(f"DECLARE_PARSER_FROM_JSON({name}) {{")
    if subtypes:
        out += body_parse
        out += ["", "    return result;", "}"]
    else:
        out.append(f"    auto result = std::make_shared<{name}>();")
        out += body_parse
        out += ["    return result;", "}"]
    out += ["", f"DECLARE_PARSER_TO_JSON({name}) {{", "    JsonWrapper json;",
            "    if (object) {"]
    out += body_put
    out += ["    }", "    return json;", "}", "", "} // namespace TgBot", ""]
    return "\n".join(out), None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--spec", default=os.path.join(ROOT, "api.json"))
    ap.add_argument("--write", action="store_true",
                    help="overwrite the generated .cpp files")
    ap.add_argument("--only", help="comma-separated type names to limit to")
    ap.add_argument("--check", action="store_true",
                    help="exit non-zero if any non-exception type differs from "
                         "the generator (the CI parity gate)")
    args = ap.parse_args()
    spec = json.load(open(args.spec, encoding="utf-8"))
    only = set(args.only.split(",")) if args.only else None

    match = mismatch = 0
    mism_list, skip_list = [], []
    for name in spec["types"]:
        if only and name not in only:
            continue
        if name in HAND_WRITTEN:
            skip_list.append((name, "hand-written"))
            continue
        gen, reason = gen_type(spec, name)
        if gen is None:
            skip_list.append((name, reason))
            continue
        path = os.path.join(SRC_DIR, f"{name}.cpp")
        # Text mode normalises line endings, so the check is CRLF/LF agnostic.
        cur = open(path, encoding="utf-8").read() if os.path.exists(path) else ""
        if gen == cur:
            match += 1
        else:
            mismatch += 1
            mism_list.append(name)
        if args.write:
            open(path, "w", encoding="utf-8", newline="\n").write(gen)

    # Skips that are NOT declared hand-written mean the generator can't model a
    # type yet -- that must fail the gate so it's either fixed or declared.
    unexpected = [n for n, r in skip_list if n not in HAND_WRITTEN]

    print(f"byte-identical: {match}")
    print(f"differs       : {mismatch}  {mism_list[:20]}")
    print(f"hand-written  : {len(skip_list) - len(unexpected)}")
    if unexpected:
        print(f"UNMODELLED    : {unexpected}")

    if args.check:
        if mismatch or unexpected:
            print("\nPARITY CHECK FAILED: regenerate with "
                  "`python scripts/gen_parsers.py --write` or update HAND_WRITTEN.")
            return 1
        print("\nPARITY OK")
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())
