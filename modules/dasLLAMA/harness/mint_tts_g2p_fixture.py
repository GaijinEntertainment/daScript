#!/usr/bin/env python3
# Mint tests/_tts_fixtures/g2p_corpus.json - the 200-sentence front-end fixture the TTS
# text-normalizer, tokenizer/tagger and G2P tests compare against.
#
# Source of truth is the G2P fidelity experiment (results/phonemes.json: every arm's phoneme
# output per sentence, the KittenTTS-normalized text each arm was fed) plus spaCy's
# en_core_web_sm tokenization + PTB tags of that normalized text (the tokenizer/tagger oracle)
# and the heteronym annotations of scripts/heteronym_score.py rendered into the misaki
# inventory with the calibrated ARPAbet table.
#
#   python mint_tts_g2p_fixture.py <experiment_root> [out.json]
#
# Runs inside the experiment's venv (misaki 0.9.4, spacy 3.8 + en_core_web_sm 3.8.0,
# g2p_en 2.1.0 - see results/env.json there).
import json, os, sys

# The spoken form the rig scores against where the upstream normalizer's reading (`norm`, what
# the phoneme and tagger cells still compare on) is not what a person says: units left as
# their symbols, a fraction read digit by digit, a duration glued to the next word, a decade
# misspelled, pence called cents. Hand-corrected; the reference arms are re-scored on the same.
CORRECTED_EXPECTED = {
    "hv017": "The soft cushion broke the man's fall.",
    "nm006": "The package weighs two point five kilograms and ships tomorrow.",
    "nm007": "He ran the marathon in three twelve and forty-five seconds last spring.",
    "nm008": "The server has sixty-four gigabytes of memory and twelve cores.",
    "nm009": "Water boils at one hundred degrees Celsius at sea level.",
    "nm011": "Please read chapter twelve, pages three hundred forty to three hundred fifty-eight.",
    "nm015": "The recipe calls for one half cup of sugar.",
    "nm018": "The signal arrived one hundred fifty milliseconds after the trigger.",
    "nm020": "The processor runs at three point two gigahertz under load.",
    "nm023": "The nineteen sixties produced remarkable music.",
    "nm025": "The temperature dropped to five degrees Fahrenheit overnight.",
    "nm026": "We shipped two hundred fifty units at fourteen pounds and fifty pence each.",
    "nm027": "The measurement was zero point seven five millimeters plus or minus zero point zero two millimeters.",
}

def main():
    root = sys.argv[1]
    out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus.json")
    scripts = os.path.join(root, "scripts")
    sys.path.insert(0, scripts)
    os.environ.setdefault("HF_HOME", os.path.join(root, ".hf"))
    import g2p_common as G
    import spacy
    nlp = spacy.load("en_core_web_sm", enable=["tok2vec", "tagger"])

    # heteronym annotations: the ANNOT table of heteronym_score.py, nothing else from it
    hs_src = open(os.path.join(scripts, "heteronym_score.py"), encoding="utf8").read()
    hs_src = hs_src.split("def bare")[0].replace("import g2p_common as G", "")
    ns = {"__file__": os.path.join(scripts, "heteronym_score.py"), "G": G}
    exec(hs_src, ns)
    cm = G.load_calibration()["arpabet_misaki"]
    het = {}
    for idx, word, a1, a2 in ns["ANNOT"]:
        if word is None:
            continue
        het[f"ht{idx:03d}"] = {
            "word": word,
            "form1": G.arpabet_to_ipa(G.flap(a1.split(), cm["flap"]), "misaki", cm["opts"]),
            "form2": G.arpabet_to_ipa(G.flap(a2.split(), cm["flap"]), "misaki", cm["opts"]),
        }

    recs = json.load(open(os.path.join(root, "results", "phonemes.json"), encoding="utf8"))
    rows = []
    for r in recs:
        e = r["arms"]["E_misaki_cmudict"]
        doc = nlp(r["norm"])
        row = {"id": r["id"], "category": r["category"], "text": r["text"], "norm": r["norm"],
               "ps_misaki": e["ps_misaki"], "ps_espeak": e["ps_espeak"],
               "tokens": [[t.text, t.tag_, t.whitespace_] for t in doc]}
        if r["id"] in CORRECTED_EXPECTED:
            row["expected"] = CORRECTED_EXPECTED[r["id"]]
        if r["id"] in het:
            row["heteronym"] = het[r["id"]]
        rows.append(row)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w", encoding="utf8") as f:
        json.dump(rows, f, ensure_ascii=False, indent=0)
    tags = sorted({t[1] for r in rows for t in r["tokens"]})
    print(f"wrote {out_path}: {len(rows)} rows, {sum('heteronym' in r for r in rows)} heteronym rows, "
          f"{os.path.getsize(out_path)} bytes, {len(tags)} tags: {' '.join(tags)}")

if __name__ == "__main__":
    main()
