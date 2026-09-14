# library/ - dasLLAMA behind a C ABI

One daslang source, `dasllama_lib.das`, built by **both** standalone backends and driven from
C, C++ and daslang. It carries three surfaces over the one engine - a text model that completes a
prompt token by token, a speech-to-text model that turns an audio file into its transcript, and a
text-to-speech model that writes a WAV.
Nothing in it is dasLLAMA-specific machinery: it is the shape any daslang engine takes when a
foreign host has to call into it.

```
dasllama_lib.das   the library - [export_c] entry points over the dasllama facade
main.c             a C host          (both backends)
main.cpp           a C++ host        (the -ctx backend only)
main.das           a daslang host    (the -lib backend, through generated bindings)
```

## The two backends

| | `daslang -lib` | `daslang ... -ctx` |
|---|---|---|
| what it emits | a native shared library with a C ABI | a C++ header + one translation unit |
| who compiles the engine | the daslang JIT (LLVM) | your C++ compiler |
| in the artifact | no compiler, no interpreter | no compiler, no interpreter, no JIT |
| C host | yes | yes |
| C++ host | proxies over C | yes - the context's own class, in native C++ types |
| daslang host | yes, through the generated bindings | needs a shared build of the generated TU |
| build cost here | ~2 min of codegen | ~10 min and ~6 GB to compile a ~50 MB TU |

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

# speech to text, on either backend
bin/dasllama_host_c_ctx --asr <ggml-whisper.bin> modules/dasLLAMA/models/jfk_ask_not.wav
bin/daslang examples/dasLLAMA/library/main.das -- --asr <ggml-whisper.bin> --audio <audio-file>

# speech synthesis, on either backend
bin/dasllama_host_cpp_ctx --tts <pocket-tts.gguf> "Hello from a daslang library." hello.wav
bin/daslang examples/dasLLAMA/library/main.das -- --tts <pocket-tts.gguf> --text "Hello." --out hello.wav
```

Any GGUF the engine reads works; the small ones are the quickest way to see it run - the
browser examples beside this folder name theirs in `models.json`.

## What crosses, and what does not

The dasllama facade is das-shaped: blocks, arrays, moved structs. A C result has to be something
`daslib/c_api_header.das` can spell - a scalar, a string, a pointer, an enum, a vector or a POD
struct - so the library exposes a flat surface over the facade instead of re-exporting it:

- `open` / `close` - load a model and open one session over it
- `arch`, `n_layers`, `n_vocab`, `context_size` - what got loaded
- `set_sampling` - temperature, top-k, seed
- `prefill` - encode a prompt and run it through the model
- `next_piece` - one token's text, advancing the session
- `prefill_tps`, `gen_tps`, `reason` - the numbers, and why a call said no
- `asr_open` / `asr_close` / `asr_transcribe` / `asr_speed` - speech to text: an audio file in,
  its whole transcript out
- `tts_open` / `tts_close` / `tts_say` / `tts_voice` / `tts_set_voice` / `tts_speed` - speech
  synthesis: text in, a PCM16 WAV out

Streaming is a **pull**, not a callback: a daslang `block` cannot cross a C ABI, so the caller
loops on `next_piece` instead of handing the engine a sink. The raw completion path carries no
stop token either - the caller decides when it has read enough, the way a chat program stops on
its template's stop ids. The same constraint is why `asr_transcribe` answers with the whole
transcript rather than a segment stream, and why `tts_say` writes a WAV instead of handing back
PCM: the engine's streaming forms take a block, and an array does not cross as a result.

All three surfaces share one job queue. It is opened by whichever one opens first and destroyed
when no model is open, so a host can hold all three at once.

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

Both emissions pass `--disable-module dasVulkan`, and the `-ctx` one drops `dasLLVM` as well:

- the GPU tier would pull tens of megabytes of kernel C++ into a translation unit that is
  already large;
- the tune framework (`llvm_tune`) is a macro module with no AOT form. A standalone context has
  no interpreter to fall back on, so every function it reaches must have a C++ body - a module
  marked no-AOT as a whole has none, and the emission stops with that module named.

A build that wants the GPU tier in the jitted library drops the `dasVulkan` line from the
`-lib` command; the `-ctx` command cannot drop the `dasLLVM` one.
