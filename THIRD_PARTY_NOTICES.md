# Third-party notices

daslang redistributes the components below under their own licences. Each licence text is
installed beside this file in the SDK's documentation directory under the name in the last
column; in the source tree it sits at the path given. A component that is only consulted at
development time (an oracle, a training corpus, a tagger that labels training data) is marked
dev-time: nothing of it ships, and the row records what the shipped artifact was derived from.

## Core runtime and tools

| Component | Upstream | Licence | Used for | Installed as |
|---|---|---|---|---|
| uriparser | https://github.com/uriparser/uriparser | BSD-3-Clause | URI parsing (`3rdparty/uriparser`) | `URIPARSER.LICENSE` |
| dag_noise | Gaijin Entertainment | BSD-3-Clause | noise generators (`include/dag_noise`) | `DAG_NOISE.LICENSE` |
| vecmath | Gaijin Entertainment | BSD-3-Clause | the SIMD vector-math layer (`include/vecmath`); the JIT's aarch64 polynomial rail mirrors its kernels | `VEC_MATH.LICENSE` |
| {fmt} | https://github.com/fmtlib/fmt | MIT | formatting (`3rdparty/fmt`) | `FMT.LICENSE` |
| fast_float | https://github.com/fastfloat/fast_float | MIT (elected) | float parsing (`include/fast_float`) | `FAST_FLOAT.LICENSE` |
| Luau | https://github.com/luau-lang/luau | MIT (Lua.org copyright) | the shortest-float emitter (`src/misc/luau_float2string.cpp`) | `LUAU.LICENSE` |
| dastest | Dmitri Granetchi | MIT | the test framework (`dastest/`) | `dastest/LICENSE` (with dastest) |
| das-fmt | Dmitri Granetchi | MIT | the formatter (`utils/das-fmt/`) | `utils/das-fmt/LICENSE` (with das-fmt) |
| OpenSSL | https://www.openssl.org | Apache-2.0 | TLS in dasHV (`modules/dasHV`) | `OPENSSL.LICENSE` |
| Droid Sans Mono | The Android Open Source Project | Apache-2.0 | the OpenGL example's font (`examples/opengl`) | `droidsansmono.LICENSE` (with the example) |
| glTF Sample Assets | The Khronos Group | per asset (see the file) | the glTF example's models (`examples/gltf/media`) | `GLTF_SAMPLE_ASSETS.LICENSE` (with the example) |

## dasLLAMA (`modules/dasLLAMA`)

| Component | Upstream | Licence | Used for | Installed as |
|---|---|---|---|---|
| llama.cpp / ggml | https://github.com/ggml-org/llama.cpp | MIT | the GGUF format, the quantization schemes and the reference kernels the engine ports | `LLAMA_CPP.LICENSE` |
| Parakeet conversion | https://github.com/ggml-org (ggml authors) | MIT | the parakeet-tdt ASR bins and their conversion | `PARAKEET.LICENSE` |
| Silero VAD | https://github.com/snakers4/silero-vad | MIT | the voice-activity weights shipped as `models/silero_vad.bin` | `SILERO.LICENSE` |
| vLLM | https://github.com/vllm-project/vllm | Apache-2.0 | the tq4 KV codec's stage-1-only design the engine shares with it (`ARCHITECTURE_INVARIANTS.md`) | `VLLM.LICENSE` |
| KittenTTS | https://github.com/KittenML/KittenTTS | Apache-2.0 | the kitten-nano / kitten-mini weights (`harness/convert_kitten.py`), the text normalizer `dasllama_textnorm.das` ports, the chunking rule and the phoneme symbol table | `APACHE-2.0.LICENSE` |
| Kokoro-82M | https://huggingface.co/hexgrad/Kokoro-82M | Apache-2.0 | the kokoro-82m weights and voices (`harness/convert_kokoro.py`), the StyleTTS2-derived decoder and vocoder `dasllama_styletts2.das` ports, the phoneme vocabulary | `APACHE-2.0.LICENSE` |
| misaki | https://github.com/hexgrad/misaki | Apache-2.0 | the US-English gold and silver lexicons packed into `tts_g2p.bin`, the part-of-speech and function-word rules `dasllama_g2p.das` ports | `APACHE-2.0.LICENSE` |
| g2p_en | https://github.com/Kyubyong/g2p | Apache-2.0 | the GRU sequence-to-sequence out-of-vocabulary model packed into `tts_g2p.bin` | `APACHE-2.0.LICENSE` |
| CMUdict 0.7a | Carnegie Mellon University (the NLTK distribution) | BSD-2-Clause | first pronunciations packed into `tts_g2p.bin`, rendered into the misaki inventory | `CMUDICT.LICENSE` |
| Universal Dependencies English-EWT | https://github.com/UniversalDependencies/UD_English-EWT | CC BY-SA 4.0 | dev-time: the PTB-tagged training corpus of the tagger weights in `tts_postag.bin` (`harness/train_postag.py`); the weights are daslang's, the attribution is the corpus's | `UD_EWT.LICENSE` |
| spaCy `en_core_web_sm` | https://github.com/explosion/spaCy | MIT | dev-time: labels the silver prose the tagger trains on (`harness/mint_postag_silver.py`) and the test fixture's tokens | `SPACY.LICENSE` |

The three TTS GGUFs, `tts_g2p.bin` and `tts_postag.bin` are built locally by
`modules/dasLLAMA/performance/build_tts_data.das` and never enter the repository; the build
writes a `.LICENSE` sidecar beside each file naming the rows above that it was derived from.
The development-time oracles (espeak-ng and phonemizer for the reference phoneme arm, the
PyTorch and ONNX reference runs, the ASR transcriber and the MOS predictor the quality rig
scores with) ship nothing and are not listed.
