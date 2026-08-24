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

## Silero VAD (MIT)

The voice-activity-detection implementation and the shipped `models/silero_vad.bin` weights
derive from the Silero VAD project (https://github.com/snakers4/silero-vad), Copyright (c)
2020-present Silero Team, MIT License - see `LICENSE.SILERO`.

## Model weights (not distributed in this repository)

Model weight files this module loads are fetched separately and carry their own terms; in
particular the NVIDIA Parakeet TDT and Canary-Qwen checkpoints are released under
CC-BY-4.0 (attribution required if the weights, or converted forms of them, are
redistributed), and each LLM checkpoint carries its publisher's model license. Nothing in
this repository redistributes those weights.
