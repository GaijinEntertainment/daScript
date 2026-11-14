# library/ - dasLLAMA behind a C ABI

One daslang source, `dasllama_lib.das`, built by **both** standalone backends and driven from
C, C++ and daslang. It carries four surfaces over the one engine - a text model that completes a
prompt token by token, a speech-to-text model that turns a live sample stream into the text of
each utterance, a translation model, and a text-to-speech model that speaks into memory. The
library reads and writes no file but the models, and a host that keeps its models in its own
storage hands them over as bytes instead: the host owns the microphone, the speaker and any WAV.
Nothing in it is dasLLAMA-specific machinery: it is the shape any daslang engine takes when a
foreign host has to call into it.

```
dasllama_lib.das   the library - [export_c] entry points over dasllama/dasllama_core
main.c             a C host          (both backends)
main.cpp           a C++ host        (the -ctx backend only)
main.das           a daslang host    (the -lib backend, through generated bindings)
```

## The two backends

| | `daslang -lib` | `daslang ... -ctx` |
|---|---|---|
| what it emits | a native shared library, or a static archive, with a C ABI | a C++ header + one translation unit |
| who compiles the engine | the daslang JIT (LLVM) | your C++ compiler |
| in the artifact | no compiler, no interpreter | no compiler, no interpreter, no JIT |
| C host | yes | yes |
| C++ host | proxies over C | yes - the context's own class, in native C++ types |
| daslang host | yes, through the generated bindings | needs a shared build of the generated TU |
| build cost here | ~2 min of codegen | ~10 min and ~6 GB to compile a ~110 MB TU |

The C surface is identical either way, because one describer writes both headers. The C++
surface is not: a jitted library has no C++ source to put underneath, so its C++ half is inline
proxies over C, while a standalone context defines real methods in native types.

## Build and run

Every target is opt-in - each one compiles the whole engine.

```sh
# -lib: the engine jitted into examples/dasLLAMA/library/_out/dasllama_lib.so
cmake --build build --target dasllama_lib_jit
bin/daslang examples/dasLLAMA/library/main.das -- --model <model.gguf> --prompt "Once upon a time" --tokens 64

# the same library from C
cmake --build build --target dasllama_host_c_jit
bin/dasllama_host_c_jit <model.gguf> "Once upon a time" 64

# -ctx: the engine emitted as C++ and linked into the host
cmake --build build --target dasllama_host_cpp_ctx
bin/dasllama_host_cpp_ctx <model.gguf> "Once upon a time" 64
cmake --build build --target dasllama_host_c_ctx   # the same context, through its C entry points
bin/dasllama_host_c_ctx <model.gguf> "Once upon a time" 64

# speech to text, on either backend: the host streams a PCM16 WAV in 10 ms chunks, like a microphone
bin/dasllama_host_c_ctx --asr <ggml-whisper.bin> modules/dasLLAMA/models/silero_vad.bin modules/dasLLAMA/models/jfk_ask_not.wav
bin/daslang examples/dasLLAMA/library/main.das -- --asr <ggml-whisper.bin> --audio <file.wav>

# speech synthesis, on either backend
bin/dasllama_host_cpp_ctx --tts <pocket-tts.gguf> "Hello from a daslang library." hello.wav
bin/daslang examples/dasLLAMA/library/main.das -- --tts <pocket-tts.gguf> --text "Hello." --out hello.wav
```

Any GGUF the engine reads works; the small ones are the quickest way to see it run - the
browser examples beside this folder name theirs in `models.json`.

## Three ways to pick the kernels

The engine's inner loops are kernels the build stamps for a CPU class. A `-lib` build picks
them one of three ways, and `dasllama_lib_kernels()` says in one line which the library carries:

| target | kernels | for |
|---|---|---|
| `dasllama_lib_jit` | the building box's own | a library that runs where it is built |
| `dasllama_lib_fat_shared` | the floor class (`x86-avx2`, `arm-neon`) plus a clone of every kernel for each class above it; `create` reads the CPU once and picks the best set | one binary for any PC |
| `dasllama_lib_forced_shared` | the floor class's profile, whatever box builds it | a known target - a console - built on another machine |

Each has a static twin (`dasllama_lib_host_static`, `..._fat_static`, `..._forced_static`): an
archive holding the engine alone, which the host links with the daslang library (`libDaScript` -
the whole one: the engine registers a module that lives in its compiler half). Every variant is named `dasllama_lib`, because the artifact's name is
the prefix of its C entry points, so each sits in `_out/<mode>_<linkage>/` beside the C host
that links it (`dasllama_host_c_<mode>_<linkage>`).

```sh
cmake --build build --target dasllama_host_c_fat_shared
cd examples/dasLLAMA/library/_out/fat_shared
./dasllama_host_c_fat_shared --asr <asr-model> <silero_vad.bin> <file.wav>
```

The two environment variables behind the modes work on any `daslang -lib` command line:

```sh
# fat: every kernel for the floor class and for each class above it
DAS_JIT_BASELINE=x86-avx2 DAS_TUNE_MODE=fat daslang -lib dasllama_lib.das -output out/dasllama_lib
# forced: the kernels one tune file names - a class profile, or a box's own tune sidecar
DAS_JIT_BASELINE=x86-avx2 DAS_TUNE_FORCE=<file.tune.json> daslang -lib dasllama_lib.das -output out/dasllama_lib
```

A built library reads no tune file and writes none: its kernels are the ones the build stamped.
The three modes are `-lib` builds; the `-ctx` backend has no tune framework and runs the reference
kernels.

## Two ways to load a model

**From a path.** `open`, `asr_open`, `translate_open`, `tts_open` take a file the engine reads
itself - a GGUF, or a prepared image (`.dlim`) baked from one.

**From memory.** `asr_open_memory`, `translate_open_memory` and `asr_listen_memory` take bytes the
host already holds - an engine with its own file system or resource packs. A model's bytes are a
prepared image, and the library borrows them in place: nothing is copied, and the host keeps them
until the matching `close`. They start on a `dasllama_lib_image_alignment()` boundary (16 KiB); a
pointer off it is refused. `asr_listen_memory` takes the voice detector's own file, not an image,
and copies what it needs, so those bytes are free when the call returns.

A prepared image is baked for one CPU class. Bake it with the same force the library was built
with, on any box that can run that class:

```sh
DAS_JIT_BASELINE=x86-avx2 DAS_TUNE_FORCE=<file.tune.json> daslang -jit utils/dasllama-convert/main.das -- -m <model.gguf> -o <model.dlim>
```

## Speech in, speech out

`--interpret` is the whole chain as one host run: a WAV plays the microphone, each utterance is
printed with its translation, and the translation is spoken into a second WAV.

```sh
bin/dasllama_host_c_jit --interpret <asr-model> <silero_vad.bin> <translation-model> <pocket-tts.gguf> <in.wav> <out.wav>
bin/dasllama_host_c_jit --interpret <asr.dlim> <silero_vad.bin> <translation.dlim> <pocket-tts.gguf> <in.wav> <out.wav> --from-memory
```

With `--from-memory` the host reads the voice detector and every model given as a `.dlim` into
its own memory first and opens them from the bytes. The speech model still opens from its path.

## What crosses, and what does not

The engine is das-shaped: blocks, arrays, moved structs. A C result has to be something
`daslib/c_api_header.das` can spell - a scalar, a string, a pointer, an enum, a vector or a POD
struct - so the library exposes a flat surface over it instead of re-exporting it:

- `open` / `close` - load a model and open one session over it
- `arch`, `n_layers`, `n_vocab`, `context_size` - what got loaded
- `set_sampling` - temperature, top-k, seed
- `prefill` - encode a prompt and run it through the model
- `next_piece` - one token's text, advancing the session
- `prefill_tps`, `gen_tps`, `reason` - the numbers, and why a call said no
- `asr_open` / `asr_close` / `asr_speed` - load and drop a speech-to-text model
- `asr_listen` / `asr_feed` / `asr_listen_end` - speech to text from a live stream: mono samples
  in at any rate and chunk size, the text of each utterance the Silero VAD closes out
- `translate_open` / `translate_close` / `translate_direction` / `translate_text` - load and drop
  a translation model, read its direction, translate a line
- `asr_open_memory` / `asr_listen_memory` / `translate_open_memory` / `image_alignment` - the same
  loads from bytes the host holds in memory
- `kernels` - one line naming the CPU kernels this build runs
- `tts_open` / `tts_close` / `tts_voice` / `tts_set_voice` / `tts_speed` - load and drop a
  text-to-speech model, pick its voice
- `tts_speak` / `tts_pcm` / `tts_pcm_rate` - speech synthesis into memory: mono f32 samples the
  host plays or saves

The library requires `dasllama/dasllama_core`, the engine without the file decoders, so it links
neither the `audio` nor the `stbimage` module.

Streaming is a **pull**, not a callback: a daslang `block` cannot cross a C ABI, so the caller
loops on `next_piece` instead of handing the engine a sink. The raw completion path carries no
stop token either - the caller decides when it has read enough, the way a chat program stops on
its template's stop ids. The same constraint is why `asr_feed` returns the text it closed rather
than calling back, and why `tts_pcm` is a pointer into the library: an array does not cross as a
result.

All four surfaces share one job queue. It is opened by whichever one opens first and destroyed
when no model is open, so a host can hold all four at once.

Three details a host has to respect, all of them in the generated header:

- a returned string points into that instance's string heap and is valid until the next call on
  it - copy what you keep;
- the empty daslang string arrives as `NULL`;
- a daslang panic never unwinds into C **under the `-lib` backend**: the call returns zero and
  `dasllama_lib_last_error(ctx)` carries the text. That is a different channel from this
  library's own `reason()`, which reports refusals like "no model is open".

The last one is where the two backends disagree today, and the header does not say so: the
`-ctx` backend's C wrappers call the context method directly, with no catch, so a panic there
prints its daslang call stack and ends the process. A host that must survive one drives the
jitted library. It is also why `open` checks that the model file exists before handing the path
to the loader - a wrong path is the caller mistake worth refusing cleanly on both backends.

## Two things this library does that a script does not

**The job queue is opened persistently.** The engine panics on a queue nobody configured, and
scripts get one from `with_job_que() { ... }` - a scope. A library is entered and left once per
call, so `open` calls `create_job_que` + `setup_dasllama_jobque` instead, and `close` destroys
only a queue this library made.

**The library times itself.** `stats(session)` reads counters the engine's one-call `generate`
fills in, and a caller pulling token by token never runs it, so `prefill_tps` / `gen_tps` here
count their own microseconds.

## Why the backends drop modules

Both emissions pass `--disable-module dasVulkan`, and the `-ctx` one drops `dasLLVM` and `dasCUDA`
as well:

- the GPU tier would pull tens of megabytes of kernel C++ into a translation unit that is
  already large;
- the tune framework (`llvm_tune`) is a macro module with no AOT form, and so is the CUDA
  module on a build that carries it. A standalone context has no interpreter to fall back on, so
  every function it reaches must have a C++ body - a module marked no-AOT as a whole has none,
  and the emission stops with that module named.

A build that wants the GPU tier in the jitted library drops the `dasVulkan` line from the
`-lib` command; the `-ctx` command cannot drop the `dasLLVM` or `dasCUDA` one.
