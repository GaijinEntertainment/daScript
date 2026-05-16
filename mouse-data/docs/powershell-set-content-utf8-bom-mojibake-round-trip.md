---
slug: powershell-set-content-utf8-bom-mojibake-round-trip
title: PowerShell Set-Content -Encoding UTF8 corrupts em-dashes and other non-ASCII when round-tripping files — what's the safe way to edit UTF-8 files from PowerShell?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# Don't use PowerShell `Get-Content` / `Set-Content` for UTF-8 round-trips on Windows

Windows PowerShell 5.1's text I/O has two failure modes that compound when you sweep many files:

1. **`Get-Content -Raw` without `-Encoding UTF8`** reads in the system default codepage (often Windows-1252 / "ANSI"). UTF-8 bytes for `—` (E2 80 94) get reinterpreted as three Latin-1 chars `â€"`. Round-tripping back through `Set-Content` writes those chars in whatever the output encoding chooses.

2. **`Set-Content -Encoding UTF8`** writes UTF-8 **with BOM** (EF BB BF prefix). Even if you read clean, you're now adding a BOM the original didn't have. Many tools, diffs, and parsers treat the BOM as part of the first line (`ï»¿options gen2` in a git diff is the giveaway).

Together: a sweep over 128 source files mangled every em-dash, en-dash, and arrow into Latin-1 mojibake AND prepended a BOM to every file. Caught by:

    git diff -- '*.das' | head -50           # shows -options gen2 / +ï»¿options gen2 and -— / +â€"
    [System.IO.File]::ReadAllBytes($f)[0..2] # 0xEF 0xBB 0xBF means BOM was added

## The safe path: Python with explicit binary I/O

When you need to bulk-rewrite UTF-8 files (especially with possibly-mixed BOM/no-BOM input), drop to Python and handle bytes explicitly:

    with open(path, "rb") as f:
        raw = f.read()
    has_bom = raw.startswith(b'\xef\xbb\xbf')
    if has_bom:
        raw = raw[3:]
    text = raw.decode('utf-8')
    # ... modify text ...
    out = text.encode('utf-8')
    if has_bom:
        out = b'\xef\xbb\xbf' + out
    with open(path, "wb") as f:
        f.write(out)

Binary mode also avoids the CRLF translation that text mode adds on Windows. Files round-trip byte-perfect except for the intended edit.

## Recovery if you already corrupted files

    git checkout HEAD -- <dir>/    # nested-git repos: cd into the repo first

Don't try to "fix" the mojibake in place — restore from git and redo with a UTF-8-safe tool.

## When you must stay in PowerShell 5.1

For surgical single-file edits, prefer `Edit` / `Write` from the harness (they're encoding-aware and preserve line endings). If you must use PowerShell, pass `-Encoding UTF8NoBOM` (PowerShell 6+) — **5.1 doesn't have it**, and there's no clean substitute. `[System.IO.File]::WriteAllText($path, $text, [System.Text.UTF8Encoding]::new($false))` (the `$false` means "no BOM") is the workaround but fragile.

## Questions
- PowerShell Set-Content -Encoding UTF8 corrupts em-dashes and other non-ASCII — what's the safe way to edit UTF-8 files from PowerShell?
- Why does my git diff show `ï»¿options` and `â€"` instead of `options` and `—` after a script edited the file?
- How do I bulk-rewrite UTF-8 files on Windows without adding a BOM?
- What encoding does Windows PowerShell 5.1 Set-Content -Encoding UTF8 actually write?

## Questions
- PowerShell Set-Content -Encoding UTF8 corrupts em-dashes and other non-ASCII when round-tripping files — what's the safe way to edit UTF-8 files from PowerShell?
