#!/usr/bin/env python3
# Train the TTS front end's part-of-speech tagger and pack it, with the tokenizer's exception
# table, into tts_postag.bin for dasllama_postag.das.
#
# Tagger: a greedy left-to-right averaged perceptron over the PTB tagset (the `xpos` column of
# UD English-EWT), the classic feature template (word, 3-char suffix, first char, the two
# previous tags, neighbours' words and suffixes) widened with word shape, 2/4-char suffixes,
# 2/3-char prefixes and word bigrams, plus a table of unambiguous frequent words.
# Tokenizer data: spaCy's English special-case table (contractions, abbreviations with their
# periods), filtered to the characters that survive text normalization.
#
#   python train_postag.py <ud treebank dir>... --out tts_postag.bin [--iters 10] [--prune 0.05]
#       [--seed 1] [--fixture tests/_tts_fixtures/g2p_corpus.json]
#
# Every directory's train split trains; the FIRST directory's dev/test splits score (EWT first,
# GUM second). --fixture adds the agreement with spaCy's tags on the 200-sentence corpus.
#
# Needs spacy + en_core_web_sm (the exception export); run inside the g2p experiment's venv.
# Prints dev/test accuracy and the packed size; the test accuracy lands in the file header, and
# the das loader refuses a file below its floor.
import argparse, os, random, struct
from collections import Counter, defaultdict

START = ["-START-", "-START2-"]
END = ["-END-", "-END2-"]
MAGIC = 0x53504F54  # 'TPOS'
VERSION = 1


def read_conllu(path):
    sents, cur = [], []
    for line in open(path, encoding="utf8"):
        line = line.rstrip("\n")
        if not line:
            if cur:
                sents.append(cur)
                cur = []
            continue
        if line.startswith("#"):
            continue
        cols = line.split("\t")
        if "-" in cols[0] or "." in cols[0]:
            continue  # multiword-token range / empty node
        form, xpos = cols[1], cols[4]
        if xpos == "_":
            continue
        cur.append((form, xpos))
    if cur:
        sents.append(cur)
    return sents


def normalize(word):
    if "-" in word and word[0] != "-":
        return "!HYPHEN"
    if word.isdigit() and len(word) == 4:
        return "!YEAR"
    if word[0].isdigit():
        return "!DIGITS"
    return word.lower()


def shape(word):
    # capitalization and character-class silhouette: X caps, Xx title, x lower; d = has a digit,
    # h = inner hyphen, q = apostrophe
    s = "X" if word.isupper() else ("Xx" if word[:1].isupper() else "x")
    if any(ch.isdigit() for ch in word):
        s += "d"
    if "-" in word[1:]:
        s += "h"
    if "'" in word:
        s += "q"
    return s


def features(i, word, context, prev, prev2):
    # i indexes `context`, which carries the two START pads in front
    f = []

    def add(name, *args):
        f.append(name + "\x1f" + "\x1f".join(args))
    add("bias")
    add("i suffix", word[-3:])
    add("i suffix2", word[-2:])
    add("i suffix4", word[-4:])
    add("i pref1", word[0])
    add("i pref2", word[:2])
    add("i pref3", word[:3])
    add("i shape", shape(word))
    add("i-1 tag", prev)
    add("i-2 tag", prev2)
    add("i tag+i-2 tag", prev, prev2)
    add("i word", context[i])
    add("i-1 tag+i word", prev, context[i])
    add("i-1 tag+i suffix", prev, word[-3:])
    add("i-1 word", context[i - 1])
    add("i-1 suffix", context[i - 1][-3:])
    add("i-1 word+i word", context[i - 1], context[i])
    add("i-2 word", context[i - 2])
    add("i+1 word", context[i + 1])
    add("i+1 suffix", context[i + 1][-3:])
    add("i word+i+1 word", context[i], context[i + 1])
    add("i+2 word", context[i + 2])
    return f


class Perceptron:
    def __init__(self, classes):
        self.classes = sorted(classes)
        self.weights = defaultdict(dict)   # feature -> {tag: weight}
        self._totals = defaultdict(float)
        self._tstamps = defaultdict(int)
        self.i = 0

    def predict(self, feats):
        scores = defaultdict(float)
        for f in feats:
            w = self.weights.get(f)
            if not w:
                continue
            for tag, wt in w.items():
                scores[tag] += wt
        # highest score, ties toward the lexicographically largest tag (the das side does the same)
        return max(self.classes, key=lambda t: (scores[t], t))

    def update(self, truth, guess, feats):
        def upd(f, c, v):
            key = (f, c)
            self._totals[key] += (self.i - self._tstamps[key]) * self.weights[f].get(c, 0.0)
            self._tstamps[key] = self.i
            self.weights[f][c] = self.weights[f].get(c, 0.0) + v
        self.i += 1
        if truth == guess:
            return
        for f in feats:
            upd(f, truth, 1.0)
            upd(f, guess, -1.0)

    def average(self):
        for f, w in self.weights.items():
            for c, wt in list(w.items()):
                key = (f, c)
                total = self._totals[key] + (self.i - self._tstamps[key]) * wt
                avg = round(total / self.i, 3)
                if avg:
                    w[c] = avg
                else:
                    del w[c]


NUMBER_WORDS = ("zero one two three four five six seven eight nine ten eleven twelve thirteen fourteen "
                "fifteen sixteen seventeen eighteen nineteen twenty thirty forty fifty sixty seventy eighty "
                "ninety hundred thousand million billion trillion").split()


def make_tagdict(sents, freq_min=20, ambiguity=0.97):
    counts = defaultdict(Counter)
    for s in sents:
        for w, t in s:
            counts[w][t] += 1
    tagdict = {}
    for w, tc in counts.items():
        tag, mode = tc.most_common(1)[0]
        n = sum(tc.values())
        if n >= freq_min and mode / n >= ambiguity:
            tagdict[w] = tag
    # the normalizer spells every number out, a shape treebanks of written text rarely carry
    for w in NUMBER_WORDS:
        tagdict[w] = "CD"
        tagdict[w.capitalize()] = "CD"
    return tagdict


def tag_sentence(model, tagdict, words):
    context = START + [normalize(w) for w in words] + END
    prev, prev2 = START
    out = []
    for i, w in enumerate(words):
        tag = tagdict.get(w)
        if tag is None:
            tag = model.predict(features(i + 2, w, context, prev, prev2))
        out.append(tag)
        prev2, prev = prev, tag
    return out


def accuracy(model, tagdict, sents):
    n = c = 0
    for s in sents:
        words = [w for w, _ in s]
        for got, (_, want) in zip(tag_sentence(model, tagdict, words), s):
            n += 1
            c += got == want
    return c / n


def train(train_sents, iters, seed):
    classes = {t for s in train_sents for _, t in s}
    tagdict = make_tagdict(train_sents)
    model = Perceptron(classes)
    rng = random.Random(seed)
    for it in range(iters):
        n = c = 0
        for s in train_sents:
            words = [w for w, _ in s]
            context = START + [normalize(w) for w in words] + END
            prev, prev2 = START
            for i, (w, truth) in enumerate(s):
                guess = tagdict.get(w)
                if guess is None:
                    feats = features(i + 2, w, context, prev, prev2)
                    guess = model.predict(feats)
                    model.update(truth, guess, feats)
                prev2, prev = prev, guess
                n += 1
                c += guess == truth
        rng.shuffle(train_sents)
        print(f"iter {it + 1}: train {c / n:.4f}", flush=True)
    model.average()
    return model, tagdict


def prune(model, threshold):
    dropped = 0
    for f in list(model.weights):
        w = model.weights[f]
        for c in list(w):
            if abs(w[c]) < threshold:
                del w[c]
                dropped += 1
        if not w:
            del model.weights[f]
    return dropped


def fnv1a64(data):
    h = 0xCBF29CE484222325
    for b in data:
        h ^= b
        h = (h * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def spacy_exceptions():
    """spaCy's English special cases whose surface survives normalization (letters, digits,
    apostrophe, period, hyphen): chunk -> its token texts."""
    import spacy
    from spacy.attrs import ORTH
    nlp = spacy.load("en_core_web_sm", enable=[])
    keep = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'.-")
    out = {}
    for chunk, parts in nlp.tokenizer.rules.items():
        if not all(ch in keep for ch in chunk):
            continue
        texts = []
        for p in parts:
            orth = p.get(ORTH, p.get("ORTH")) if isinstance(p, dict) else p
            texts.append(orth if isinstance(orth, str) else nlp.vocab.strings[orth])
        if "".join(texts) != chunk:
            continue
        out[chunk] = texts
    return out


WEIGHT_SCALE = 256.0   # weights ship as int16 in 1/256 steps; the perceptron's are 3-decimal averages


def pack(path, model, tagdict, exceptions, test_acc, dev_acc, source):
    tags = model.classes
    tag_id = {t: i for i, t in enumerate(tags)}
    feats = []
    weights = []
    seen = {}
    for f, w in model.weights.items():
        h = fnv1a64(f.encode("utf8"))
        if h in seen:
            raise SystemExit(f"feature hash collision: {f!r} vs {seen[h]!r}")
        seen[h] = f
        items = sorted(w.items())
        feats.append((h, len(weights), len(items)))
        for c, wt in items:
            q = max(-32768, min(32767, int(round(wt * WEIGHT_SCALE))))
            weights.append((tag_id[c], q))
    feats.sort()
    buf = bytearray()
    buf += struct.pack("<IIfff", MAGIC, VERSION, test_acc, dev_acc, WEIGHT_SCALE)
    src = source.encode("utf8")
    buf += struct.pack("<H", len(src)) + src
    buf += struct.pack("<I", len(tags))
    for t in tags:
        b = t.encode("utf8")
        buf += struct.pack("<B", len(b)) + b
    td = sorted((w.encode("utf8"), tag_id[t]) for w, t in tagdict.items())
    buf += struct.pack("<I", len(td))
    for b, ti in td:
        buf += struct.pack("<H", len(b)) + b + struct.pack("<B", ti)
    # columnar: every column is one contiguous run the loader reads straight through
    buf += struct.pack("<I", len(feats))
    for h, _off, _n in feats:
        buf += struct.pack("<Q", h)
    for _h, off, _n in feats:
        buf += struct.pack("<I", off)
    for _h, _off, n in feats:
        buf += struct.pack("<H", n)
    buf += struct.pack("<I", len(weights))
    for ti, _q in weights:
        buf += struct.pack("<B", ti)
    for _ti, q in weights:
        buf += struct.pack("<h", q)
    exc = sorted((k.encode("utf8"), [p.encode("utf8") for p in v]) for k, v in exceptions.items())
    buf += struct.pack("<I", len(exc))
    for k, parts in exc:
        buf += struct.pack("<H", len(k)) + k + struct.pack("<B", len(parts))
        for p in parts:
            buf += struct.pack("<H", len(p)) + p
    with open(path, "wb") as f:
        f.write(buf)
    with open(path + ".LICENSE", "w", encoding="utf8") as f:
        f.write("tts_postag.bin - trained weights, see the dasLLAMA THIRD_PARTY_NOTICES.md\n\n"
                f"Trained on: {source}\n"
                "UD English-EWT (CC BY-SA 4.0) - the gold part-of-speech training data\n"
                "spaCy en_core_web_sm (ExplosionAI GmbH, MIT) - the tokenizer exception table and the "
                "silver tags over public-domain prose (Project Gutenberg)\n")
    return len(buf), len(feats), len(weights), len(td), len(exc)


def fixture_agreement(model, tagdict, path):
    """Tag the fixture's spaCy tokens and report agreement with spaCy's tags - the front end's
    lexicon rules were tuned on those, so this is the number that predicts G2P fidelity."""
    import json
    rows = json.load(open(path, encoding="utf8"))
    n = c = 0
    het_n = het_c = 0
    disagreements = Counter()
    for r in rows:
        words = [t[0] for t in r["tokens"]]
        want = [t[1] for t in r["tokens"]]
        got = tag_sentence(model, tagdict, words)
        for w, g, x in zip(words, got, want):
            n += 1
            c += g == x
            if g != x:
                disagreements[(w, x, g)] += 1
            if "heteronym" in r and w.lower() == r["heteronym"]["word"]:
                het_n += 1
                het_c += g == x
    print(f"fixture agreement with spaCy: {c / n:.4f} over {n} tokens; heteronym words {het_c}/{het_n}")
    for (w, x, g), k in disagreements.most_common(15):
        print(f"   {w!r}: spaCy {x} vs ours {g} x{k}")
    return c / n


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("treebank_dirs", nargs="+", help="UD treebank dirs; the first one's test/dev splits score, every train split trains")
    ap.add_argument("--out", required=True)
    ap.add_argument("--iters", type=int, default=10)
    ap.add_argument("--prune", type=float, default=0.05)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--fixture", help="tests/_tts_fixtures/g2p_corpus.json - report tag agreement with spaCy on it")
    a = ap.parse_args()

    def split(d, which):
        files = [f for f in os.listdir(d) if f.endswith(f"-ud-{which}.conllu")]
        return read_conllu(os.path.join(d, files[0])) if files else []
    train_sents = [s for d in a.treebank_dirs for s in split(d, "train")]
    dev_sents = split(a.treebank_dirs[0], "dev")
    test_sents = split(a.treebank_dirs[0], "test")
    print(f"train {len(train_sents)} sents / {sum(map(len, train_sents))} tokens, "
          f"dev {sum(map(len, dev_sents))}, test {sum(map(len, test_sents))}")
    model, tagdict = train(train_sents, a.iters, a.seed)
    dev_acc = accuracy(model, tagdict, dev_sents)
    test_acc = accuracy(model, tagdict, test_sents)
    print(f"averaged: dev {dev_acc:.4f} test {test_acc:.4f}, tagdict {len(tagdict)}, features {len(model.weights)}")
    dropped = prune(model, a.prune)
    dev_acc = accuracy(model, tagdict, dev_sents)
    test_acc = accuracy(model, tagdict, test_sents)
    print(f"pruned |w|<{a.prune}: dropped {dropped}, dev {dev_acc:.4f} test {test_acc:.4f}, features {len(model.weights)}")
    if a.fixture:
        fixture_agreement(model, tagdict, a.fixture)
    exceptions = spacy_exceptions()
    source = "UD English xpos (" + ", ".join(os.path.basename(os.path.normpath(d)) for d in a.treebank_dirs) + \
             "), averaged perceptron; spaCy en tokenizer exceptions"
    size, nf, nw, ntd, nexc = pack(a.out, model, tagdict, exceptions, test_acc, dev_acc, source)
    print(f"wrote {a.out}: {size} bytes, {nf} features, {nw} weights, {ntd} tagdict, {nexc} exceptions")


if __name__ == "__main__":
    main()
