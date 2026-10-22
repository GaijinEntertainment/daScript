"""Stamp a staged site's own scripts and styles with their content hash, in place:
    python3 site-dasllama/stamp_assets.py _site _site_dasllama
Every `src`/`href` naming a `files/...js|css` path (relative, `../` prefixes included) gains
`?v=<sha1 prefix>` of the file it resolves to under the staged tree. A browser then fetches a
changed script under a new URL and keeps an unchanged one cached - a plain reload never runs a
cached script under a newer page. A reference that already carries a query, or that resolves to
no staged file, is left as it is. The Sphinx tree (`doc/`) versions its own assets and is skipped.
"""
import hashlib
import os
import re
import sys

REF = re.compile(rb'((?:src|href)=")((?:\.\./)*files/[^"?#]+\.(?:js|css))(")')


def content_hash(path, memo):
    if path not in memo:
        with open(path, "rb") as f:
            memo[path] = hashlib.sha1(f.read()).hexdigest()[:10].encode()
    return memo[path]


def stamp_page(page, memo):
    with open(page, "rb") as f:
        html = f.read()
    here = os.path.dirname(page)
    stamped = [0]

    def stamp(m):
        target = os.path.normpath(os.path.join(here, m.group(2).decode()))
        if not os.path.isfile(target):
            return m.group(0)
        stamped[0] += 1
        return m.group(1) + m.group(2) + b"?v=" + content_hash(target, memo) + m.group(3)

    out = REF.sub(stamp, html)
    if out != html:
        with open(page, "wb") as f:
            f.write(out)
    return stamped[0]


def stamp_tree(root):
    memo, pages, refs = {}, 0, 0
    for dirpath, dirnames, filenames in os.walk(root):
        if os.path.relpath(dirpath, root) == ".":
            dirnames[:] = [d for d in dirnames if d != "doc"]
        for name in filenames:
            if name.endswith(".html"):
                n = stamp_page(os.path.join(dirpath, name), memo)
                pages += n > 0
                refs += n
    return pages, refs


def main(roots):
    for root in roots:
        if not os.path.isdir(root):
            print(f"stamp_assets: {root} is not a directory", file=sys.stderr)
            return 1
        pages, refs = stamp_tree(root)
        print(f"stamp_assets: {root}: {refs} reference(s) stamped across {pages} page(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
