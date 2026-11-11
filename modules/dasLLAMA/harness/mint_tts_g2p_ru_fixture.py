#!/usr/bin/env python3
# Mint tests/_tts_fixtures/g2p_corpus_ru.json - the 200-sentence Russian front-end fixture the
# letter-to-sound test compares against: stress-marked sentences (`+` before the stressed vowel;
# the sentences are written for this fixture and marked by `tts_mark_stress`) and the phonemes
# kokoro-ru's own front end (`ru_g2p.py`: espeak-ng over the recompiled acute-aware Russian data,
# then its vowel reduction and long щ) reads them as, its stress pass stubbed out - the fixture
# isolates letter-to-sound from stress, so a stress disagreement can never move its numbers.
#
#   python mint_tts_g2p_ru_fixture.py <tsv: a sentence, a tab, the sentence stress-marked> <kokoro-ru dir> [out.json] [--count 200]
#
# Runs under PYTHONUTF8=1 in a venv with misaki and phonemizer-fork importable; <kokoro-ru dir> is a
# checkout of the zaakirio/kokoro-ru repository (ru_g2p.py, kokoro-config.json, espeak-data/).
import argparse, json, os, re, sys, types

FIXTURE_SENTENCE = re.compile(r"^[а-яё+\s.,!?;:\-—«»\"()]+$", re.IGNORECASE)


class _NoAccent:
    def load(self, **kw):
        pass

    def process_all(self, text):
        return text


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("sentences")
    ap.add_argument("kokoro_ru")
    ap.add_argument("out", nargs="?", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus_ru.json"))
    ap.add_argument("--count", type=int, default=200)
    a = ap.parse_args()
    sys.modules["ruaccent"] = types.SimpleNamespace(RUAccent=_NoAccent)
    sys.path.insert(0, a.kokoro_ru)
    from ru_g2p import RuG2P
    g2p = RuG2P(espeak_data=os.path.join(a.kokoro_ru, "espeak-data"), vocab_path=os.path.join(a.kokoro_ru, "kokoro-config.json"))
    rows = []
    with open(a.sentences, encoding="utf8") as f:
        for line in f:
            text, _, marked = line.rstrip("\n").partition("\t")
            text, marked = text.strip(), marked.strip()
            if not (20 <= len(marked) <= 200) or not FIXTURE_SENTENCE.match(marked) or "+" not in marked:
                print(f"skipped (shape): {text}")
                continue
            ps, oov = g2p.phonemize_accented(marked)
            if oov or len(marked.split()) != len(ps.split()):
                print(f"skipped (the reference front end): {text}")
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
