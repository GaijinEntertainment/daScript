#!/usr/bin/env python3
# The reference half of the G2P sweep: run the reference front end (arm E of the fidelity
# experiment - the reference normalizer, spaCy tokens and tags, the misaki lexicon rules with the
# CMUdict + GRU fallback) over fresh sentences and write what the das side must reproduce.
#
#   python tts_g2p_sweep.py <experiment_root> <out.json> <text or conllu file>... [--n 2000] [--seed 7]
#
# Sentences are mined from the given files: `.conllu` files contribute their `# text = ` lines
# (the silver sets from mint_postag_silver.py already carry normalized prose), anything else is
# split into sentences the way mint_postag_silver.py does. Output rows: id, text, norm, ps_misaki.
# Feed the JSON to harness/tts_g2p_sweep.das for the comparison. Runs inside the experiment's venv.
import argparse, json, os, random, sys


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("root")
    ap.add_argument("out")
    ap.add_argument("sources", nargs="+")
    ap.add_argument("--n", type=int, default=2000)
    ap.add_argument("--seed", type=int, default=7)
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.root, "scripts"))
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    os.environ.setdefault("HF_HOME", os.path.join(a.root, ".hf"))
    os.environ.setdefault("NLTK_DATA", os.path.join(a.root, ".nltk"))
    import g2p_common as G
    import mint_postag_silver as M

    sents = []
    for path in a.sources:
        text = open(path, encoding="utf8", errors="replace").read()
        if path.endswith(".conllu"):
            sents.extend(l[len("# text = "):].strip() for l in text.splitlines() if l.startswith("# text = "))
        else:
            for para in M.paragraphs(M.book_body(text)):
                sents.extend(M.sentences(para))
    random.Random(a.seed).shuffle(sents)
    sents = sents[:a.n]
    print(f"{len(sents)} sentences")

    arm = G.MisakiArm(fallback="cmudict", pos=True, target="misaki")
    rows = []
    for i, s in enumerate(sents):
        try:
            norm = G.shared_normalize(s)
        except ValueError:
            continue
        ps, _meta = arm(norm)
        rows.append({"id": f"sw{i:04d}", "text": s, "norm": norm, "ps_misaki": ps})
    with open(a.out, "w", encoding="utf8") as f:
        json.dump(rows, f, ensure_ascii=False, indent=0)
    print(f"wrote {a.out}: {len(rows)} rows")


if __name__ == "__main__":
    main()
