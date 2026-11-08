#!/usr/bin/env python3
# Mint tests/_tts_fixtures/g2p_corpus_ru.json - the 200-sentence Russian front-end fixture the
# letter-to-sound test compares against: stress-marked sentences (RUAccent's `+` before the stressed
# vowel, as the Balalaika corpora carry them) and the phonemes kokoro-ru's own front end
# (`ru_g2p.py`: espeak-ng over the recompiled acute-aware Russian data, then its vowel reduction and
# long щ) reads them as, RUAccent itself stubbed out - the fixture isolates letter-to-sound from
# stress, so a stress disagreement can never move its numbers.
#
#   python mint_tts_g2p_ru_fixture.py <parquet with an `accent` and a `punct` column> <kokoro-ru dir> [out.json] [--count 200] [--stride 20]
#
# Runs in a venv with pyarrow, misaki and phonemizer-fork importable; <kokoro-ru dir> is a checkout
# of the zaakirio/kokoro-ru repository (ru_g2p.py, kokoro-config.json, espeak-data/).
import argparse, json, os, re, sys, types

FIXTURE_SENTENCE = re.compile(r"^[а-яё+\s.,!?;:\-—«»\"()]+$", re.IGNORECASE)


class _NoAccent:
    def load(self, **kw):
        pass

    def process_all(self, text):
        return text


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("parquet")
    ap.add_argument("kokoro_ru")
    ap.add_argument("out", nargs="?", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus_ru.json"))
    ap.add_argument("--count", type=int, default=200)
    ap.add_argument("--stride", type=int, default=20, help="every n-th eligible sentence, so the fixture spreads over the corpus")
    a = ap.parse_args()
    import pyarrow.parquet as pq
    sys.modules["ruaccent"] = types.SimpleNamespace(RUAccent=_NoAccent)
    sys.path.insert(0, a.kokoro_ru)
    from ru_g2p import RuG2P
    g2p = RuG2P(espeak_data=os.path.join(a.kokoro_ru, "espeak-data"), vocab_path=os.path.join(a.kokoro_ru, "kokoro-config.json"))
    table = pq.read_table(a.parquet, columns=["accent", "punct"])
    rows = []
    eligible = 0
    for i in range(table.num_rows):
        marked = str(table.column("accent")[i]).strip()
        text = str(table.column("punct")[i]).strip()
        if not (20 <= len(marked) <= 200) or not FIXTURE_SENTENCE.match(marked) or "+" not in marked:
            continue
        eligible += 1
        if eligible % a.stride != 1:
            continue
        ps, oov = g2p.phonemize_accented(marked)
        if oov or len(marked.split()) != len(ps.split()):
            continue
        rows.append({"id": f"ru{len(rows):03d}", "text": text, "marked": marked, "ps": ps})
        if len(rows) >= a.count:
            break
    with open(a.out, "w", encoding="utf8") as f:
        json.dump(rows, f, ensure_ascii=False, indent=1)
        f.write("\n")
    print(f"wrote {a.out}: {len(rows)} sentences")


if __name__ == "__main__":
    main()
