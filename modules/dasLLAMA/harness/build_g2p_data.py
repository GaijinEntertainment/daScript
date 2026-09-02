#!/usr/bin/env python3
# Pack the TTS grapheme-to-phoneme data into tts_g2p.bin for dasllama_g2p.das:
#   - the gold and silver US-English lexicons (misaki 0.9.4 `us_gold.json` / `us_silver.json`,
#     Apache-2.0), values either one phoneme string or a part-of-speech dictionary,
#   - CMUdict 0.7a (BSD-2, the NLTK distribution) with every entry's first pronunciation
#     ALREADY rendered into the misaki inventory through the calibrated ARPAbet table
#     (results/calibration.json of the G2P fidelity experiment), so the runtime fallback is a
#     plain lookup,
#   - the g2p_en 2.1.0 GRU sequence-to-sequence OOV model (Apache-2.0, checkpoint20.npz) with its
#     grapheme and ARPAbet output alphabets.
# Every lexicon is a byte-sorted string table (offset columns + one key blob + one value blob),
# looked up by binary search on the mapped bytes - nothing is parsed at load.
#
#   python build_g2p_data.py <experiment_root> --out tts_g2p.bin [--local-additions words.json]
#
# Runs inside the experiment's venv (misaki, g2p_en importable; NLTK_DATA under the root).
import argparse, json, os, struct, sys

MAGIC = 0x50324754  # 'TG2P'
VERSION = 1


def read_cmudict(path):
    """word -> first pronunciation (ARPAbet with stress digits), lowercased keys, file order.
    The NLTK rows read `WORD <variant> PH PH ...`; the classic rows `WORD  PH PH ...` or
    `WORD(2)  PH PH ...` - all three shapes land here."""
    out = {}
    for line in open(path, encoding="latin-1"):
        if line.startswith(";;;") or not line.strip():
            continue
        parts = line.split()
        word = parts[0].lower()
        if "(" in word and word.endswith(")"):
            word = word[:word.index("(")]
        phones = parts[1:]
        if phones and phones[0].isdigit():
            phones = phones[1:]
        if word not in out and phones:
            out[word] = phones
    return out


def string_table(entries):
    """entries: sorted list of (key bytes, value bytes) -> packed section bytes."""
    key_blob = bytearray()
    val_blob = bytearray()
    key_off = [0]
    val_off = [0]
    for k, v in entries:
        key_blob += k
        val_blob += v
        key_off.append(len(key_blob))
        val_off.append(len(val_blob))
    buf = bytearray()
    buf += struct.pack("<I", len(entries))
    buf += struct.pack(f"<{len(key_off)}I", *key_off)
    buf += struct.pack(f"<{len(val_off)}I", *val_off)
    buf += struct.pack("<I", len(key_blob)) + key_blob
    buf += struct.pack("<I", len(val_blob)) + val_blob
    return buf


def encode_value(v):
    if isinstance(v, str):
        return b"\x00" + v.encode("utf8")
    assert isinstance(v, dict) and "DEFAULT" in v, v
    out = bytearray(b"\x01")
    out += struct.pack("<B", len(v))
    for tag, ps in v.items():
        tb = tag.encode("utf8")
        out += struct.pack("<B", len(tb)) + tb
        if ps is None:
            out += struct.pack("<BH", 0, 0)
        else:
            pb = ps.encode("utf8")
            out += struct.pack("<BH", 1, len(pb)) + pb
    return bytes(out)


def lexicon_section(d):
    entries = sorted((k.encode("utf8"), encode_value(v)) for k, v in d.items())
    return string_table(entries)


def tensor_section(arrays, graphemes, phonemes):
    buf = bytearray()
    buf += struct.pack("<I", len(arrays))
    for name, a in arrays:
        nb = name.encode("utf8")
        buf += struct.pack("<B", len(nb)) + nb
        buf += struct.pack("<B", a.ndim) + struct.pack(f"<{a.ndim}I", *a.shape)
        buf += a.astype("<f4").tobytes()
    for alphabet in (graphemes, phonemes):
        buf += struct.pack("<I", len(alphabet))
        for s in alphabet:
            sb = s.encode("utf8")
            buf += struct.pack("<B", len(sb)) + sb
    return buf


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("root")
    ap.add_argument("--out", required=True)
    ap.add_argument("--local-additions", help="json {word: misaki phonemes} merged over the gold tier")
    ap.add_argument("--focus-words", help="also write the tag-keyed gold words, one per line, lowercased - the "
                    "tagger's silver prose is sampled around them (mint_postag_silver.py --focus-words)")
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.root, "scripts"))
    os.environ.setdefault("NLTK_DATA", os.path.join(a.root, ".nltk"))
    import numpy as np
    import g2p_common as G
    import misaki, g2p_en

    data_dir = os.path.join(os.path.dirname(misaki.__file__), "data")
    gold = json.load(open(os.path.join(data_dir, "us_gold.json"), encoding="utf8"))
    silver = json.load(open(os.path.join(data_dir, "us_silver.json"), encoding="utf8"))
    if a.local_additions:
        gold.update(json.load(open(a.local_additions, encoding="utf8")))
    if a.focus_words:
        focus = sorted({w.lower() for w, v in gold.items() if isinstance(v, dict)})
        with open(a.focus_words, "w", encoding="utf8") as f:
            f.write("\n".join(focus) + "\n")
        print(f"wrote {a.focus_words}: {len(focus)} tag-keyed words")
    cmu_path = os.path.join(os.environ["NLTK_DATA"], "corpora", "cmudict", "cmudict")
    cmu = read_cmudict(cmu_path)
    calib = G.load_calibration()["arpabet_misaki"]
    cmu_misaki = {w: G.arpabet_to_ipa(G.flap(ph, calib["flap"]), "misaki", calib["opts"]) for w, ph in cmu.items()}

    graphemes = ["<pad>", "<unk>", "</s>"] + list("abcdefghijklmnopqrstuvwxyz")
    ckpt = np.load(os.path.join(os.path.dirname(g2p_en.__file__), "checkpoint20.npz"))
    # the GRU's output alphabet, as g2p_en declares it
    phonemes = ["<pad>", "<unk>", "<s>", "</s>"] + [
        "AA0", "AA1", "AA2", "AE0", "AE1", "AE2", "AH0", "AH1", "AH2", "AO0", "AO1", "AO2", "AW0", "AW1", "AW2",
        "AY0", "AY1", "AY2", "B", "CH", "D", "DH", "EH0", "EH1", "EH2", "ER0", "ER1", "ER2", "EY0", "EY1", "EY2",
        "F", "G", "HH", "IH0", "IH1", "IH2", "IY0", "IY1", "IY2", "JH", "K", "L", "M", "N", "NG", "OW0", "OW1",
        "OW2", "OY0", "OY1", "OY2", "P", "R", "S", "SH", "T", "TH", "UH0", "UH1", "UH2", "UW", "UW0", "UW1", "UW2",
        "V", "W", "Y", "Z", "ZH"]
    assert len(phonemes) == ckpt["fc_b"].shape[0], (len(phonemes), ckpt["fc_b"].shape)
    arrays = [(k, ckpt[k]) for k in ["enc_emb", "enc_w_ih", "enc_w_hh", "enc_b_ih", "enc_b_hh",
                                     "dec_emb", "dec_w_ih", "dec_w_hh", "dec_b_ih", "dec_b_hh", "fc_w", "fc_b"]]

    buf = bytearray()
    buf += struct.pack("<II", MAGIC, VERSION)
    source = ("misaki 0.9.4 us_gold/us_silver; CMUdict 0.7a first pronunciations via the calibrated "
              "ARPAbet->misaki table; g2p_en 2.1.0 checkpoint20").encode("utf8")
    buf += struct.pack("<H", len(source)) + source
    for d in (gold, silver, cmu_misaki):
        buf += lexicon_section(d)
    buf += tensor_section(arrays, graphemes, phonemes)
    with open(a.out, "wb") as f:
        f.write(buf)
    with open(a.out + ".LICENSE", "w", encoding="utf8") as f:
        f.write("tts_g2p.bin - derived data, see the dasLLAMA THIRD_PARTY_NOTICES.md\n\n"
                "misaki us_gold.json / us_silver.json (hexgrad, Apache License 2.0)\n"
                "CMUdict 0.7a (Copyright (C) 1993-2008 Carnegie Mellon University, BSD 2-Clause), "
                "first pronunciations rendered into the misaki inventory\n"
                "g2p_en checkpoint20.npz (Kyubyong Park, Apache License 2.0)\n")
    print(f"wrote {a.out}: {len(buf)} bytes; gold {len(gold)} (pos-conditioned "
          f"{sum(isinstance(v, dict) for v in gold.values())}), silver {len(silver)}, cmudict {len(cmu_misaki)}, "
          f"gru {sum(v.size for _, v in arrays)} floats, {len(graphemes)} graphemes, {len(phonemes)} phonemes")


if __name__ == "__main__":
    main()
