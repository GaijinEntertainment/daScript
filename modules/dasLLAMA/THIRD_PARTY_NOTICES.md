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

## vLLM (Apache-2.0)

The MoE prefill's expert-bucketing design and the per-expert tile-selection ladders were
informed by the vLLM project's fused-MoE kernels and tuned-configuration tables
(https://github.com/vllm-project/vllm), Apache License 2.0 - see `LICENSE.VLLM`. No vLLM
source code is included in this repository.

## Silero VAD (MIT)

The voice-activity-detection implementation and the shipped `models/silero_vad.bin` weights
derive from the Silero VAD project (https://github.com/snakers4/silero-vad), Copyright (c)
2020-present Silero Team, MIT License - see `LICENSE.SILERO`.

## Text-to-speech front end

The TTS front end and the two TTS families reimplement, and pack data from, the following
works. The packed files (`tts_g2p.bin` and its American-only twin `tts_g2p_en_us.bin`, `tts_postag.bin`, `kitten-nano.gguf`,
`kitten-mini.gguf`, `kokoro-82m.gguf`, the `tts_oracle/` dumps) are built by the `harness/` scripts into the
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
