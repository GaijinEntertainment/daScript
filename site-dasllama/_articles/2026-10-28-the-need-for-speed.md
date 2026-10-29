---
date: 2026-10-28
tag: daslang
title: A game scripting language that runs speech-to-text 5x faster than llama.cpp on CPU.
lede: How a scripting language built for game logic ended up running speech, text and translation models, with the numbers, the losses, and the three macros that did it.
---

<!-- figures: every pair is a row of the ladder (site-dasllama/ladder.html, /api/runs; the checked-in
     store modules/dasLLAMA/performance/records/<box>.json), dasllama 0.6.6. The m5 / epyc9r14 / gnr /
     graviton4 / rtxpro4500 rows: 2026-10-21/22, das 792defade (the 0.6.6 release-17 rig). The
     rtx5060ti rows (the two Vulkan rows of the first table, the whole second table, and the
     fitting-model ratios in the Vulkan paragraph): 2026-10-25, the bench exe built and minted at
     896bd646d, llama.cpp b11798 (9febe63) Vulkan, the host-experts reference at -ngl 99 -ncmoe
     <n_layers>, GPU clock pinned at 2392 MHz; the RTX PRO 4500 rows 2026-10-21. The Whisper-on-M1
     "about 5x" is the first port's figure against whisper.cpp of that day and is dated as such in
     the text; the ladder's current pair is cited beside it. The home-assistant latency figure is a
     team member's own report of his setup (Mac mini M4, October 2026, dasllama 0.6.6), not a ladder
     row. The expert-pool mechanism: modules/dasLLAMA/ARCHITECTURE_GPU_VULKAN_POOL.md and
     ARCHITECTURE_GPU_VULKAN_STREAM.md; the meet is moe_prefill_meet in dasllama_blocks.das; the
     N-row decline for host layers is vk_rdec_token_n_rows in dasllama_vulkan_decode.das. The
     tune grid is k4q8_tile_gen in dasllama_math_gen.das; the dispatch classes are in
     dasllama_vulkan_classes.das (434 [vk_dispatch] classes at 313fc07cad). -->

daslang is a statically typed scripting language built for games. We wrote an LLM and speech
inference engine in it, called dasllama, with no C++ and no hand-written assembly in the hot
path. On CPU it transcribes speech up to 5x faster than llama.cpp, and runs Qwen3.6-35B and
Gemma-4 at 1.7-2.8x the prefill speed (that is your time to first token) and 20-40% faster
decode. On Apple GPUs it is 10-35% ahead of llama.cpp Metal. On Vulkan it is at parity for
models that fit the card, and on a 16 GB card running a 35B mixture-of-experts that does not
fit, it decodes 2x and prefills 5x faster than llama.cpp with the same split. Every
number below is a measured pair, same model, same machine, same day, with the receipt on the
public ladder.

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
More on that below.

This is the story of how a game scripting language ended up here.

## The modest goal

daslang started in 2019 at Gaijin Entertainment. The brief was narrow: a scripting language
fast enough for game logic over an entity-component system, so that nothing ever has to be
rewritten in C++ later. Strong static typing, a fast compiler, no data marshaling at the C++
boundary, and three ways to run the same source: an interpreter for iteration, ahead-of-time
C++ for shipping, and later an LLVM JIT. It did the job. It has been running in production at
Gaijin ever since.

In 2026 a game team wanted a voice translator. You are in a random squad, your squadmate speaks
a language you do not, and the game should transcribe, translate and speak it back, live. The
constraint is the one every in-game inference project hits: the GPU is busy. It is drawing the
game. Whatever runs the models runs on the CPU, next to the game, on whatever the player owns.

llama.cpp was the obvious choice and it did not fit. It did not hit the latency on the CPU
budget we had, and it is a very large dependency to carry inside a game executable for a
feature that needs a few small models. So we tried writing the speech-to-text path in daslang.
The language was fast enough for games. Was it fast enough for this?

## The surprise

The first port was Whisper, on an M1. It came out about 5x faster than whisper.cpp of the day.
whisper.cpp has moved since then; today's ladder has that pair close on Apple silicon and wide
on the x86 and Graviton servers. The newer speech models kept the gap, and text-to-speech landed
the same way:

| Workload, CPU | Machine | das | reference | gain |
|---|---|---|---|---|
| Whisper large-v3-turbo, 3 min clip | Apple M5 Max | 17.0 s | whisper.cpp 21.7 s | 1.27x |
| Whisper large-v3-turbo, 3 min clip | EPYC 9R14 | 10.9 s | whisper.cpp 35.1 s | 3.2x |
| Qwen3-Omni-30B audio-in, 11 s clip | Apple M5 Max | 1.45 s | llama.cpp 9.35 s | 6.4x |
| Qwen3-Omni-30B audio-in, 3 min clip | Apple M5 Max | 33.8 s | llama.cpp 158.4 s | 4.7x |
| Pocket TTS, 20 sentences | Granite Rapids | 7.8 s | pocket-tts 41.6 s | 5.3x |
| Kitten TTS mini, 20 sentences | Graviton4 | 10.4 s | KittenTTS 65.0 s | 6.2x |

Source: the [ladder](../ladder.html), dasllama 0.6.6, measured 2026-10-21/22.

Nobody wrote clever kernels for that. The first kernel was written by hand. The second one made
the scope obvious: every operation times every weight format times every CPU times every model
shape. That is thousands of kernels, and the only way to be fast on all of them by hand is to
have a hundred contributors each tuning their own box. We do not. So instead of writing the
third kernel, we wrote a macro.

## The realization

daslang has Lisp and Haxe style macros: compile-time code that reads and rewrites the typed
AST. Everything interesting in dasllama is one of them. Three of them are the reason the
numbers above exist.

**`[tune]`: the machine picks the kernel, not a human.** A kernel is written once, as a plain
reference function. The annotation above it declares a grid of permutations: the dot-product
instruction, the vector width, the register tile, the unroll. The reference body runs on the
interpreter and on any machine the generator declines. Under the JIT, a tuner runs the whole
grid on the actual CPU with the actual model, records the winner in a small JSON sidecar next
to the application, and the next compile stamps that winner onto the function. Nothing about
the callers changes. This is a real one, the 4-bit K-quant tile:

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

llama.cpp ships universal kernels, hand-tuned by very good people for the CPUs they have. We
ship one reference per operation and a tuner, and the release carries a profile per CPU class:
NEON, i8mm, SVE, AVX2, VNNI-512 and AMX. A box the profile covers is tuned on its first
compile. A box it does not cover tunes itself once and keeps the sidecar. The 2.3x prefill on
EPYC and the 2.8x on Graviton are not a better algorithm. They are the right permutation for
that core and that model, chosen by measurement rather than by a person.

**Templates: permutations across formats are stamped, not written.** The GPU side has the same
combinatorics, formats times operations times fusion choices. A kernel there is a class; the
variants are a few overridden constants on a shared template, and a macro turns each class into
a dispatch plus a SPIR-V or Metal shader. Four fused add-norm-quantize kernels, in full:

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

The Vulkan backend alone is over four hundred of these dispatch classes, most of them exactly
this shape: a template and two or three overrides. When a new weight format arrives, the body
gets one more branch and every kernel that reads weights picks it up.

**Homogeneity: one language from the GGUF parser to the shader.** The loader, the tokenizer,
the scheduler, the CPU kernels, the Metal and Vulkan shaders and the OpenAI-compatible server
are the same language, in the same process, compiled by the same JIT. The shader body in the
snippet above is daslang; the SPIR-V emitter is a daslang library too. There is no FFI between
the model graph and the kernel, no string-templated shader source, no second build system. When
a kernel needs a new primitive, the language grows it. dasllama has pushed a lot into daslang
this way: the 16-bit and 8-bit vector families with closed fp16 arithmetic, a set of SIMD
intrinsics, a job queue rewritten three times for latency with its own trace visualizer, and the
tune framework itself.

The honest summary of the advantage is this: a hand-tuned engine is fast where its authors have
hardware. A self-tuning engine written in one language is fast wherever the tuner has run, and
the cost of a new format or a new card is one more row in a grid, not one more file per kernel.

## Consequences

Once the CPU path was ahead, the same machinery went to the GPU, where games have little use
for it but local inference has every use. Metal came first, then Vulkan.

On Apple silicon the gap is widest at prefill, the number you feel as time to first token, and
it is there on the CPU rows of the same machine too:

| Apple M5 Max, tok/s | das | llama.cpp | gain |
|---|---|---|---|
| Qwen3.6-35B-A3B prefill, Metal | 4430 | 3305 | 1.34x |
| Qwen3.6-35B-A3B decode, Metal | 133 | 113 | 1.18x |
| Qwen3.8-27B prefill, Metal | 1026 | 788 | 1.30x |
| Gemma-4-26B-A4B prefill, Metal | 4257 | 3762 | 1.13x |
| Gemma-4-26B-A4B decode, Metal | 119 | 106 | 1.12x |
| Qwen3.6-35B-A3B prefill, CPU | 265 | 157 | 1.69x |
| Qwen3.6-35B-A3B decode, CPU | 60.5 | 49.6 | 1.22x |

Vulkan is the humbling one. The card zoo changes the story every time we touch a new card. On
an RTX 5060 Ti the engine was ahead of llama.cpp Vulkan through the summer. We then put the
full suite on an RTX PRO 4500 Blackwell for the release sweep, and that card came out at parity
or slightly behind. Same engine, different card, different answer. Back on the 5060 Ti with the
same release, the models that fit the card are a few percent ahead at prefill and at parity on
decode:

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
2026-10-21/22; the RTX 5060 Ti rows 2026-10-25, the bench built at 896bd646d. Parity is what we
claim for Vulkan on a model that fits, and the 4500's prefill gap is open work.

## When the model does not fit the card

The interesting Vulkan rows are the ones where it does not fit. Qwen3.6-35B-A3B at Q4_K_M is
23 GB and gemma-4-26B-A4B is 17 GB; the 5060 Ti has 16 GB. Both are mixture-of-experts models,
and almost all of those bytes are experts, of which each token touches a handful. The usual
answer is to keep the experts in host RAM and the rest of the model on the card, and llama.cpp
does that with `-ncmoe`. With that same split on the same card:

| Model, 5060 Ti 16 GB, experts on host | das | llama.cpp | gain |
|---|---|---|---|
| Qwen3.6-35B-A3B prefill | 1115 tok/s | 211 tok/s | 5.3x |
| Qwen3.6-35B-A3B decode | 51.0 tok/s | 25.7 tok/s | 2.0x |
| gemma-4-26B-A4B prefill | 1328 tok/s | 287 tok/s | 4.6x |
| gemma-4-26B-A4B decode | 46.8 tok/s | 26.2 tok/s | 1.8x |
| Qwen3.6-35B-A3B decode, 4 streams | 52.6 tok/s | 59.6 tok/s | 0.88x |

Source: the [ladder](../ladder.html), measured 2026-10-25, the bench built at 896bd646d, llama.cpp
b11798 Vulkan at `-ngl 99 -ncmoe <n_layers>`.

This is not a kernel win. It is a scheduling one, and it only works because the whole engine is
one program. Three pieces:

**A hot expert pool on the card.** Expert picks are skewed: a layer's tokens land on a few of
its experts far more often than on the rest, and the card decodes an expert six to ten times
faster than the host does. So whatever room the card has left after the dense weights and the
KV cache becomes expert slots, and every token's picks split: the card runs the hits, the host
sums the misses, and the two meet at the layer's output. The host owns the policy. It counts
picks per expert, halves the counts every 256 rows so the pool follows the conversation, and
swaps the coldest slot for the hottest miss, one expert per decode step.

**Misses land in the pool by DMA.** A decode step's misses are already in pinned host memory
for the CPU kernels to read. The same bytes are copied to the pool's coldest unpicked slot on
the transfer queue, in the shadow of the step, so the next token finds them and the host spends
nothing on the fill.

**Prompts meet in the middle.** A prefill walks the prompt in windows of up to 4096 rows. For
each routed layer the window's experts are sorted by how many rows picked them, busiest first.
The card claims experts from the busy end, streaming each one's weights across the link once
and running all of its rows at once, as fast as the transfer ring frees. The CPU claims experts
from the rare end, the ones with a handful of rows each, where streaming the weights would cost
more than the math. There is no cost model: both sides take from their end until the cursors
meet, so the faster side simply takes more. The experts the card streamed stay in the pool, so
the decode that follows starts warm. That is the 5x: the link carries each busy expert once per
window instead of once per token, and the rare experts never cross it. The gain grows with the
window; it is capped at 4096 rows today by a Vulkan buffer-range limit, not by the design.

The four-stream row is the honest one. With four conversations at once, llama.cpp's host
experts take all four rows in one pass per expert. Ours step one row at a time in this mode,
four host passes per step, and it wins by 12%. The batched form exists in the engine for the
prompt window and for speculative decoding; wiring it into the four-stream decode is the next
piece of work, and it is the same piece the big-box plan below needs.

The losses, since you will look for them anyway. Qwen3-ASR on Granite Rapids runs at half the
speed of llama.cpp, and at parity on EPYC, where the Apple and Graviton rows are at 5x: the
int8 path on those x86 cores is not tuned yet. Qwen3.6-35B prefill on Granite Rapids is 0.91x.
Kitten TTS nano, the smallest model on the board, is 0.7-0.85x on x86 servers. Kokoro on x86 is
a wash. Each of these is a row on the ladder with both command lines in its receipt, and each
is being worked on.

## What it is for

**A voice assistant on a CPU.** Speech-to-text, a small LLM, and text-to-speech in one process
with no GPU. Parakeet transcribes an 11-second clip in 0.14 s on the M5 Max CPU and 0.17 s on
Graviton4; Pocket TTS speaks twenty sentences in 4-9 s on CPU. The dasllama benchmark suite
grew out of exactly this: a home assistant that has to answer before you notice it is thinking.

One of us runs such an assistant at home on a Mac mini M4: Parakeet for ears, Qwen3.6 for the
brain, Kokoro for the voice. It used to be parakeet-cli for the ears and MLX for the other
two. Replacing all three with dasllama cut end-to-end latency by about 30%, and the answers
got better, because the dasllama server compiles a forced tool call's schema to a grammar and
constrains the decode to it, so the model's function calls parse the first time instead of
being retried.

**Local coding models.** Qwen3.6-35B-A3B at 133 tok/s decode and 4430 tok/s prefill on an M5
Max, 60 tok/s decode on its CPU alone. Gemma-4-26B-A4B, gpt-oss-20b and Qwen3.8-27B are on
the board at the same quality bar. These are the "good enough and private" models, and the
prefill numbers are what decide whether a 20 KB context feels instant.

**In-game speech.** The original goal. Transcription, translation and synthesis on the CPU
next to a running game, without a 100 MB dependency. That work is in progress at Gaijin.

## What is next

**CUDA.** The same kernels, through the same macros, emitted as CUDA. The emitter is done and
the first kernels are running; numbers get published when they pass the pairing rule against
llama.cpp CUDA on the same card, not before.

**Big mixture-of-experts on one card.** The hot pool above is the small version of this. The
direction is one big card, a lot of ordinary RAM, and an NVMe behind it, serving a model that
today takes six cards. The routing is sparse; the bandwidth is the whole problem; and a
self-tuning engine is the right tool for a problem that is different on every box.

**More cards, more boxes.** The ladder takes submissions. Every row carries both command
lines, both commits, the file hashes and the probed hardware.

## Try it

- `pip install dasllama`, or brew, scoop, `.deb`, `.rpm`: [dasllama.io](https://dasllama.io/)
- The ladder with every receipt: [dasllama.io/ladder](../ladder.html)
- Every family the engine loads: [features](../features.html)
- The method, in full: [daslang.io/dasllama](https://daslang.io/dasllama.html#methodology)
- Source, BSD 3-clause: [github.com/GaijinEntertainment/daScript](https://github.com/GaijinEntertainment/daScript)

If you have a Mac and want a voice assistant on it, it is the best option we know of today.
If you want Qwen3.6-35B on that Mac, it prefills at 1.34x and decodes at 1.18x llama.cpp on
Metal, 1.7x and 1.2x on the CPU. If you have a 16 GB Vulkan card and want a 35B
mixture-of-experts on it, it decodes at 2.0x and prefills at 5.3x llama.cpp with the same
split. If you have a box we do not have, run the suite and send the row.

Every family on the [features page](../features.html) loads from its stock file, auto-detected, and is
verified token for token against its reference engine. llama.cpp, whisper.cpp, MLX and vLLM were the yardsticks and the
reading material throughout; none of this exists without them.
