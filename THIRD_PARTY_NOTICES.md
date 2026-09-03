# Third-party notices

daslang redistributes the components below under their own licences. Every licence text is
installed with the SDK. A core component's licence goes into the documentation directory
under the name in the last column; a tool's or an example's licence is installed beside that
tool or example. The same column gives the source-tree path. Module-specific notices live with
the module and install beside these.

## Core runtime and tools

| Component | Upstream | Licence | Used for | Licence file |
|---|---|---|---|---|
| uriparser | https://github.com/uriparser/uriparser | BSD-3-Clause | URI parsing (`3rdparty/uriparser`) | `URIPARSER.LICENSE` (`3rdparty/uriparser/COPYING`) |
| dag_noise | Gaijin Entertainment | BSD-3-Clause | noise generators (`include/dag_noise`) | `DAG_NOISE.LICENSE` (`include/dag_noise/LICENSE`) |
| vecmath | Gaijin Entertainment | BSD-3-Clause | the SIMD vector-math layer (`include/vecmath`); the JIT's aarch64 polynomial rail mirrors its kernels | `VEC_MATH.LICENSE` (`include/vecmath/LICENSE`) |
| {fmt} | https://github.com/fmtlib/fmt | MIT | formatting (`3rdparty/fmt`) | `FMT.LICENSE` (`3rdparty/fmt/LICENSE`) |
| fast_float | https://github.com/fastfloat/fast_float | MIT (elected) | float parsing (`include/fast_float`) | `FAST_FLOAT.LICENSE` (`include/fast_float/LICENSE`) |
| Luau | https://github.com/luau-lang/luau | MIT (Lua.org copyright) | the shortest-float emitter (`src/misc/luau_float2string.cpp`) | `LUAU.LICENSE` (`src/misc/LUAU.LICENSE`) |
| dastest | Dmitri Granetchi | MIT | the test framework (`dastest/`) | `dastest/LICENSE`, installed with dastest |
| das-fmt | Dmitri Granetchi | MIT | the formatter (`utils/das-fmt/`) | `utils/das-fmt/LICENSE`, installed with das-fmt |
| OpenSSL | https://www.openssl.org | Apache-2.0 | TLS in dasHV (`modules/dasHV`) | `OPENSSL.LICENSE` (`modules/dasHV/OPENSSL.LICENSE`) |
| Droid Sans Mono | The Android Open Source Project | Apache-2.0 | the built-in font of dasStbImage and the OpenGL example | `droidsansmono.LICENSE` (`modules/dasStbImage/fonts/`), installed with the fonts and at the doc root |
| glTF Sample Assets | The Khronos Group | per asset (see the file) | the glTF example's models (`examples/gltf/media`) | `GLTF_SAMPLE_ASSETS.LICENSE` (`examples/gltf/`), installed with the example |

## Modules with their own notices

- **dasLLAMA** (`modules/dasLLAMA/THIRD_PARTY_NOTICES.md`, installed as
  `DASLLAMA_THIRD_PARTY_NOTICES.md`): llama.cpp / ggml, whisper.cpp, vLLM, Silero VAD, and the
  text-to-speech set (StyleTTS2, KittenTTS, Kokoro, misaki, g2p_en, CMUdict, spaCy, Universal
  Dependencies English-EWT), each with its `LICENSE.*` file installed beside this one.
