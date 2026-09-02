#!/usr/bin/env python3
# Mint silver part-of-speech training data for train_postag.py: public-domain prose, run through
# the text normalizer the TTS front end uses (so spelled-out numbers, expanded abbreviations and
# kept apostrophes look the way the tagger will see them), tokenized and tagged by spaCy's
# en_core_web_sm - the tagger the lexicon's part-of-speech rules were tuned against.
#
#   python mint_postag_silver.py <experiment_root> <out_dir> <text file>... [--max-sents 30000]
#
# Writes <out_dir>/en_silver-ud-train.conllu (id, form, _, _, xpos columns - what train_postag.py
# reads). Project Gutenberg headers and footers are cut at their START/END markers. Runs inside
# the experiment's venv.
import argparse, os, random, re, sys


def book_body(text):
    start = re.search(r"\*\*\* ?START OF.*?\*\*\*", text)
    if start:
        text = text[start.end():]
    end = text.find("*** END OF")
    return text[:end] if end >= 0 else text


def paragraphs(text):
    for para in re.split(r"\n\s*\n", text):
        para = " ".join(para.split())
        if len(para) > 40 and not para.isupper():
            yield para


# a sentence ends at . ! ? (plus an optional closing quote) followed by whitespace and a
# capital (plus an optional opening quote)
SENT_END = re.compile(r"([.!?][\"'\u201d\u2019]?)(\s+)(?=[\"'\u201c\u2018]?[A-Z])")


def sentences(para):
    start = 0
    for m in SENT_END.finditer(para):
        s = para[start:m.end(1)].strip()
        start = m.end()
        if 20 <= len(s) <= 400:
            yield s
    s = para[start:].strip()
    if 20 <= len(s) <= 400:
        yield s


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("root")
    ap.add_argument("out_dir")
    ap.add_argument("texts", nargs="+")
    ap.add_argument("--max-sents", type=int, default=30000)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.root, "scripts"))
    os.environ.setdefault("HF_HOME", os.path.join(a.root, ".hf"))
    import g2p_common as G
    import spacy
    nlp = spacy.load("en_core_web_sm", enable=["tok2vec", "tagger"])

    sents = []
    for path in a.texts:
        text = book_body(open(path, encoding="utf8", errors="replace").read())
        for para in paragraphs(text):
            sents.extend(sentences(para))
    random.Random(a.seed).shuffle(sents)
    sents = sents[:a.max_sents]
    print(f"{len(sents)} sentences from {len(a.texts)} texts")

    # the reference normalizer strips apostrophes; the das one keeps them, so shield them
    def normalize(s):
        s = s.replace("\u2019", "'").replace("\u2018", "'").replace("'", "QQAPOSQQ")
        try:
            s = G.shared_normalize(s)
        except ValueError:
            return None   # the reference normalizer's own crash shapes (a trailing-dot version, a bare comma)
        return s.replace("QQAPOSQQ", "'")

    os.makedirs(a.out_dir, exist_ok=True)
    out_path = os.path.join(a.out_dir, "en_silver-ud-train.conllu")
    n_tok = 0
    with open(out_path, "w", encoding="utf8") as f:
        normalized = [n for n in (normalize(s) for s in sents) if n]
        for doc in nlp.pipe(normalized, batch_size=256):
            toks = [t for t in doc if not t.is_space]
            if len(toks) < 3:
                continue
            for i, t in enumerate(toks, 1):
                f.write(f"{i}\t{t.text}\t_\t_\t{t.tag_}\n")
            f.write("\n")
            n_tok += len(toks)
    print(f"wrote {out_path}: {n_tok} tokens")


if __name__ == "__main__":
    main()
