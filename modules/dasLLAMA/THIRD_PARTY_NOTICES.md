# dasLLAMA third-party notices

This module contains code derived from, and ships or consumes artifacts governed by, the
following third-party works. The referenced license texts are checked in beside this file
and install with the SDK.

## llama.cpp / ggml (MIT)

Portions of this module's inference kernels, quantization codecs, tokenizer tables, and
file-format handling derive from the llama.cpp / ggml project
(https://github.com/ggml-org/llama.cpp), Copyright (c) 2023-2026 The ggml authors, MIT
License - see `LICENSE.LLAMA_CPP`. The GGUF container and the `ggml-*` model/vocabulary
fixture formats originate there.

## whisper.cpp (MIT)

The Parakeet ASR implementation (`dasllama/dasllama_parakeet.das`) and parts of the
Whisper-family audio front-end derive from the whisper.cpp project
(https://github.com/ggml-org/whisper.cpp), Copyright (c) 2023-2026 The ggml authors, MIT
License - see `LICENSE.PARAKEET`. Its `ggml-*.bin` model container is read by this module.

## GigaAM (MIT)

The GigaAM ASR implementation (`dasllama/dasllama_gigaam.das`) reimplements the GigaAM v3
model of the GigaAM project (https://github.com/salute-developers/GigaAM), Copyright (c) 2024
GigaChat Team, MIT License - see `LICENSE.GIGAAM`; no GigaAM source is included. Its weights
(`ai-sage/GigaAM-v3` on Hugging Face, MIT License) are read from the `cstr/gigaam-v3-GGUF`
conversion (https://huggingface.co/cstr/gigaam-v3-GGUF), a format conversion of those weights
whose MIT License is inherited from `ai-sage/GigaAM-v3`; neither is redistributed here. The Russian
test and bench clips (`benchmarks/asr/corpus/ru_*.wav`, minted on-box by
`benchmarks/asr/fetch_fleurs_ru.py`, never committed) are cut from the FLEURS `ru_ru` test split
(`google/fleurs`, https://huggingface.co/datasets/google/fleurs, Google), Creative Commons
Attribution 4.0 International - see `LICENSE.CC-BY-4.0`; `ru_fleurs.json` beside them names each
clip's FLEURS id and carries its reference transcription.

The Russian word-error measurements in `PERF_LEDGER.md` also read the Golos test sets (SberDevices,
https://www.openslr.org/114/, through the Hugging Face re-hosts `bond005/sberdevices_golos_10h_crowd`
and `bond005/sberdevices_golos_100h_farfield`), under SberDevices' "Public license with attribution
and conditions reserved" (https://github.com/sberdevices/golos/blob/master/license/en_us.pdf, a
reworking of CC BY-SA 4.0 that is not a Creative Commons license). No Golos audio or text is
redistributed here or used by a test.

## vLLM (Apache-2.0)

The MoE prefill's expert-bucketing design and the per-expert tile-selection ladders were
informed by the vLLM project's fused-MoE kernels and tuned-configuration tables
(https://github.com/vllm-project/vllm), Apache License 2.0 - see `LICENSE.VLLM`. No vLLM
source code is included in this repository.

## MLX / mlx-lm (MIT)

The Metal prefill's recurrent-scan kernel - the packed state-column layout of `MetalDnScanT` in
`dasllama/dasllama_metal_kernels.das` - derives from the gated delta scan kernel of the mlx-lm
project (https://github.com/ml-explore/mlx-lm), and other Metal prefill kernels were informed by
the MLX project's Metal kernels (https://github.com/ml-explore/mlx), Copyright (c) 2023 Apple
Inc., MIT License - see `LICENSE.MLX`.

## Silero VAD (MIT)

The voice-activity-detection implementation and the shipped `models/silero_vad.bin` weights
derive from the Silero VAD project (https://github.com/snakers4/silero-vad), Copyright (c)
2020-present Silero Team, MIT License - see `LICENSE.SILERO`.

## Firefox Translations (MPL-2.0)

The Bergamot translation students the engine serves (`model.<pair>.intgemm.alphas.bin` with the
`vocab.<pair>.spm` beside it) are Mozilla's Firefox Translations models
(https://github.com/mozilla/firefox-translations-models, https://github.com/mozilla/translations),
Copyright Mozilla Foundation, Mozilla Public License 2.0. The files are fetched from Mozilla's
translation-model bucket byte for byte; the ru-en student the interpreter example ships is mirrored
at https://huggingface.co/borisbat/dasllama-nmt with the licence text beside it.

## Text-to-speech front end

The TTS front end and the three TTS families reimplement, and pack data from, the following
works. The packed files (`tts_g2p.bin` and its American-only twin `tts_g2p_en_us.bin`, `tts_postag.bin`, `kitten-nano.gguf`,
`kitten-mini.gguf`, `kokoro-82m.gguf`, `pocket-tts-<language>.gguf`, the `tts_oracle/` dumps) are built by the `harness/` scripts into the
model store (`performance/build_tts_data.das`), each beside a license file naming these
sources; nothing under this repository redistributes the data itself.

- **misaki** (https://github.com/hexgrad/misaki), Apache License 2.0 - see `LICENSE.APACHE-2.0`.
  The grapheme-to-phoneme rules of `dasllama/dasllama_g2p.das` derive from its English module,
  its `us_gold.json` / `us_silver.json` lexicons are packed into `tts_g2p.bin`, and so are its
  British `gb_gold.json` / `gb_silver.json` lexicons - the two dialects merged into one table
  per tier. The British half of the front-end test fixture
  (`tests/_tts_fixtures/g2p_corpus_gb.json`) is its British front end's own output.
- **CMUdict 0.7a**, Copyright (C) 1993-2008 Carnegie Mellon University, BSD 2-Clause - see
  `LICENSE.CMUDICT`. First pronunciations, rendered into the front end's inventory, are packed
  into `tts_g2p.bin` as the fallback lexicon.
- **g2p_en** (https://github.com/Kyubyong/g2p), Copyright Kyubyong Park and Jongseok Kim, Apache
  License 2.0 - see `LICENSE.APACHE-2.0`. Its GRU spelling-model weights (`checkpoint20.npz`)
  are packed into `tts_g2p.bin`; the decoder is reimplemented in `dasllama/dasllama_g2p.das`.
- **KittenTTS** (https://github.com/KittenML/KittenTTS), Apache License 2.0 - see
  `LICENSE.APACHE-2.0`. The text normalizer of `dasllama/dasllama_textnorm.das` reimplements the
  semantics of its `normalize_text`, with fixes; `dasllama/dasllama_kitten.das` carries its
  `TextCleaner` symbol table and driver conventions, and the model weights of
  `KittenML/kitten-tts-nano-0.8` and `KittenML/kitten-tts-mini-0.8` (Apache License 2.0; ONNX
  graphs, `voices.npz`, `config.json` at the revisions pinned in `harness/kitten_graph.py`) are
  converted into `kitten-<size>.gguf` by `harness/convert_kitten.py`. The model architecture is
  StyleTTS2 (Yinghao Aaron Li et al., MIT License - see `LICENSE.STYLETTS2`) as the checkpoints
  instantiate it, reimplemented in `dasllama/dasllama_tts_blocks.das`.
- **Kokoro** (https://github.com/hexgrad/kokoro), Apache License 2.0 - see `LICENSE.APACHE-2.0`.
  The `hexgrad/Kokoro-82M` weights and voice packs (Apache License 2.0, at the revision pinned in
  `harness/kitten_graph.py`) are converted into `kokoro-82m.gguf` by `harness/convert_kokoro.py`;
  `dasllama/dasllama_kokoro.das` carries the pipeline's vocabulary and style-row rule. The model
  architecture is StyleTTS2 (Yinghao Aaron Li et al., MIT License - see `LICENSE.STYLETTS2`; the
  kokoro package's decoder and iSTFTNet modules are adapted from it), shared with KittenTTS in
  `dasllama/dasllama_styletts2.das` over the block home.
- **Pocket TTS** (https://github.com/kyutai-labs/pocket-tts), Kyutai, MIT License - see
  `LICENSE.POCKET_TTS` (the reference implementation, reimplemented in
  `dasllama/dasllama_pocket.das`; no Kyutai source is included).
  The `kyutai/pocket-tts` weights, per language, and their unigram SentencePiece tokenizers are
  Creative Commons Attribution 4.0 International - see `LICENSE.CC-BY-4.0` - converted into
  `pocket-tts-<language>.gguf` by `harness/convert_pocket.py` at the revisions the package's
  configs pin. The bundled voice clips come from `kyutai/tts-voices`: the `voice-zero`
  and `voice-donations` folders are CC0, the `vctk` clips (VCTK, CSTR, University of Edinburgh)
  and the `alba-mackenna` clip CC BY 4.0; the package's two clips from CC BY-NC folders are
  not shipped. Each `.LICENSE` sidecar lists its file's clips and their sources.
- **spaCy** (https://github.com/explosion/spaCy), Copyright ExplosionAI GmbH, MIT License - see
  `LICENSE.SPACY`. Its English tokenizer exception table is exported into `tts_postag.bin`, and
  its `en_core_web_sm` tagger provides the silver part-of-speech tags the tagger trains on.
- **Universal Dependencies English-EWT** (https://github.com/UniversalDependencies/UD_English-EWT),
  CC BY-SA 4.0 - see `LICENSE.UD_EWT`. The gold part-of-speech training data of the tagger in
  `tts_postag.bin`; the treebank itself is not redistributed.
- **Project Gutenberg** texts (public domain) are the silver-tagged training prose.

## Model weights (not distributed in this repository)

Model weight files this module loads are fetched separately and carry their own terms; in
particular the NVIDIA Parakeet TDT and Canary-Qwen checkpoints are released under
CC-BY-4.0 (attribution required if the weights, or converted forms of them, are
redistributed), and each LLM checkpoint carries its publisher's model license. Nothing in
this repository redistributes those weights.
