---
date: 2026-10-28
tag: daslang
title: A game scripting language that runs speech-to-text 5x faster than llama.cpp on CPU.
lede: How we ended up writing an inference engine and the macros that did it.
---

daslang started in 2019 at Gaijin Entertainment. Back then we called it daScript.

The brief was narrow: a scripting language fast enough for game logic over an entity-component system,
so that nothing ever has to be rewritten in C++ later.

Strong static typing, a fast compiler, no data marshaling whatsoever.
Three ways to run the same source: an interpreter for iteration or live patching,
ahead-of-time C++ for shipping, and an LLVM-based JIT.

It did the job. It has been running in production at Gaijin ever since.
We never stopped making it faster.

We recently wrote an engine for LLMs, speech recognition (ASR) and text-to-speech (TTS) in it, called dasllama.
No C++, no shaders written separately in HLSL, MSL, GLSL or CUDA. All daslang.

Benchmarks took an unexpected amount of effort.
But now we can actually say that dasllama has its moments:

| Workload | Machine | das | reference | gain |
|---|---|---|---|---|
| Qwen3-ASR-0.6B, 11 s clip | Apple M5 Max, CPU | 0.46 s | llama.cpp 2.52 s | 5.5x |
| Gemma-4 E2B audio-in, 11 s clip | Apple M5 Max, CPU | 1.11 s | llama.cpp 7.55 s | 6.8x |
| Whisper large-v3-turbo, 3 min clip | Graviton4, CPU | 14.6 s | whisper.cpp 89.9 s | 6.1x |
| Pocket TTS, 20 sentences | EPYC 9R14, CPU | 9.1 s | pocket-tts 40.4 s | 4.4x |
| Qwen3.6-35B-A3B prefill | EPYC 9R14, CPU | 425 tok/s | llama.cpp 181 tok/s | 2.3x |
| Qwen3.6-35B-A3B decode | EPYC 9R14, CPU | 22.5 tok/s | llama.cpp 16.2 tok/s | 1.4x |
| Gemma-4-26B-A4B prefill | Graviton4, CPU | 256 tok/s | llama.cpp 91 tok/s | 2.8x |
| Qwen3.6-35B-A3B prefill | Apple M5 Max, Metal | 4430 tok/s | llama.cpp 3305 tok/s | 1.34x |
| Qwen3.6-35B-A3B decode | Apple M5 Max, Metal | 133 tok/s | llama.cpp 113 tok/s | 1.18x |
| Qwen3.6-35B-A3B decode | RTX PRO 4500 24 GB, Vulkan | 166 tok/s | llama.cpp 167 tok/s | 1.00x |
| Qwen3.6-35B-A3B decode, experts on host | RTX 5060 Ti 16 GB, Vulkan | 51.0 tok/s | llama.cpp 25.7 tok/s | 2.0x |
| Qwen3.6-35B-A3B prefill, experts on host | RTX 5060 Ti 16 GB, Vulkan | 1115 tok/s | llama.cpp 211 tok/s | 5.3x |

Source: the [ladder](../ladder.html), dasllama 0.6.6, measured 2026-10-21/22; the two RTX 5060 Ti rows
2026-10-25, the bench built at 896bd646d. We also lose some rows, and they are on the same page.

This is the story of how a game scripting language ended up here.

## The most modest goal

In 2026 a game team wanted a voice translator.
You are in a random squad. Your squadmate just cursed at you in a language you do not know.
Now the game should transcribe, translate and speak it back. Live. Loud.
Here is the kicker: the GPU is busy, and not with tokens (shocking). It's drawing.
So the translator has to run next to the game on whatever scraps are left of the CPU the player owns.

llama.cpp was the obvious choice until it was not. It did not hit the latency mark.
It's a very large dependency that's hard to integrate; all that for the sake of a few small models.

Let's try daslang. After all, the language was fast enough for games. What could possibly go wrong?

## The surprise

The first port was Whisper, on an M1. It came out about 5x faster than whisper.cpp of the day.

whisper.cpp has done some excellent work since; we did not sleep much either.
As we added more models, the trend continued. It still holds today:

| Workload, CPU | Machine | das | reference | gain |
|---|---|---|---|---|
| Whisper large-v3-turbo, 3 min clip | Apple M5 Max | 17.0 s | whisper.cpp 21.7 s | 1.27x |
| Whisper large-v3-turbo, 3 min clip | EPYC 9R14 | 10.9 s | whisper.cpp 35.1 s | 3.2x |
| Qwen3-Omni-30B audio-in, 11 s clip | Apple M5 Max | 1.45 s | llama.cpp 9.35 s | 6.4x |
| Qwen3-Omni-30B audio-in, 3 min clip | Apple M5 Max | 33.8 s | llama.cpp 158.4 s | 4.7x |
| Pocket TTS, 20 sentences | Granite Rapids | 7.8 s | pocket-tts 41.6 s | 5.3x |
| Kitten TTS mini, 20 sentences | Graviton4 | 10.4 s | KittenTTS 65.0 s | 6.2x |

Source: the [ladder](../ladder.html), dasllama 0.6.6, measured 2026-10-21/22.

The first kernel was written by hand, and it's no longer with us.
Because the second one made the scope obvious: every operation times every weight format times every CPU times every model shape.
That's something you do when you have a hundred contributors each tuning their own box.
So instead of writing the third kernel, we wrote a macro.

## The macro advantage

daslang has Lisp/Haxe-style macros: compile-time code that reads and rewrites the AST.
They made dasllama possible and sometimes even interesting.

**Along came the tuner**

`[tune]` was the first thing which came online. Just after loop annotations, that is.

```das
[hint(unsafe_range_check, noalias=x, noalias=w)]
def dv_w16u4(w, x : float const?; n : int64) {
    var acc = 0.0
    for [vectorize, vectorize_width=16, unroll_count=4] (j in range64(n)) {
        acc += unsafe(w[j]) * unsafe(x[j])
    }
    return acc
}
```

These loop annotations look oddly familiar. LLVM exposes several different ones: 'unroll', 'unroll_and_jam', 'distribute', to name a few.
So the very first thing we did was expose those to the language; and the right combination often just gets you there.
On that one box, that is. If you can find the right combination. The right unroll count. The right width.

Or you just press the ultimate I Win button - `[tune]`.
Under the JIT, all valid permutations are compiled and raced at the first launch.
Winners are then written to a small JSON sidecar next to the application.
Next time you run it - the winner is picked automagically.

A similar concept was then extended to a function macro and it practically wrote itself.
First came a way to constrain permutations, then a way to pick an order of allowed intrinsics.
When all else fails, the function falls back to the reference implementation.

This is a real dasllama kernel, the 4-bit K-quant tile. The annotation above it declares a grid of permutations -
the dot-product instruction, the vector width, the register tile, the unroll:

```das
[tune_perm(mr = 8),
 tune_perm(dot = "maddubs",  width = 256, mr = 8,  requires = "avx2"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 8,  requires = "avxvnni|avx512vnni"),
 tune_perm(dot = "vpdpbusd", width = 512, mr = 16, requires = "avx512vnni,avx512bw"),
 tune_perm(dot = "vpdpbusd", width = 256, mr = 16, requires = "avx512vnni,avx512bw"),
 tune_perm(dot = "amx_bf16", width = 512, mr = 16, nrsplit = 2, requires = "amx-bf16,avx512bf16,avx512vnni"),
 tune_perm(dot = "amx_bf16", width = 512, mr = 16, nrsplit = 1, requires = "amx-bf16,avx512bf16,avx512vnni"),
 tune(gen = "dasllama_gemm_gen::k4_tile",
      fallback = "dot_vpdpbusd_width512_mr16;dot_vpdpbusd_width256_mr8;dot_maddubs_width256_mr8;mr8")]
def k4q8_tile_gen(var yp : float?; kqg : uint8 const?; ksg : uint8 const?;
                  xqp : int8 const?; xsp : float const?; xbsp : int const?;
                  n, d, g, t0 : int64) {
    kq_tile_ref(4, yp, kqg, ksg, xqp, xsp, xbsp, n, d, g, t0)
}
```

(The real one also lists seven companion generators, the GEMV, layout and AMX config paths that
ride the same winner. Cut for length.)

llama.cpp ships universal kernels, hand-tuned by very good people for the CPUs they have.
dasllama comes with a reference per operation and a tuner.

It also comes with a pre-tuned sidecar per CPU class: NEON, i8mm, SVE, AVX2, VNNI-512 and AMX.
That is because tuning takes time. If you did not win the sidecar lottery - your box gets to sit and profile for maybe 20 minutes, once per release.

So that 2.3x prefill on EPYC and the 2.8x on Graviton?
The tuner does not invent a better algorithm. It finds the right permutation for that one core and that one model on that one box.

**Templates to the rescue**

The GPU side is a similar story in a completely different package.
There is no LLVM to lean on, and replacing an intrinsic or changing the width alone hardly does it.

There are more choices to be made - per API, per fusion, per format.
So a kernel becomes a class. The variants are overridden constants or functions from a shared template.

A platform-specific macro then turns each class into a dispatch plus a SPIR-V, Metal, or CUDA shader.

Here are three variants of a fused add-norm-quantize kernel:

```das
[vk_dispatch(name = "cls_ar_rqx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqX : ArRqT {
    override STOREX = true
    [spirv_kernel(local_size_x = 256)]
    def run { ar_rq_body() }
}

[vk_dispatch(name = "cls_ar_rqk", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqK : ArRqT {
    override KBLK = true
    [spirv_kernel(local_size_x = 256)]
    def run { ar_rq_body() }
}

[vk_dispatch(name = "cls_ar_rqkx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqKX : ArRqT {
    override KBLK = true
    override STOREX = true
    [spirv_kernel(local_size_x = 256)]
    def run { ar_rq_body() }
}
```

The Vulkan backend alone is over four hundred of these dispatch classes.
Most of them are a template and two or three overrides.

When a new weight format arrives, the body gets one more branch and every kernel that reads weights picks it up.

**Homogeneity what?**

What's left is this pesky issue of shader language.
Vulkan speaks SPIR-V, Metal speaks MSL, CUDA speaks C++.

dasllama is written in daslang: the loader, the tokenizer, the scheduler, the CPU kernels,
the Metal, CUDA and Vulkan shaders, networking and the OpenAI-compatible server.
Even the dasllama.io backend with its SQL ladder database.

The shader body in the example above is daslang. The SPIR-V emitter is a daslang library too.
There is no FFI between the model graph and the kernel, no string-templated shader source, no second build system.

When a kernel needs a new primitive, the language grows it. dasllama has pushed a lot into daslang.
16-bit and 8-bit vector families, fp16 arithmetic, a set of SIMD intrinsics.
We rewrote the job queue three times for latency, and it grew its own trace visualizer in the process.

And then there is the tune framework itself.

A hand-tuned engine is fast when its authors have hardware.
A self-tuning engine is fast when the macro can produce good kernels for the tensor shapes at hand.

Then maybe the cost of a new format or a new card is one more row in a grid.

## When all you have is a hammer

Once the CPU was balling, we just could not stop. Metal came first, then Vulkan,
and now CUDA is coming to the rescue.
Sometimes games have GPU time to spare, but for local inference it's bread and butter.

Macs are easy. There are only so many of them. You get a new laptop and you just have to push it (I do):

| Apple M5 Max, tok/s | das | llama.cpp | gain |
|---|---|---|---|
| Qwen3.6-35B-A3B prefill, Metal | 4430 | 3305 | 1.34x |
| Qwen3.6-35B-A3B decode, Metal | 133 | 113 | 1.18x |
| Qwen3.8-27B prefill, Metal | 1026 | 788 | 1.30x |
| Gemma-4-26B-A4B prefill, Metal | 4257 | 3762 | 1.13x |
| Gemma-4-26B-A4B decode, Metal | 119 | 106 | 1.12x |
| Qwen3.6-35B-A3B prefill, CPU | 265 | 157 | 1.69x |
| Qwen3.6-35B-A3B decode, CPU | 60.5 | 49.6 | 1.22x |

Now Vulkan is a whole different animal. The card zoo changes the story every time we touch a new card.

Things were looking so good on an RTX 3060, until an RTX 5060 came in the mail - and brought a whole new set of extensions.
Then on an RTX 5060 Ti the engine was ahead of llama.cpp Vulkan through the summer,
until we ran the full model suite on RunPod's RTX PRO 4500 Blackwell for the release sweep.

It came out at "parity", aka sometimes slightly behind.
Same engine, different card. The macros needed to generate different kernels, and perhaps we needed to race a few of them, too:

| Vulkan, tok/s | das | llama.cpp | gain |
|---|---|---|---|
| Qwen3.6-35B-A3B decode, RTX PRO 4500 | 166 | 167 | 1.00x |
| Qwen3.8-27B decode, RTX PRO 4500 | 38.8 | 38.4 | 1.01x |
| Qwen3.6-35B-A3B prefill, RTX PRO 4500 | 4955 | 5556 | 0.89x |
| Qwen3VL-4B prefill, RTX 5060 Ti | 5223 | 4316 | 1.21x |
| Gemma-4 E4B prefill, RTX 5060 Ti | 4554 | 3970 | 1.15x |
| gpt-oss-20b decode, RTX 5060 Ti | 124 | 111 | 1.12x |
| Qwen3VL-4B decode, RTX 5060 Ti | 81.2 | 80.5 | 1.01x |
| Gemma-4 E4B decode, RTX 5060 Ti | 63.7 | 63.6 | 1.00x |

Source: the [ladder](../ladder.html); the M5 Max and RTX PRO 4500 rows dasllama 0.6.6, measured
2026-10-21/22; the RTX 5060 Ti rows 2026-10-25, the bench built at 896bd646d.

## Sometimes it's NOT a nail

The interesting Vulkan rows are the ones where the model does not fit on the GPU.

Qwen3.6-35B-A3B at Q4_K_M is 23 GB and Gemma-4-26B-A4B is 17 GB; the 5060 Ti has 16 GB.
Both are mixture-of-experts (MOE) models, and almost all of those gigabytes are experts.

The usual answer is to keep the experts in host RAM and the rest of the model on the card, and llama.cpp
does that with `-ncmoe`:

| Model, 5060 Ti 16 GB, experts on host | das | llama.cpp | gain |
|---|---|---|---|
| Qwen3.6-35B-A3B prefill | 1115 tok/s | 211 tok/s | 5.3x |
| Qwen3.6-35B-A3B decode | 51.0 tok/s | 25.7 tok/s | 2.0x |
| Gemma-4-26B-A4B prefill | 1328 tok/s | 287 tok/s | 4.6x |
| Gemma-4-26B-A4B decode | 46.8 tok/s | 26.2 tok/s | 1.8x |
| Qwen3.6-35B-A3B decode, 4 streams | 52.6 tok/s | 59.6 tok/s | 0.88x |

Source: the [ladder](../ladder.html), measured 2026-10-25, the bench built at 896bd646d, llama.cpp
b11798 Vulkan at `-ngl 99 -ncmoe <n_layers>`.

This is not a kernel win. And for once in this story, it's not about macros. Well, almost.

Not all experts are equally popular. For a given layer, some get picked far more often than the rest.
It so happens that the GPU typically decodes an expert an order of magnitude faster than the CPU.
So whatever room the card has left after the dense weights and the KV cache becomes expert cache slots.

For decode, the GPU runs the hits, the CPU adds in the misses.
Experts are already in pinned host memory - where the CPU can read them directly.
The CPU tracks expert usage and replaces the coldest cached expert with the hottest miss via DMA.
That way it hopefully becomes a hit for the next token.

A prefill follows a very different plan - it walks the prompt in windows of up to 4096 rows.
Experts are sorted by frequency. The GPU runs experts from the busy end, streaming each once per window.
The CPU runs the rare end - to save the GPU the DMA time, which at some point may cost more than the math it saves.

There is no cost model, no clever heuristic. Both sides take from opposite ends until they meet.

Sometimes a disk cache gets involved (qwen-next from NVMe on a machine with 64 GB of RAM) and a third queue gets to play.
Experts from that queue enter the sorted list as they are streamed.

The gain grows with the window. On Vulkan it's 4096.
CUDA already goes to 8192 and runs faster. There are more tricks up that sleeve.

What's with the macros then?
The CPU still needs to read the same weight formats and repacks as the GPU, even when they are not optimal for it.
Which means someone has to write yet another set of kernels.

## Why so slow?

Short version. Not done yet.

That 4-stream row? We have not implemented all the CPU and GPU shapes yet. Some still need a short prefill-like shape.

Qwen3-ASR on Granite Rapids runs at half the speed of llama.cpp, and at "parity" on EPYC, where the Apple and Graviton rows are at 5x:
the int8 path on those x86 cores is not tuned yet either.

Qwen3.6-35B prefill on Granite Rapids is 0.91x.

Kitten TTS nano, the smallest model on the board, is 0.7-0.85x on x86 servers. Kokoro on x86 is a wash.

Each of these is a work in progress.

## What's it good for then?

**A voice assistant.** ASR -> LLM -> TTS in one process with or without a GPU.
That's how the dasllama benchmark suite came to be - a home assistant has to answer before you notice it is thinking.

A colleague runs one at home on a Mac mini M4: Parakeet for ears, Qwen3.6 for the brain, Kokoro for the voice.
It used to be MLX + parakeet-cli - now he waits about 30% less for an answer.

Part of that speedup comes from fewer tool-call retries.
dasllama constrains decoding to the tool call's schema, so malformed calls don't need a second attempt.

**Local coding models.** Qwen3.6-35B-A3B, Gemma-4-26B-A4B, and lately Qwen3.8-27B.
Qwen3.8-Flash-Next is getting there.

**In-game chatter.** Transcription, translation and synthesis on the CPU next to a running game.
That work is in progress at Gaijin.

## What's next

**CUDA.** The emitter is done and the same kernels are already running through similar macros.
No published benchmarks yet (hint: it's very fast).

**Big MOE on one card.** The Qwen3.6-35B-A3B above is the proof of concept on a small scale.
Qwen3.8-Flash-Next is larger, and already works on Vulkan - though it can use more love, and more CUDA.
The direction is one big card and a lot of ordinary RAM, with a fast NVMe on the side.
The model which today takes six cards can surely work on one given the right tool for the job.

**More cards, more boxes, more models.** The ladder takes submissions.
dasllama and daslang are open source. We read every PR.

## Try it

- `pip install dasllama`, or brew, scoop, `.deb`, `.rpm`: [dasllama.io](https://dasllama.io/)
- The ladder with every receipt: [dasllama.io/ladder](../ladder.html)
- Every family the engine loads: [features](../features.html)
- Source, BSD 3-clause: [github.com/GaijinEntertainment/daScript](https://github.com/GaijinEntertainment/daScript)

If you have a Mac and want a voice assistant on it, it is the best option we know of today. ASR, LLM and TTS in one server, with no faster alternative that we have found.
If you have a 16 GB Vulkan card and want a 35B MOE on it, it decodes at 2.0x and prefills at 5.3x llama.cpp.
If you have a box we do not have, run the suite and send the row.

Every family on the [features page](../features.html) loads from its stock file. We verify the output token for token against its reference engine.
llama.cpp, whisper.cpp, MLX and vLLM were also the yardsticks and the reading material throughout.
There would be no dasllama without them.

LLMs helped me write this article, among many other things during daslang and dasllama development.
