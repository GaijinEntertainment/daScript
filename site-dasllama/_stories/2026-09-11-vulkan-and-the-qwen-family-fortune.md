---
date: 2026-09-11
tag: engine
title: Vulkan and the Qwen family fortune.
lede: Twenty-seven Qwen files, a 16 GB card, a friend's bug report, a card rented for the price of a coffee, and a fortune that reads definitely, probably, maybe.
---

<!-- figures: every first-card row is pp512 / tg128 tokens per second on Boris's RTX 5060 Ti 16 GB (Windows
     11, driver 616.56), ours from benchmarks/lcpp_bench.das (-jit --for-debug-purposes -r 3 -p 512 -n 128
     -t 16 under DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048),
     llama.cpp b10660 (build-vulkan-357) llama-bench -p 512 -n 128 -ngl 99 -fa 1 -t 16 on the same box
     the same day; both engines on NV_cooperative_matrix2 unless the row says KHR (ours DASLLAMA_COOPMAT=mm,
     theirs GGML_VK_DISABLE_COOPMAT2=1). Dense and hybrid rows: 2026-09-06/07 at the merge of PR #3954
     (das 245481f64) [BORIS: confirm the sha the 9/6-9/7 board rows were taken at]; the KHR figures (the
     27B 221 -> 395 -> 741 against 677 / 675, the 4B 3051 -> 4764): 2026-09-08 at PR #3978 (das 536d9b15c);
     the three MoE rows: 2026-09-09 at das 469893890 on the MoE arc's branch (PR #3988); the 30B-A3B Q4_K_M
     and Coder-Next rows are the expert-split form (llama.cpp -ot with the same experts on the CPU, its
     -ngl 99 fails the load). The 3060 -> 5060 Ti beat is Boris's, August 2026 [BORIS: month, and whether
     the 3060 ran cm2]. Stas's report: PR #3988's branch (debug-jit, the x86-vnni512 class profile, no box
     mint) on his RTX 5080 16 GB / Ryzen 9 9950X / Gentoo / GCC 16, -p 512 -n 128 -r 3 -t 16, flash
     attention both sides, the same 35B UD-IQ2_XXS file (10756586464 bytes): ours 3659.95 +- 169.56 /
     144.91 +- 8.90 with DASLLAMA_GPU_VRAM_MB=14000 (1471.53 / 68.46 unpinned, the resident driver
     declined: "needs 16464 MB of 10406 MB"), llama.cpp b10660 Release built from source with Vulkan SDK
     1.4.357 5083.76 +- 62.08 / 140.28 +- 4.30 (his b9888 Gentoo package 5023.82 / 141.46); his scaling
     table against PR #3988's 5060 Ti rows (2853.1 / 71.6 and 2995.8 / 107.7): 1.78x / 1.22x pp512, 1.96x
     / 1.35x tg128, on a 448 -> 960 GB/s step. The rented-card rows: RunPod, RTX 5080 16 GB, 84 SMs, driver
     580.173.02, 8 vCPU, $0.39 an hour, 2026-09-10/11 on the parity arc's branch (PR #4002, das tip
     918fc69c5, merged as 1fced5114), ours -r 5 -p 512 -n 32 -t 8 under the same env plus
     DASLLAMA_GPU_VRAM_MB=15500, llama.cpp b10660 llama-bench -p 512 -n 32 -r 5 -ngl 99 -fa 1 on the same
     pod; the second table is tg32, not tg128; the 30B and the 27B Q3_K_XL decode figures predate the
     decode levers (das 8b3b23ba8). The per-lever rates (35B 3304 -> 3835 -> 4236; 27B 1496 -> 1708; 9B
     4369 -> 5133; 35B tg 130.2 -> 146.7; the kernel 27 us alone / 56 in the model / 43 after the flush;
     4217 -> 4673 for NonWritable; the arm-matched 2049 against 2080 on the 5060 Ti) are followup_vulkan.md
     item 45 and PERF_LEDGER.md's rows of 2026-09-10/11; the driver-death beat is item 45's follow-up (8)
     (Windows LiveKernelEvent 141, nvlddmkm event 153, the stocked suite of 47 files on 2026-09-11). -->

Boris had an RTX 3060 in August, and things were working so well. Then he got an RTX 5060 Ti,
ran the same models, and everything was slower than llama.cpp. Nothing in the engine had
changed. The card had. That is what Vulkan is like, and it is why the Qwen family got the
first half of September.

## Two targets

There were two targets from the start. The first was to catch up on Vulkan, where the engine
lagged, and Qwen was the biggest piece of the lag. The second was Qwen3.8-27B on a 16 GB
card: the model people want to run at home, and the one that did not fit.

People do run it there. How they run it is a zoo. Four bits, three bits, two bits, mixed so
that the layers that matter keep more and the rest keep fewer, eight weight formats on one
trunk and the same eight on every other. So a good part of the two weeks went into feeding
that zoo into the engine, one format at a time, until every file in it loads and runs. On the
plus side, it now reads all of that. On the minus side, every format is one more mouth to
feed, on every kernel, on every card.

## What a Qwen is

From one perspective Qwen is twenty-seven files, 0.5 to 48 GB, dense and mixture-of-experts,
with and without a draft head, in those eight formats. From the other it is the shape of a
layer.

Every layer in every Qwen is a head and a tail. The head is how a token looks at the tokens
before it. The tail is what it does with what it saw. Qwen has two heads and two tails. The
attention head keeps a cache of everything it has seen and reads it back for every token. The
deltanet head keeps a fixed-size memory instead, one 128 by 128 matrix per head, and rewrites
it as tokens pass, so its cost per token never grows. The dense tail is one feed-forward block
every token walks through. The routed tail is a mixture of experts: a small router picks a few
experts out of many for each token, and only those run.

Each generation added exactly one piece. Qwen2.5 is the attention head with a dense tail.
Qwen3 dropped a bias and added a norm. Qwen3-30B made the tail routed. Qwen3.5 added the
deltanet head, three layers in four. Qwen3.6-35B has both new pieces, plus a shared expert
and a draft head. Qwen3.8-27B is the same hybrid trunk with a dense tail and a draft head. Two
heads, two tails, a drafter on top: that is the family, and every one of the twenty-seven
files is a point on that grid.

With Vulkan we record the work of one token once, assembled layer by layer from a head and a
tail, and replay the recording for every token after. One recording serves all twenty-seven
files of The Qwen Family.

## The walk

Walking the walk: it is always one model at a time, smallest first, and a model that does not
work is fixed before the next one is measured. Otherwise that crazy, and not so rare, scenario
of one token per second takes the better half of your day to notice.

| model | dasLLAMA (pp512 / tg128) | llama.cpp (pp512 / tg128) |
|---|---|---|
| Qwen2.5-0.5B Q8_0 | 42355 / 411.8 | 32338 / 381.9 |
| Qwen3-4B Q4_K_M | 5150 / 117.6 | 5142 / 126.3 |
| Qwen3.5-9B Q8_0 | 3227 / 43.8 | 2799 / 44.0 |
| Qwen3.8-27B UD-IQ4_XS | 865.4 / 23.5 | 814.6 / 24.1 |
| Qwen3-30B-A3B UD-IQ2_XXS | 3521.2 / 133.7 | 3520.0 / 116.6 |
| Qwen3.6-35B-A3B UD-IQ2_XXS | 2995.8 / 107.7 | 2853.1 / 71.6 |
| Qwen3-30B-A3B Q4_K_M, 18.6 GB | 749.8 / 66.5 | 612.5 / 42.2 |
| Qwen3-Coder-Next Q4_K_M, 48.5 GB | 381.3 / 27.8 | 118.1 / 22.1 |

<small>RTX 5060 Ti 16 GB, Windows. The last two do not fit the card. Both engines keep the same experts on the CPU; ours streams them over the bus per token.</small>

Three of the twenty-seven did not work on the first try. Qwen2.5 has an extra bias term in
its attention, and the GPU path had quietly refused the whole family over it. The Qwen3.5-4B
asked for one byte more than 4 GiB of memory where it should have asked for none, and the
size check believed it. And Qwen1.5-MoE, the 2024 ancestor, needed the same bias again on
another path.

The 27B landed. Qwen3.8-27B UD-IQ4_XS, 14.3 GB, runs whole on the 16 GB card at 865 / 23.5
against 815 / 24.1, and its 4-bit siblings read the same way. The End? Right???

## Things are never that simple

What if it were not this card? What if someone ran a 4080, or an AMD, or, oh no, an Intel?
None of those have cooperative matrix 2, the extension the 5060 Ti has. They have the older
cooperative matrix, the one every card past a certain age can run. On that arm the K-quant
formats, the zoo's most common ones, had no matrix tile at all, and the 27B prefilled at a
third of the rate: 221 tokens per second against 677. The first tile we wrote came in at half
the speed of the integer kernel it replaced. What a nightmare.

Sometimes you resort to the sincerest form of flattery. We consulted the yardstick's kernels
and found that back in 2024 kernels were a different shape, and had never heard of the
extensions the 5-series brought. The 27B went from 395 to 741. Who knew.

## The other other card

So we gave dasLLAMA to a friend to try. Well, we gave it to everyone to try; it is on GitHub
for anyone to play with. Try he did. Stas ran the 35B on his Linux box, an RTX 5080, against
the same llama.cpp build, and sent a report. Two things in it. The engine had not even taken
the card by default: it kept 3.8 GB of a 16 GB card in reserve against a hazard only Windows
has, decided the model did not fit, and ran the slow path.

Pinning the memory by hand put it on the card. It read 3660 / 145 against llama.cpp's
5084 / 140. Nightmare came true. Again.

We were like: no, we are not going to ssh into a friend's box for a week. It is rent-a-card
day. $0.39 an hour, so a day's work costs a coffee at Starbucks. What a time to be alive.

We quickly found that the reason was one line in the driver's extension list. By quickly we
mean half a day, after adding the driver's extension list to the log. Who would have thought
something like that could be useful. The Windows driver on the 5060 Ti
offers an extension that decodes four weights in one instruction, and our tiles use it. The
Linux driver does not list it, on a card that has the hardware, so every tile fell back to
decoding one weight at a time. llama.cpp's tiles fell back the same way. Its fallback was just
shaped for the fall. When it comes to the zoo of PC hardware you can fall back on previous
years of development, if you have them, that is. dasLLAMA could only fall back on July.

So a day and a half went into the fallback, one lever at a time, each one confirmed on the
rented card before the next. The Qwen3.6-35B hybrid, the one from the report, prefilled at
3304 tokens per second when the day started. A shorter inner step for the 2-bit and 3-bit
formats: 3835. Decoders that compute two weights and keep one: 4236. The block scales cached
in fast memory, which is what the K-quants of the Qwen3.5-9B and the Qwen3.8-27B wanted: the
27B went from 1496 to 1708, the 9B from 4369 to 5133. Then the decode side, where the small
kernels rule: the deltanet step keeping its state in registers, two halves of a projection
that no longer wait on each other, three small kernels folded into one on every model. The
35B's token rate went from 130 to 147.

Two things on the way we did not expect.

The first took an evening. A kernel we could time alone at 27 microseconds took 56 inside the
model, same shape, same data, and every suspect turned out innocent: not cold weights, not a
cold table of buffers, not a barrier in the way. We ended up flushing every cache on the card
before every dispatch, which is the sort of thing you do at eleven at night, and there it
was: 43. The cost was the kernel's own code. Its inner loop had been unrolled eight times,
the whole weight decoder copied into each, and the model streams 250 MB of weights between
two uses of that kernel, so the card threw the code out and read it back every single time.
One copy instead of eight, and the penalty went with it.

The second: the compiler tells the driver a buffer is read-only only when the kernel says
so, and most kernels had not said so, so the driver ordered every load behind every store to
any other buffer. Turns out the daslang compiler knows when a variable is read-only.
Amazing. The 35B went from 4217 to 4673 on that alone.

One side note on measuring there. The profiler on the rental box lies about kernels that overlap:
it makes them take turns while it watches. One lever it praised made the model slower, and
after that we believed the token counter and nothing else.

| model | dasLLAMA (pp512 / tg32) | llama.cpp (pp512 / tg32) |
|---|---|---|
| Qwen3.5-0.8B Q8_0 | 29016 / 382.7 | 29957 / 480 |
| Qwen3.5-9B UD-Q5_K_XL | 5145 / 102.9 | 5751 / 112.8 |
| Qwen3.8-27B UD-IQ4_XS | 1708.5 / 41.5 | 1684.9 / 47.4 |
| Qwen3.8-27B UD-Q3_K_XL | 1596.3 / 41.3 | 1665.2 / 49.3 |
| Qwen3-30B-A3B UD-IQ2_XXS | 5926 / 173.6 | 6005 / 219.1 |
| Qwen3.6-35B-A3B UD-IQ2_XXS | 5156 / 146.7 | 5247 / 145.2 |

<small>RTX 5080 16 GB, Linux, the one-weight decode arm on both engines; 32 generated tokens, not 128. The 30B's and the 27B Q3_K_XL's decode figures were taken before the decode work.</small>

The construction cannot be finished. Only stopped. This is where it stopped, for now. The
End. Right???

## Will it run on your card?

Definitely, if it is one of these two: an RTX 5060 Ti under Windows 11 with the 616 driver,
the one card we have measured to the bit, or an RTX 5080 under Linux with the 580 driver,
measured for a day and a half on a rented box. With the right driver and the right operating
system, of course.

Probably, if your card has KHR cooperative matrix: NVIDIA since Turing, AMD since RDNA3,
Intel's Arc, as far as we know. That arm was rebuilt this month and measured on the 5060 Ti
with the mode forced, so it has to work. Not that we ever tried it on anything else.

Maybe, if it has neither. Below cooperative matrix we run an integer dot-product tile and the
plain matrix-vector family. The tests dispatch every kernel of that arm on every format with
the modes forced off, and nobody has ever run it on a card that really lacks them, never mind
raced it. Below that - no 8- and 16-bit storage, no subgroup operations, no integer dot
product - the tier switches itself off, says why once, and the CPU takes over.

For the CPU we have a tuner: the first start races every kernel's spellings on your machine
and keeps the winners. For Metal we have one vendor, the same tuner, and a Mac to test on.
For Vulkan we have a tile pick that is a formula over the card's core count and its matrix
mode, a lane table measured on one card, and our best wishes for the rest. Works on my PC.

## Why Vulkan was last to the party

Because the CPU and Metal are fixed targets. A kernel that works there keeps working, and the
tuner says when it stops being the fastest. Vulkan is a moving target. The 3060 was fine, and
the 5060 Ti read slower without a line of code changing, because the new card had a matrix
mode the yardstick already used and we did not. Then the rented card did it again the other
way round: the same card family, a different operating system, one extension missing from the
driver's list. The rest of this engine is development. This part is keeping a zoo fed.

And one we have not caught. Twice in one day, while the test suite ran file after file on the
5060 Ti, Windows logged the GPU resetting after a timeout, and the second time the display
driver died with the box otherwise fine. Every test stayed green. We ran the 47 files one at a
time, and none of them did it alone. Don't you love it when your monitor goes black, never to
return until the reboot.

## The gift that keeps on giving

dasLLAMA is written in daslang, and its Vulkan kernels are daslang classes: a buffer is a
member, a constant is a member, the body is a method, and the compiler writes the GPU code.
Two weeks of Qwen pushed on that compiler as hard as anything has. A kernel can now read a
weight straight out of the plane it lives in instead of copying the block first, which is
what the fast tile needed. When a kernel says a buffer is only read, the compiler passes that
on to the driver, and when the kernel does not say, the compiler now works it out, which was
the 11 percent above, for every kernel at once. And a loop that cannot be unrolled is a
compile error now, not a slow kernel.

For a game developer this is all oddly familiar. Thirteen weight formats, three tile widths,
two decode arms and the cooperative-matrix arm are one kernel template with a few knobs, 137
kernels in all, 78 of them stamped from a template. A material by light by platform matrix is the same problem
with other nouns, and the same infrastructure now serves both.

## The door

dasLLAMA started on a Mac, because a Mac laptop was the machine at hand in a hospital bed.
Most of our friends do not run one. Vulkan is the door for them, and two weeks of the Qwen
family fortune is what it took to get the door to open on the one PC we have, and then on one
we rented for an afternoon. Works on my PC. Probably on yours. Definitely. Maybe. The End.
Right???
