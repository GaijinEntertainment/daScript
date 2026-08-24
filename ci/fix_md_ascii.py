#!/usr/bin/env python3
"""Keep tracked .md files ASCII: transliterate in place, or --check for CI.

Default mode rewrites every offending file and prints what changed; --check
writes nothing and exits 1 when any file needs fixing (the CI gate). A file
whose non-ASCII cannot be transliterated (untranslatable script, new symbol)
fails both modes and must either gain a mapping here or an exemption below.
Files that are not valid UTF-8 are repaired first (CP-1252 reads and
double-encoded runs — the mojibake this gate exists to prevent).
"""
import argparse
import re
import subprocess
import sys
import unicodedata
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

# vendored or deliberately non-English content, never rewritten
EXEMPT_PREFIXES = (
    "3rdparty/",
)
EXEMPT_FILES = {
    "utils/internal/das-herd/LANGUAGE_SUPPORT_PLAN.md",  # non-English samples are the subject
    "history/examples/dictation/CADMUS_PLAN.md",         # archived non-English content
    "examples/text/README.md",                           # text-rendering demo strings
    "modules/dasLLAMA/README.md",                        # model prompt samples
    "modules/dasLLAMA/qwen2_audio_plan.md",              # model prompt samples
    "history/dasLLAMA/audio_models_plan.md",             # model prompt samples
    "plans/dasllama-tutorials.md",                       # non-English tutorial samples
}

# em dash gets surrounding-space normalization; plain dashes map directly
DASH = {"—"}
SENTINEL = "\x00"

MAP = {
    "–": "-", "−": "-", "‑": "-",
    "‘": "'", "’": "'", "“": '"', "”": '"', "…": "...",
    " ": " ", "‍": "", "️": "",
    "→": "->", "←": "<-", "↔": "<->", "⇒": "=>", "⇔": "<=>",
    "⟹": "==>", "⟺": "<==>", "⇄": "<->", "↗": "up", "➜": "->",
    "▶": "->", "►": "->", "▸": "->", "◄": "<-", "▲": "^",
    "×": "x", "÷": "/", "·": "*", "±": "+/-", "≈": "~",
    "≤": "<=", "≥": ">=", "≠": "!=", "≡": "==", "≪": "<<",
    "≫": ">>", "≳": ">~", "≲": "<~", "≙": "=^", "√": "sqrt",
    "∞": "inf", "∫": "integral", "∘": "o", "∧": "&&", "∀": "forall",
    "∃": "exists", "∈": " in ", "∉": " not in ", "∪": " union ",
    "⊥": "_|_", "⊙": "(.)", "⊗": "(x)", "⊕": "(+)", "¬": "!",
    "‖": "||", "⟨": "<", "⟩": ">", "⌈": "[", "⌉": "]",
    "§": "sec.", "°": " deg", "¶": "para.", "†": "+", "©": "(c)", "€": "EUR", "£": "GBP", "®": "(R)", "™": "(TM)",
    "¹": "^1", "²": "^2", "³": "^3", "⁰": "^0", "⁴": "^4",
    "⁵": "^5", "⁻": "^-", "₀": "_0", "₁": "_1", "₂": "_2",
    "ᵢ": "_i", "ᵀ": "^T", "½": "1/2", "¼": "1/4", "⅐": "1/7",
    "µ": "u", "μ": "u",
    "α": "alpha", "β": "beta", "γ": "gamma", "δ": "delta",
    "ε": "eps", "θ": "theta", "λ": "lambda", "π": "pi",
    "σ": "sigma", "Γ": "Gamma", "Δ": "Delta", "Π": "Prod",
    "Σ": "Sum",
    "─": "-", "│": "|", "├": "+", "└": "+", "┌": "+",
    "┐": "+", "┘": "+", "▁": "_", "▣": "[#]",
    "✅": "DONE", "✓": "yes", "✗": "no", "✘": "no", "❌": "FAIL",
    "⚠": "[!]", "⏳": "[pending]", "⏸": "[paused]", "⏎": "<enter>",
    "ℹ": "[i]", "⚪": "[white]",
    "\U0001f7e2": "[green]", "\U0001f7e1": "[yellow]", "\U0001f511": "[key]",
    "\U0001f6a7": "[wip]", "\U0001f41e": "[bug]", "\U0001f916": ":robot:",
    "\U0001f469": "(dev)", "\U0001f4bb": "",
}


def list_targets():
    out = subprocess.run(["git", "ls-files", "*.md"], capture_output=True, text=True,
                         cwd=REPO, check=True).stdout.split("\n")
    for rel in out:
        rel = rel.strip()
        if not rel or rel.startswith(EXEMPT_PREFIXES) or rel in EXEMPT_FILES:
            continue
        yield rel


def read_repaired(path: Path):
    raw = path.read_bytes()
    try:
        return raw.decode("utf-8"), False
    except UnicodeDecodeError:
        pass
    # Preserve valid UTF-8 runs and decode only invalid bytes as CP-1252. Treating
    # the whole file as CP-1252 corrupts mixed files: an isolated legacy byte can
    # otherwise turn every valid UTF-8 punctuation sequence into mojibake.
    mixed = raw.decode("utf-8", errors="surrogateescape")
    repaired = []
    for ch in mixed:
        code = ord(ch)
        if 0xDC80 <= code <= 0xDCFF:
            repaired.append(bytes((code - 0xDC00,)).decode("cp1252", errors="replace"))
        else:
            repaired.append(ch)
    text = "".join(repaired)
    for _ in range(3):  # collapse double-encoded runs until stable
        try:
            text2 = text.encode("cp1252").decode("utf-8")
        except (UnicodeEncodeError, UnicodeDecodeError):
            break
        if text2 == text:
            break
        text = text2
    return text, True


def transliterate(text: str):
    out = []
    residual = set()
    for ch in text:
        if ord(ch) < 128:
            out.append(ch)
            continue
        if ch in DASH:
            out.append(SENTINEL)  # spacing fixed below
            continue
        if ch in MAP:
            out.append(MAP[ch])
            continue
        decomp = unicodedata.normalize("NFKD", ch)
        plain = "".join(c for c in decomp if not unicodedata.combining(c))
        if plain and all(ord(c) < 128 for c in plain):
            out.append(plain)
            continue
        if unicodedata.combining(ch):
            continue  # stray combining mark: drop
        out.append(ch)
        residual.add(ch)
    text = "".join(out)
    text = re.sub("(?m)^([*_]) ?" + SENTINEL + " ?", r"\1- ", text)  # dash opening emphasis: no space, or "* " reads as a bullet
    text = re.sub(" ?" + SENTINEL + " ?(?=\n|$)", " -", text)  # line-end dash: no trailing space
    text = re.sub(" ?" + SENTINEL + " ?", " - ", text)
    return text, residual


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true", help="report only; exit 1 when dirty")
    args = ap.parse_args()

    dirty, failed = [], []
    for rel in list_targets():
        path = REPO / rel
        text, was_broken = read_repaired(path)
        if all(ord(c) < 128 for c in text) and not was_broken:
            continue
        fixed, residual = transliterate(text)
        if residual:
            failed.append((rel, residual))
            continue
        dirty.append(rel)
        if not args.check:
            path.write_bytes(fixed.encode("ascii"))

    for rel in dirty:
        print(("needs fix: " if args.check else "fixed: ") + rel)
    for rel, residual in failed:
        chars = " ".join(f"U+{ord(c):04X}" for c in sorted(residual))
        print(f"UNTRANSLATABLE: {rel}: {chars} - add a mapping to ci/fix_md_ascii.py or an exemption", file=sys.stderr)

    if failed or (args.check and dirty):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
