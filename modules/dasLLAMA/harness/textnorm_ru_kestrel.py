#!/usr/bin/env python3
# The Russian normalizer's scoreboard against Google's Kestrel normalizer, on the Russian part of the
# Sproat and Jaitly text normalization data (Kaggle dataset
# `richardwilliamsproat/text-normalization-for-english-russian-and-polish`, CC BY-SA 4.0: a folder
# `ru_with_types/output-000NN-of-00100` of tab-separated lines - semiotic class, written token,
# Kestrel's spoken form; `<eos>` lines end a sentence). Two steps:
#
#   python textnorm_ru_kestrel.py sample --data <ru_with_types dir> --out <dir> [--per-class 1500] [--files 5]
#       rebuilds sentences from the first `--files` files, keeps the ones holding a token of a number
#       class, takes `--per-class` sentences a class (seed 0), writes <dir>/sample.txt (one sentence a
#       line) and <dir>/sample.jsonl (each sentence with its number tokens and Kestrel's readings)
#   bin/daslang -jit modules/dasLLAMA/harness/textnorm_ru_run.das -- <dir>/sample.txt <dir>/ours.txt
#       our normalizer over the sample, in the same order
#   python textnorm_ru_kestrel.py score --out <dir> [--ours <dir>/ours.txt]
#       per class, how many of Kestrel's readings appear in our line (lowercase, ё folded to е), and
#       every disagreement to <dir>/disagreements.jsonl
#
# Kestrel is a guide, not the truth: it reads "142 с." as seconds and "по 16 декабря" as шестнадцатый,
# so a lower number in one class can be a better reading; read the disagreements before trusting a drop.
import argparse, collections, json, os, random, re

CLASSES = ["DATE", "CARDINAL", "ORDINAL", "MEASURE", "TIME", "DECIMAL", "MONEY", "FRACTION", "TELEPHONE", "DIGIT"]
NO_SPACE_BEFORE = set(".,:;!?)»%")
NO_SPACE_AFTER = set("(«")


def join(tokens):
    out = ""
    for t in tokens:
        if out and t[0] not in NO_SPACE_BEFORE and out[-1] not in NO_SPACE_AFTER:
            out += " "
        out += t
    return out


def sentences(path):
    tokens, spans = [], []
    with open(path, encoding="utf8") as f:
        for line in f:
            parts = line.rstrip("\n").split("\t")
            if parts[0] == "<eos>":
                if tokens:
                    yield tokens, spans
                tokens, spans = [], []
                continue
            if len(parts) < 3:
                continue
            cls, before, after = parts[0], parts[1], parts[2]
            tokens.append(before)
            if cls in CLASSES:
                spans.append({"class": cls, "token": before, "after": after})


def sample(args):
    rng = random.Random(0)
    by_class = collections.defaultdict(list)
    for n in range(args.files):
        for tokens, spans in sentences(os.path.join(args.data, f"output-{n:05d}-of-00100")):
            if not spans or len(tokens) > 60:
                continue
            row = {"text": join(tokens), "spans": spans}
            for cls in {s["class"] for s in spans}:
                by_class[cls].append(row)
    rows, seen = [], set()
    for cls in CLASSES:
        pool = by_class.get(cls, [])
        rng.shuffle(pool)
        taken = 0
        for row in pool:
            if taken >= args.per_class:
                break
            if row["text"] in seen:
                continue
            seen.add(row["text"])
            rows.append(row)
            taken += 1
        print(f"  {cls:10s} {len(pool):7d} sentences available, {taken} taken")
    os.makedirs(args.out, exist_ok=True)
    with open(os.path.join(args.out, "sample.jsonl"), "w", encoding="utf8", newline="\n") as f:
        for row in rows:
            f.write(json.dumps(row, ensure_ascii=False) + "\n")
    with open(os.path.join(args.out, "sample.txt"), "w", encoding="utf8", newline="\n") as f:
        for row in rows:
            f.write(row["text"].replace("\n", " ") + "\n")
    print(f"{len(rows)} sentences written to {args.out}")


def fold(s):
    return " " + " ".join(re.sub(r"[^а-я0-9 ]+", " ", s.lower().replace("ё", "е")).split()) + " "


def score(args):
    with open(os.path.join(args.out, "sample.jsonl"), encoding="utf8") as f:
        rows = [json.loads(line) for line in f if line.strip()]
    with open(args.ours or os.path.join(args.out, "ours.txt"), encoding="utf8") as f:
        ours = [line.rstrip("\n") for line in f]
    if len(ours) != len(rows):
        raise SystemExit(f"{len(ours)} normalized lines for {len(rows)} sentences")
    agree, total = collections.Counter(), collections.Counter()
    with open(os.path.join(args.out, "disagreements.jsonl"), "w", encoding="utf8", newline="\n") as out:
        for row, line in zip(rows, ours):
            folded = fold(line)
            for span in row["spans"]:
                total[span["class"]] += 1
                kestrel = fold(span["after"])
                if kestrel.strip() and kestrel in folded:
                    agree[span["class"]] += 1
                else:
                    out.write(json.dumps({"class": span["class"], "token": span["token"], "kestrel": span["after"],
                                          "text": row["text"], "ours": line}, ensure_ascii=False) + "\n")
    print(f"{len(rows)} sentences; a token agrees when Kestrel's spoken form appears in our output")
    for cls in sorted(total, key=lambda c: -total[c]):
        print(f"  {cls:10s} {agree[cls]:5d} / {total[cls]:5d} = {100.0 * agree[cls] / total[cls]:5.1f}%")


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="step", required=True)
    sample_parser = sub.add_parser("sample")
    sample_parser.add_argument("--data", required=True, help="the ru_with_types folder")
    sample_parser.add_argument("--out", required=True)
    sample_parser.add_argument("--per-class", type=int, default=1500)
    sample_parser.add_argument("--files", type=int, default=5)
    score_parser = sub.add_parser("score")
    score_parser.add_argument("--out", required=True)
    score_parser.add_argument("--ours", help="our normalized lines; default <out>/ours.txt")
    args = ap.parse_args()
    (sample if args.step == "sample" else score)(args)


if __name__ == "__main__":
    main()
