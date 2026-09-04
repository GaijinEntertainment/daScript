#!/usr/bin/env python3
# Mint tests/_tts_fixtures/g2p_corpus_gb.json - the British half of the front-end fixture.
#
# The same 200 normalized sentences as g2p_corpus.json (which this script reads, so the two
# fixtures can never drift apart in row set or in text), through the reference front end in
# its British configuration - misaki `en.G2P(british=True)` with the espeak `en-gb` fallback,
# exactly what the Kokoro reference pipeline builds for a `b*` voice. Every row records
# whether the fallback fired: our own out-of-vocabulary chain is American (CMUdict, then the
# GRU) rewritten into the British inventory, so a row the reference answered from espeak is
# not a lexicon-parity row.
#
# Also minted: the British inventory itself, read off gb_gold.json (the vocabulary check any
# British phoneme string must pass), and a handful of out-of-vocabulary words with what the
# reference's espeak answers for them in both dialects.
#
#   python mint_tts_g2p_gb_fixture.py [out.json]
#
# Runs inside the G2P experiment's venv (misaki 0.9.4 with espeak-ng, spacy 3.8 +
# en_core_web_sm). Without espeak the fallback cannot be built and the script refuses: a
# fixture minted with no fallback would silently mark every row lexicon-clean.
import json, os, sys

OOV_PROBES = ["flombastic", "zymurgical", "gorbleflax", "sprindle", "thromberly", "quastive"]


def main():
    out_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus_gb.json")
    us_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures",
                           "g2p_corpus.json")
    from misaki import en, espeak
    import misaki.data

    gb_fallback = espeak.EspeakFallback(british=True)
    us_fallback = espeak.EspeakFallback(british=False)

    fired = []

    class Watched:
        """the reference's own espeak fallback, recording which tokens reached it"""

        def __call__(self, token):
            fired.append(token.text)
            return gb_fallback(token)

    g2p = en.G2P(british=True, fallback=Watched(), unk="")

    data_dir = os.path.dirname(misaki.data.__file__)
    gb_gold = json.load(open(os.path.join(data_dir, "gb_gold.json"), encoding="utf8"))
    vocab = set()
    for v in gb_gold.values():
        for s in ([v] if isinstance(v, str) else v.values()):
            if s:
                vocab.update(s)

    rows = []
    for r in json.load(open(us_path, encoding="utf8")):
        fired.clear()
        ps, _tokens = g2p(r["norm"])
        rows.append({"id": r["id"], "norm": r["norm"], "ps_gb": ps.strip(), "oov": bool(fired)})

    class Bare:
        def __init__(self, text):
            self.text = text

    oov_words = [{"word": w, "ps_us_ref": us_fallback(Bare(w))[0], "ps_gb_ref": gb_fallback(Bare(w))[0]}
                 for w in OOV_PROBES]

    doc = {"gb_vocab": "".join(sorted(vocab)), "rows": rows, "oov_words": oov_words}
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w", encoding="utf8") as f:
        json.dump(doc, f, ensure_ascii=False, indent=0)
    print(f"wrote {out_path}: {len(rows)} rows, {sum(r['oov'] for r in rows)} with an "
          f"out-of-vocabulary word, {len(oov_words)} probe words, {len(vocab)} inventory symbols, "
          f"{os.path.getsize(out_path)} bytes")


if __name__ == "__main__":
    main()
