---
license: other
license_name: per-file
license_link: https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasLLAMA/THIRD_PARTY_NOTICES.md
language:
  - en
language_bcp47:
  - en-US
  - en-GB
pipeline_tag: text-to-speech
library_name: dasllama
tags:
  - daslang
  - dasllama
  - gguf
  - kittentts
  - kokoro
  - styletts2
---

# dasLLAMA text-to-speech models

The three speech models dasLLAMA serves, converted to GGUF, and the two front-end packs every
one of them loads. dasLLAMA is the daslang inference engine
(https://github.com/GaijinEntertainment/daScript, `modules/dasLLAMA`); the models are the
StyleTTS2 lineage - KittenTTS nano and mini, Kokoro-82M - behind one facade and one das-native
text front end (a normalizer, a part-of-speech tagger, a grapheme-to-phoneme pass). No espeak-ng,
no phonemizer: the front end is data, and the data is in the two packs.

## Files

| file | what | bytes | sha256 |
|---|---|---|---|
| `kitten-nano.gguf` | KittenTTS nano 0.8, f32 | 59331456 | `4556948c36a29e4be5ad521e597a20ae817059c404fea3c5d935afa73506d9da` |
| `kitten-mini.gguf` | KittenTTS mini 0.8, f32 | 295975008 | `e127a95c6ffa390eccc5956e7156ff055e9f30078cf16361cbcd0eea6c21fdb5` |
| `kokoro-82m.gguf` | Kokoro-82M v1.0 with its 54 voice packs, f32 | 352965024 | `e0d6584b5b650730bd62e7b105a2e9502384bbe3fbebf3030113560355e6232f` |
| `tts_g2p.bin` | the grapheme-to-phoneme pack: misaki's gold and silver lexicons in both English dialects (one merged table per tier), CMUdict 0.7a rendered into the American inventory, the g2p_en GRU spelling model | 14011554 | `d7b6afea7a0901a877d10531054d3f967845c58f415f0ea937decd8a3081ccd5` |
| `tts_postag.bin` | the tokenizer exception table and the averaged-perceptron PTB tagger | 12566510 | `38c2e85f7fef3e57d561d2aa0af25fccda4276376ba1993c3dbc2ae0ebfa57b4` |

The two packs sit beside whichever GGUF you load; the loader reads them from the model's
directory. The GGUFs carry f32 weights: dasLLAMA quantizes the served layouts to Q8_0 at first
load and keeps the result beside the file as a prepared image, so the f32 file is also the
reference lane.

## Use

```
dasllama-server --tts kitten-nano.gguf          # POST /v1/audio/speech, the OpenAI shape
daslang utils/dasllama-server/txt2wav.das -- --model kokoro-82m.gguf --voice af_heart --text "Hello." --out hello.wav
```

Kitten's eight voices are `expr-voice-2-m` through `expr-voice-5-f`, and the upstream's alias
names (`Bella`, `Jasper`, ...) are accepted for them. Kokoro ships 54 packs, of which the front
end drives the 28 English ones: the 20 American (`af_*`, `am_*` - `af_heart`, `am_adam`, ...)
and the 8 British (`bf_*`, `bm_*` - `bf_emma`, `bm_george`, ...), each phonemized in its own
dialect. The other 26 packs speak languages the front end does not phonemize yet, so they are
not listed and are refused. Every model speaks at 24 kHz. `GET /v1/stats` on the server lists
the served model's voices.

## Provenance

- `kitten-nano.gguf` / `kitten-mini.gguf`: converted by `modules/dasLLAMA/harness/convert_kitten.py`
  from `KittenML/kitten-tts-nano-0.8` at `7a1db645b1f3ab9420761d87428e042b9cec3f26` and
  `KittenML/kitten-tts-mini-0.8` at `c02725660cea441db4c383af69f1f26f5cd00947` (the ONNX weights,
  the voice table, the config).
- `kokoro-82m.gguf`: converted by `modules/dasLLAMA/harness/convert_kokoro.py` from
  `hexgrad/Kokoro-82M` at `f3ff3571791e39611d31c381e3a41a3af07b4987` (`kokoro-v1_0.pth` and the
  voice packs).
- `tts_g2p.bin`: `modules/dasLLAMA/harness/build_g2p_data.py` over misaki 0.9.4 (`us_gold.json`,
  `us_silver.json`, `gb_gold.json`, `gb_silver.json`), CMUdict 0.7a (the NLTK distribution) and
  g2p_en 2.1.0 (`checkpoint20.npz`), plus `harness/g2p_local_additions.json`.
- `tts_postag.bin`: `modules/dasLLAMA/harness/train_postag.py` over UD English-EWT and Project
  Gutenberg prose tagged by spaCy's `en_core_web_sm`; the tokenizer exception table is spaCy's.

The whole set is rebuilt by `modules/dasLLAMA/performance/build_tts_data.das`. Parity against
the reference implementations (block by block, and the front end sentence by sentence on a
200-sentence corpus) is the test suite under `modules/dasLLAMA/tests/test_tts_*.das`.

## Licences, per file

| file | licence | terms |
|---|---|---|
| `kitten-nano.gguf`, `kitten-mini.gguf` | Apache-2.0 | KittenML's weights, converted; `LICENSE.APACHE-2.0` |
| `kokoro-82m.gguf` | Apache-2.0 | hexgrad's weights and voices, converted; the architecture is StyleTTS2 (MIT, `LICENSE.STYLETTS2`) |
| `tts_g2p.bin` | Apache-2.0 and BSD-2-Clause | misaki and g2p_en (Apache-2.0), CMUdict (`LICENSE.CMUDICT`, Carnegie Mellon University) |
| `tts_postag.bin` | CC BY-SA 4.0 | the tagger weights are trained on UD English-EWT (`LICENSE.UD_EWT`); the exception table and the silver tags come from spaCy (MIT, `LICENSE.SPACY`); Gutenberg prose is public domain |

Each `.LICENSE` sidecar beside a file names its sources; the full texts are in this repository.
The engine that reads these files is under the daslang licence in its own repository.
