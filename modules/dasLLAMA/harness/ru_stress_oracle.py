"""The PyTorch reference for the Russian stress-context model (`dasllama_g2p_ru_stress.das`).

    python ru_stress_oracle.py --seed-head <checkpoint dir>
    python ru_stress_oracle.py --checkpoint <dir> --corpus <sentences.jsonl> --catalog <forms.jsonl> \
                               --tokens <tests/_tts_fixtures/ru_stress_tokens.json> --dumps <models dir>/tts_oracle/ru_stress

`--seed-head` writes `stress_head.pt` beside a pretrained encoder: a `Linear(hidden, 8)` drawn under a
fixed seed - the parity rail compares numbers, not readings. `--corpus` is a text corpus the caller
supplies, one {"id", "text", "form"} row a line, `form` a word of the catalog the sentence holds.

The second form mints both fixtures from the checkpoint directory the converter read: the tokenizer
fixture (about 200 of the corpus's sentences plus marked, Latin, digit, long-word,
punctuation and over-length cases - each sentence's ids and the codepoint offsets of every piece, the
HuggingFace tokenizer run on the lowercased text, truncated to 96 pieces) and one TTSO dump a sentence
for the encoder cases: the ids, the last hidden state, the target word's codepoint span, the head's raw
logits over the pooled pieces, the catalog's ordinal mask and the masked argmax. A sentence whose
lowercasing changes its length is left out, since the das side lowercases codepoint for codepoint.
"""
import argparse
import json
import os
import re
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tts_oracle_io import write_dump  # noqa: E402

N_ORDINALS = 8
MAX_PIECES = 96
HEAD_SEED = 290
STRESS = "́"
VOWELS = "аеёиоуыэюя"
LATIN_TAILS = ["Он купил iPhone и MacBook Pro.", "Сеть Wi-Fi не работала.", "Café и naïve - чужие слова.",
               "Это был Bluetooth-адаптер.", "Читайте The New York Times!", "Прошивка ESP32 на Arduino.",
               "Он сказал: OK, deal.", "Пакет Ubuntu.", "Strasse и Straße.", "Ссылка: https://example.org/a_b?c=1."]
DIGIT_TAILS = ["Это было в 2024 году.", "Цена 1 250,50 руб.", "Звоните +7 (495) 123-45-67.", "Счёт 3:2 в пользу гостей.",
               "Температура -15°C.", "Рост на 12,5%.", "Глава 7, пункт 3.1.", "В 10:30 и в 22.45."]
ODD_TAILS = [" \U0001F642 Смайлик.", " 中文 иероглифы.", " Ǆ и ǅ.", " неразрывный пробел.",
             " ноль‍ширины и­mягкий перенос.", " табуляция\tи\r\nперевод.", " греческий Ἀθῆναι.",
             " стрелка → и «ёлочки» — и „лапки“…"]


def seed_head(checkpoint):
    import torch
    from transformers import AutoConfig
    cfg = AutoConfig.from_pretrained(checkpoint)
    torch.manual_seed(HEAD_SEED)
    head = torch.nn.Linear(cfg.hidden_size, N_ORDINALS)
    path = os.path.join(checkpoint, "stress_head.pt")
    torch.save(head.state_dict(), path)
    print(f"wrote {path}: Linear({cfg.hidden_size}, {N_ORDINALS}) under seed {HEAD_SEED}")


def locate(text, form):
    m = re.search(rf"(?<![а-яё]){re.escape(form)}(?![а-яё])", text)
    return m.span() if m else None


def read_rows(path):
    with open(path, encoding="utf8") as f:
        return [json.loads(line) for line in f]


def catalog_masks(path):
    masks = {}
    for row in read_rows(path):
        for v in row["variants"]:
            masks[row["form"].lower()] = masks.get(row["form"].lower(), 0) | (1 << (v["ordinal"] - 1))
    return masks


def mark_first_vowel(text, form):
    span = locate(text.lower(), form)
    if span is None:
        return None
    for i in range(span[0], span[1]):
        if text[i].lower() in VOWELS:
            return text[:i + 1] + STRESS + text[i + 1:]
    return None


def every_kth(rows, count):
    step = max(1, len(rows) // count)
    return rows[::step][:count]


def pick_token_cases(rows):
    """The tokenizer fixture's sentences, each with the property it covers."""
    rows = [r for r in rows if len(r["text"].lower()) == len(r["text"])]
    cases = [("plain", r["text"]) for r in every_kth(rows, 140)]
    cases += [("yo", r["text"]) for r in every_kth([r for r in rows if "ё" in r["text"].lower()], 12)]
    cases += [("short-i", r["text"]) for r in every_kth([r for r in rows if "й" in r["text"].lower()], 8)]
    hosts = every_kth(rows[3::11], len(LATIN_TAILS) + len(DIGIT_TAILS) + len(ODD_TAILS))
    cases += [("latin", h["text"] + " " + tail) for h, tail in zip(hosts, LATIN_TAILS)]
    cases += [("digits", h["text"] + " " + tail) for h, tail in zip(hosts[len(LATIN_TAILS):], DIGIT_TAILS)]
    cases += [("odd-chars", h["text"] + tail) for h, tail in zip(hosts[len(LATIN_TAILS) + len(DIGIT_TAILS):], ODD_TAILS)]
    cases += [("long-word", r["text"]) for r in every_kth([r for r in rows if re.search("[А-Яа-яЁё]{16,}", r["text"])], 10)]
    cases += [("punct", r["text"]) for r in every_kth([r for r in rows if re.search("[«»—…„“]", r["text"])], 10)]
    marked = [m for m in (mark_first_vowel(r["text"], r["form"]) for r in every_kth(rows[1::7], 40)) if m]
    cases += [("stress-mark", m) for m in marked[:15]]
    joined = [" ".join(r["text"] for r in rows[i * 9:i * 9 + 9]) for i in range(5)]
    cases += [("over-length", j) for j in joined]
    return cases


def pick_encoder_cases(rows, masks):
    rows = [r for r in rows if len(r["text"].lower()) == len(r["text"]) and r["form"] in masks
            and locate(r["text"].lower(), r["form"])]
    return every_kth(rows, 20)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seed-head", default="", help="write a seeded stress_head.pt into this checkpoint directory and stop")
    ap.add_argument("--checkpoint", default="")
    ap.add_argument("--corpus", default="")
    ap.add_argument("--catalog", default="")
    ap.add_argument("--tokens", default="", help="the tokenizer fixture json to write")
    ap.add_argument("--dumps", default="", help="the directory the encoder dumps and their manifest go to")
    a = ap.parse_args()
    if a.seed_head:
        seed_head(a.seed_head)
        return
    import torch
    from transformers import AutoModel, AutoTokenizer
    tok = AutoTokenizer.from_pretrained(a.checkpoint)
    rows = read_rows(a.corpus)

    out = []
    for kind, text in pick_token_cases(rows):
        enc = tok(text.lower(), truncation=True, max_length=MAX_PIECES, return_offsets_mapping=True)
        out.append({"kind": kind, "text": text, "ids": enc["input_ids"],
                    "offsets": [x for se in enc["offset_mapping"] for x in se]})
    with open(a.tokens, "w", encoding="utf8") as f:
        head = json.dumps({"checkpoint": os.path.basename(os.path.normpath(a.checkpoint)), "max_pieces": MAX_PIECES})
        f.write(head[:-1] + ', "cases": [\n')
        f.write(",\n".join(json.dumps(c, ensure_ascii=False, separators=(",", ":")) for c in out))
        f.write("\n]}\n")
    print(f"wrote {a.tokens}: {len(out)} sentences, {sum(len(c['ids']) for c in out)} pieces, "
          f"{sum(1 for c in out if len(c['ids']) == MAX_PIECES)} truncated")

    masks = catalog_masks(a.catalog)
    model = AutoModel.from_pretrained(a.checkpoint, dtype=torch.float32)
    model.eval()
    head = torch.load(os.path.join(a.checkpoint, "stress_head.pt"), map_location="cpu")
    os.makedirs(a.dumps, exist_ok=True)
    manifest = []
    with torch.no_grad():
        for i, r in enumerate(pick_encoder_cases(rows, masks)):
            text = r["text"].lower()
            span = locate(text, r["form"])
            enc = tok(text, truncation=True, max_length=MAX_PIECES, return_offsets_mapping=True, return_tensors="pt")
            hidden = model(input_ids=enc["input_ids"], attention_mask=enc["attention_mask"]).last_hidden_state[0]
            sel = [j for j, (s, e) in enumerate(enc["offset_mapping"][0].tolist()) if e > s and s < span[1] and e > span[0]]
            pooled = hidden[sel].mean(0)
            logits = pooled @ head["weight"].T + head["bias"]
            mask = masks[r["form"]]
            masked = [float(logits[o]) if mask & (1 << o) else -1e9 for o in range(N_ORDINALS)]
            ordinal = int(np.argmax(masked)) + 1
            name = f"case_{i:02d}.ttso"
            write_dump(os.path.join(a.dumps, name), {
                "ids": enc["input_ids"][0].numpy(),
                "hidden": hidden.numpy().astype(np.float32),
                "span": np.array(span, dtype=np.int32),
                "logits": logits.numpy().astype(np.float32),
                "mask": np.array([mask], dtype=np.int32),
                "ordinal": np.array([ordinal], dtype=np.int32)})
            manifest.append({"file": name, "id": r["id"], "text": r["text"], "form": r["form"], "span_start": span[0],
                             "span_end": span[1], "mask": mask, "ordinal": ordinal, "n_pieces": int(enc["input_ids"].shape[1])})
    with open(os.path.join(a.dumps, "manifest.json"), "w", encoding="utf8") as f:
        json.dump({"checkpoint": os.path.basename(os.path.normpath(a.checkpoint)), "head_seed": HEAD_SEED, "cases": manifest},
                  f, ensure_ascii=False, indent=1)
    print(f"wrote {len(manifest)} encoder cases to {a.dumps}")


if __name__ == "__main__":
    main()
