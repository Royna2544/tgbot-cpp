#!/usr/bin/env python3
"""Fetch the canonical machine-readable Telegram Bot API spec.

api.json in this repo is sourced from PaulSonOfLars/telegram-bot-api-spec rather
than maintained by hand. Run this to refresh it (optionally pinning to a tag or
commit for reproducibility):

    python scripts/fetch_api_spec.py                 # latest (main)
    python scripts/fetch_api_spec.py --ref v10.1     # a specific spec release
"""
import argparse
import json
import os
import urllib.request

URL = ("https://raw.githubusercontent.com/PaulSonOfLars/"
       "telegram-bot-api-spec/{ref}/api.json")

# The spec revision is pinned in scripts/api_spec.ref for reproducible builds.
# Bump that file (and regenerate) to move to a newer Bot API version.
_REF_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                         "api_spec.ref")


def _default_ref():
    try:
        with open(_REF_FILE) as f:
            return f.read().strip()
    except OSError:
        return "main"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default=_default_ref(),
                    help="git branch, tag, or commit of the spec repo "
                         "(default: the pin in scripts/api_spec.ref)")
    ap.add_argument("--out", default="api.json", help="output path (default: api.json)")
    args = ap.parse_args()

    url = URL.format(ref=args.ref)
    print(f"Fetching {url}")
    with urllib.request.urlopen(url) as response:
        raw = response.read()

    # Validate it parses before overwriting the local copy.
    spec = json.loads(raw)
    with open(args.out, "wb") as f:
        f.write(raw)

    print(f"Wrote {args.out}: {spec.get('version')} "
          f"({len(spec.get('methods', {}))} methods, "
          f"{len(spec.get('types', {}))} types)")


if __name__ == "__main__":
    main()
