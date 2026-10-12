"""Generate Unicode property lookups from the pinned, unmodified UCD files."""
from pathlib import Path
import argparse
import hashlib
import json

ROOT = Path(__file__).resolve().parents[3]
VERSION = "17.0.0"
DATA = ROOT / "3rdparty" / "unicode" / VERSION
OUTPUT = ROOT / "daslib" / "unicode_text_data.das"


def properties():
    decompositions, classes, folds = {}, {}, {}
    range_start = None
    for line in (DATA / "UnicodeData.txt").read_text(encoding="utf-8").splitlines():
        fields = line.split(";")
        codepoint, combining = int(fields[0], 16), int(fields[3])
        if fields[1].endswith(", First>"):
            range_start = codepoint
        elif fields[1].endswith(", Last>"):
            assert range_start is not None
            if combining:
                classes.update((cp, combining) for cp in range(range_start, codepoint + 1))
            range_start = None
        elif combining:
            classes[codepoint] = combining
        if fields[5] and not fields[5].startswith("<"):
            mapping = tuple(int(cp, 16) for cp in fields[5].split())
            assert 1 <= len(mapping) <= 2
            decompositions[codepoint] = mapping
    for line in (DATA / "CaseFolding.txt").read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        source, status, mapping, _ = (field.strip() for field in line.split(";"))
        if status in ("C", "F"):
            target = tuple(int(cp, 16) for cp in mapping.split())
            assert 1 <= len(target) <= 3
            folds[int(source, 16)] = target
    return decompositions, classes, folds


def table(name, values):
    rows = [", ".join(f"0x{value:x}u" for value in values[index:index + 12])
            for index in range(0, len(values), 12)]
    return f"let private {name} <- fixed_array(\n    " + ",\n    ".join(rows) + "\n)\n"


def generate():
    decomp, classes, folds = properties()
    result = """// Copyright 1991-2025 Unicode, Inc. SPDX-License-Identifier: Unicode-3.0
// Generated from Unicode 17.0.0 UCD; source licence: 3rdparty/unicode/LICENSE.txt.
options gen2
options indenting = 4
module unicode_text_data shared
require daslib/algorithm

"""
    for prefix, mappings, width in (("DECOMP", decomp, 2), ("FOLD", folds, 3)):
        keys = sorted(mappings)
        result += table(prefix + "_KEYS", keys)
        result += table(prefix + "_COUNTS", [len(mappings[key]) for key in keys])
        for column in range(width):
            result += table(f"{prefix}_{column}", [mappings[key][column] if column < len(mappings[key]) else 0 for key in keys])
    keys = sorted(classes)
    result += table("CLASS_KEYS", keys) + table("CLASS_VALUES", [classes[key] for key in keys])
    result += """
def private lookup(keys : auto const; codepoint : uint) : int {
    let index = lower_bound(keys, codepoint)
    return index < length(keys) && keys[index] == codepoint ? index : -1
}
def public unicode_data_decomposition(codepoint : uint) : uint3 {
    //! Internal UCD lookup: x/y are scalars, z is their count; zero count means no mapping.
    let index = lookup(DECOMP_KEYS, codepoint)
    return index < 0 ? uint3(0u) : uint3(DECOMP_0[index], DECOMP_1[index], DECOMP_COUNTS[index])
}
def public unicode_data_casefold(codepoint : uint) : uint4 {
    //! Internal full default folding lookup: x/y/z are scalars, w is their count.
    let index = lookup(FOLD_KEYS, codepoint)
    return index < 0 ? uint4(0u) : uint4(FOLD_0[index], FOLD_1[index], FOLD_2[index], FOLD_COUNTS[index])
}
def public unicode_data_combining_class(codepoint : uint) : uint {
    //! Internal canonical combining class lookup; unlisted scalars have class zero.
    if (codepoint < 0x300u) return 0u
    let index = lookup(CLASS_KEYS, codepoint)
    return index < 0 ? 0u : CLASS_VALUES[index]
}
"""
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    arguments = parser.parse_args()
    output = generate()
    manifest = {"version": VERSION, "license": "Unicode-3.0", "files": [
        {"file": file.name, "source": f"https://www.unicode.org/Public/{VERSION}/ucd/{file.name}",
         "sha256": hashlib.sha256(file.read_bytes()).hexdigest()}
        for file in sorted(DATA.glob("*.txt"))]}
    manifest_text = json.dumps(manifest, indent=2) + "\n"
    manifest_path = DATA / "manifest.json"
    if arguments.check:
        if OUTPUT.read_text(encoding="utf-8") != output or manifest_path.read_text(encoding="utf-8") != manifest_text:
            raise SystemExit("Unicode tables or manifest are stale")
    else:
        OUTPUT.write_text(output, encoding="utf-8")
        manifest_path.write_text(manifest_text, encoding="utf-8")


if __name__ == "__main__":
    main()
