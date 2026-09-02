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

The TTS front end reimplements, and packs data from, the following works. The packed files
(`tts_g2p.bin`, `tts_postag.bin`) are built by the `harness/` scripts into the model store,
each beside a `.LICENSE` sidecar naming these sources; nothing under this repository
redistributes the data itself.

- **misaki** (https://github.com/hexgrad/misaki), Apache License 2.0 - see `LICENSE.APACHE-2.0`.
  The grapheme-to-phoneme rules of `dasllama/dasllama_g2p.das` derive from its English module,
  and its `us_gold.json` / `us_silver.json` lexicons are packed into `tts_g2p.bin`.
- **CMUdict 0.7a**, Copyright (C) 1993-2008 Carnegie Mellon University, BSD 2-Clause - see
  `LICENSE.CMUDICT`. First pronunciations, rendered into the front end's inventory, are packed
  into `tts_g2p.bin` as the fallback lexicon.
- **g2p_en** (https://github.com/Kyubyong/g2p), Copyright Kyubyong Park and Jongseok Kim, Apache
  License 2.0 - see `LICENSE.APACHE-2.0`. Its GRU spelling-model weights (`checkpoint20.npz`)
  are packed into `tts_g2p.bin`; the decoder is reimplemented in `dasllama/dasllama_g2p.das`.
- **KittenTTS** (https://github.com/KittenML/KittenTTS), Apache License 2.0 - see
  `LICENSE.APACHE-2.0`. The text normalizer of `dasllama/dasllama_textnorm.das` reimplements the
  semantics of its `normalize_text`, with fixes.
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
