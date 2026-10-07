# dasLLAMA Performance Ledger

**Living document.** The perf backlog for the whole dasLLAMA engine - CPU, Metal, and the
audio/ASR towers: what each finding costs today, what the fix would change, and which
hypotheses were measured and refuted.

Standing rule (Boris, 2026-07-01): any performance possibility spotted while doing wave work
gets a note HERE instead of being acted on mid-wave - the model waves optimize for correctness
and coverage; this ledger is the backlog for the perf pass that follows them. Every entry says
what it costs today and what the fix would change.

## Entries

- **MEASURED (2026-10-07, `direction-grade`, `debug-jit`) - the forced tool call under the grammar, on Anton's tool bench.**
  M1 Max, Metal, one stream, the server run from the tree under `-jit` with the m1 tune manifest, Anton's `bench_llm.py`
  tool part (the house prompt, a nonce a run, no-think, 256-token budget), 12 runs an arm; `auto` = `tools` alone,
  `forced` = the same with `tool_choice: "required"`, which samples the call through the dasLR1 grammar.

  | file | auto ok | forced ok | auto warm total | forced warm total | tokens |
  |---|---|---|---|---|---|
  | pure Q4_0 (4.52 bpw) | 6/12 | 12/12 | 1.29 s | 1.34 s | 40 |
  | pure IQ4_XS (4.27 bpw) | 12/12 | 12/12 | 1.31 s | 1.35 s | 40 |

  Q4_0's six misses were plain-text answers (`finish: stop`, no call), not malformed arguments; the grammar turns every
  reply into a well-formed `control` call. The constraint adds about 40-50 ms to a 40-token call (3-4 %): the piece table
  is built on the first constrained request (a second, folded into that request's time) and each token probes the
  candidate list against the grammar.
- **STATED (2026-10-07) - the token constraint's two vocabulary-sized allocations.** `Session.cand` (16 B an entry)
  grows to the whole row only when a constraint refuses every candidate of the probe window: 16 x vocab bytes a
  session, 2.4 MB at a 151936-entry vocabulary (Qwen), 4.2 MB at 262144 (gemma); it is `@scratch @exact_size` and
  reused, so the growth is paid once a session. `utils/dasllama-server`'s token piece table holds one string a token
  a served slot, built on the slot's first constrained request: about (24 + the piece's bytes) x vocab, 6 MB at
  151936 and 10 MB at 262144, freed with the slot. Neither is measured; the formulas are the record.

- **MEASURED (2026-10-06, `direction-grade`, `debug-jit`) - the 4-bit formats of Qwen3.6-35B-A3B at equal bytes, and
  four races on their Metal expert GEMVs.** M1 Max (MacBookPro18,2, 64 GB), Metal, one stream, the sidecar
  re-minted on the binary the same day (0 winner changes against the one the mtime rule had called stale);
  `benchmarks/lcpp_bench.das` as the `-jit` script, `-m <gguf> --ngl 99 -n 64 -r 2 --mtp-ab --for-debug-purposes`
  under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json`, decode = the tg-real64 row, MTP off; perplexity
  = 256 wikitext-2 positions teacher-forced after a 256-id prefill (a small sample: differences past the second decimal
  are noise); tool call = Anton's house prompt (`bench_llm.py`, a random nonce a run, `control` well-formed), 12 runs.
  The pure files are `llama-quantize --pure` from unsloth's BF16 shards; the two UD files are unsloth's. Decode rates
  are tok/s; bpw is the file's bits a weight.

  | file | bpw | tg | PPL | hits/256 | tool x12 |
  |---|---|---|---|---|---|
  | Q8_0 (reference) | 8.5 | 72.1 | 4.62 | 173 | 12 |
  | UD-Q4_K_M (Q4_K/Q5_K experts, Q8_0 rest) | 5.4 | 71.6 | 4.83 | 173 | 12 |
  | pure Q4_K | 4.52 | 81.1 | 4.93 | 174 | 11 |
  | pure IQ4_XS | 4.27 | 84.4 | 4.95 | 169 | 12 |
  | pure Q4_0 | 4.52 | 85.1 | 4.95 | 169 | 4 |
  | UD-IQ4_XS (IQ3_S gate/up experts, IQ4_XS down) | 4.3 | 73.6 | 4.97 | 167 | 12 |

  - Q8_0 is the one row near bandwidth: 3.5 GB a token at 72 tok/s is 250 GB/s. The 4-bit files move 1.9 GB a token
    at 85 tok/s, 160 GB/s: the 4-bit expert GEMVs reach 60 % of the q8 GEMV's bytes a second, so a 4.5-bpw file at
    the q8 kernel's rate would decode near 130 tok/s. The q8 kernel's shape - one simdgroup a row, four 32-weight
    chunks loaded ahead of the FMA chains - is the difference (`followup_metal.md` 45).
  - Pure Q4_0 slips the tool call on 8 of 12 runs, the same dropped `"arguments":` key every time, at the perplexity
    of the asymmetric formats: the symmetric codebook lands that near-tie wrong. Pure IQ4_XS is the fewest bytes at
    the K-quant files' quality, and the format to put against mlx's 4-bit group-64 affine weights (4.5 bpw, 19 GB).
  - Four kernel races on the gathered (expert) GEMVs, each the e2e tg minutes apart on the same binary and sidecar:
    the IQ3_S f4-slab twin on the gathered site, 73.6 -> 75.2 on the UD-IQ4_XS file (shipped, the dense twin's crown
    gates it); the same twin at sixteen rows a threadgroup 74.2 (fewer threadgroups a 512-row expert cost more than
    the halved staging saved); the grid read from constant memory instead of the staged float4 slab 69.8 (scattered
    constant reads, four scalar loads a word - vector literals do not hoist); the q40 identity codebook folded to
    arithmetic 85.3 +- 0.4 against 85.1 +- 0.5 (noise); the iq4 codebook held in registers as four packed words
    61.4 against 84.4 (seven ALU ops a nibble lose to one threadgroup read). The codebook read and the staging are not
    where a 4-bit expert GEMV's time goes.
- **MEASURED (2026-10-06, `audio-shared-kernels`) - the audio chains' kernels moved to templates both homes stamp read as
  before on both boxes.** whisper large-v3-turbo q8 served (`dasllama-server` with the ASR model alone, `main.das -- --asr
  <bin>`; `harness/served_bench.das --no-chat --asr-url <server> --clip jfk_ask_not.wav --reps 8`), master and the branch
  alternated twice in one sitting on each box, one life a server (the master lives first); the branch moves the Metal
  layernorm, bias, clamp, GEGLU, im2col, row gather, K/V store and conv0 stamps and the Vulkan tower's seam, im2col, gather
  and K/V classes onto the shared templates, so the served encode and decode ride every one of them.
  - M5 Max, Metal, ms a request, master / branch: 229.6 / 228.8 (cv 0.5% / 0.7%); the second pair 242.5 / 243.0, both void
    at cv 3.1% / 2.9% (the box warmed under the four lives).
  - RTX PRO 4500, Vulkan, ms a request, master / branch: 51.3 / 51.7 and 50.8 / 49.5 (cv 1.9% / 2.4% / 1.8% / 0.8%); a
    first sitting read every row void on that box's round-trip spread (cv 6-28%), with one master life refused at its
    second rep because the transcript differed from the first - the Vulkan chain's text moved between reps on master.
  - Long-lived device memory the branch adds, bytes: the Vulkan chat tail's rows `cap x (hid + gate + out) x 4` beside its
    host copy `cap x out x 4` (the whisper-class projector widths, a few MB at the 1500-row cap), its projector q8 planes
    beside the block records (`d x stack x mm1_out + mm2_in x proj_dim` bytes); parakeet's Vulkan resident (its q8 planes and
    norms plane, as canary's); the Metal fronts' conv0 panel `nch x 32 x 4` once an encoder (32 KB at 256 channels).

- **MEASURED (2026-10-06, `direction-grade`) - Pocket's text prompt on the device takes a third of its request on
  both boxes; the thirteen TTS kernels moved to templates both homes stamp read as before.** The prompt seat runs the
  chunk's text rows through the frames slab's layers at the voice's positions (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`,
  `ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain`), the keys and values back to the host; the last layer ends at
  its K/V on every rail, including the CPU's. `dasllama-server` with the TTS model alone, `harness/served_bench.das
  --no-chat --tts-url <server> --reps 8`, one sentence, master and the branch alternated twice in one sitting on each
  box (the master lives first); the stages are the engine's own stage clock. The M5's tune sidecar predates the binary.
  - M5 Max, Metal, ms a request, master / branch: Pocket q8 67.5 / 49.5 and 67.5 / 49.4 (4.56 s of speech on master,
    4.64 on the branch - the device prompt moves the EOS frame by one), its stages prompt 15.5 / 2.1, backbone
    38.4 / 34.2 (the attention's unrolled loads and the frames call's upload of rows the device already holds),
    codec 8.8 / 8.6; Kokoro-82M 73.5 / 73.5 (a third branch life 73.5, two branch lives void at cv 3.3% and 3.4%),
    its stages the same to the tenth (decoder 41.22 / 41.29); Kitten mini 64.5 / 64.5 (one branch life void at cv
    4.1%), decoder 29.37 / 29.36.
  - RTX PRO 4500, Vulkan (driver 580.159), ms a request, master / branch: Pocket q8 114.7 / 68.1 and 114.2 / 67.9,
    its stages prompt 50.0 / 3.8, backbone 50.2 / 49.2, codec 8.7 / 8.8; Kokoro-82M 48.3 / 48.1 and 48.3 / 47.9 (the
    first master row void at cv 9.3%), its stages the same to the tenth (bert 4.29 / 4.29, decoder 13.9 / 13.9);
    Kitten mini 48.6 / 46.4 and 52.5 / 47.8, every row void on spread (that box's round trip jitters), its stages the
    same to the tenth (decoder 10.5 / 10.4). The shared reduce base the row kernels now derive puts one call under
    every `WgReduceBase` reduce (eight audio-tower stamps and five TTS stamps moved by it, `harness/vk_spv_diff.das`):
    the backbone and bert stages above ride it, and whisper large-v3-turbo q8 served (`main.das -- --asr <bin>`,
    `served_bench.das --no-chat --asr-url <server> --clip jfk_ask_not.wav --reps 8`) reads 48.8 / 48.5 on the one
    round both sides held under 3% (54.2 / 54.7 on the other, both void).
  - Quality, `harness/tts_rig.py` on the 200-sentence corpus, flag-free on the M5 (the Metal seats serve), WER % /
    UTMOS before -> after: Pocket f32 4.18 / 4.373 -> 3.95 / 4.368, q8 4.41 / 4.363 -> 4.23 / 4.354, kq 3.91 / 4.325 ->
    3.77 / 4.314, stuart-kq 3.32 / 4.137 -> 3.27 / 4.139 (the device prompt moves an EOS frame here and there);
    kitten-nano 3.09 / 3.980 -> 3.09 / 3.976, kitten-mini 2.77 / 4.331 -> 2.77 / 4.331, kokoro 2.73 / 4.501 ->
    2.73 / 4.501.
- **MEASURED (2026-10-05, `direction-grade`, `debug-jit`) - the depth-1 NextN round on the M1 Max: the draft at two
  rows, then chained into the verify's command buffer.** M1 Max (MacBookPro18,2, 64 GB), Metal,
  Qwen3.6-35B-A3B-MTP UD-Q4_K_M, `benchmarks/lcpp_bench.das` as the `-jit` script (`-no-module-cache`):
  `-m <gguf> --ngl 99 -n 64 -r 2 --mtp-ab --prof --for-debug-purposes` under `JOBQUE_PROFILING=1
  DAS_LOG_LEVEL=info DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json` (the sidecar re-minted on
  the binary by `harness/dasllama_tuner.das`), `DAS_TUNE_POLICY` and `DASLLAMA_PIN_BACKEND` unset; the
  tg-real64 row is the mean and stdev over 2 reps of 8 prompts, the per-round split one untimed
  `forward_profile` window of 36-37 rounds, each arm one commit - `add367ec1` (the four-row tile, the host
  chain), `bf99297f5` (two rows), `3826f59ff` (two rows, chained). Decode rates are MTP off / on, tok/s, the
  parenthesis the on arm against the off arm; the round's parts are ms a round. The chained round parks its
  draft step through the verify, so the pool holds one more step than the host chain did: its logits rows
  `max(ng, 2) x vocab x 4` bytes (1.99 MB at one or two streams, 0.99 MB a stream past that on this vocabulary)
  beside a few activation rows.

  | | `add367ec1` | `bf99297f5` | `3826f59ff` |
  |---|---|---|---|
  | tg-real64 off / on | 71.8 +/- 0.0 / 69.1 +/- 0.1 (0.96x) | 71.7 +/- 0.0 / 73.9 +/- 0.9 (1.03x) | 71.7 +/- 0.0 / 74.3 +/- 0.0 (1.04x) |
  | acceptance | 76.4% (440/576) | 76.4% (440/576) | 76.4% (440/576) |
  | mtp.draft, host wall | 3.92 | 2.55 | 0.01 |
  | mtp.verify (its GPU wait) | 20.9 (20.5) | 20.5 (20.1) | 23.1 (22.6), the draft's GPU inside it |
  | mtp.walk + mtp.replay | 0.57 | 0.60 | 0.57 |

  - The four-row tile cost a solo stream a second pair of rows over the draft layer and the 417 MB Q6_K
    classifier (248320 x 2048); the two-row tile drafts the same tokens (0 of 288 rounds differ,
    `DASLLAMA_MTP_DEBUG=trace`, `-r 1`, the same flags) and the one-row form does not: 36 of 257 differ, that
    run an uncommitted probe edit of `bf99297f5` (`nrows = ng`, the rows forms' buffers and uniforms forced on
    for a draft step), no commit carries it (`followup_metal.md` 44).
  - The chain removed the draft's join, its 1 MB logits readback and the host argmax - 0.4 ms of the 2.55;
    the draft's GPU work now sits inside the verify's wait. The chained round drafts and commits the two-row
    host chain's tokens on every traced round (0 of 288 differ, the same trace flags).
  - The row-cost ladder at `a8655daf0`, `harness/batch_rows_probe.das -m <gguf> --bs 1,2,4 --steps 32` (two
    sessions a row) and `--sameslab --bs 1` (one session), same flags, ms a step: the single-row driver
    13.6; the rows driver at one row 15.5 (x1.14), at two rows 19.8 (x1.45), at four 31.2 (x2.29), every
    ratio against the single-row driver's step. The verify's 22.2 ms of GPU is that two-row step plus the
    draft layer, two classifier passes and the draft-slab re-warm; two rows route to two expert sets, so the
    step streams their union. At 1.76 tokens a round over a 1.45x step the depth-1 ceiling here is 1.21x.
  - The same-slab probe declines at two rows on this hybrid (`followup_metal.md` 43), so the two-row figure is
    the two-session batch step, not the verify itself.
- **MEASURED (2026-10-05, `direction-grade`) - Axpy and Concat as kernels both GPU homes stamp read as before on
  both.** The two kernels moved to one class template each (`dasllama_gpu_kernels_common.das`); on Metal their
  arguments became one struct and Axpy lost two offsets no site passed, on Vulkan the body became a function the
  entry calls (Concat's SPIR-V 52 bytes longer, Axpy's 196 shorter for the dropped offsets; 25 of the 27 TTS
  stamps byte-identical). `dasllama-server` with the TTS model alone (`main.das -- --tts <gguf>`),
  `harness/served_bench.das --no-chat --tts-url <server> --reps 8`, one sentence, master and the branch alternated
  twice in one sitting on each box; the decoder stage is the engine's own stage clock and is the stage that runs
  both kernels. The M5's tune sidecar predates the binary.
  - M5 Max, Metal, ms a request, master / branch / master / branch: Kokoro-82M 73.7 / 73.8 / 73.6 / 73.8 for
    5.68 s of speech (the last two void at cv 2.9% and 3.5%), its decoder 41.29 / 41.30 / 41.30 / 41.27; Kitten
    mini 64.6 / 64.7 / 64.5 / 64.8 for 5.84 s (the third void at cv 3.9%), its decoder 29.44 / 29.37 / 29.38 / 29.27.
  - RTX PRO 4500, Vulkan (driver 580.173), decoder stage ms, master / branch / master / branch: Kokoro-82M 14.14 /
    14.04 / 13.91 / 13.97; Kitten mini 10.54 / 10.53 / 10.57 / 10.63. The request walls there read 46.3-52.4
    (Kokoro) and 46.1-48.1 (Kitten mini) with seven of the eight rows void on spread - the round trip jitters on
    that box, the stage clock does not.

- **MEASURED (2026-10-04, `direction-grade`) - a served media turn's first token, and the speech rows beside
  mlx-audio.** M5 Max, Metal, `dasllama-server` with one model (`main.das -- -m <gguf> --image-mmproj <mmproj>` or
  `--audio-mmproj <mmproj>`, `-s 1 --ctx 16384`), the pinned llama-server as `-m <gguf> --mmproj <mmproj> -ngl 99
  -np 1 -c 16384`, `harness/served_bench.das --url <server> --no-chat --reps 5 --image <jpg> --chat-clip <wav>`
  (tagged `served`, `out-of-process`), a 60 s rest before each life; the tune sidecar predates the binary. The
  first token of a question about a 640x480 image or an 8 s clip under a system line no server has seen, ms,
  ours / the reference; a row over 3% cv is marked void.
  - The paged KV pool grew by doubling, and a doubling copied and zero-filled every cached page inside one
    request: Qwen3-VL 4B's image row read cv 7.9% with prefills of 116 ms beside ones of 158, 189 and 271. With
    the pool's address room reserved at scheduler creation, twenty prefills in a row read 116-118 ms and the
    row cv 0.9-1.2% over three runs. A model whose K/V passes the 4 GiB reserve (Qwen2-Audio, 524 KB a
    position) still pays one copy a server life: 246 ms, once.
  - A media prompt prefilled as three calls - text, rows, text - and each short call paid the call's floor
    (about 20 ms): Voxtral Mini's clip read 214.7 with 102 of encode and 84 of prefill (21 + 41 + 20). As one
    call it reads 181.0; Qwen3-VL 4B's image 185 to 148; gemma-4 E2B's image 115 to 82 and E4B's 164 to 98,
    once the body carries each row's token id.
  - Images: gemma-4 E2B 81.7 / 141.4, E4B 98.3 / 206.9, 12B 164.3 / 371.2; gemma-3 4B 426.6 / 466.8, 12B
    561.0 / 666.1; Qwen3-VL 4B 147.2 / 196.5 (the reference void, 3.2%), 8B 220.8 / 295.5; Qwen2.5-Omni 3B
    188.8 (void, 3.8%) / 224.6, 7B 257.0 (void, 5.7%) / 349.3.
  - Clips: gemma-4 E2B 77.9 / 125.8 (the reference void, 6.3%), E4B 107.4 / 187.2; Qwen2.5-Omni 3B
    219.3 / 294.5, 7B 348.2 / 465.6; Ultravox 1B 132.0 / 147.9 (the reference void, 6.1%), 8B 219.1 / 292.3;
    Voxtral Mini 181.6 / 189.1 (the reference void, 5.3%), Small 439.6 / 503.6 (the reference void, 4.2%);
    Qwen2-Audio 465.0 / 508.2. The E-series rows and the 12B image are single re-runs after their change; the
    rest are one sweep taken before the instrument's lead-in request, which is what voids the two Omni images.
  - Asked again under the same line, both servers answer off their cache within a few ms of each other on
    every row; the one row the reference leads is Voxtral Small, 167.8 / 161.8.
  - Speech, `main.das -- --tts <gguf>` against `mlx_audio.server` (mlx-audio 0.5.7), `served_bench.das
    --no-chat --tts-url <server> --reps 8`, one sentence: ms a request / s of speech / real-time factor.
    Kokoro-82M (the same voice, ours q8, theirs bf16): 73.1 / 5.50 / 0.013 against 108.9 / 5.47 / 0.020; three
    sentences 218.9 / 17.5 / 0.013 against 323.5 / 17.1 / 0.019. Kitten nano 22.5 / 5.24 / 0.004 against
    75.1 / 7.93 / 0.009 and mini 64.6 / 5.84 / 0.011 against 242.7 / 7.55 / 0.032 - their files are the 0.8
    release and speak the sentence longer, so the factor is the comparison. Pocket (ours the English 2026-04
    file at q8 and kq, theirs Kyutai's without-voice-cloning weights at full and 8 bit): 67.7 / 4.64 / 0.015
    and 61.7 / 4.80 / 0.013 against 200.3 / 4.96 / 0.040 (void, 8.6%) and 152.6 / 5.12 / 0.030 (void, 6.7%).
  - The M5 and M4 boards' Metal rows this arc re-routes are withdrawn until a mint on a fresh sidecar: the
    ASR rows of whisper tiny and large-v3-turbo, Canary-Qwen, Qwen3-ASR 0.6B, Qwen3-Omni 30B and gemma-4 E2B,
    and the image rows of gemma-4 E2B and E4B.

- **MEASURED (2026-10-04, `direction-grade`) - canary's mel and subsample front run on the Metal tower.**
  M5 Max, Metal, canary-qwen-2.5b (the q8_0 decoder, the f32 encoder), in process as the server's
  ASR worker runs it (the engine's stage clock, the fourth transcription of a clip), the tune sidecar
  predating the binary. The front seat was filled by the Vulkan driver alone, so on Metal the mel
  (a twiddle GEMM on the CPU) and the subsample stack ran on the CPU beside 80 ms of blocks on the
  device.
  - A 64 s clip: 1274 ms before - mel 239, front 258, blocks 80, projection 7, prefill 56, decode
    629 - and 772 after, the mel and the front together 8.0. An 8 s clip: 163 ms before, 106 after
    (3.5 for the two).
  - The device rows sit 2.1e-3 rel-l2 from the CPU chain's on the jfk clip (3.1e-4 with the CPU
    front ahead of the device blocks); the transcripts are equal. The reading is `test_model_image.das`'s
    canary `mtower` arm (`canary mtower encode rel-l2`).

- **MEASURED (2026-10-03, `direction-grade`) - the Metal tower reads a whisper encoder's q8 planes.**
  M5 Max, Metal, whisper large-v3-turbo, `dasllama-server` with the ASR model alone
  (`main.das -- --asr ggml-large-v3-turbo.bin`), the reference whisper.cpp `6fc7c33` as
  `whisper-server -m <bin> --inference-path /v1/audio/transcriptions`, `harness/served_bench.das
  --no-chat --asr-url <server> --clip <wav> --reps 5` (tagged `served`, `out-of-process`), a 60 s
  rest before each life, every request a clip no server has heard; the tune sidecar predates the
  binary, so the CPU kernels ran their fallback stamps. Clips of 8 s, 11 s and 64 s (one 30 s
  window, one, three), ms a transcription.
  - The tower declined a q8 encoder, and whisper serves q8: the encoder ran on the CPU and read
    911.7 / 951.4 / 3092.8. The reference reads 131.3 / 147.1 / 601.5 on the f16 file and
    127.1 / 139.8 / 554.8 on its q8_0 file (0.87 GB).
  - The block GEMMs off the q8 blob on the plain q8 tile: 240.5 / 253.1 / 876.7. On the prefill
    ladder: 153.5 / 166.1 / 608.9. With the stem's second conv off the same blob: 137.4 / 149.0 /
    541.0, on the 1.2 GB q8 image. The f32 encoder lane on the same tower reads 132.3 / 140.0 /
    517.9 on a 4.3 GB image.
  - A window's stages on the q8 lane (the encoder's own stage clock, in process): blocks 102.5 ms
    (94.0 on the f32 lane), the stem 2.2 (15.6 on the CPU), the decode 21.2, the cross K/V 6.4,
    the mel 1.4.

- **MEASURED (2026-10-03, `direction-grade`) - a served Qwen3-ASR transcription decodes on the device, and a
  repeated clip measures the other server's prompt cache.** M5 Max, Metal, `dasllama-server` with the
  ASR model alone (`main.das -- --asr <gguf> --mmproj <mmproj>`), the pinned llama-server as
  `-m <gguf> --mmproj <mmproj> -ngl 99 -np 1`, `harness/served_bench.das --no-chat --asr-url <server>
  --clip <wav> --reps 5` (tagged `served`, `out-of-process`), a 60 s rest before each life; the tune
  sidecar predates the binary, so the CPU kernels ran their fallback stamps.
  - The ASR worker's context carried no Metal mode, so the decoder - a `Model` session - prefilled
    and decoded on the CPU: Qwen3-ASR-0.6B Q8_0 read a 4 s clip in 127.5 ms. The worker takes the
    engine's mode for a two-file model: 50.5 ms. Inside it (the worker's own stage clock): encode
    10.7 ms, the 75-row prefill 6.7, twelve decode steps 29.0 - 2.4 ms a token, where the reference's
    own timing line reads 2.65.
  - The reference keeps the prompts it has evaluated, audio rows included: the same clip sent again
    costs it one prompt token (`cached_tokens` 79 of 80) and reads 37.4 ms, and a clip one sample
    off costs the whole prompt. The instrument's rows before this entry repeated one clip and so
    read that cache; every request now uploads a clip no server has heard.
  - Unseen clips, ours / the reference, ms a transcription: Qwen3-ASR-0.6B 4 s 50.5 / 60.7, 8 s
    76.6 / 98.1 (the reference's cv 6.1%, void); Qwen3-ASR-1.7B 4 s 85.5 / 95.1, 8 s 130.8 / 164.4.

- **MEASURED (2026-10-02) - the K/V mirror's block codecs on the whole-model driver
  (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-kv-block-codecs`, `set_gpu_kv_dtype` / the server's `--kv-dtype`): a
  q8_0 mirror is 17/32 of the f16 mirror's bytes and tq4 9/32, and on a model whose context was asked whole
  the bytes they give back are resident layers - the 35B at 131072 positions goes from seven routed layers'
  experts on the host to none and reads 3.1-3.4x the prompt rate; on a model the card holds whole they cost
  the long prompt 6% (q8_0) and 16% (tq4) and a token nothing (direction-grade).**

  zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link), `bin/Release/daslang.exe -jit
  modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <file> --ngl 0 -p <P> -n <N> -r <reps> -t 16 --kv <codec>
  --for-debug-purposes` under `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU=1 DAS_JOBQUE_THREADS=16`;
  `DAS_TUNE_POLICY`, `DASLLAMA_PIN_BACKEND` and `DASLLAMA_COOPMAT` unset (the registry's own pick `x64-gen`
  served the CPU kernels, the device ran cm2), one process a row - a second load of a model in one process
  is barred, so the codecs cannot race interleaved. `--kv` asks the mirror's codec and makes the bench's
  sessions in it, so each row is a served shape. A cell is the mean of its reps and their standard deviation.
  Every row is its own process, so every comparison between two rows of the tables below is direction-grade.

  Llama-3.2-1B-Instruct Q4_K_M, whole on the card at the model's 131072 positions under every codec, two
  reps, 1.9-2.1 GB of the card held by the desktop:

  | mirror | pp512 | tg128 | pp8192 | tg64 after 8192 |
  | :--- | ---: | ---: | ---: | ---: |
  | f16 | 19337 +/- 69 | 331.0 +/- 25.8 | 16854 +/- 83 | 344.7 +/- 1.2 |
  | q8_0 | 21873 +/- 44 | 330.7 +/- 5.5 | 15860 +/- 161 | 338.0 +/- 0.0 |
  | tq4 | 18454 +/- 267 | 344.2 +/- 0.5 | 14150 +/- 5 | 327.9 +/- 2.0 |

  Qwen3-30B-A3B-Instruct-2507 Q4_K_M (17.3 GB, 48 routed layers), `DASLLAMA_GPU_CTX_MAX=32768
  DASLLAMA_GPU_CTX_STRICT=1` added, three reps, the four processes back to back with 1924 MB of the card
  held by the desktop:

  | mirror | routed layers' experts on the host | pp512 | tg128 |
  | :--- | ---: | ---: | ---: |
  | f16, the round's first process | 25 of 48 | 409.9 +/- 89.6 | 43.7 +/- 2.6 |
  | q8_0 | 21 of 48 | 572.5 +/- 26.2 | 49.8 +/- 1.7 |
  | tq4 | 19 of 48 | 586.9 +/- 13.2 | 52.5 +/- 4.0 |
  | f16, its last | 25 of 48 | 516.0 +/- 3.8 | 46.7 +/- 0.9 |

  Qwen3.6-35B-A3B UD-IQ2_XXS (10.3 GB, 40 layers, a deltanet hybrid), `DASLLAMA_GPU_CTX_MAX=131072
  DASLLAMA_GPU_CTX_STRICT=1` added, three reps, the four processes back to back with 1916-1930 MB held:

  | mirror | routed layers' experts on the host | pp512 | tg128 |
  | :--- | ---: | ---: | ---: |
  | f16, the round's first process | 7 of 40 | 863.2 +/- 35.1 | 89.8 +/- 0.5 |
  | q8_0 | 3 of 40 | 1544.6 +/- 7.0 | 100.0 +/- 0.8 |
  | tq4 | none | 2962.7 +/- 34.7 | 111.6 +/- 0.6 |
  | f16, its last | 7 of 40 | 966.8 +/- 11.3 | 84.7 +/- 1.1 |

  The count of hosted layers follows the room the desktop leaves, so a codec's rows compare only at one
  share: the entry below reads the 30B under f16 at 28 of 48 layers (517.9 / 41.8) with 4.6 GB held, and
  its parity run the 35B at 10 of 40; both stand for their share, and the rows above for theirs.

  The NextN round over the codecs: `--mtp-ab -n 64 -r 2` in place of `-p` / `-n` / `-r` (real text, eight
  prompts, depth 1, the off and on arms in one process a row), the head on the device in every row and no
  call passed to the CPU rails:

  | model | mirror | off | on | on / off | drafts accepted |
  | :--- | :--- | ---: | ---: | ---: | ---: |
  | Qwen3.5-0.8B-MTP Q8_0 | f16 | 316.6 +/- 10.7 | 397.6 +/- 6.8 | 1.26 | 81.9% |
  | | q8_0 | 317.6 +/- 3.9 | 392.0 +/- 1.3 | 1.23 | 81.3% |
  | | tq4 | 314.1 +/- 1.1 | 394.8 +/- 0.3 | 1.26 | 82.7% |
  | Qwen3.5-4B-MTP Q8_0 | f16 | 76.5 +/- 3.1 | 115.4 +/- 0.1 | 1.51 | 81.3% |
  | | q8_0 | 78.3 +/- 0.3 | 115.8 +/- 0.3 | 1.48 | 82.0% |
  | | tq4 | 74.5 +/- 5.3 | 114.8 +/- 0.6 | 1.54 | 82.3% |
  | Qwen3.5-9B-MTP UD-Q5_K_XL | f16 | 59.1 +/- 1.2 | 88.5 +/- 0.2 | 1.50 | 80.7% |
  | | q8_0 | 59.3 +/- 0.3 | 87.2 +/- 0.5 | 1.47 | 78.8% |
  | | tq4 | 59.4 +/- 0.6 | 88.9 +/- 1.5 | 1.50 | 82.6% |

  The round's rate does not follow the codec (the on arm moves 1.5% at most across a model's three rows,
  direction-grade) and neither does the acceptance (78.8-82.7%): the draft and the verify read the blocks
  through the token command's attention, which the codec does not slow. These nine rows were read at
  372db0f70, before the codec arms' block loads were factored into shared helpers: the codec stamps' words
  differ at the tip (`harness/vk_spv_diff.das`: 13 of 13 moved), the arithmetic they encode does not. A
  re-read of the 0.8B at the tip, the tree merged over master at 42c4013cd and the desktop holding 2.4 GB
  of the card, accepts the same drafts (81.9%, 81.3%, 82.7%) at lower rates on every engine: the f16 row
  with master's `dasllama/` and `benchmarks/` checked out in place reads 281-285 off and 327-350 on, this
  tree 291-292 and 339-340 back to back, its q8_0 row 255 and 332, its tq4 row 272 and 337.

  The per-op rails under the session's codec (`ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`):
  Qwen3-30B-A3B-Instruct-2507 Q4_K_M with no context pin - the whole-model driver declines the 17.3 GB
  file and the per-op rails serve - `-p 512 -n 128 -r 1`, one process a row. The tq4 row before this entry's
  change is the same command on the tree where the decode block and the prefill chain both refused a tq4
  session and its attention ran on the CPU, a compare across two commits (direction-grade):

  | session codec | pp512 | tg128 |
  | :--- | ---: | ---: |
  | f16 | 662.6 | 59.9 |
  | q8_0 | 733.5 | 63.6 |
  | tq4, its attention on the CPU (before) | 274.8 | 31.3 |
  | tq4, the block's f16 mirror hydrated out of the rotated basis | 688.2 | 65.0 |

  The three served rows land one warm-up argmax and logit (`ency` 15.93165 after the prompt, `and`
  11.371113 after the first step): the block attends its own float rows under every codec, and the codec
  is the cache's.

  The 1B's long prompt is the shadow's price (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-kv-block-codecs`): the
  flash tiles read f16 rows, so each layer of each window dequantizes the span it attends first, and tq4
  adds the rotation of the window's rows and the un-rotation of its attention rows. The decode attention
  reads the blocks natively and the token rate does not follow the codec beyond the rows' own spread (the
  1B's tg128 reads 331-344 with f16's own two reps 26 apart, and at 8192 positions 328-345; direction-grade).
  The MoE rows are the reason the codecs exist on this driver: a hosted layer costs a fence round-trip a
  token and a host callback a window, so every layer the smaller mirror brings back shows in both rates; the
  35B under q8_0 still hosts three layers and its prompt rate reads half of tq4's, which hosts none
  (direction-grade).

  What the codecs allocate, beside a mirror 17/32 or 9/32 of the f16 one's bytes. On the device: the f16
  shadow `ksh_dev` / `vsh_dev`, `(seq_cap x kv_dim + 131072) x 2` B a plane with `kv_dim` the widest
  attention layer's K/V row - 128.2 MiB a plane on Llama-3.2-1B (512 wide) at 131072 positions, 2.25 MiB on
  Qwen2.5-0.5B (128 wide) at 8192; the window's staged rows `pf_kst` / `pf_vst`, `512 x kv_dim x 4` B a
  plane (1 MiB and 256 KiB on the same two); the token command's staged rows `kst_dev` / `vst_dev`,
  `kv_dim x 4 x nb` B a plane with `nb` the command's rows, eight at most (16 KiB on the 1B at eight rows,
  512 B on the 0.5B at one), beside `stage_tok`, 32 B a row; tq4's sign row, 2048 B. On the host, the
  per-op rails' hydrate arrays `g_dat_hyd_k` / `g_dat_hyd_v`, which a tq4 session now reaches: `pos x
  kv_dim x 4` B each, 8 MiB at 4096 positions of a 512-wide row and 512 KiB at 1024 of a 128-wide one.

  The bars the codec cells hold, each constant with the reading behind it. The readings come from the
  cells' own logs on zen2 (the box of this entry, `DASLLAMA_GPU=1`, `DASLLAMA_COOPMAT`, `DASLLAMA_PIN_BACKEND`
  and `DAS_TUNE_POLICY` unset, eight job threads), run through `bin/Release/daslang.exe -jit
  modules/dasLLAMA/tests/run.das -- --changed`:
  - `test_gpu_resident_qwen2.das` (Qwen2.5-0.5B Q8_0): `Q8KV_BAR_REL` 0.08 of the step's max logit (reads
    0.39-0.72 of 9.4-18); `CODEC_DIST_REL` 1.15, the device's summed distance from the f16 chain over the CPU
    codec chain's own (reads 0.95-1.01, the rows one step off 1.6-3.7); `CODEC_PAIR_REL` 0.8, the two codec
    chains' distance over that same distance (reads 0.20-0.61, one step off 1.5-3.7); `ROWS_BARS`, the share
    of layer 0's stored bytes that differ from the CPU cache's - `k` 0.02 (reads 0.002), `v_step` 0.05
    (reads 0.007-0.010), `v_prefill_of_ctrl` 0.6 of the control's share (reads 0.39 and 0.12 against 0.97);
    `CODEC_ROWS_CTRL` 0.2 in `_kv_codec_cells.das`, a row against the next one (reads 0.36-0.98), and
    `CODEC_APPLIED_REL` 0.5 there, the least the device's distance from the f16 chain may be of the CPU
    codec chain's (every resident carrier reads 0.95-1.08; a driver serving f16 rows in the codec's place
    would read near 0).
  - `test_gpu_resident_codec_llama_k.das` / `_llama_iq.das` (Llama-3.2-1B Q4_K_M and IQ4_XS):
    `Q8KV_BAR_REL` 0.10 (reads 0.25-0.42 of 8.4-9.4); `CODEC_DIST_REL` 1.15 (reads 0.99 and 1.00, one step
    off 4.7 and 5.0); `CODEC_PAIR_REL` 1.4 (reads 1.02 and 1.13, one step off 4.9 and 5.1) - tq4 moves these
    carriers a point a row, three times the kernels' rounding, so the two codec chains part as two
    independent quantizations do.
  - `test_gpu_perop_kv_codec.das` / `_k.das` / `_iq.das` (Qwen3-0.6B Q8_0, Qwen3-4B Q4_K_M, Qwen2.5-1.5B
    IQ3_XS): `PeropBars.rows` 0.05, the share of layer 0's cache bytes off the CPU chain's (reads 0 to
    4e-5); `PeropBars.dist_rel` 1.0, the device's summed distance from the f16 chain over the CPU codec
    chain's (reads 0.05 and 0.31 on the 0.6B where the device prefilled, 0.90 over rows the CPU quantized,
    0.28 and 0.80 on the 4B, 0.79 under q8_0 on the 1.5B), and 0.5 for the 1.5B's tq4 cell, whose CPU chain
    tq4 moves nine points a row (reads 0.11, one step off 0.84).
  - `test_gpu_resident_hybrid.das`: the block-codec cell holds the NextN draft, verify, warmed head rows
    and rounds under q8_0 to the file's f16 bars; the gated cell (Qwen3.5-0.8B Q8_0) holds
    `GATED_Q8KV_BAR_REL` 0.06 (reads 0.21-0.31 of a 10.7-12.3 max logit), `GATED_CODEC_DIST_REL` 1.15
    (reads 1.08, one step off 9.2) and `GATED_CODEC_PAIR_REL` 0.8 (reads 0.63, one step off 9.3) - the
    head gate applied twice under tq4 reads 9.2; `DN_LOGIT_BAR_REL_KQ_Q8OUT` 0.08 is the mixed twin's
    bar (one step reads 0.91 of a 12.3 max logit, the rest 0.42-0.60), and `F32_DIST_REL` 1.15 its
    witness against the file's f32 chain on the CPU - the device's summed distance from it over the CPU q8
    chain's own (reads 0.91 at 40 tokens, 3.42 against 3.77, and 0.93 at 600, one step off 21 and 25): at
    the step the old 6% bar missed, the device sits 0.63 and the CPU chain 0.50 from the f32 row, on
    either side of it, and the device's pick is the f32 chain's.
  - `test_gpu_resident_gemma3_1b.das` and `test_gpu_resident_gemma4_e2b.das` hold their block-codec cells
    to the gemma rig's own bars (`GEMMA_LOGIT_BAR_REL` 0.16 under q8_0: gemma-3-1b at 1100 tokens reads
    0.46-1.37 against bars of 3.3-5.1; `GEMMA_PPL_RATIO` 1.05 under tq4: 1.022 against the CPU chain's
    1.033 on gemma-3-1b, 1.032 against 1.032 on E2B). `test_gpu_resident_moe.das` holds its twin under
    q8_0 to `MOE_LOGIT_BAR_REL` 0.20 (reads 0.83-1.17 against bars of 2.1-2.3, the f16 mirror's reading)
    and under tq4 to `MOE_CODEC_DIST_REL` 1.15 (reads 1.06, one step off 5.0) and `MOE_CODEC_PAIR_REL`
    1.4 (reads 1.11, one step off 5.0).
  - `_resident_regions.das` (the driver against itself): under a tq4 mirror `TQ4_CTRL_SHARE` 0.6 holds a
    tolerance row to that share of its distance from the one-token-off row (reads 0.09-0.44) in place of
    a share of the peak, which no tq4 carrier holds - two window splits of one prompt read 0.8-2.0 apart
    at a 12 max logit, the split q/k norm against the fused one 1.1-3.5 at 13-15 (Qwen3-0.6B; that split
    form sits 0.94 of the CPU tq4 chain's distance from the f16 chain, the fused one 1.02), and the
    Llama K-quant carrier, bit for bit under tq4, has a row one off 1.9 away.

  Evidence, through the same run. Kernel cells, `test_vulkan_kv_codec_kernels.das`: `test_vkc_q8_store` and
  `test_vkc_tq4_store` hold the stores byte for byte the CPU's, `test_vkc_q8_da_attn` and
  `test_vkc_tq4_da_attn` the attention stamps within 6e-8 to 2.5e-6 of the CPU attention over the
  dequantized rows at a 1e-4 bar, `test_vkc_shadow` and `test_vkc_tq4_unrot` the shadows and the un-rotation
  bit for bit. GPU against CPU: the cells of the four bullets above, each log carrying `resident driver
  armed` with the mirror's codec (the per-op cells the attention block's census witness) and no `resident
  override passed a call` line for a served call. The driver against itself:
  `test_gpu_resident_regions_q8kv.das` and `test_gpu_resident_regions_tq4kv.das` (26 cells each), and
  `test_vulkan_mint_kv_codecs`. On Qwen2.5-0.5B a tq4 mirror sits 51.6 summed over nine rows from the f16
  chain and the CPU's tq4 chain 54.3 - the codec's own distance, which the distance cells measure on both
  chains (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-kv-tq4-basis`).
- **MEASURED (2026-10-02) - the remaining families through the server, smallest up.** M5 Max, Metal,
  `dasllama-server` against the pinned llama-server, `harness/served_bench.das` as the qwen ladder
  ran it; the 24B and up at 2 reps (the rows' turns run 17 s a reply on a 40 GB model). Every
  served entry of this date read its system prompt off the corpus a line at a time; the
  instrument reads it a `%%` segment at a time since (`harness/_corpus.das`), the prompt's
  characters pinned by `--sizes` either way, so a re-run's token counts differ by a few. Decode
  tok/s at 3k / 9k, then warm TTFT ms, then cold TTFT:
  - Mistral-7B Q4_K_M 100.5 / 83.1 vs 98.7 / 85.0; 38.4 / 50.8 vs 68.7 / 94.0; 1015 / 4103 vs
    1377 / 5998. Decode a tie at the weight-read floor.
  - Llama-3.1-8B Q8_0 62.8 / 54.3 vs 61.8 / 55.7; 44.2 / 62.1 vs 64.9 / 84.0; 1028 / 4266 vs
    1265 / 6212. The same.
  - Qwen1.5-MoE-A2.7B Q8_0 146.3 / 113.8 vs 134.8 / 105.8; 44.0 / 50.0 vs 48.2 / 61.8; 576 / 1513
    vs 693 / 2702.
  - Mistral-Small-3.1-24B Q4_K_M 35.8 / 23.6 vs 34.9 / 22.8 (both 9k rows void at cv 14-15%: the
    14 GB dense model runs hot); 94 / 167 vs 180 / 369; 3253 / 12966 vs 4151 / 16834.
  - Qwen3.8-27B UD-Q3_K_XL, self-speculative (the file carries its NextN layer, `blk.64`, so
    `--mtp` arms with no head sidecar): 35.7 / 32.0 vs 29.7 / 29.7; 133 / 128 vs 282 / 297;
    3586 / 12520 vs 4553 / 15455. Plain (a TOML `mtp = false` - the `--mtp` flag is a bool and
    `--mtp 0` reads as on) 29.2 at 3k: the round is +22% served, as it is in a flat session
    (`q27probe`: 29.5 -> 36.3 tok/s, 45 of 55 drafts accepted; a paged session 36.2). The split
    `mtp-Qwen3.8-27B-Q8_0.gguf` pinned over it reads 34.8 / 31.4 - the same head, a separate
    `metal-mtp` image lane.
  - Llama-3.3-70B Q4_K_M 12.0 / 7.9 vs 11.5 / 7.8; 243 / 390 vs 601 / 912; 10911 / 40055 vs
    14121 / 55697. Decode at the floor on both; the 3k-to-9k fall is 80 layers of 1024-wide f16
    K/V (2.9 GB a step) on top of the 40 GB of weights.
  - Qwen3-4B Q4_K_M again after the partial-page attach: warm TTFT 36.4 / 67.6 -> 32.4 / 42.4,
    decode 144.1 / 110.9 unchanged.

- **MEASURED (2026-10-02, `direction-grade`) - phi-3 through the server: a head of 96 and an untied K-quant embedding.**
  M5 Max, Metal, Phi-3.5-mini-instruct Q4_K_M (32 heads of 96 over 32 K/V heads - 384 KB of f16 K/V
  a row, so the mirror clamps the context to 10922 and a 9k step reads 3.4 GB of K/V) against the
  pinned llama-server, `harness/served_bench.das` as the qwen ladder ran it.
  - Before: decode 107.5 / 64.4 tok/s at 3k / 9k against 125.7 / 81.5, warm TTFT 96.9 / 213.7 ms
    against 43.6 / 65.1, cold 9k 4894 ms against 4298 - the one family the reference led on every
    row. Three shapes: the fused decode attention's gate refused a head of 96 (the chunked pair at
    depth), the device prefill pair's gate wanted a head on the 64 lattice (the staged trio and the
    panels form, no mirror-fed window), and the greedy chain gathered a winner's row from a Q8 or
    tied-K6 table alone (the untied Q4_K embedding keeps its fp32 table: no chain, a host turnaround
    a step).
  - After the three: decode 125.1 / 80.4 (a tie at the K/V read floor: 7.8 / 12.0 ms a step on the
    trace, 14.1 ms chunked at 9k), warm TTFT 34.4 / 53.2, cold 603 / 2856, the windows `kv=mirror`.
  - The fp32 gather chains every untied K-quant carrier that never chained: the Q4_K_M mistrals and
    llamas, Qwen3-4B Q4_K_M (its 9k row read a tie), gemma-3-12b Q4_K_M.

- **MEASURED (2026-10-02) - the qwen ladder through the server, smallest up.** M5 Max, Metal,
  `dasllama-server` against the pinned llama-server, `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`,
  f16 KV, 200-token replies, 5 reps, a rest before each life; `--mtp` on the NextN carriers,
  which llama-server serves plain). Decode tok/s at 3k / 9k of context, ours against the reference,
  then warm TTFT ms:
  - Qwen2.5-0.5B Q8_0 498.5 / 409.9 vs 312.5 / 310.8; 11.8 / 18.8 vs 25.5 / 38.4.
  - Qwen3-0.6B Q8_0 366.8 / 252.2 vs 275.8 / 217.5; 13.2 / 21.1 vs 25.6 / 39.1. Both fall with
    depth on the f16 K/V read - 28 layers of a 1024-wide K/V, about a gigabyte a step at 9k.
  - Qwen3.5-0.8B-MTP Q8_0 381.7 / 355.6 vs 244.8 / 267.5; 13.6 / 16.1 vs 27.2 / 33.3.
  - Qwen3-4B Q4_K_M 144.0 / 110.6 vs 137.7 / 111.2; 36.4 / 67.6 vs 51.3 / 69.5 - a tie at 9k:
    the 3k-to-9k cost (2.1 ms a step over 36 layers of 1024-wide f16 K/V) is the read both pay.
  - Qwen3.5-4B-MTP Q8_0 130.3 / 125.3 vs 90.3 / 86.6; 37.7 / 44.1 vs 62.4 / 72.8.
  - Qwen3.5-9B-MTP UD-Q5_K_XL 86.2 / 84.6 vs 71.5 / 68.2; 46.5 / 49.6 vs 93.2 / 106.8.
  - Qwen3.6-27B-MTP Q4_K_M 37.1 / 34.8 vs 26.2 / 18.4; 133.6 / 133.5 vs 276.0 / 497.9. The 16 GB
    dense model sits at the bandwidth floor plain (26 tok/s) and runs hot: its rows read cv 10-22%
    on both servers, the 9k row re-measured alone after a rest (23.7 right after the 3k rows).
  - Qwen3-30B-A3B Q4_K_M 131.2 / 102.2 vs 117.8 / 95.1; warm TTFT 69.0 / 98.3 vs 66.6 / 89.8 -
    the one row the reference led: the prefix cache attached whole 64-row pages (2944 of 3000,
    8768 of 8844 cached) where llama-server reuses all but the turn's new tokens (2981, 8825), so
    the warm turn prefilled 57-76 tokens through the routed experts against its 19. With the page
    past the whole hits copied for its shared rows (`ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page`)
    the same run reads 2981 / 8825 cached and warm TTFT 48.9 / 61.1 ms; decode 132.8 / 102.4.
  - Qwen3.6-35B-A3B-MTP UD-Q4_K_M 141.3 / 132.1 vs 92.8 / 89.4; 56.8 / 58.2 vs 86.7 / 98.1.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention's block-codec stamps carry every head class.**
  M5 Max, Metal, `benchmarks/decode_step_trace.das -p 9000 -n 128 --bucket 128 --kv q8_0`, GPU ms a
  step, `DASLLAMA_METAL_ATTN_D=0` (the chunked pair) against the fused form.
  - Llama-3.2-1B Q8_0 (a head of 64, 8 K/V heads): 5.98 / 5.99 chunked, 4.87 / 4.89 fused - 167 ->
    205 tok/s at 9k on the q8_0 mirror, where the head of 64 had no quant stamp and paid the pair.
  - gemma-3-1b Q8_0 (a head of 256, one global layer in six at 9k, the rest a 512-key window):
    3.97 chunked, 4.02 fused on the q8_0 mirror - level; the f16 mirror's own A/B on the same run
    reads 3.35 -> 3.17. The wide quant stamp buys nothing a model this light on attention can
    show; it exists so the block codecs leave the chunked pair with the float mirrors.

- **MEASURED (2026-10-02, `direction-grade`) - the server attaches gemma-4's assistant drafter, and the same-slab
  verify's arena demand is the session's slice once.** M5 Max, Metal, gemma-4-26B-A4B-it Q4_K_M
  with `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` beside it, through `dasllama-server --mtp` and
  `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token replies, 5 reps, a rest before
  each life).
  - Plain, the 26B decodes 112.9 / 107.0 tok/s at 3k / 9k of context (the pinned llama-server
    92.6 / 87.1). The server never attached the drafter: its speculation gate asked for a NextN head.
    Attached, 139.8 (void) / 49.8 - the 9k row a regression, every round a draft, a declined verify
    and a plain step. The batch driver summed each verify row's `mirror_arena_demand`, and the
    same-slab verify names ONE session for every row: at 9k the 26B's slice is a 16384-row cap, and
    five of it pass the 4096 MB arena ceiling, where five of the 4096-row slice at 3k did not.
    Counted once: 136.7 (cv 3.7%) / 131.2 (cv 4.0%) tok/s, warm TTFT 51.8 / 66.8 ms; the decode
    cv is the rounds' acceptance moving turn to turn, and sits past the 3% bar on both rows.
  - The scheduler probe (a 60-token free run on a number ramp at 9k, `MetalMode.required`): the
    exception `batch decode declined: mirror` before, 10.6 ms a token after, 42 drafted / 16
    accepted - the ramp accepts poorly, prose through the server far better.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention serves a head of 512 and a model of two head
  classes, its wide stamps a slice group a threadgroup.** M5 Max, Metal, gemma-4-E2B-it Q8_0 (a
  head of 512 on the full-context layers, of 256 under a 512-key window on the sliding ones, 8
  heads over one K/V head) through `dasllama-server` and `harness/served_bench.das` (tagged
  `served`, f16 KV, 200-token replies, 5 reps, a rest before each life).
  - Decode at 3k / 9k of context: 141.2 / 126.6 tok/s on the chunked pair, 145.0 / 139.2 on the
    fused form; the pinned llama-server 130.3 / 122.5. The same run: warm TTFT 27.9 / 35.8 ms
    against 51.2 / 63.5, cold 304 ms (void) / 956 against 511 / 1705.
  - The wide stamps' first form - a lane walking two or four quads of the head in turn - lost to
    the chunked pair at every depth (`decode_step_trace`, GPU ms a step at 400 / 3000 / 9000
    keys: 6.07 / 6.44 / 7.33 chunked, 6.63 / 7.17 / 7.78 fused): a decode step is some 540
    serial dispatches, so a dispatch costs what its longest threadgroup takes, and a slice took a
    wide head four times a narrow one's time. With a threadgroup per slice group and a simdgroup
    per 128 dims, a slice's scores summed through threadgroup memory, the fused form reads 6.07 /
    6.26 / 6.56. More slice groups on a wide head (64 against 16) read worse at depth (6.70 at
    9000 with the split form), so the cap stays at 16.
  - The head-of-256 stamp takes the same form: gemma-2-2b-it Q8_0 decodes 154.0 / 143.0 tok/s at
    3k / 7k (145.3 / 135.0 on the lane-walking form, llama-server 140.8 / 132.2), and
    Qwen3.6-35B-A3B under self-speculation 144.9 (void) / 134.3 at 3k / 9k (144.4 / 132.1).
  - The narrow row (Llama-3.2-1B Q8_0, two streams at 512 / 9000) reads 532.9 / 391.0 tok/s
    against 533.3 / 394.2 before: the narrow stamps' source is unchanged.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention carries the attention logit soft cap.**
  M5 Max, Metal, gemma-2-2b-it Q8_0 (a head of 256, the cap, a sliding window on alternate
  layers; context 8192) through `dasllama-server` and `harness/served_bench.das` (tagged
  `served`, f16 KV, 200-token replies, 5 reps, a rest before each life). Decode at 3k / 7k of
  context: 142.3 / 124.4 tok/s on the chunked pair, 145.3 / 135.0 on the fused form; the pinned
  llama-server 141.4 / 132.5. The same run: warm TTFT 25.3 / 30.4 ms against 118.1 / 63.3, cold
  3k 288 ms (void) against 467. The two-stream batch row on a model with no cap (Llama-3.2-1B
  Q8_0, `lcpp_bench --npl 2 --npl-plen 512 | 9000`) reads 535.3 / 393.3 tok/s after the kernel
  took the uniform, 533.3 / 394.2 before. The kernel cells' cap was 30 on a fixture whose scores
  stay under 1, where an uncapped kernel passed; they cap at 0.05 now.

- **MEASURED (2026-10-02, `direction-grade`) - a prefill window's K/V rows live in the KV mirror, and a released
  session's mirror goes to the next session on the same pages**
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror`). M5 Max, Metal, `dasllama-server`
  through `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token replies, 5 reps, a rest
  before each model, the same text on both servers). Before: every continuation gathered the
  cached rows on the host into f32 panels (11 ms at 3k of context, 33 ms at 9k on Llama-3.2-3B),
  and the first decode step uploaded them again into the f16 mirror. After: a warm turn's window
  finds the previous turn's mirror under its attached pages and copies nothing.
  - Warm TTFT at 3k / 9k of context, ms, before -> after (reference):
    Llama-3.2-3B Q8_0 42.8 / 77.6 -> 29.4 / 34.9 (pinned llama-server 38.2 / 51.5, the 9k row
    void at cv 3.9%); Llama-3.2-1B Q8_0 19.9 / 33 -> 16.2 / 19.8 (llama-server 24.2 / 24.3, both
    void); gpt-oss-20b mxfp4 58.5 / 82.3 -> 51.6 / 59.1 (llama-server 85.7 / 103.9, the earlier
    rested run - today's reference life answered 503 at its calibration turn);
    Qwen3.6-35B-A3B UD-Q4_K_M with self-speculation 65 / 79 -> 51.8 / 56.2 (mlx_lm.server on the
    4-bit MLX quant 72.4 / 87.5).
  - The 3B's warm window at 3k: host setup 11.4 -> 0.07 ms, GPU 25.9 -> 23.1 ms for the 52
    uncached rows; a cold window's K/V readback 20 -> 6 ms (halves copied, no host convert); the
    two-row window that closes a turn 30-45 -> 17 ms.
  - Cold TTFT and decode read inside their run-to-run spread on every model (the cold rows void
    on both servers at these reps).
  - The form serves under the `attn_dev` crown on f16 KV only (`followup_metal.md` row 36).
    `MetalCvtHalf` and `MetalKvTwin` are `MetalHalfRows` now: one half converter, scaled, at a
    byte offset.

- **MEASURED (2026-10-02, `direction-grade`) - a weight plane cached before the residency set existed was never
  pinned, and a prefill after a decode turn repaid its wiring.** M5 Max, Metal, Llama-3.2-3B
  Q8_0, `dasllama-server` through `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token
  replies, 5 reps). The image's baked dev-W plane (5.4 GB, a page wrap of the mapped file) is
  seeded ahead of the first `residency_flush`, and `residency_note` was a no-op until that flush
  made the set. A decode turn longer than about a second un-wires the plane; the first command
  buffer of the next prefill then spends 20 to 24 ms in the kernel driver span (kernelStart ->
  kernelEnd), its GPU time unchanged. A 100-token reply (0.76 s) does not show it, a 150-token one
  (1.15 s) does; `DASLLAMA_METAL_RESIDENCY=0` reads the same, the plane being outside the set
  either way.
  - The first note makes the set. Warm TTFT at 3k / 9k of context: 61.7 / 100.0 ms before,
    42.8 / 77.6 after (the pinned llama-server, rested: 37.2 / 52.6). A cold prefill that follows
    a decode turn loses the same span.
  - What is left of a warm turn at 3k: 11 ms gathering the cached history into the f32 K/V panels
    on the CPU (33 ms at 9k - it scales with the cached rows), 26 ms of GPU for the 52 uncached
    rows, 4 ms of HTTP. The gather is the next cost: the decode mirror holds the same rows in
    f16, and a continuation that read its history there would copy nothing.

- **MEASURED (2026-10-02, `direction-grade`) - the single row's decode takes the fused single-pass attention past the
  single-dispatch ceiling.** M5 Max, Metal, Llama-3.2-1B Q8_0 (16 layers, 32 heads of 64 over 8 KV
  heads), f16 KV, `bin/daslang -jit modules/dasLLAMA/benchmarks/decode_step_trace.das -- -m <gguf>
  -p <depth> -n 256 --bucket 256`, one process a depth, greedy chain adaptive; debug-jit readings,
  not board rows. The step's wall is its GPU time at every depth.
  - ms a step at depth 0 / 3000 / 9000: 2.90 / 3.61 / 5.41 on the chunked pair (the single row's
    form before), 2.77 / 3.02 / 3.50 on the fused form - 344 / 275 / 182 tok/s against
    361 / 329 / 280. The chunked pair costs about 0.28 us a cached key a token, the fused form
    about 0.08.
  - The chunked pair's depth cost does not follow the bytes it reads: at depth 9000 it adds 2.51 ms
    on the f16 mirror, 3.06 on f32, 2.80 on q8_0 and 2.54 on tq4.
  - At this entry's reading the chunked pair still served a head of 64 or 256 on a block-codec
    mirror and every model with a soft cap or a second head class; the entries above it moved each
    of those to the fused form, and the pair stays as its `DASLLAMA_METAL_ATTN_D=0` control.
  - The fused form carries a sliding layer's window and a layer's attention sinks. gpt-oss-20b
    mxfp4 (a head of 64, sinks, 128-key sliding layers on alternate layers), `decode_step_trace`
    as above with `DASLLAMA_METAL_ATTN_D` 0 then 1: 134.8 tok/s at depth 3000 and 101.5 at 9000
    on the chunked pair, 154.6 and 140.2 on the fused form. Through `dasllama-server`
    (`harness/served_bench.das`, tagged `served`, `out-of-process`, `direction-grade`, a rested box, the same text on both servers):
    decode 132.1 / 99.9 tok/s at 3k / 9k of context before, 150.5 / 135.4 after; the pinned
    reference server reads 143.0 / 133.1. A probe reading, not a parity cell: against the CPU
    chain over 400 forced steps (`harness/forced_feed_probe.das <gguf> --steps 400`) the fused
    form's largest logit difference is 3.48 and its mean 1.02 with no argmax flip, the chunked
    pair's 3.45 and 1.01; the gates are arm11 of the decode suite and the matrix's two-class cell.
  - The speculative verify's rows and a head of 256 (the WIDE stamps) take the fused form too.
    Qwen3.6-35B-A3B UD-Q4_K_M under self-speculation, one stream through `dasllama-server`
    (`harness/served_bench.das --sizes 3000,9000 --reply 200 --reps 5`, tagged `served`, `out-of-process`, `direction-grade`, both
    decode rows past the 3% cv bar - the accept rate moves with the reply): 138 tok/s at 3k of
    context and 115 at 9k on the chunked pair, 147 and 133 on the fused form; `mlx_lm.server`
    on its 4-bit build reads 116 and 112 in the same series.
  - The four narrow stamps' source moved by the shared rescale helper and two regrouped
    expressions, the arithmetic unchanged; the batch row that rides them
    (`lcpp_bench --ngl 99 --npl 2 --npl-plen 512|9000 -r 3 --for-debug-purposes`, Llama-3.2-1B
    Q8_0) reads 540.9 / 396.6 tok/s summed before, 535.0 / 394.9 after the rescale helper, and
    533.3 / 394.2 with the window start and the masked key in the slice loop.

- **MEASURED (2026-10-01) - the attention and router races on the stamps that ship, and the bytes
  the folded snapshot holds.** M5 Max, `bin/daslang -jit modules/dasLLAMA/harness/router_race.das`
  and `.../attn_depth_race.das`, one process each, no flags; both harnesses dispatch the engine's
  own stamps through their generated builders, where they timed hand-written copies before.
  - `router_race` (2048 positions x 256 experts over 2048, best of its rounds): the slab GEMV
    (`MetalMoeRouterB`) 0.647 ms, off the double dot by 8.1e-7 of 4.24; the half tensor GEMM
    (`MetalHalfMulMmTH128`) 0.0485 ms, x13.3, off by 1.39e-3.
  - `attn_depth_race` (2048 queries at 8192 rows of context, 16 heads of 256 over 2 KV heads,
    cumulative stages): QK 6.25 ms staged against 3.02 device; + softmax 7.85 against 5.37; + AV
    19.84 against 8.06; + the K/V twin pass 19.85 against 8.12 - x2.44. Against the attention
    computed in double from the f32 operands the staged trio is off by 7.4e-7 and the device pair
    by 7.0e-7 of 8.2e-4. The lab copies read 23.1 against 8.2 ms on the same shape (the device
    attention pair's entry below, direction-grade: another commit).
  - A speculative session's rollback snapshot (`Session.mtp_snap`) holds the conv history, the
    recurrent state and the n-gram ring: `(long_length(dn_conv_state) + long_length(dn_state) +
    ple_hist_rows x hc_dim) x 4` bytes a session, sized on the first CPU-rail snapshot - 151 MB of
    state at a 27B hybrid's geometry (48 recurrent layers x 32 heads x 128 x 192), the ring
    `((ple_conv_kernel - 1) x ple_ngram_size + 1) x hc_count x dim` floats on a hyper-connection
    carrier and nothing elsewhere. The conv and state buffers were the session's before; the ring
    is the addition.
- **MEASURED (2026-10-01) - a plain MoE the card does not hold whole, with its context asked whole
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-hot-pool`, `set_gpu_ctx_strict` / the server's `--ctx`): the first routed
  layers' experts on the host through the split token command and the cut window chain, the mirror whole
  - the form wins the long prompt by 5.7x and loses decode by a third to the per-op rails
  (direction-grade), so it arms only where the context was asked.** zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link, the
  desktop holding 4.6 GB of the card), Qwen3-30B-A3B-Instruct-2507 Q4_K_M (17.3 GB, 48 routed layers of
  128 experts, 8 picked; the plan puts the first 28 layers' experts on the host beside a 32768-position
  f16 mirror, no pool fits), `bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das --
  -m <file> --ngl 0 -p <P> -n <N> -r 1 -t 16 --for-debug-purposes` under `DASLLAMA_ALLOW_UNTUNED=1
  DASLLAMA_GPU=1 DAS_JOBQUE_THREADS=16` (`DAS_TUNE_POLICY`, `DASLLAMA_COOPMAT` unset; the second row adds
  `DASLLAMA_GPU_CTX_MAX=32768 DASLLAMA_GPU_CTX_STRICT=1`, the pin the form arms under, the first
  `DASLLAMA_GPU_RESIDENT=0`), one rep, one process a row - a second load of a model in one process is barred,
  so the two forms cannot race interleaved - direction-grade across the rows and in every sentence below
  that sets one row against the other:

  | form | pp512 | tg128 | pp8192 | tg64 after 8192 |
  | :--- | ---: | ---: | ---: | ---: |
  | the per-op rails (`DASLLAMA_GPU_RESIDENT=0`: master's shape for this file) | 398.14 | 64.03 | 93.39 | 64.52 |
  | the whole-model driver, 28 of 48 layers' experts on the host, the mirror on device | 517.88 (290.36 with the first window's one-time cost inside the pass) | 41.81 | 533.97 | 47.79 |

  The per-op rails' decode does not slow with the position (64.0 at 640, 64.5 at 8192, direction-grade); the
  host-layer form's token is 24 ms against 15.6 (direction-grade) - the 28 fence round-trips a token, the cost
  the hyper-connection carrier pays too - so it keeps the per-op rails' place for an unasked context and
  takes the asked one, where an 8192-token prompt costs 15 s against 88 s (direction-grade). Its prefill is
  flat with the length: a 512-row window records in 0.85-0.90 s and submits in 0.07-0.09 s whatever the
  position (the same command and environment as the second row with `DASLLAMA_GPU_PROF=1` added, its
  `vk_rdpf` lines; pp2048 reads 551.5 there), and the first window of a process pays 0.45 s once (the host
  callback's scratch, the host combine's pipelines) - a pp512 pass that includes it reads 290. The form's
  landings are small beside the planes: the decode's `hx_host`, `hacc_host` and `hacc_dev` hold `dim*4*nb`
  bytes each and `hlog_host` `ner*4*nb` (2048 x 4 x 1 = 8 KB and 128 x 4 = 512 B here), the prefill's
  `pf_hx_host`, `pf_hacc_host` and `pf_hacc_dev` `np*dim*4` each and `pf_hlog_host` `np*ner*4` (4 MB and
  256 KB at the 512-row window), `pf_ident_dev` and `pf_ones_dev` `np*4` (2 KB), the decode's `ones_dev`
  `MAX_ROUTED_SLOTS*4`; the pool over the host layers takes one slot's gate, up and down planes per hosted
  layer per slot (`resident_hot_slot_bytes_host_layers`), none here. The same file on this card at the bench's shape
  before this change: the whole-model driver declined (17.3 GB of weights past the room) and the per-op
  rails served, which is the first row. Parity: `test_gpu_resident_moe.das`'s `test_gpu_resident_moe_host_layers`
  (this file at 32768 asked whole, 27-28 layers hosted, one and two windows) and
  `test_gpu_resident_moe_ctx_strict` (Qwen3.6-35B-A3B UD-IQ2_XXS at 131072 asked whole: 10 of 40 layers
  hosted, the region 131072 positions, two windows) pass on this box, run as `bin/Release/daslang.exe -jit
  dastest/dastest.das -- --test modules/dasLLAMA/tests/test_gpu_resident_moe.das` under `DASLLAMA_GPU=1
  DASLLAMA_PARITY_FULL=1 DAS_JOBQUE_THREADS=8`; the file's one-window hybrid and no-shared-expert cells
  red on the cm2e census floor on this card (117 of 120, 134 of 144) exactly as on master run through
  this binary - a miss of the witness's own format list, which lacked `iq3s4`, the form this box serves
  IQ3_S planes in - unchanged by this entry.
- **MEASURED (2026-10-01) - the server's ASR worker on the team beside a GPU-decoding LLM: a
  64-second parakeet clip 281 -> 140 ms a transcription.** M5 Max, `dasllama-server` with the
  Qwen3.6-35B-A3B on Metal (streams 1) and parakeet-tdt-0.6b-v3 f32, `POST
  /v1/audio/transcriptions` seven times, medians, wall time at the client (out-of-process;
  direction-grade across the rows: a server life a dispatch mode).

  | ASR worker | LLM idle | while the LLM decodes 400 tokens | that decode, tok/s |
  |---|---|---|---|
  | inline (`hybrid` before) | 281 ms | 390 ms | 136 |
  | team (`--team-dispatch team`, and `hybrid` now) | 140 ms | 178 ms | 138 |

  A Metal-served decode publishes nothing to the team, so the worker's joint, predictor and mel
  split across it at no cost to the stream. mlx-audio 0.5.7 reads 162 ms on the clip in its own
  process (external: `model.generate(clip)` on `mlx-community/parakeet-tdt-0.6b-v3` loaded through
  `mlx_audio.stt.utils.load_model`, the median of five calls after one warmup).
- **MEASURED (2026-10-01) - the FastConformer block loop on the half tensor GEMM: 115 -> 63 ms for
  a 64-second clip.** M5 Max, parakeet-tdt-0.6b-v3 f32, `harness/asr_stage_probe.das -r 5` on a
  64 s clip; every before -> after pair of this entry is the probe on the tree before and after
  the change (direction-grade), save where a sentence says one process. Per encode: 187 -> 133 ms, of which the CPU front stays 66 and the rel table 6. The
  chain's weighted GEMMs ran the f16-staged simdgroup tile stamp (`enc_f32_mm`, ~4.8 T MAC/s over
  the ~570 G MAC of 24 blocks); they read a device halfword twin of the blob through the half x
  half tensor GEMM now. The encoder rows read 0.0042 rel-l2 from the CPU rail on the twin and
  0.0039 on the tiles (jfk), the transcript the CPU's on both - every rel-l2 on jfk in this entry
  is `test_parakeet_tower_metal`'s log (`tests/test_model_image.das`, `tests/run.das --suite image
  --arm mtower`), the 64 s clip's the probe's.
  - What the 63 ms holds, by leaving parts out (best of three, three rounds): the per-head
    attention loop 25 (41 dispatches a block - a pack, three tile GEMMs and a softmax a head), the
    weighted GEMMs 27 (~18 T MAC/s with their half converts), the norms, the depthwise conv and
    the rest ~10.
  - mlx-audio 0.5.7 on the same clip: 162 ms whole (external, the call of the entry above; its
    encoder 64 and its decode loop 97 timed apart in the same process). Ours whole, team
    dispatch: ~220 (front 66, blocks 63, rel table 6, mel 8, decode 80).
  - **The subsample front on the device, the rel table kept a session: the encode 133 -> 66 ms,
    the clip whole ~153.** The front's three convs, two 1x1 GEMMs and the pre-projection run ahead
    of the blocks in the same command buffer (66 ms of CPU, team-dispatched, to under 5), and the
    rel table is built once a frame count (6 ms an encode before). The same probe, per pass:
    encode 66, decode 79 (the predictor 38, the joint 25, the pick 13, the encoder projection 2),
    mel 8. Rows 0.0057 rel-l2 from the CPU rail (jfk), the transcript the CPU's.
  - **The TDT joint as a column pass over its live inputs, the pick in one exp pass: decode 79 ->
    61 ms, the clip whole ~133.** The joint row is a ReLU's output, so the joint net's weight laid
    as input columns adds only the live ones (a transposed copy a session, 21 MB): the joint 25 ->
    16 ms with the logits split across the team's lanes. One thread alone runs it 56 - the weight
    streams from memory either way, and the row-dot it replaces had the team. The token mass is
    the log-sum pass's partial sum: the pick 12.7 -> 6.9. Left: the predictor 31.5 (four 640 x
    2560 GEMVs a token over 26 MB of f32).
  - **Run inline - one thread, as the server's ASR worker does by default - the same clip reads
    ~263 ms:** encode 69, decode 165 (the predictor 86, the joint 45, the encoder projection 23,
    the pick 7), mel 29. It read 790 before the front and the twin.
  - **The attention over every head at once on the device pair: the encode 57.0 -> 44.4 ms.** One
    process, the two forms in turn, seven timed encodes a form after two dropped (per head 56.9 -
    57.1, all heads 44.3 - 44.5), under the `attn_dev` crown. Five dispatches a block replace
    forty-one: the half operands, the pair's QK twice (content, rel), the rel-shift softmax, the
    pair's AV. Rows 0.0056 rel-l2 from the CPU rail on the clip against the per-head form's 0.0059
    (jfk: 0.0042 against 0.0057), the transcript the CPU's. Not taken: the rel GEMM computes the
    whole `mp x wwpad` slab though a row reads `npos` of its `2 npos - 1` columns - a band skip
    halves its 31 G MAC and its slab's bytes.
  - **The decode step as one team publish over half-precision weights: decode 61 -> 33 ms, team
    dispatch (inline 165 -> 146).** `asr_stage_probe -r 5`, the 64 s clip, per pass. By change:
    the predictor's weights read as halves, 31.5 -> 15.7 ms - twice the rate at half the bytes,
    so a token's predictor is bound by the 26 MB it read; the joint's columns as halves, no change
    (13.9) - its lanes read a column table in short runs a column apart; the predictor's stages
    and the joint in one publish, the two together 29.6 -> 24.6; the joint table in 256-logit
    blocks, a token step 88 -> 81 us and a blank step 50 -> 34 (a 512 block reads 107 and 48, a
    128 block 82 and 30, with the inline joint 147 -> 159 us); the log-sum's cut and the max and
    argmax in one pass, the pick 6.8 -> 3.1 ms. Now, per pass: a token step ~83 us x 260, a blank
    step ~40 us x 42, the pick 3.1 ms, the encoder projection 2.6. A bare six-stage publish over
    18 lanes measures 14 us and a one-stage one 5, so what is left is the step's bytes - 7.4 MB
    of predictor halves and ~5 MB of live joint columns a token, about 150 GB/s. The transcripts
    are the reference's token for token (jfk, v2 and v3).
  - **What the routes hold, in bytes.** The device twin of the blob is half the blob: 1.25 GB for
    the 2.51 GB 0.6B file, resident from the first encode to the weights drop. The all-heads score
    slabs are `heads x mp x (nkp + wwpad) x 2` bytes - 33 MB at 64 s of audio and eight heads -
    and the route stands down past 256 MB. A session's decode halves are `(8 layers + 1) x pd^2 x
    2` bytes for the predictor (13.9 MB at two layers of 640) and `blocks x pd x 256 x 2` for the
    joint table (10.8 MB at 8198 logits).
- **MEASURED (2026-10-01) - the prefill chunk on the CPU: a prompt alone runs flat from 512 tokens
  up, and a decoding stream waits one chunk's time a token while another prompt prefills.** M5 Max
  CPU, `utils/dasllama-server/server_bench.das -b 1 -n 32 --long-prompt 2048 --chunks
  64,128,256,512,1024,2048`, one process a model, paged sessions.

  | chunk | 64 | 128 | 256 | 512 | 1024 | 2048 |
  |---|---|---|---|---|---|---|
  | Llama-3.2-3B Q4_K_M, lone prompt, tok/s | 357 | 388 | 401 | 410 | 413 | 402 |
  | its decoding stream's gap, p95 ms | 204 | 378 | 736 | 1425 | 2733 | 5020 |
  | Qwen3.5-0.8B Q8_0, lone prompt, tok/s | 1343 | 1658 | 1832 | 1892 | 1788 | 1991 |
  | its decoding stream's gap, p95 ms | 50 | 92 | 153 | 283 | 547 | 1032 |

  The idle quantum at 512 is right for the CPU - nothing past it. The chunk a decoding stream
  waits out is a token count, so its stall is the model's: 512 tokens is 0.28 s on the 0.8B and
  1.4 s on the 3B (its token gap is 14 ms with nothing prefilling).
  - **The CPU's chunk beside a decoding stream defaults to 64.** The stall follows the chunk
    down (50 ms on the 0.8B, 204 on the 3B, ~460 on Mistral-7B Q4_K_M) and the prefilling prompt
    pays 13 - 29% of its rate, only while a stream decodes; under 32 tokens a batch leaves the
    batched kernels (the 7B's 2048-token prompt read 19 s against 11). A GPU's chunk stays one
    window. Measured and not taken: sizing the chunk to a 250 ms stall from the last chunk's
    rate - on the 3B and the 7B it lands on 64 anyway (311 - 324 and 459 - 462 ms with a
    64-token floor), and on the 0.8B it keeps 512 (268 - 283 ms); `--chunk` is the same knob.
- **MEASURED (2026-10-01) - both engines held to what an M4 can run, on the M5 Max: prefill 1.3 -
  1.4x mlx-lm's, decode level, the warm hit 20 - 30 ms behind.** The 35B through each server
  (ours streams=1 with speculation; the series of the checkpoint entry below, medians of three,
  two spaced lives an arm). Ours under the box's manifest with its `metal_tensor` list replaced by
  the M4 Pro's (`records/m4.tune.2b2e79baf8ab.json`: `mulmm_bf16`, `kq_mvb2_k4_r2` and the
  `kq_rows_*` forms - no routed, K-quant GEMM or attention tensor crown); mlx-lm 0.32.0 over MLX
  0.32.3 built with a deployment target of 15.0, which compiles its Metal-4 kernels out
  (`MLX_METAL_NO_NAX`; no `_nax` kernel in its metallib, 6322 in the stock one). The mlx-lm columns
  are external - `mlx_lm.server --model mlx-community/Qwen3.6-35B-A3B-4bit --chat-template-args
  '{"enable_thinking":false}' --max-tokens 4096 --prompt-cache-size 32 --prompt-cache-bytes
  8589934592`, the same client series - and every column is direction-grade: a server life an arm.

  | | ours, M4 crowns | mlx-lm, no Metal-4 kernels | ours, M5 crowns | mlx-lm, stock |
  |---|---|---|---|---|
  | cold 3k, tok/s (TTFT) | 2773 / 2794 (1.07 s) | 1980 (1.50 s) | 4959 (0.60 s) | 4480 (0.66 s) |
  | cold 9k, tok/s (TTFT) | 2517 / 2534 (3.6 s) | 1892 (4.82 s) | 4603 (1.98 s) | 4217 (2.16 s) |
  | warm TTFT, 3k / 9k | 0.09 / 0.10 s | 0.06 / 0.08 s | 0.05 / 0.08 s | 0.06 - 0.08 s |
  | decode, 3k / 9k, tok/s | 121 / 103 | 113 - 120 / 106 - 111 | 139 / 114 | 110 / 105 |

  - The warm hit's short window takes the in-kernel gather K4 form without the routed tensor
    crown: 64 ms of GPU for 20 tokens against 43 on the M5's crowns.
  - The numbers are the M5 Max's GPU on the M4's forms, not an M4's: the ratio between the engines
    is what reads across, and the M4's own race predates the device attention pair and the half
    scan.
  - **A life run inside 14 minutes of back-to-back GPU load reads 1.5 - 1.8x slow with no thermal
    warning from the OS.** Ours: cold 3k 1568 tok/s, decode 91 / 85, a 2945-token window 950 ->
    1740 ms of GPU; five minutes idle and four between lives put it back (2773, 2794). mlx-lm's
    lives in the same heat read 1705 and 1524 against 1980 cool. A series is read from spaced lives.
- **MEASURED (2026-10-01) - pointer loads in every fully unrolled Metal kernel loop: 3% of a dense
  K-quant window, nothing on the 35B's served path.** 88 loads over the K-quant, IQ and Q4_0 GEMV
  and staging kernels, the legacy routed stamps, the deltanet norm and gate and the staged
  attention moved from `buf[base + k]` to a pointer taken outside the loop
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing`). M5 Max, Mistral-7B Q4_K_M,
  `p0_ko_probe -n 512 --kprof --text skills/design_philosophy.md`, the indexed tree then the
  pointer tree (direction-grade): the dense K4 stage 69.7 -> 66.6 ms and 19.8 -> 19.3, K6 20.3 -> 20.4, the window
  as it serves 149.7 -> 145.4 ms. The 35B through the server reads as before the sweep (cold 3k
  0.62 s, 9k 2.02 s, decode 139 / 114 tok/s) - its expert stamps and its scan took the form
  already. Decode on the dense model is not read: `lcpp_bench -n 128 -r 12` as a `-jit` script
  spreads 20% a row (78 - 86 +- 13 - 18 tok/s), past any difference the loads could make.
- **MEASURED (2026-10-01) - a reused prompt on a deltanet hybrid attaches a checkpoint; warm time
  to first token 0.6 s -> 0.05 s at 3k, 2.1 s -> 0.08 s at 9k.** M5 Max, the 35B, the server at
  streams=1 with speculation, a client script outside the tree (`bench_llm.py <server url> <model>
  <tag>`: a fixed system prompt cut from `skills/design_philosophy.md`, a primed request, three
  more with other user turns over `/v1/chat/completions`; client side, medians of three;
  out-of-process, and direction-grade against the tree before the change): warm 3k 0.05 s (server
  side 48 / 48 / 60 ms), warm 9k 0.08 s (57 / 58 / 71 ms), every request after the prime a hit;
  mlx-lm 0.32.0's server on the same series reads 0.06 - 0.08 s (external, the `mlx_lm.server`
  command of the entry above). Cold is where it was: 3k 0.60 s (4959 tok/s), 9k 1.98 s
  (4603), decode 139 / 114 tok/s. A hit's time is one short window: 17 - 21 tokens read 43 ms at no
  depth and 45 ms of GPU at 9k, plus 6 - 10 ms of setup.
  - **The short window was 59 ms: the routed gate and up took the in-kernel gather form below 256
    tokens.** `p0_ko_probe -n 17 --kprof --depth 9108`: `metal_moe_mulmm_k4` 38.8 of 74 ms. The
    gathered half panel at every window (`--gather-min 256,1`, three interleaved rounds each):
    17 tokens 58.9 -> 42.7 ms, 64 80.3 -> 56.0, 128 95.4 -> 65.8, 224 119.2 -> 83.5; the logits
    move 0.16 - 0.67 of a largest 20 - 25, the half twin against the integer form.
  - **What is left in a short window is the weight stream.** 17 tokens pick 136 expert rows over
    77 experts, each a 32-row tile: the K4 pair 17.2 ms and the K5 down 7.0 at 0.2 ms a dispatch,
    which is 590 KB an expert plane at ~137 GB/s - a decode step's 7.3 ms plus 4.3 us an expert
    plane past its eight. A shorter tile does not buy it back (an 8-row tile costs 0.62 of a
    32-row one).
  - **Stopping a prefill at the checkpoint costs one window, so the chunk ends there.** With the
    chunk left at its quantum the cold 3k prompt ran 2048 + 897 + 20 tokens, 0.65 s (4556 tok/s);
    a checkpoint within a chunk and a half ending the chunk runs 2945 + 20, 0.60 s (4959).
  - Open: the seam row of the draft head declines at the first window of every attached prompt
    (a quality-only miss, 26 - 34 a series, as before the cache); the close tokens of a finished
    turn eval as a 2 - 3 token window, 20 - 35 ms after the reply's last token.
- **MEASURED (2026-10-01) - the job queue's workers spinning the CPU window through Metal work
  are the box's "slow mode".** M5 Max, the 35B, `p0_ko_probe -n 2048 --depth 10240 --text
  skills/design_philosophy.md --hold none`, four walks of five windows back to back. Under the
  30 ms CPU spin window (`--cpu-spin`) the process holds 1086-1296% CPU while the GPU does the
  work (a sample: the workers in the spin loop's clock read) and the walks read 366 / 388 / 417 /
  421 / 475, then 361 / 379 / 400 / 417 / 439, then 363 / 389 / 426 / 466 / 512, then 430 / 458 /
  481 / 504 / 521 ms - 19% slower from the third walk on. Under the GPU-served window (500 us;
  13-16% CPU) the four walks read 365 / 388 / 415 / 421 / 468, 361 / 380 / 400 / 417 / 440, 359 /
  379 / 399 / 418 / 438, 362 / 382 / 409 / 429 / 452. The Metal overrides now report a served
  forward (`metal_served`). The server, cold prompts, one stream, MTP on, before -> after
  (direction-grade, the client series of the checkpoint entry above): 3k 4820 -> 4959 tok/s, 9k
  4224 -> 4574; decode 138 / 111 -> 137 / 113. mlx-lm 0.32.0's server on the same series: 4480 /
  4217, decode 110 / 105 (external). A later request of a long series still creeps
  (9k windows 431 / 460 / 482 / 506 against 362 / 380 / 400 / 418 in the probe) with the CPU idle:
  that part is the GPU's own.
  - **The largest gap this entry left on the reused-prompt case:** a deltanet hybrid attached
    no prefix, so a repeated 9k system prompt cost its whole prefill again (TTFT 2.1 s) where
    mlx-lm restores a checkpoint (0.06-0.08 s) - closed by the checkpoint entry above.

- **MEASURED (2026-10-01) - a Metal prefill window by kernel, on both engines, and the three
  forms the reading bought.** M5 Max, Qwen3.6-35B-A3B-MTP UD-Q4_K_M, the device attention pair
  crowned. `p0_ko_probe --kprof` replays every captured dispatch in a command buffer of its own
  (`metal_kernel_profile`), so a kernel's GPU time reads on the rows the window really computes:
  the kernels sum to 436 ms where the window serves in 428, so nothing hides in overlap. mlx-lm
  0.32.0 the same way (external: its model's `__call__` of every module class wrapped to force an
  evaluation, a script outside the tree; 536 ms against 363 as it serves, about 0.15 ms of sync a
  call, so its big modules read true and its small ones high). A 2048-token
  window at depth 0, ours ms (mlx-lm instrumented, external): the recurrent scan 70 (41); the expert GEMMs
  146 - K4 96, K5 47, K6 3 (gate 44 + up 53 + down 43); the router 27 (9); the dense half GEMMs
  95 (in_proj_qkv 41, z 25, out 25, shared expert 31, attention projections ~38); reduce 12,
  activation 11, gather 8, count 7, bucket 5; the attention trio 11.
  - **The router as a half GEMM** (`pf_enc_router_mm`; `harness/router_race.das`, 2048 x 256 over
    2048): the slab GEMV 0.647 ms, the tensor GEMM over halves 0.048 - x13.4, off the double dot
    by 1.4e-3 of 4.2 against the GEMV's 8e-7. The window 428 -> 402 ms.
  - **The scan** (`harness/dn_scan_race.das`, a layer at 2048 tokens, thirty dispatches a command
    buffer). mlx-lm's scan kernel raced beside ours on its own layout (external; an arm the harness
    does not keep): 1.05 ms on half rows, 1.81 on f32. Ours as it stood:
    2.25 on f32 rows with the gates in the walk, 1.77 on half rows with the gates prepared - the
    same algorithm, 1.7x slower. The whole difference was addressing: `cv[cb + koff + i]` under a
    uint index against a pointer and a constant offset
    (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing`). With the pointer form: 1.48 on
    f32 rows, 0.97 on half rows - under mlx-lm's. Two simdgroups a threadgroup read 10% under
    four. Preparing the gates alone bought 9% (70 -> 64 ms a window); half rows without the
    pointer form bought nothing net. The window 402 -> 368 ms; the scan 70 -> 29.
  - **The K-quant expert stage on pointer loads**: K4 95.5 -> 88.4 ms, K5 46.7 -> 40.8; the window
    368 -> 356.
  - **The standing** (`--depth 10240 --text skills/design_philosophy.md`, the windows in turn):
    362 / 380 / 400 / 418 / 445 ms; mlx-lm 0.32.0 on its synthetic ids 365 / 377 / 396 / 412 / 431
    (external, direction-grade).
- **MEASURED (2026-10-01) - what a routed tile costs, for the tile ladder.** One process each,
  hand MSL over a 2048 x 2048 x 2048 half GEMM, device operands (scratch races, not kept):
  - **A tile's live rows do not make it cheaper.** The A tensor's row extent set under the
    descriptor's m: a 128-row tile reads flat from 128 live rows to 1 (0.280 -> 0.274 ms); a
    32-row tile reads 0.330 at 32, 0.636 at 24, 0.414 at 16, 0.287 at 8 - a partial extent takes
    an edge path that is slower, not faster. Padding is not avoided by telling the op the count.
  - **A tile's cost is mostly fixed.** The whole GEMM by tile height: 8 rows 0.825 ms, 16 0.461,
    32 0.332, 64 0.276, 128 0.281 - a tile of 16 costs 0.69 of a tile of 32, 8 costs 0.62, 64
    costs 1.66, 128 costs 3.38. Fewer, taller tiles win; a remainder of 33..64 rows is one 64-row
    tile at 1.66 against two 32-row tiles at 2.0, each also staging the plane again.
  - **An idle threadgroup costs 0.68 ns** (the 128-row expert stamp over a grid no expert fills:
    32768 threadgroups 0.022 ms, 131072 0.086). A window's idle launches total about 6 ms, most
    of it the K5 stamps' 32-strip grid.
  - **The synthetic probe ids route a MoE onto half its experts** (last routed layer: 127 of 256
    idle, the busiest 1729 rows, 78% of the rows in 128-row tiles); a real prompt leaves 51 idle,
    59% in 128-row tiles, and 6656 rows in 335 tiles of 32 padded to 10720. The 64-row rung read
    level on the synthetic ids for that reason; it is owed a reading on text (`--text`).
  - From the two expert stamps' times and those counts: the multiply runs 22.7 T multiply-adds a
    second staged (30.7 device-direct at 128 rows), staging is 7% of a 128-row stamp and 23% of
    the remainder stamp, and the remainder stamp's pad rows are 29% of it.

- **MEASURED (2026-10-01) - a Metal prefill window's cost against the context behind it: the
  attention is the whole growth, and the device pair
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device`) takes 2.8x off it.** M5 Max,
  Qwen3.6-35B-A3B-MTP UD-Q4_K_M (10 attention layers of 40, 16 heads of 256 over 2 KV heads), the
  box's own mint, `bin/daslang -jit modules/dasLLAMA/harness/p0_ko_probe.das -- -m <gguf> -n 2048
  --depth 10240`: one session's 2048-token windows in turn, the staged trio and the device pair
  alternating in one process. Staged, by the rows of context behind the window (0 / 2048 / 4096 /
  6144 / 8192): 452 / 493 / 563 / 599 / 694 ms. The device pair: 427 / 440 / 466 / 483 / 521 -
  the five windows 2800 -> 2318 ms, the deepest x1.33. mlx-lm 0.32.0 on the same box
  (external, direction-grade: `mlx-community/Qwen3.6-35B-A3B-4bit`, its model called a window at a
  time over one cache by a script outside the tree): 365 / 377 / 396 / 412 / 431. The kernels alone, one layer at 2048 queries on 8192 rows of context
  (`harness/attn_depth_race.das`, GPU time, best of 3): staged 23.1 ms - QK 9.5, row stats 1.6, AV
  12.0 - against 8.2 - the K/V twin pass 0.1, QK 3.0, the softmax write 2.3, AV 2.7; both within
  7e-7 to 9e-4 of the attention computed in double from the f32 operands, the pair the closer of
  the two at every score spread tried. The time does not move with the scores. The tune race
  (`race_attn_dev`, 512 queries on 1536 rows, 8 heads of 128): 0.344 -> 0.191 ms.
  - **The staged tensor AV twin gains nothing at depth** (`attn_avmm` force-crowned through a
    manifest copy: 446 / 490 / 558 / 590 / 682): threadgroup staging is the cost, not the multiply.
  - **Idea, not built: the softmax write reads the row twice.** A thread holds 80 scores of a
    10240-key row; kept in registers between the max and the exp they save one of three slab
    traversals.
  - **Idea, not built: a continuation window gathers every prior K/V row from the session's host
    cache into f32 panels** (`kv_load_row`, 335 MB at 8192 rows of context). The device pair reads
    halves, so an f16 session's rows could land as halves directly.
- **MEASURED (2026-10-01) - the knockout probe's shares are not additive on a routed model.**
  Knocking a family out upstream of a router feeds the router stale rows, the picks collapse onto
  few experts, and the expert GEMMs' padded tiles shrink: `act_cvt` read 28-45 ms of a 150 ms
  window on the 35B, and removing two of its three convert passes a layer outright moved the window
  151 -> 149 ms (512 tokens) and 446 -> 443 (2048), the logits bit for bit. `--census` counts the
  dispatches instead (`metal_cvt_half` 120 -> 40 a window). On a routed model a family's cost is
  read by replacing it with a form that keeps the rows valid, as `--depth` does for the attention.
- **MEASURED (2026-10-01) - the gated delta scan packed eight state columns a simdgroup**
  (`MetalDnScan`): the scan x1.5 from 64 tokens up at the 35B's geometry, level at one row; the
  window 160 -> 150 ms at 512 tokens, 526 -> 475 at 2048. **The routed tall pair from a mean of 32
  rows an expert** (`set_metal_moe_tall_avg`; `p0_ko_probe --tall-avg 32,64,128`, three interleaved
  rounds, logits bit for bit): the 2048 window 474 -> 449 ms; a 64-row rung between the pair's
  stamps read level (+2% at 512) and is not carried.

- **MEASURED (2026-10-01) - the NextN head on the hyper-connection chain
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`, `#hc-verify-rows`): the round loses on this form while
  the pool's hits sit near 40% on real text, and the real-text token is 45 ms against the synthetic
  row's 34 - the hits, not the head, are the lever, and the routed pick trace with its offline pool
  sim (`DASLLAMA_MOE_TRACE`, `harness/hot_pool_sim.das`) says where it goes.** zen2 (Ryzen 16 threads,
  RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link, 3.9-4.3 GB held by the desktop), Qwen3.8-Flash-Next
  UD-IQ4_XS with its split head (`mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf`, `DASLLAMA_MTP_HEAD`),
  `bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <shard 1> --mtp-ab
  --mtp-depth 1 -p 0 -r 1 -t 16 --for-debug-purposes` under `DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`,
  eight real-text prompts of 128 tokens, one rep a prompt, one process a row. Acceptance on the CPU
  rail (no `DASLLAMA_GPU`; the rail does not move it): 456 of 571 drafts accepted, 79.9% (per prompt
  64.1-95.5%) - the same 456 of 571 under `--mtp-depth 2` and `3`, since the CPU round is one token
  deep and the flag reaches the seated rounds alone - tg-real128 off 7.07 / on 7.08 tok/s (per prompt
  off 7.0-7.1, on 5.8-8.6; 7.15 / 7.15 at depth 3), the CPU round flat. On the device (`DASLLAMA_GPU=1`,
  the head resident, the pool at the plan's 32 slots serving the verify rows through the rows chain):
  tg-real128 off 22.49 -> on 18.77 tok/s, x0.83 at 407 of 619 accepted, 65.8% (per prompt off 20.1-24.8,
  on 16.0-21.7, x0.74-x0.94 at 47.1-77.8%); a second process 22.85 -> 18.99 at the same 65.8%
  (direction-grade across the two); tg-real64 22.57 -> 18.57 and 22.61 -> 18.73 at 62.7%. The device
  accepts 14 points under the CPU (65.8 against 79.9, the two rails' processes, direction-grade): the
  device draft and the verify rows route their near-ties apart, as every arm pair on this model does
  (`ARCHITECTURE_ENGINE.md#moe-pick-tape`). The round under
  `DASLLAMA_GPU_PROF=1` (debug-jit): the draft 4 ms, the verify 81 ms of which the device 25 and the
  host's sums of the two rows' misses 56, the pool serving 48-53% of the rows' picks; a plain real-text
  token 45 ms = device 22 + the host's sums 22.7 at hits 31-45% a layer (the per-layer line of the
  profile: hits, picks and swaps a routed layer) - the entry below's synthetic ids read 70-83% hits and
  29.7 tok/s, a third over the 22.5 of real text (two runs, direction-grade). So a round costs 85 ms for
  1.66 tokens (51 ms a token) against a plain token's 45. The trace (`DASLLAMA_MOE_TRACE=<file>` over the
  same command: every host expert step's layer, rows, k ids and weights, a mark a prompt; 24.7 MB for
  the eight prompts' two arms, 1084090 decode picks and 1927680 window rows) replayed through
  `bin/Release/daslang.exe -jit modules/dasLLAMA/harness/hot_pool_sim.das -- <trace> --policy heat
  --per-layer` reproduces the live run: 37.5% of the decode picks on the device against the profile's
  31-45%, 2.38 placements a token, a token modeled at 45.7 ms (the device 22.0 + the misses 21.7 + the
  placements 2.0, from the profile's costs: a miss 72 us, a placement 570 us of host gather + 270 us of
  upload) against the 45 measured, and `--per-layer` matches the profile's line layer for layer - the
  instrument is calibrated; the policies it ranks are `followup_vulkan.md` row 127's table, which owns
  the lever. The round rides the same lift - its verify rows at 85% hits cost a token's misses, not
  two - so `mtp_auto_arms` leaves it off where the resident driver sums the experts on the host
  (`gpu_resident_experts_host`), and a slot's `mtp=1` arms it by hand. The new device planes the rows
  form adds, at this model's shape (dim 2560, hc 4, the verify's nb 2): the wide rows, the carry's
  source, the rows' carry source, the normed streams and the gated rows 80 KB each, the head's cat
  planes 160 KB each (two), the embed rows 20 KB, the side panel 80 KB, the rest under 40 KB - 1.4 MB
  a driver; the prompt's head warm at the 512-row window: the head's cat plane 42 MB (its f16 twin 21,
  its Q8 image 10.5 + 1.3), the wide-residual source and the stream-major scratch 21 MB each - 117 MB
  a driver. Parity (the CPU chain is the reference; the split command's own one-row steps are a
  rounding control, not parity): `test_gpu_resident_hc.das`'s draft cell (the device draft against the
  CPU `forward_mtp` on one token, wide carry and row, `LOGIT_BAR_REL` 0.20 and `CARRY_BAR_REL` 0.05 of
  the row's max: argmax 271 on both at 9.57 / 9.50, the logits row 0.22 within 1.91, the wide carry
  0.06 within 1.10, the controls 3.00 and 3.50, the head's one select replayed) and its verify cell
  (one round: the draft 271, verify row 0's argmax 12 rejecting it, one rollback; the two rows against
  the CPU's one-row steps at `WIDE_BAR_REL` 0.35 and `WIDE_CARRY_BAR_REL` 0.25, the other row as each
  row's control - rows 1.52 and 3.10 within 3.47 and 4.24, the carry rows 0.49 and 0.49 within 1.74 and
  1.02, the controls 12.53 and 12.06, 4.86 and 4.81 - and the rounding control against the split
  command's own one-row steps on the same picks at `SPLIT_BAR_REL` 0.12 and `SPLIT_CARRY_BAR_REL` 0.10 -
  rows 0.42 and 0.82 within 1.14 and 1.52, the carry rows 0.07 and 0.30 within 0.73 and 0.39, the
  controls 11.62 and 11.74, 5.10 and 5.12; 97 selects replayed, 2 of the CPU draft's own picks off the
  device's); kernel
  cell `test_vkd_ple_side`'s commit arm (the position off the token record, three rows committed to the ring).

- **LANDED (2026-09-29) - the hot expert pool on the hyper-connection carrier
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-hot-pool`), the `iq3s4` device format under it, and the resident
  plan sizing the pool with the mirror.** zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB, 3.9-4.3 GB held
  by the desktop, NVIDIA driver 616.56), Qwen3.8-Flash-Next UD-IQ4_XS, `bin/Release/daslang.exe -jit
  benchmarks/lcpp_bench.das -- --ngl 0 -p 512 -n 128 -r 6 -t 16 --for-debug-purposes` under
  `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1` (`DAS_TUNE_POLICY` unset - the box's
  sidecar; `DASLLAMA_COOPMAT` at its default, cm2 on this card; `DASLLAMA_IQ3S_SERVE=auto`, the `iq3s4`
  form), one process a row, the rows of one day on one branch - direction-grade across the rows:

  | pool | the window's hits | mirror | pp512 | tg128 |
  | :--- | :--- | :--- | ---: | ---: |
  | the plan's own: 32 slots (4025 MB) | on the device | 114518 positions (2684 MB) | 286.01 +/- 8.85 | 29.74 +/- 0.50 |
  | 32 slots (`DASLLAMA_GPU_HEAT=32`) | on the device | 32768 (`DASLLAMA_GPU_CTX_MAX`, 768 MB) | 288.81 +/- 8.72 | 29.93 +/- 0.49 |
  | the plan's own: 32 slots | the host's | 64413 (1509 MB) | 121.49 +/- 4.08 | 29.70 +/- 0.44 |
  | 32 slots | the host's | 32768 (768 MB) | 128.39 +/- 0.72 | 30.04 +/- 0.41 |
  | none (`DASLLAMA_GPU_HEAT=0`) | - | 32768 (768 MB) | 128.76 +/- 0.36 | 19.38 +/- 0.06 |
  | none, the plan before this entry | - | 203419 (4767 MB) | 110.33 +/- 2.08 | 2.93 +/- 0.03 |

  The reference rows (external): llama.cpp b10659 (`6c84c7d5d`, the qwen4exp commit), the Vulkan and
  the CUDA 13.4 builds' `llama-bench -m <shard 1> -ngl 99 -ncmoe 48 -t 16 -p 512 -n 128 -r 3` with the
  48 layers' experts on the CPU - Vulkan pp512 91.66 / tg128 13.07, CUDA 147.3 +/- 21 / 17.32
  (`ARCHITECTURE_MEASUREMENT.md#host-experts-reference`). Decode is 1.55x the host-experts form (30.04
  against 19.38) and 1.71x the CUDA row (29.74 against 17.32) - on the bench's synthetic ids, whose picks
  the pool holds at 70-83%; real text reads 22.5 tok/s at 31-45% hits (the NextN entry above); the prompt's row is 2.24x the host-experts
  form (288.81 against 128.76) and 1.96x the CUDA row (288.81 against 147.3) once the window's hits run
  through the batch arm's chain beside the host (the middle rows: the pool serving decode alone leaves
  the prompt's row where it was). Over four windows and 32 steps the pool placed 1656 experts - the 1536 of its fill and
  120 swaps - for 0.9 s of gather and 0.45 s of upload, once a load. The bottom row is the box paging: the plan filled the card to 681 MB under its
  room beside a 4.3 GB desktop, the driver's own warning fired (a decode token under 40 GB/s over the
  resident weights), and the row this ledger's entry below reads at 110.13 / 19.66 was the same plan
  on a quieter desktop - its prompt row already paid some of it. The token under
  `DASLLAMA_GPU_PROF=1`, warm, 32 slots: 32.3 ms, of it the host's sums of the misses 8.6 ms (29.5 with
  every pick on the host), the segments and the hit chains 22 ms, the last segment's wait 1.5 ms; the
  device serves 70-83% of the picks on the bench's synthetic ids, 74.6% in the parity cell's steps.
  Research before the kernels (`bin/Release/daslang.exe -jit harness/vk_gemv_probe.das -- 2560 672`,
  the expert gate shape, this card): `iq3s4` decodes at 418 GB/s streaming and 7.2 us a cold plane of
  0.9 MB (iq4xs 294-352 GB/s, 9.2 us; iq3s 400 GB/s, 8.3 us; k4 402 GB/s, 8.1 us; q8 418 GB/s, 11.1
  us), the host's chain 61 us an expert (the `DASLLAMA_GPU_PROF=1` token report: the host's routed sums
  over the picks it summed) - the device is six to ten times the host on a hit. Placing an expert in a
  slot (the same report's pool line, the gather and upload clocks summed over the placements): the
  gather of its three planes 0.57 ms and the upload of 2.6 MiB 0.27 ms (the gather parallel over rows;
  it read 1.85 ms while one expert's rows ran on one lane), so a pool of 32 x 48 slots fills in 1.3 s,
  once a model load, at the first window's host steps; a full pool swapped 6-20 experts over 32 tokens.
  The pool's bytes are `slots x routed layers x (the three expert planes' slot bytes)`: 32 x 48 x 2.6
  MiB = 4025 MB, and 64 slots would be 8050 MB, which the plan yields only beside the whole mirror. A
  decode that starts with no prompt fills two slots a layer a step and reads 14-21 tok/s over its first
  128 tokens (a tg128 row run with no prompt row before it in the process). Parity: `test_gpu_resident_hc.das` run directly through dastest (`bin/Release/daslang.exe -jit
  dastest/dastest.das -- --test modules/dasLLAMA/tests/test_gpu_resident_hc.das` under `DASLLAMA_GPU=1
  DASLLAMA_PARITY_FULL=1`) with the pool armed, a 48-token prompt through the window chain (its hits on
  the device) and six fed steps - the prompt's row 0.46 against 2.00, the steps 0.37-0.68 against bars
  1.70-2.56, the one-step-off control 5.15-11.98, argmax equal on all seven rows, 1536 experts
  uploaded, 18793 of 23040 window slot rows and 2359 of 2880 decode picks on the device, 36 of the CPU's
  own picks off the tape; the bench's frozen pinned-greedy fixture (`lcpp_bench`'s fixture gate)
  reproduces its opening tokens with the pool serving. Kernel cells:
  `test_vkd_hot_combine`, the `iq3s4` stamps' fourteen cells, `test_resident_hot_slots` and
  `test_vk_coopmat_default_and_tile_pick`'s per-32 column arm (model-free).

- **LANDED (2026-09-29) - the Vulkan whole-model driver on the hyper-connection carrier
  (Qwen3.8-Flash-Next UD-IQ4_XS, 94 GB, 512 experts top-10): every plane but the routed expert stacks
  resident, the token command and the prompt's windows recorded as 49 segments cut after each layer's
  router, the host summing the routed experts between them
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`, `#hc-window-chain`).** zen2 (Ryzen 16 threads, RTX
  5060 Ti 16 GB on a PCIe 4.0 x8 link, 4 GB held by the desktop), `bin/Release/daslang.exe -jit
  benchmarks/lcpp_bench.das -- --ngl 0 -p 512 -n 128 -t 16 -r 3 --for-debug-purposes` under
  `DASLLAMA_GPU=1 DASLLAMA_IQ3S_SERVE=lut DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`: tg128 19.66
  +/- 0.19 / 18.07 +/- 0.25 / 18.93 +/- 0.11 tok/s over three processes (direction-grade; the CPU chain
  6.89; the reference rows, external - llama.cpp b10659 `llama-bench -ngl 99 -ncmoe 48 -t 16 -p 512 -n
  128 -r 3`, the experts on the CPU: Vulkan 13.07, CUDA 17.32), pp512 110.13 +/- 0.37 through the
  window chain (63.18 +/- 0.80 with the prompt read on the CPU, the form before it; llama.cpp Vulkan
  91.66, CUDA 147.3 +/- 21, external - both stream the 55.4 GiB of experts across the link per
  512-token batch, `nvidia-smi dmon -s t` reading 10.5-12.0 GB/s under CUDA and 4.8-7.2 under Vulkan,
  external). The token under `DASLLAMA_GPU_PROF=1`: 52.6 ms, of it the host's
  routed sums 29.5 ms, the 49 segments' submits and waits the rest, the last segment's wait 1.6 ms.
  Resident bytes: 4.98 GiB (deltanet 2.09, hc mixers 0.64 as 97 q8 down/up pairs of [10240 x 320], the
  n-gram key [3840 x 10240] and value [3840 x 2560] planes 49 MB, token_embd 0.63, attention 0.59, head
  0.49, router 0.23, shexp 0.23); the conv ring 10 rows x 10240 floats a region (400 KB), the token's
  segment landings 10 KB + 2 KB + 10 KB host-visible, 49 command buffers a region; the window's panels
  at 512 rows: five [512 x 10240] float planes of 21 MB each (the wide rows, the normed streams, the up
  projection, the gated rows, the side panel), the key rows 21 MB, the streams' Q8_0 image 5.9 MB, the
  side rows 7.9 MB + 2.2 MB, the landings 5.2 MB + 1.1 MB + 5.2 MB host-visible, 49 command buffers;
  the 55.4 GiB of routed experts and the 26.8 GiB n-gram table stay on the host. Parity (the cell's
  form before the hot pool; the entry above carries its 48-token form): `tests/run.das` cell
  `test_gpu_resident_hc.das`, a 24-token prompt through the window chain and six fed steps, the
  CPU arm routing as the device arm did through the decode pick tape
  (`ARCHITECTURE_ENGINE.md#moe-pick-tape`) - every row within 0.20 x max|logit| (the prompt's row
  0.88 against 2.55, the steps 0.62-0.86 against bars 2.00-2.48, argmax equal on all seven), the
  one-step-off control 5.81-10.64, 1440 selects replayed, the CPU's own decode top-k off the tape 62
  of 2880 picks; without the tape the arms read 1.8-4.1 with two argmax flips, and the CPU chain on
  `DAS_TUNE_POLICY=reference` reads 3.4-7.0 against the tuned chain with three flips of seven rows - a
  probe's reading, the model's own near-tie sensitivity. The bench's frozen pinned-greedy fixture
  reproduces 8/8 opening tokens with decode on the device. Kernel cells: `test_vulkan_kernels.das`
  `test_vkd_hc_seams` (four cells), `test_vkd_hc_rows` (the rows forms, four cells) and
  `test_vkd_ple_side` (two positions and the window's panel form), the deltanet sigmoid stamps under
  their silu controls.

- **LANDED (2026-09-28) - the Metal batch rail's hyper-connection arm (Qwen3.8-Flash-Next tg128@4):
  the batched step opens the rows' wide residual, runs the mixers through the shared layer body,
  gathers each row's n-gram heads on its own session and writes each row's conv ring in place.**
  M5 Max, `lcpp_bench --ngl 99 --npl 4 --npl-plen 512 -n 128 -r 5 --for-debug-purposes` under the
  box's scratch manifest: tg128 56.86 +/- 0.05 tok/s, tg128@4 119.58 +/- 2.51 summed (2.10x; reps
  124.0 / 118.5 / 117.9 / 118.9 / 118.6 - the first rep of every run reads 3-5% high, a 3-rep read
  lands at cv 3-9%, five reps hold cv 2%). The step is device-bound: `--prof` reads the batched step
  at setup 0.08 / encode 1.5 / wait 31.7 ms (gpu 30.7) against the single step's 1.0 / 1.4 / 19.8
  (gpu 18.9), so four rows cost 1.63x one row's GPU time and the host is 5% of the wall. The MoE
  siblings on this box read the same shape (Qwen3-30B-A3B 165 -> 314, 1.9x; Qwen3.6-35B-A3B 128 ->
  269, 2.1x): four rows' top-10 of 512 experts barely overlap, so the expert traffic scales with the
  rows and only the dense and attention passes amortize. Prediction on record before the run:
  140-180 summed (2.5-3.2x) - missed on the expert-traffic term. Parity: the batch probe holds
  B=2 and B=4 to 5e-4 logits against the single step over 16 steps (0 argmax flips), and
  `mtp-dff-3.8fn` pins it in the suite.

- **OPEN (2026-09-27) - the Vulkan resident driver's NextN prompt warm: the head's slab warmed on
  the window chain, and what it costs a prompt.** Where the driver homes a NextN head and the
  round is on (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`) every window of a
  resident prefill encodes one more layer's worth of rows after the trunk: the last layer's FFN
  over the whole window instead of its last thirty-two rows, eh_proj [2dim -> dim] and the head's
  k and v projections [dim -> kvd] over the window's rows, two norms, a feed requant, the head's
  q/k norm and the rope store - on Qwen3.5-0.8B-MTP the eh_proj plane's 2,228,224 bytes and the k
  and v planes' 557,056 each read once a window, the last layer's three FFN planes (3,899,392 each)
  over 512 rows in place of 32. The chain's planes grow by the head's staging - the embed rows, the
  shifted post-norm rows and the [enorm ; hnorm] image with its feed: `PF_WINDOW x (4 dim + 8 dim
  + 2 dim + dim / 4) bytes` under a Q8_0 feed, `PF_WINDOW x (16 dim) bytes` under the f16 feed, plus
  `PF_WINDOW x cos_elems x 4` for the rope rows - 512 x 14,592 = 7,471,104 bytes at dim 1024 on the
  Q8_0 feed, 512 x 16,384 = 8,388,608 on the f16 feed, and 131,072 of rope rows at a 64-wide plane;
  the head rows' readback moves `(npos - 1) x kvd x 2 sides x 2` bytes a prompt (1 MB a 512-row
  prompt on the 0.8B's f16 rows). Owed from the pod: pp512 of `Qwen3.5-0.8B-MTP-Q8_0.gguf` under
  `DASLLAMA_GPU=1` with `set_mtp_spec` on against off (`lcpp_bench --mtp-ab -p 512` with the
  `DASLLAMA_GPU_PROF=1` window profile: the `vk_rdpf` submit wall per window with the warm against
  without), and the round's acceptance on the SpecBench chat corpus against the CPU round's on the
  same box - the reading the warm exists for.

- **OPEN (2026-09-27) - the Vulkan resident driver's same-slab speculative verify: its row
  buffers and its round against the CPU verify.** Where the driver homes a NextN head
  (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`) the per-row planes hold
  max(regions, depth + 1) rows, eight at most, so a one-region load gains `depth` rows.
  A row is 2 x vocab x 4 bytes of logits (the device plane and its host landing) beside the
  activation rows: 4 dim + 2 hid + 2 qd + 2 kvd (+ 2 qd under a gated q) floats, the quant
  rows' wide + wide / 8 + qd + qd / 8 bytes, the attention partials' n_heads x (splits x (hs + 2)
  + 1) floats, the host x, carry, rope and TokMeta rows, the picks' 520 bytes, on a hybrid the deltanet projection (cd + di floats)
  and o rows (the step's fixed 8,192 + 1,024 + 32,768 bytes), and under the head its three cat
  rows (6 dim floats), its TokMeta row and two rope rows. On Qwen3.5-0.8B-MTP (vocab 248,320,
  dim 1024, hid 3584, qd 2048, kvd 512, gated q, cd 6144, di 2048, 8 heads of 256, a 64-wide rope
  row) that is 1,986,560 bytes of logits and 197,384 of the rest, 2,183,944 a row, plus the
  partials' 8 x (splits x 1,032) + 32 (132,128 at 16 splits): one region gains 2,316,072 bytes at
  depth 1 (nb 1 -> 2) and 16,212,504 at depth 7 (nb 8), both at 16 splits. The plan's scratch
  term counts the same rows. Owed from the pod: `lcpp_bench --mtp-ab` on the 0.8B-MTP under
  `DASLLAMA_GPU=1` with `--prof` and `JOBQUE_PROFILING=1` - the `mtp.verify` section against a
  one-row step's wall and against the CPU verify's on the same box; the reject's `mtp.rollback`
  section against the `mtp.snapshot` + `mtp.replay` the CPU restore paid (the recurrent state's
  round trip, about 20 MB down and up on the 0.8B, retired: the verify command copies each
  recurrent layer's slot state and ring pair after every row but its last into a rollback scratch
  of `depth x n_rec x (nvh x ds x ds + 2 x cd x (dconv - 1)) x 4` bytes, and the reject copies one
  row back - on Qwen3.5-0.8B-MTP (18 recurrent layers, 16 heads of 128, cd 6144, dconv 4)
  21,528,576 bytes at depth 1 and 150,700,032 at depth 7, counted in the plan's scratch term,
  `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`); the verify command's own
  `DASLLAMA_GPU_PROF=1` ledger (`vk_rdec gpu avg/verify` with its `dn avg/verify` line and
  `vk_rdec host wall/verify`, `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`) against the
  one-row `vk_rdec gpu avg/token` line of the same run's off arm, role by role - under the greedy
  `--mtp-ab` arms' pick ask the verify lands the picks and two hidden rows alone (the
  2 x vocab x 4-byte logits plane, its host copy, the four row copies and both host argmaxes gone
  from the round), the re-warm runs no q GEMV and the store pass over the k heads alone
  (`warm_q` gone from the ledger, `warm_qknrope` smaller), and no head row is read back (the `kv
  readback` piece gone from `host wall/verify`). The on/off rate with the drafts' acceptance, as
  measured: `benchmarks/lcpp_bench.das --mtp-ab -n 64 -r 2` (`-r 4` on the 0.8B) as the `-jit`
  script (debug-jit) under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`,
  `DAS_TUNE_POLICY` unset (the box profile), the resident driver at cm2 coopmat mode, on the
  RTX PRO 4500 pod, tok/s off / on, the off arm the plain tg64: Qwen3.5-0.8B-MTP-Q8_0 444.7
  +/- 9.5 / 574.9 +/- 14.3 (x1.29, acceptance 81.9%); Qwen3.5-4B-MTP-Q8_0 138.0 / 202.5 (x1.47,
  82.6%); Qwen3.5-9B-MTP UD-Q5_K_XL 103.8 / 149.3 (x1.44, 82.7%); Qwen3.8-27B-Q4_K_M with its
  Q8_0 head, the image rail off, 37.1 / 54.3 (x1.46, 69.9%) - direction-grade, the on and off
  arms in one process per row. The stamped ledgers at 66b143122 on the 0.8B under the same
  command with `DASLLAMA_GPU_PROF=1`, microseconds: the verify's host wall 2917 (GPU 2870), the
  draft's 491 (GPU 449), against a one-row step's 2571 (GPU 2357). Measured on the RTX PRO 4500
  pod - `benchmarks/lcpp_bench.das --model Qwen3.5-0.8B-MTP-Q8_0.gguf --mtp-ab -n 96 -r 1
  --for-debug-purposes` as the `-jit` script (debug-jit) under `DASLLAMA_GPU=1
  DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16 DAS_LOG_LEVEL=info DASLLAMA_GPU_PROF=1`,
  `DAS_TUNE_POLICY` unset (the box profile), the resident driver at cm2 coopmat mode,
  microseconds a round from the `DASLLAMA_GPU_PROF=1` ledger: the two-row verify's `dn_seq`
  reads 421 against the one-row `step` 243 and the rollback copies (`roll`) 74 - a recurrent
  layer's row costs its dispatch latency (16 workgroups, one a head), not the state's reload. A
  form that looped the rows inside one dispatch a layer with the scratch stored in-kernel and the
  final ring pair written by a last-arriving workgroup was bit for bit the chained steps and read
  `dn_seq` 721 under the same command and environment at its own commit (direction-grade, two
  commits): the loop serializes the rows a workgroup already ran back to back, and the epilogue's
  barrier and staged copy add to every layer - the per-row dispatch stays. What moves `dn_seq` is
  more workgroups a head (the ds x ds state split across workgroups), a step-kernel change the
  one-row decode shares.

- **LANDED (2026-09-28) - the head K-row bar of `tests/test_gpu_resident_hybrid.das`
  (`HEAD_K_BAR_REL = 0.08`, twice the deltanet bar): the prompt warm's K rows against the CPU
  warm's.** The cell `test_gpu_resident_hybrid_mtp_head_warm`, through `tests/run.das` under
  `-jit`, `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`, `DAS_TUNE_POLICY` unset
  (the box profile), the resident driver at cm2 coopmat mode, on the RTX PRO 4500 pod, on
  Qwen3.5-0.8B-MTP-Q8_0 over the 40-token prompt: the widest miss reads 0.05 of the CPU row's max
  under both codecs - f16 mirrors 0.357 at row 38 on a 0.577 bar, f32 mirrors
  (`DASLLAMA_VK_KV32=1`) 0.278 at row 2 - and the one-down control lands 7.3 to 7.6 bars away.
  The bar is twice the deltanet bar because the row is the f16-stored projection of a hidden row
  already held within the deltanet bar; the control is tightened to two bars from it.

- **OPEN (2026-09-27) - the Vulkan resident driver's NextN draft head: its plane and slot bytes,
  and the device draft against the CPU draft.** Where the driver takes a model's head
  (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`) the plan grows by the head's eight
  planes and one K/V slot a region; a headless model plans what it did. The planes are
  (2 dim x dim + dim x qrows + 2 dim x kvd + qd x dim + 3 dim x hid) weights at their formats' block
  bytes - Qwen3.5-0.8B-MTP-Q8_0 (dim 1024, the gated q's 4096 rows, kvd 512, qd 2048, hid 3584,
  every head plane Q8_0 at 34 bytes a 32-weight block): eh_proj 2,228,224 bytes, q 4,456,448, k and
  v 557,056 each, o 2,228,224, gate, up and down 3,899,392 each, 21,725,184 in all. The slot is
  regions x seq_cap x kvd x 2 sides x 2 bytes (f16), regions x seq_cap x 2,048 bytes on the 0.8B:
  8,388,608 at one region of 4096 positions, 67,108,864 at four regions of 8192. The norms plane
  gains (5 dim + 2 head_size) x 4 bytes (22,528 on the 0.8B) and the draft's staging 3 x 2 dim x 4
  bytes a row of the per-row planes (24,576 a row at dim 1024, 122,880 at dim 5120; the next entry
  prices the rows). Owed from the pod: the 0.8B-MTP's `resident image`
  and `NextN draft head in the arena` load lines (the bytes read, not computed); the draft's wall
  against the CPU `forward_mtp` - `lcpp_bench --mtp-ab` under `DASLLAMA_GPU=1` with `--prof` and
  `JOBQUE_PROFILING=1`, the `mtp.draft` section against the CPU draft's on the same box - including
  the hydrate's upload where one runs, a submit of its own beside the draft's (the drafted row
  stays on the device, so no readback follows the command, and the greedy round takes the pick the
  seat answers, the vocab x 4-byte logits row neither landing nor re-read by a host argmax); the
  draft command's own `DASLLAMA_GPU_PROF=1` ledger (`vk_rdec gpu avg/draft` and `vk_rdec host
  wall/draft`, `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`), the classifier's share of it
  against the one-row `cls` role; and the acceptance the
  device draft reads off the head's slab the window chain warmed (the entry above) - the pod
  measures.

- **OPEN (2026-09-27) - the Vulkan resident driver's speculative carry: its landing plane and its
  step cost on a NextN-headed model.** The landing plane (`RDec.hid_host`, host-visible, allocated
  only where the driver is prepared with `carry`) is dim x 4 x nb bytes, nb the per-row planes'
  rows (a row a region, or the speculative verify's rows where more, eight at most):
  Qwen3.5-0.8B-MTP (dim 1024) 4,096 bytes a row, 32,768 at eight. Each step moves
  dim x 4 bytes a row over the logits' path (4 KB a token on the 0.8B), and the last layer's
  residual step leaves the q8 down GEMV's epilogue for the row-storing stamp, one dispatch more a
  token - while the speculative knob is on; with it off the step pays none of this and keeps the
  fused forms (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-spec-knob-gate`). Owed from the pod:
  tg128 of `Qwen3.5-0.8B-MTP-Q8_0.gguf` with `set_mtp_spec` on against `Qwen3.5-0.8B-Q8_0.gguf`
  (the same trunk, no carry) under `DASLLAMA_GPU=1`, with the `DASLLAMA_GPU_PROF=1` token profile
  of each, so the step's delta is read rather than estimated.

- **LANDED (2026-09-27) - the Metal speculative round on the hyper-connection model
  (Qwen3.8-Flash-Next + its split shared head): the verify's rows carry the wide residual into the
  draft head, the n-gram side input takes the panel form with the accepted rows committed after the
  walk, and every row form picks the deltanet z activation through `enc_dn_zgate`.** M5 Max,
  `lcpp_bench --ngl 99 --mtp-ab -r 3 --for-debug-purposes` under the box's scratch manifest, tg-real128
  over eight prompts, greedy, depth 1: off 55.32 +/- 0.49 tok/s, on 59.97 +/- 0.96 (1.08x), 1323 of 1716
  drafts accepted (77.1%; per prompt 55.6% to 98.4%, x0.94 to x1.23 - the round loses at 56%
  accept and wins from ~68%). The counting probe is token-exact at depth 1, 2 and 4 (100% accept,
  spec 2x plain wall) and the prose probe at depth 1 (77%, 0 flips). The same shape as the 9B's
  round (`followup_metal.md` row 26): an accept rate this high buys 1.77 tokens a round and the
  round returns a tenth of it, so the verify's two rows cost most of a plain step here too - the
  stage split (`harness/mtp_ruler.das`) is the next lever. The CPU round on this model is a loss
  (`followup_general.md` row 178).

- **LANDED (2026-09-27) - IQ3_S served as a 16-entry codebook plane on the CPU rail (`iq3s4`,
  `ARCHITECTURE_ENGINE_FORMATS.md#served-form`, `DASLLAMA_IQ3S_SERVE=auto|grid|lut`): every IQ3_S weight
  is a signed odd 1..15 times its block scale, so grid entry, qh bit and sign fold at transcode into
  one nibble over `-15, -13, .., 15`, decoded by the iq4xs LUT kernels - one `tbl` per 16 nibbles
  in place of a 2 KB grid gather, one load per 4 weights.** Qwen3.8-Flash-Next UD-IQ4_XS (gate/up
  experts IQ3_S) on the M5 Max, 18 threads, `lcpp_bench --for-debug-purposes -p 512 -n 128 -r 3`
  under the box's scratch manifest, one binary, warm passes (the pass after a mint is void: cv 7%):
  grid pp512 134.1 +/- 2.0 / tg128 18.98 +/- 0.13, lut 140.6 +/- 0.7 / 26.9 +/- 0.5, llama.cpp at 2b129ccfa
  107.8 +/- 3.5 / 19.9 +/- 0.4. `decode_prof -n 128 -t 18` under `JOBQUE_PROFILING=1`: 59.5 -> 40.3 ms a
  token; `mm_moe` 25.1 -> 6.7 ms, the attention block 19.5 / 20.0, the hc mixes 5.8 / 6.0,
  `mm_moe_dn` 5.0 / 3.5, the head 2.9 / 2.9. Predictions on record before the run: `mm_moe` ~7 ms,
  the token ~42 ms, tg128 ~25 - all three held. Cost: the planar image 98.1 -> 105.5 GB (+23% on
  the IQ3_S planes, the scale rows unchanged); the dequant is bit-identical to the grid form's
  (`test_kquant`). The GPU tiers keep the grid form - their gather is a device read. Provenance:
  the four bench logs and two profile logs of the 2026-09-27 session, `followup_general.md` 170.

- **LANDED (2026-09-27) - the Vulkan TTS dedup pass: the Pocket row GEMV (`TtsPkGemvT`) folds
  its own subgroup reduction onto the shared `WgReduceBase`, and the pass's bench rows against
  master's.** Box: the pod - the RunPod RTX PRO 4500 Blackwell, Linux, the Vulkan backend - one
  process at a time.

  The reduction fold's ground is the bits, not a race: the folded stamp writes the same output
  as the shipped one (max abs diff 0 over both served frame-loop shapes, ln_qkv 1024 -> 3072 and
  ln_gelu 1024 -> 4096), its workgroup memory is 16912 bytes under the device's 49152-byte cap,
  and the served Pocket row below reads the same on the folded tree as on master. The one-off
  probe that timed the two forms did not land, so its figures are not this ledger's.

  The bench rows, `direction-grade`, the same instrument on the same box at two named commits,
  one process a run: `./bin/daslang -jit modules/dasLLAMA/harness/tts_synth.das -- --model
  <file> --voice <voice> --out <dir> --limit 20` with `kitten-nano.gguf` and `kitten-mini.gguf`
  on `Bella`, `kokoro-82m.gguf` on `af_heart`, `pocket-tts-en-q8.gguf` on `alba`, under
  `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16 DAS_LOG_LEVEL=info`,
  `DAS_TUNE_POLICY` and `DASLLAMA_COOPMAT` unset, no other overrides; the mean generation wall
  (`gen_ms`) a sentence over the 20 sentences, with its min and max. At the dedup tree
  7e06befab against master 45277047d: kitten-nano 45 ms (16-431) against 44 (15-369), kitten-mini
  94 (36-653) against 91 (36-688), kokoro 87 (37-641) against 86 (37-654), Pocket q8 128 (60-634)
  against 133 (63-654) - within noise. The Metal row of the same instrument on the M5 is owed.

  The reflect pad's two `TtsPkRows` copies (the last generator stage's one `TtsReflect1` dispatch
  became two), on the device clock (`pfq_*`, 200 submits after a warm-up, `debug-jit`, the probe
  a one-run harness file that did not land) over t2 rows of c2 channels, both copies a submit:
  c2 8: 4.8 us at t2 2 through 4096, 6.5 at 65536, 12.0 at 262144; c2 24: 4.8 through 4096, 10.6
  at 65536, 24.9 at 262144; c2 64: 4.8 through 256, 6.4 at 4096, 18.6 at 65536, 173.7 at 262144.
  The pair costs a dispatch's floor at every row count a sentence produces, so the path ships no
  gate on the row count.
- **LANDED (2026-09-26) - the StyleTTS2 and Pocket seats ride the Vulkan TTS driver
  (`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain`): the kitten and kokoro families' seven
  seats and the Pocket codec and frames seats on the Vulkan tower's knob, the front end on the
  f32-exact tile, the decoder and generator convs on the f16 tile, every slab through the shared
  host writer.** Box: the pod - the RunPod RTX PRO 4500 Blackwell, Linux - every das figure
  `-jit` on this tree under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16
  DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The served rows:
  the first 20 sentences of the g2p corpus through `./bin/daslang -jit
  modules/dasLLAMA/harness/tts_synth.das -- --model <gguf> --voice <voice> --out <dir> --limit
  20`, the mean generation wall a sentence over the 20 with the first sentence's wall apart (it
  pays the device bring-up and the slab builds; of the spread only the min named below is in the
  record, the rest is the run's log). Pocket q8 (`pocket-tts-en-q8.gguf`, alba): 132 ms a
  sentence, the first 641, a steady sentence about 99 - the prompt 27 ms of it on the CPU chain
  (the prompt seat since serves it on the device, the entry above), the backbone 62, the codec 10. kokoro (`kokoro-82m.gguf`): 98 ms,
  the first 785, min 36, the steady sentences about 62, against the torch CUDA reference row of
  52 ms a sentence (`external`: `harness/tts_ref_bench.py --device cuda --models
  kokoro-82m:af_heart --limit 20` on the pod under
  `ARCHITECTURE_MEASUREMENT.md#the-tts-reference-instrument`'s recipe; the package versions that
  run printed are not in this branch's record). kitten-nano (`kitten-nano.gguf`): 54 ms against
  its onnxruntime CUDA EP reference row of 132 (`external`: the same script with `--device cuda
  --models kitten-nano`, onnxruntime-gpu with the cu12 pip libraries, providers CUDA then CPU);
  kitten-mini 302 against 4166 (the mini graph runs mostly off the provider). The LSTM recurrence's share,
  from the same run's per-role GPU ledger under `DASLLAMA_GPU_PROF=1`: a `TtsLstmDir` step 14 us
  at kokoro's H = 256, 22 ms of a kokoro sentence (`followup_vulkan.md` 106). The device
  bring-up - the process's first seat call, reported under the info log - 254..257 ms at the 64
  MB upload staging chunk against 322..349 at 256 MB, `direction-grade` (a process a chunk
  size): the pinned staging buffer costs 0.4 ms a MB (101 ms at 256 MB, 26 at 64) and a chunk
  past 64 MB buys no rate - the Pocket frames slab's 322 MB uploads in 27 ms on either, a 1.3 GB
  Llama file's resident upload reads 129 and 156 ms at 64 MB against 196 and 127 at 256 (two runs
  a size, the run-to-run noise: `benchmarks/lcpp_bench.das -p 32 -n 0 -r 1`,
  `Llama-3.2-1B-Instruct-Q8_0`), the Pocket first sentence 708 -> 641 ms with the chunk. The
  attach costs, same box and flags, `direction-grade` (two commits a pair): a seat's scratch is
  sized a quarter past the call's need, so a longer sentence after a shorter one reuses it -
  kokoro's scratch builds over the 20 sentences 9 -> 6, the second sentence's decoder 154 -> 23
  ms; each seat's host readback buffer is sized to the rows it copies out, never the widest slot
  - the Pocket codec's 765 MB -> 0.6 MB for a wave, the kokoro decoder's up to 717 MB before; the
  Pocket frames slab (322 MB) builds in 70 ms against 201 with the q8 rows dequantized over the
  job pool. What the seats hold: the Pocket frames slab 322 MB, the kokoro decoder's scratch past
  3 GB at its widest sentence, the Pocket codec's one-shot column slot about 4 GB at the 512-frame
  cap (`followup_vulkan.md` 108, 109).
- **LANDED (2026-09-26) - three TTS parity bars widened to cover the pod's x64 CPU chain beside
  the M5 Max's.** Every reading `-jit` through `tests/run.das -- --area tts` on the named box: the
  pod (the RunPod RTX PRO 4500, Linux; its x64 CPU chain is the arm under compare) and the M5 Max
  (Metal). `SOURCE_BAR` (`tests/_tts_parity.das`) 5e-3, absolute - the harmonic source against
  the oracle's, where the phase past 1e5 radians turns a ulp of an increment into radians: the M5
  Max reads under 1e-4, the pod's x64 chain 2.5e-3 on kitten's ONNX law. `GPU_CODEC_SERVED_BAR`
  (`tests/test_tts_pocket.das`) 2e-1 - the codec seat on the served planes against each file's
  own CPU chain: the M5 Max 1.3e-2 / 7.7e-3 (q8 / kq), the pod 9.2e-2 / 1.5e-1, all of it the
  CPU's Q8_0 activation blocks of the first layer's rows while the tower sits at 3e-6 of the CPU
  f32 chain on either box. `GPU_FRAME_FREE_BAR` (the same file) 5e-3 - the frames seat
  free-running on the f32 lane against the CPU chain's free run on the same seed: the M5 Max
  1.5e-4 and 1.1e-4 on the two stage cases, the pod 7e-5 and 1.8e-3 (lj028's 150 frames compound
  the f16 feed's rounding); the 1.8e-3 reading predates the fixes this arc landed after it, and
  the bar stands over both readings.
- **LANDED (2026-09-26) - the sampler is a candidate list when top-k is set
  (`ARCHITECTURE_ENGINE.md`, `dasllama_sampling.das`): one pass over the row selects the k
  largest logits into a heap, and the temperature, softmax, nucleus and min-p cuts and the draw run
  over those survivors; top-k off or past `SAMPLE_TOPK_FAST_CAP` (a design bound, not a timed
  pick) keeps the vocab-wide reference.** Before, a sampled decode paid k full-vocab max passes, a
  vocab-wide softmax, a full sort for top-p and a vocab-wide CDF walk per token. `direction-grade`,
  `debug-jit` (two commits, one process a run): `bin/daslang -jit utils/dasllama-server/cli.das --
  complete --model Qwen3.5-4B-Q8_0.gguf -p "<a story prompt>" -n 256 --seed 7`, once greedy and once
  with `--temp 0.7 --top-k 20 --top-p 0.95`, five reps each, the CLI's own stats line, this box
  (M5 Max, Metal, the box's tune profile, no environment overrides): greedy 105 t/s before and
  104-105 after; the chat preset 74-75 t/s before, 101-103 after - the sampler's per-token cost
  from 4.0 ms (the difference of the two rates) to under the run's noise. Prediction on record
  before the change: sampled within 5% of greedy. `lcpp_bench` carries no sampler flag (its
  `--mtp-temp` sets a temperature alone), so no board row holds the sampled rate.
- **LANDED (2026-09-25) - the Pocket TTS frame loop rides the Metal tower as the family's second
  seat (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`): the backbone step and the flow head for every
  frame, eight frames a command buffer over a per-voice device K/V slot, the EOS rule on the host
  between batches, the q8 backbone on the decode GEMV, the head's GEMVs carrying their norm and
  activations, the attention row a threadgroup a head with its scores staged.** The bare q8
  dot's route was A/B'd on this box (`harness/pocket_stage_probe.das -m pocket-tts-en-q8.gguf -t
  "The quick brown fox jumps over the lazy dog." --voice alba --reps 5`, best of 5,
  `DASLLAMA_ALLOW_UNTUNED=1` on both arms - the tower's stamps carry no crown - two processes, one a
  route, `debug-jit`): the decode split-K GEMV plus a bias add 1.228 ms a frame (backbone 40.8 ms of
  51.6), the row stamp with the bias fused 1.477 (backbone 53.3); the probe's CPU arm in the same
  process read 1.331 and 1.303, the box's control across the two runs - the row stamp's
  one-lane-a-block dot loses to the split-K walk, so the bare q8 dot's stamp is gone and every bare
  q8 linear (the backbone's) takes the decode GEMV. Box, flags and
  the 20-sentence walls as the codec seat's entry below; `direction-grade` - the CPU arm a second
  process, the codec-seat column and the rig's before-rows the previous commit's readings; both
  seats serve in these rows.

  | file | voice | audio | das tower, mean (min-max) | das tower, codec seat alone | das CPU, mean (min-max) |
  |---|---|---|---|---|---|
  | pocket-tts-en-q8 | alba | 120.8 s | 81 ms (43-186), rtf 0.0133 | 142 ms (81-227), rtf 0.0238 | 200 ms (107-264), 0.0334 |
  | pocket-tts-en-kq | alba | 121.2 s | 72 ms (37-198), 0.0118 | 117 ms (61-189), 0.0195 | 176 ms (92-232), 0.0293 |
  | pocket-tts-en-stuart-kq | stuart_bell | 143.6 s | 78 ms (42-209), 0.0109 | 134 ms (80-210), 0.0187 | 204 ms (111-280), 0.0286 |

  The loop alone - the frames hook's wall a frame, the voice slot and the batches' round trips
  inside it - through `harness/pocket_stage_probe.das -m <gguf> -t "<one sentence>" --voice
  <voice> -r 5` (a 4.6 s sentence free-running, the best of five passes each way in one process):
  on the q8 file 0.91 ms a frame on the tower against 1.29 on the CPU chain, on the kq file 0.69
  against 1.04, on the one-voice stuart-kq file 0.78 against 0.98, on the f16 file's f32 lane
  (`--f32`, every linear on the f32 row GEMV) 1.91 against 5.89. Of the GPU's own time
  the backbone's 57 dispatches take about two thirds and the head's 22 the rest; the first form
  of the loop, a dispatch per row operation (105 a frame) and an attention kernel accumulating
  in a dynamically indexed register array, cost half again as much a frame, most of it the
  attention - the rope-and-store, the residual joins and the head's operations folded into
  their GEMVs, and the attention row rewritten over staged scores, are what brought it down.
  The device footprint on the q8 file (`harness/pocket_stage_probe.das`'s attach log lines): the
  frames slab 36.3 MB of f32 rows and the q8 blob 80.3 MB, the codec slab 41.3 MB, the voice slot
  20.2 MB of K/V over 411 rows (16 layers x 2 x 411 x 768 x 4 bytes) with 0.1 MB of rope tables;
  a chunk's working buffers come from the pool. Parity, teacher-forced on
  the oracle's noise and frames against the CPU chain (`test_pocket_frames_metal`): the f32
  lane's latents, conditioning rows and EOS logits at 2e-6 rel-rms, the cell asserting the
  tower's encodes rose one a batch, and its free run - the loop's own noise, every frame fed its
  own output - at 1.5e-4 and 1.1e-4 on the two stage cases against the CPU chain's free run on
  the same seed (`GPU_FRAME_FREE_BAR`, the M5 Max; the bar's value since the pod's reading, and that reading, are the widened-bars entry above); on the served lanes (`test_pocket_frames_metal_served`, which
  logs both distances) the tower reads nearer the f32 oracle than the CPU chain does - the q8
  file's latents 0.012 against the CPU chain's 0.025, the kq file's 0.10 against 0.11 - since the
  CPU quantizes the activations it feeds a q8 or K-quant plane and the tower feeds them f32. Quality,
  `harness/tts_rig.py` on the 200-sentence corpus against the codec-seat rows: q8 4.23 / 4.364 -> 4.18 / 4.364, kq 4.04 / 4.333 -> 3.91 / 4.325, stuart-kq 3.23 / 4.117 -> 3.36 / 4.136 (WER / UTMOS, a word or two of the 2201 either way, the UTMOS within a hundredth). The
  levers left are `followup_metal.md` sec.27.

- **LANDED (2026-09-25) - the Pocket TTS codec rides the Metal tower (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`): a chunk's latents up, its samples back, one command buffer, the CPU's windowed codec
  run as one shot over the chunk on the f32-exact GEMM stamps, the K-quant transformer linears
  dequantized into the slab so the small form serves too.** Box: the M5 Max, every das figure
  `-jit` on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json` (its runtime section
  applied, the kernel winners on their fallback bodies - the sidecar predates the binary) and
  `DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The first 20
  sentences of the g2p corpus through `harness/tts_synth.das --model <gguf> --voice <voice> --out
  <dir> --limit 20` (every sentence timed from the first, so the first one pays the slab build
  and the pipeline warm-up), the mean generation wall a sentence with its min and max over the 20
  and the real-time factor of the run; the CPU arm a second process under
  `DASLLAMA_METAL_TOWER=0` (`direction-grade`). The frame loop - the backbone and the flow head -
  stays on the CPU in both arms, so the tower arm moves the codec's share only.

  | file | voice | audio | das tower, mean (min-max) | das CPU, mean (min-max) |
  |---|---|---|---|---|
  | pocket-tts-en-q8 | alba | 119.6 s | 142 ms (81-227), rtf 0.0238 | 200 ms (107-264), 0.0334 |
  | pocket-tts-en-kq | alba | 119.8 s | 117 ms (61-189), 0.0195 | 176 ms (92-232), 0.0293 |
  | pocket-tts-en-stuart-kq | stuart_bell | 142.8 s | 134 ms (80-210), 0.0187 | 204 ms (111-280), 0.0286 |

  The codec alone, one sentence's 60-odd latent frames (4.6 s of audio) through
  `harness/pocket_stage_probe.das -r 5` (the best of five passes each way in one process; a
  tower pass spreads over a fifth of its mean, and the first pays the pipeline library, about
  5 s once a process): the f16 file on the f32 lane (`--f32`) 87 ms CPU -> 23 ms tower at
  rel-rms 9.0e-7 against the CPU chain (`test_pocket_codec_metal` reads 8.7e-7 on the oracle's
  latents, the cell asserting one tower encode a call); the q8 file 70 ms -> 20 ms at 7.1e-3,
  the kq file 64 ms -> 22 ms at 1.7e-2, the stuart-kq file 77 ms -> 24 ms at 2.3e-2
  (`test_pocket_codec_metal_served` reads 1.3e-2 and 7.7e-3 on the oracle's latents
  of the q8 and kq files: the CPU's served lanes quantize their activations against the q8 and
  K-quant planes, the tower reads the planes dequantized at f32, the StyleTTS2 decode seat's
  gap); the f16-staged
  GEMM twins read the same 33 ms at 9e-4, so the seat keeps the exact stamps. The served-lane
  cell's stage clocks (`test_pocket_synthesis_metal` in `tests/test_tts_pocket.das`, a 4.08 s
  sentence, a first synthesis): the codec 61 -> 40 ms with the slab build inside it. Quality, `harness/tts_rig.py` on the 200-sentence corpus (WER
  through parakeet, UTMOS), the tower arm against `DASLLAMA_METAL_TOWER=0`: q8 4.23 / 4.364 both
  arms; kq 4.09 / 4.333 -> 4.04 / 4.333; stuart-kq 3.32 / 4.117 -> 3.23 / 4.117 (a word or two of
  the 2201 on the served lanes' 1e-2 gap, the UTMOS equal to the third digit, the audio lengths
  equal). The frames seat is the entry above.

- **LANDED (2026-09-24) - the whole StyleTTS2 synthesis rides the Metal tower
  (`ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`): seven seats from PL-BERT to the inverse STFT, the front
  end on the f32-exact GEMM stamp and an f32 attention row kernel so the durations round as the
  CPU's, the harmonic source the CPU's operation for operation, the decoder through the
  generator and the inverse STFT as one command buffer.** Box: the M5 Max, every das figure `-jit`
  on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json` (its runtime section applied,
  the kernel winners on their fallback bodies - the sidecar predates the binary) and
  `DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The first 20
  sentences of the g2p corpus through `harness/tts_synth.das --model <gguf> --voice <voice> --out
  <dir> --limit 20` (the served q8 lane, every sentence timed from the first, so the first one
  pays the slab build and the pipeline warm-up), the mean generation wall a sentence with its min
  and max over the 20, and the real-time factor of the run; the CPU arm is a second process under
  `DASLLAMA_METAL_TOWER=0` (`direction-grade`). The reference column is `harness/tts_ref_bench.py`
  (`ARCHITECTURE_MEASUREMENT.md#the-tts-reference-instrument`;
  `~/Work/tts-ab/g2p/.venv-g2p/bin/python harness/tts_ref_bench.py --device mps --threads 8
  --limit 20`, and `--device cpu --models kokoro-82m:af_heart` for the torch CPU row; torch
  2.13.0, kokoro 0.9.4, kittentts 0.8.1, onnxruntime 1.29.0, `HF_HOME=~/Work/tts-ab/g2p/.hf`):
  the same 20 sentences' `ps_espeak` phonemes fed to the packages' own models, generation only,
  each sentence timed three times and the best kept, the mean of the 20; its audio runs shorter
  on the kitten models (the package's per-voice speed prior and its 5000-sample trim), so the
  compare is the wall a sentence, not the real-time factor.

  | model | voice | audio | das tower, mean (min-max) | das CPU q8, mean (min-max) | reference |
  |---|---|---|---|---|---|
  | kokoro-82m | af_heart | 145.7 s | 112 ms (65-310), rtf 0.0154 | 331 ms (192-625), 0.0455 | torch MPS 147 ms (rtf 0.0200); torch CPU 8 threads 564 ms (0.0769) |
  | kitten-nano | Bella | 242.4 s | 37 ms (21-131), 0.0031 | 192 ms (114-283), 0.0158 | onnxruntime CPU 8 threads 207 ms (0.0259, 160 s of audio) |
  | kitten-mini | Bella | 206.6 s | 111 ms (60-412), 0.0107 | 352 ms (193-562), 0.0341 | onnxruntime CPU 8 threads 1463 ms (0.2018, 145 s of audio) |

  The first sentence carries the slab build and the pipeline warm-up: on the tower arm it reads
  310 ms on kokoro against the run's 65 ms floor, 131 against 21 on kitten-nano, 412 against 60
  on kitten-mini. Where a sentence's time goes, from the served-lane cell's stage clocks
  (`test_kitten_synthesis_metal` in `tests/test_tts_kitten.das`, the same tree, flags and box; a
  first synthesis, so the slab build sits in the decoder clock): kitten-nano's 5.8 s sentence in
  64.6 ms on the tower - bert 18.1, text 1.7, durations 3.9, prosody 3.4, decoder 37.2 (the decode
  seat, two thirds of the sentence) - against 101.3 ms on the CPU q8 chain. Parity on the f32
  lane, every seat fed the CPU chain's inputs (the parity rail's stage cells, `gpu_stage_checks` in
  `tests/_tts_parity.das`, the same tree, flags and box): PL-BERT 3e-6, the text encoder 4e-7,
  the duration encoder, raw durations and prosody under 2e-6, the durations token for token on
  every stocked oracle case, the mixed source signal 4e-7; the decode seat 3e-3 to 7e-3 on
  kitten-nano and under 2e-2 on kokoro against the CPU chain on the oracle's noise (the decoder
  and generator keep the f16-staged GEMM; kitten-nano's small decoder amplifies that staging
  twelvefold against kokoro's); the decode seat on the served q8 planes 2e-2 to 6e-2 (the CPU
  chain quantizes its activations, the tower reads the weights dequantized at f32); the seam form
  of the generator alone 1.2e-3 at most; the served lane's sample counts within a hundredth of the
  CPU chain's (139600 against 140800 on the kitten-nano sentence, equal on kokoro's). The cells
  ran through `tests/run.das -- --area tts` (`-jit`, the manifest above, the M5 Max). The remaining
  cost and the f32-exact decoder question are `followup_metal.md` sec.27.

- **LANDED (2026-09-24) - the FastConformer Metal chain's rel-pos attention rides the f32 GEMM
  builder per head (`ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`), where the per-row kernel it replaced
  was most of the encode and lost to the CPU q8 lane past a minute of audio.** Box: the M5 Max,
  every figure below `-jit` on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json`
  (its runtime section applied, the kernel winners on their fallback bodies - the sidecar
  predates the binary), no other overrides unless named. Encode alone, parakeet-TDT v3 f32,
  gb1 (199 s of audio, 2485 frames), `harness/asr_stage_probe.das -m
  ggml-parakeet-tdt-0.6b-v3-f32.bin -w gb1.wav --reps 2`, its `pk.encode_total` bucket per
  pass, `direction-grade` (three processes): the GEMM-built chain 652 ms; the CPU q8 lane
  (`DASLLAMA_METAL_TOWER=0`) 2306 ms; the CPU f32 lane (`DASLLAMA_METAL_TOWER=0 --fp32`)
  9687 ms. The per-row kernel's chain read 3.39 / 3.40 s on the same clip, and that chain with
  its attention dispatch skipped by a local edit 0.46 s - the share the rework closed - both
  from a scratch timer around `parakeet_encode`, `direction-grade`. Served cells, `debug-jit`,
  `benchmarks/lcpp_bench.das --asr -m <model> -r 2 -t 6 --ngl 99 --for-debug-purposes` with
  `DASLLAMA_ALLOW_UNTUNED=1`, one value per clip (the bench prints the best of its reps),
  `direction-grade` against the board's 2026-09-23 rows: parakeet v3 jfk 61 ms, jfk3 161 ms,
  gb1 0.96 s, hp0 1.37 s, hp0x2 3.33 s, librispeech25 1.36 s - the board's das q8 CPU rows
  158 ms / 4.35 s / 25.7 s on jfk / gb1 / hp0x2 (`performance/records/m5.json`, `-t 6`);
  canary jfk 275 ms, jfk3 763 ms, gb1 3.22 s - the board's Metal row 322 ms / 6.50 s on the
  per-row kernel. `external`, whisper.cpp at 6fc7c33 built on this box,
  `~/Work/whisper.cpp/build/bin/parakeet-cli -m models/ggml-parakeet-tdt-0.6b-v3-{f32,q8_0}.bin
  -f <clip> -t 6` (its default Metal arm; the records rig passes `-ng`), the sum of its
  `mel time` + `encode time` + `decode time` + `predict time` print lines, f32 / q8_0: jfk
  82 / 86 ms, gb1 1.06 / 1.02 s, hp0 1.63 / 1.59 s, hp0x2 4.68 / 4.59 s; its `encode time`
  print (`t_encode_us`) alone 0.63-0.64 s at gb1 and 3.52-3.53 s at hp0x2. `external`, the
  board's NeMo `--device mps` row: canary jfk 525 ms, gb1 4.36 s. The board's reference roster
  wants parakeet-cli's Metal arm beside the `-ng` one - the run above, v3 f32, `-t 6`, no env.
  What remains of the encode is the block GEMM chain on the f32 builder (the chain less its
  attention, about 0.47 s at gb1 by the same local edit) and the shift-add the softmax row
  folds. The chain's per-encode scratch grows with the frame count T and has no cap (mp = T
  to 32, nkp = T to 64, wwpad = 2T-1 to 64, all f32): the two score slabs mp x nkp and mp x
  wwpad plus the probability slab mp x nkp, and the AV rows heads x mp x hs - at gb1 (T =
  2485) 25 + 50 + 25 + 10 MB, at hp0x2 (T = 6800) 187 + 373 + 187 + 28 MB. The lanes' own
  distance: on gb1 the q8 lane flips 16 of 655 TDT durations against the f32 planes, token
  ids and text exact.
- **The resident plan and upload read one plane list (`resident_planes`,
  `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`).** The list is built twice per load - once
  for the plan's byte sum, once for the upload's reserve - and holds one 24-byte record per
  device plane: about 25 KB on a 94-layer model, a few microseconds each, against a load that
  moves gigabytes. Cost today: nothing measurable; the fix, if the list ever grows per-token
  readers, is to build it once at load and keep it on the resident state.
- **LANDED (2026-09-18) - a K/V mirror region per stream trades context for concurrency, not
  for throughput (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`).** The resident driver's
  mirror is one allocation a side; `set_gpu_resident_regions` splits it, and the residency plan
  pays for the regions out of each stream's context. gemma-4-E2B-it-Q4_K_M.gguf served by
  `daslang -jit utils/dasllama-server/main.das -- --config <toml>` (`backend = "gpu"`, the vulkan
  tier in its default cm2 mode, kv f16, the box's tuned sidecar, `DAS_JOBQUE_THREADS=16`, no other
  override) on a Threadripper 3990X with an RTX 5060 Ti 16 GB under Windows; the instrument is
  128-token chat requests at temperature 0 through `/v1/chat/completions`, a rate being completion
  tokens over the request's wall, prefill included, and the footprint `/v1/stats`'s
  `gpu_vram_bytes`. At `streams = 1`: one region of 131072 positions, a 2304 MB mirror, 3628 MiB
  on the device, one stream 147.9 / 150.8 / 149.6 tok/s, four requests queued on the one slot
  148.9 tok/s summed. At `streams = 4`: four regions of 49926 positions, a 3510 MB mirror, 4836
  MiB on the device, one stream 148.8 / 151.8 / 152.3 tok/s, four requests at once 140.9 tok/s
  summed, `gpu_cpu_passes` empty in both. The decision: the server asks for a region per stream
  (1208 MiB and 62% of the context, on this model), because four conversations then move
  together instead of waiting in a queue; the summed rate does not rise, since the batched step
  still dispatches its rows one at a time - `followup_vulkan.md` item 69's closing paragraph is
  the batched dispatch that would raise it. Before the regions the same four-stream config served
  on the per-op tier at tg128 34.0 tok/s (the server's in-process bench), its paged sessions
  handed to the CPU on every step.
- **LANDED (2026-09-18) - the StyleTTS2 harmonic source streams in frame windows
  (`ARCHITECTURE_TTS_MEMORY.md#tts-source-stream`).** The source held four tables of samples times
  harmonics for the whole chunk - the cycles, the upsampled phase, the noise draw and the sine
  rows, 6.2 MB each for a 7 s sentence and 21.5 MB each at 25 s - to hand `linear_rows` nine
  values per sample. Only the cumulative phase sum reaches across the chunk, and it runs at frame
  rate, so the source now runs 64 phase frames (19200 samples) at a time on a carried sum, the
  resamplers taking a window of columns with the same tap arithmetic and the noise stream drawn
  in row order a window at a time. Bit-identical PCM on every carrier and length (FNV over the
  samples, one probe program over both trees); on one thread, five reps, cv under 2%, the say
  takes what it took - kitten-nano 345 ms -> 343, kokoro 1.56 s -> 1.55, kitten-mini 0.91 s -> 0.91
  at 7 s, 1.23 s -> 1.24, 4.95 -> 4.95, 3.13 -> 3.14 at 25 s; heap peak after one say on top of
  the six-buffer entry below: kitten-nano 170 -> 147
  MB (7 s) and 448 -> 365 (25 s), kokoro-82m 245 -> 222 and 630 -> 554, kitten-mini 222 -> 203
  and 595 -> 524. What remains is the generator's six stream-sized buffers, which the chunk cap
  bounds; the generator itself cannot window, because every Snake block's AdaIN takes its
  statistics over the whole stream.
- **LANDED (2026-09-18) - the StyleTTS2 generator runs on six buffers, the idle release covers
  its carrier, and the chunk cap is a knob (`ARCHITECTURE_TTS_MEMORY.md#tts-generator-buffers`).** A
  Kitten or Kokoro say holds its memory in the iSTFTNet generator's `[t][c]` rows, every buffer
  the stage's whole stream and the count of them live at once the footprint: nine same-size
  fields, three of which held values dead by the time the next was written. Six fields now
  carry the same roles (the block input doubles as the pair's second conv output, the noise
  branch's rows as the stage stream, its residual as each Snake block's), which moves values
  between addresses and nothing else - the PCM is bit-identical on every carrier and length
  (FNV over the samples, before and after). Heap peak after one say, das heap counters, the q8
  lane, M5 Max, one process on the box: kitten-nano 196 -> 170 MB for a 7 s sentence and 539
  -> 448 for a 25 s one, kokoro-82m 299 -> 245 and 800 -> 630, kitten-mini 269 -> 222 and 752
  -> 595 (the model itself 42 MB; the peak is per chunk and linear in its length). On one thread,
  five reps, cv under 2%, the say takes what it took: kitten-nano 345 ms -> 345, kokoro 1.56 s
  -> 1.56, kitten-mini 0.92 s -> 0.91 at 7 s; 1.25 s -> 1.23, 5.00 -> 4.95, 3.14 -> 3.13 at 25 s.
  `tts_release_scratch` deleted the block home's globals but left the `St2Scratch` carrier
  (109 MB of the 7 s kitten-nano say's 167) to keep a noise state a synthesis redraws from its
  seed anyway; it now frees the carrier too unless the parity rail captured the noise, and the
  heap after the release reads the model alone on every carrier. `tts_set_chunk_chars` lowers
  the 400-codepoint chunk cap per model, so a memory-tight build bounds the peak directly.
  One trap on the way: the after-tree read 15% slower in the generator on the 7 s says (400 ms
  against 345, reproducible across processes) with one probe program and identical to the
  before-tree with any other - the JIT cache had pinned one codegen of that program, and the
  op buckets under the profiling rail were equal to the millisecond (`followup_general.md`
  row 154).

- **LANDED (2026-09-18) - the Pocket codec streams in windows of 16 latent frames
  (`ARCHITECTURE_POCKET.md#pocket-codec-stream`).** The codec's activations, about 20 MB per second of
  audio across the chain's ping-pong rows, were the say's and the clone's working set: 116 MB
  for a 5.7 s chunk, 233 MB for an 11 s clip (das heap counters, the English q8 file, M5 Max);
  at 16 frames they are 41 MB whatever the run's length, 21 MB at 8. On one thread (26 runs, cv
  under 1%) a window is cache-sized rows and beats the whole run: the 5.7 s decode 374 ms -> 365
  at 16 and 360 at 8, the 11 s encode 710 -> 672 and 664. On fourteen threads the rows a step
  hands the workers thin out: codec minima 81-89 ms for the whole run against 93 at 16, 94 at 8,
  106 at 4, inside a run-to-run band of a tenth (cv 5-7%, void by the discipline; the say's total
  196-217 ms whole, 204 at 16). The remaining cost is the lane split at a few hundred rows -
  `followup_general.md` row 153. What a say still leaves behind, by the heap report's blocks:
  the roster voice's state (six layers, keys and values), the model's `PocketScratch` at 57 MB,
  and the block home's own scratch globals - `conv1d_rows_transposed`'s tap lift and the
  attention head scratch at 6 MB. Nothing leaks; the scratch is sized to the largest run it saw.
  Two levers landed with it: the voice's caches reserve the rows one chunk of the budget can
  take instead of a flat 1024 (54 MB -> 19 MB a voice, cap 411 rows for a 126-row clip), and the
  tap lift runs a cache-sized block of input rows at a time from the last block down, so an
  output row sums its taps in the same order (66 MB on the whole run, 15 at 16 frames -> 4 MB;
  the whole-run decode on one thread 374 -> 360 ms for the cache-sized rows). A say now adds 87
  MB to the heap where it added 133, and `tts_release_scratch` gives the carrier and the globals
  back when the caller says idle memory matters: the browser examples' speech threads call it
  after half a second with nothing queued.
- **LANDED (2026-09-16) - the browser build runs on every engine; `+relaxed-simd` leaves the
  wasm feature string.** WebKit implements none of the relaxed opcodes, and the previous entry's
  `+relaxed-simd` put `f32x4.relaxed_max` (the backend's lowering of an `nnan` vector max, which
  the previous entry believed it never emitted) into function 298 of every release, so Safari
  refused the whole module at validation. The feature is gone and the module links
  `-sMEMORY64=2` (wasm64 pointers, the memory lowered to 32-bit). Cost measured on the storywish
  page in Chrome (this M5, `DASLLAMA_ALLOW_UNTUNED=1`, one run each, steady-state clips): Pocket
  TTS rtf 0.167-0.171 with the fused multiply-add, 0.167-0.171 without it - the split
  `mul` + `add` costs nothing measurable, so the previous entry's first lever bought reach on
  Chrome only in theory. Prefill and decode of the story model were not measured separately.

  Provenance, **direction-grade** and **out-of-process**: the two readings compare two wasm
  builds (the feature string with and without `+relaxed-simd`, same tree otherwise), and every
  figure is read from a console line outside the timed program. Box: the M5 Max Mac (this
  machine). Harness: the storywish page's own first story, reading the engine's
  `dasLLAMA tts: ... rtf` line from the Chrome console; the artifact is the wasm64 release of
  `examples/dasLLAMA/storywish` built by `daspkg release wasm` from the wasm-host worktree,
  served locally by `site-dasllama/serve.py` to Chrome. Flags: the wasm cross target (not `-jit`,
  not AOT), `DASLLAMA_ALLOW_UNTUNED=1` so `DAS_TUNE_POLICY` served the reference bodies, the
  release's 16-worker pool. One story per build, the steady-state clips after the first two.
  NOT board-grade: no record cell backs it, and an rtf read from a page console is not an
  `lcpp_bench` figure.
- **LANDED (2026-09-16) - the browser gets its FMA and an int8 dot, and the spin loop stops
  crossing into JS.** Four levers on the parrot page (Pocket TTS, zen2 box, Chrome with the window
  verified in front before AND after each run, 7 workers + team dispatch, `DASLLAMA_ALLOW_UNTUNED=1`
  so the figures are the fallback bodies'). One, `+relaxed-simd` joins the wasm feature string:
  `f32x4.relaxed_madd` is the only multiply-add wasm has, so without it every contracted
  multiply-add split back into `mul` + `add` - 8728 contractions across the module once on
  (`SIMDTernary` 1457 -> 10185). Two, `dot_k4q8` stopped walking its 32 weights one at a time: both
  nibbles land in a SIGNED `byte16` (`q & 15`, `(q >> 4) & 15` - 0..15 is non-negative, so both are
  valid int8) and feed `idot`, which every target already lowers for itself, so one body needs no
  target branch. The measure button's three runs went 1.1x -> 1.7x real time (per chunk, rtf
  0.892-0.947 -> 0.565-0.613; per stage ms, prompt 656-716 -> 313-349, backbone 1545-1731 ->
  812-909, codec 3739-4216 -> 2545-2822; head, which is not K-quant, 145-166 -> 153-173, the
  control). Three, spinning workers: 1.7x -> 2.3x, the two arms taken in ONE page session with the
  lab's spin checkbox flipped in place (`set_jobque_worker_spin`), so one process and one
  instrument. Four, the spin loop's two JS crossings per iteration - `steady_clock::now()` for the
  deadline and `this_thread::yield()` for the pause - are now a strided read and a counted volatile
  loop; clock time across all threads fell 25034 ms -> 5089 ms in a 15 s window. That last one did
  NOT move wall time here and is not expected to on this box: 64 cores against 8 busy threads, so a
  spinning worker's waste is free. It is the core-scarce box where it should pay.

  Provenance, **direction-grade** and **out-of-process**: the kernel lever compares two wasm builds
  (two commits), and every browser figure is read from a console line outside the timed program.
  Box: the zen2 Windows box, 64 logical cores, `performance/defaults/x86-avx2` in force. Harness:
  the parrot lab's own measure button (three silent says a press), reading the engine's
  `dasLLAMA tts: ... rtf` line from the page console; the artifact is the wasm64 release of
  `examples/dasLLAMA/parrot` built by `daspkg release wasm`, served locally to Chrome. Flags: the
  wasm cross target (not `-jit`, not AOT), `DASLLAMA_ALLOW_UNTUNED=1` so `DAS_TUNE_POLICY` served
  the reference bodies, 7 workers with team dispatch on. The window was verified in the foreground
  before AND after each run. The contraction count is `wasm-opt --metrics` over the two builds'
  `parrot.wasm`. The clock-time split is a Chrome CPU profile of one measure run, aggregated per
  thread by node self time. NOT board-grade: no `performance/records/zen2.json` cell backs any of
  it, and an rtf read from a page console is not an `lcpp_bench` figure.

- **REVERSED (2026-09-16) - "a browser's job queue workers park instead of spinning" (2026-09-10,
  below) rested on two premises this arc falsified, and the browser now spins.** That entry
  measured spinning as HARMFUL on the M-series box (1.1x spinning against 1.4x parked) and set
  `dasllama_jobque_spin_default` to 0 under emscripten. It was measured (a) against the scalar
  `dot_k4q8`, which made every parallel section about twice as long as it now is, and (b) with a
  spin loop that crossed into JS twice an iteration - `steady_clock::now()` for the deadline and
  `sched_yield` inside the pause - which is most of what a spinning web worker was costing the
  caller. Both are gone. On the zen2 box spinning wins, 1.7x against 2.3x. The emscripten `[init]`
  that zeroed the window is deleted: a browser now takes the one 30 ms default every other target
  takes, and `setup_dasllama_jobque` pushes it with no platform test. Ruling (Boris): a slower
  M-series result is acceptable - the box is faster to begin with and most visitors are on a PC. A
  box that prefers parking still has the knobs (`jobque_spin_us` in a profile, the setter).

- **OWED - the playback underrun counter is browser-only.** `sound_playback_underrun_frames`
  answers a real count under emscripten with pthreads and a hard 0 everywhere else, because the
  ring and its producer compile only there - native, null-device and non-pthread wasm run the
  mixer straight off the device callback and have no ring to underrun. A script therefore cannot
  tell "no underruns" from "this target does not count them". The native implementation is owed:
  a device-callback path can still miss its own deadline, and counting that would make the same
  question answerable on a desktop. It is also the lever that would make the whole ring path
  testable off a browser, which is why the browser mixer has no test today.

- **LANDED (2026-09-14) - the gemma family at parity on the KHR cooperative-matrix arm.** A card
  without `VK_NV_cooperative_matrix2` (or the mode forced, `DASLLAMA_COOPMAT=mm`) ran every gemma
  prefill at 0.33 to 0.82 of llama.cpp b10660's own KHR path (`GGML_VK_DISABLE_COOPMAT2=1`, the
  same card): the attention on the 8-row and 4-row scalar tiles, the E-series' per-layer-embedding
  projection on the CPU, every Q8_0 plane on the q8-fed mul_mm L-tile in one k chunk. Four levers,
  each pod-confirmed: the KHR flash tile (now `FaT`'s KHR arm, 16x16x16 fragments; gemma-3-1b's attention
  12578 -> 1857 us a 512-row window, E2B's 37392 -> 3547, the 12B's 83941 -> 5746), the KHR f16 GEMM
  for the projection (`F16GemmKhr`; E2B's window 102 -> 44 ms), q8 on the KHR kq tile through
  `khr_stage16` (E2B's down 14021 -> 5165 us with the split), and the wave model's k chunks on the
  KHR arm (E2B's down 48 workgroups on 82 SMs before it). Every dense carrier now sits at or above
  parity under `mm` (pp512 / tg128, ours against llama.cpp's KHR path in tok/s, the llama.cpp figures
  `external`, five reps, one model a process): gemma-3-1b Q8_0 31751 / 27962 and 373.5 / 330.4 (1.136 /
  1.131), gemma-2-2b Q8_0 14909 / 13618 and 207.8 / 202.7 (1.095 / 1.025), gemma-3-4b Q8_0 10354 /
  8637 and 145.3 / 140.8 (1.199 / 1.032), gemma-4-E2B Q8_0 13748 / 12586 and 190.8 / 180.5 (1.092 /
  1.057), gemma-4-E4B Q8_0 7556 / 6511 and 108.9 / 109.3 (1.160 / 0.996), gemma-4-12B Q4_K_M 3514 /
  3048 and 79.98 / 80.08 (1.153 / 0.999), gemma-4-12B Q8_0 3395 / 2907 and 54.88 / 53.16 (1.168 /
  1.032), gemma-4-31B Q4_K_M 1253 / 1025 and 35.89 / 35.80 (1.222 / 1.003). The 26B-A4B is the gap
  as measured then: its routed block was cm2-only, so under `mm` the resident driver declined and
  the UD-IQ3_XXS read 328 / 39.1 against 4180 / 129.3 `external` (0.078 / 0.303) on the per-op
  rails; the resident block serves a KHR-mode card now, and the row's re-measurement is owed
  (`followup_vulkan.md` item 66).
  Decode is mode-independent. Provenance, direction-grade: the RunPod RTX PRO 4500 Blackwell 32 GB
  (82 SMs) with the device created in KHR mode, `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128
  -t 16` under `DASLLAMA_COOPMAT=mm DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048
  DASLLAMA_GPU_VRAM_MB=31000` (the image rail off: a forced mode re-mints the image per box identity),
  against the prebuilt llama.cpp b10660 Vulkan `llama-bench -ngl 99 -fa 1 -t 16 -r 3` under
  `GGML_VK_DISABLE_COOPMAT2=1` on the same pod; the per-role figures from the resident prefill's
  `DASLLAMA_GPU_PROF=1` window profile. The same tip in the default cm2 mode reads gemma-3-1b at
  0.979 of llama.cpp's cm2 path on the pod (34546 against 35284 `external`).

- **LANDED (2026-09-14) - the gemma family at parity on the RTX 5060 Ti, no kernel work.** The
  pod's levers carry to the 16 GB Blackwell consumer card as they stand: every gemma carrier that
  fits sits at or above 0.95 of llama.cpp b10660 on both rows (pp512 / tg128, ours against
  llama.cpp in tok/s, the llama.cpp figures `external`, five reps, one model a process, the box
  idle): gemma-3-1b Q8_0 22213 to 22900 / 21233 and 248.6 / 214.5 (1.062 to 1.079 / 1.159),
  gemma-2-2b Q8_0 10722 to 11095 / 10737 and 121.1 / 116.8 (0.999 to 1.033 / 1.037), gemma-3-4b
  Q8_0 6321 / 5905 and 82.9 / 79.7 (1.070 / 1.040), gemma-4-E2B Q8_0 9689 / 8372 and 121.9 / 110.2
  (1.157 / 1.106), gemma-4-E4B Q8_0 4333 / 4328 and 65.06 / 63.34 (1.001 / 1.027), gemma-4-12B
  Q4_K_M 2120 / 1998 and 46.9 / 46.0 (1.061 / 1.019), gemma-4-12B Q8_0 2251 / 2156 and 30.04 /
  29.84 (1.044 / 1.007; the Q8_0 file at `DASLLAMA_GPU_MIN_CTX=1024`, its 11.8 GB against the
  card's 16; the 26B and 31B files do not fit and decline on memory). The small carriers' prefill
  on this box is bimodal by the process: unconfined, seven of fourteen gemma-3-1b runs read 19.3 to
  19.7 thousand tok/s (0.91 to 0.93) and the rest 22.7 to 23.3 thousand, the rep spread inside a
  run small either way, while llama.cpp holds 21.2 thousand (`external`) across every run; the same
  process confined to sixteen logical processors (`start /affinity`, either end of the 3990X's 128)
  reads the high level six of six times (22.2 to 22.9 thousand) and gemma-2-2b 10.7 to 11.1
  thousand against its unconfined 10.3 +- 1.0. The figures above are the confined runs for those
  two; the rest are the unconfined record sweep. The cause is the host pacing of a 25 to 50 ms window over a many-CCD
  part - `followup_general.md` item 148. Provenance, direction-grade: Boris's box (Threadripper
  3990X, RTX 5060 Ti 16 GB, 36 SMs, driver 616.56, `VK_NV_cooperative_matrix2` with the decode
  vector arm live), `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128 -t 16` under
  `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048` against the local llama.cpp b10660 Vulkan
  `llama-bench -ngl 99 -fa 1 -t 16 -r 3`; the 12B Q8_0 row from the pass's first sweep - in the
  record sweep the desktop held 4.3 GB of the card and both sides spilled to host memory (ours
  64 / 4.4, llama.cpp 793 / 4.9 `external`), a reading of nothing.

- **LANDED (2026-09-13) - the gemma family at parity on the Vulkan resident driver.** Every gemma
  carrier the model table stocks sits at or above 0.95 of llama.cpp b10660 on both rows, seven of
  nine prefills and two decodes ahead of it (pp512 / tg128 ratios, five reps, one model a process):
  gemma-3-1b Q8_0 0.974 / 1.038, gemma-2-2b Q8_0 1.023 / 0.970, gemma-3-4b Q8_0 1.017 / 0.988,
  gemma-4-E2B Q8_0 1.038 / 0.981, gemma-4-E4B Q8_0 0.994 / 0.950, gemma-4-12B Q4_K_M 1.026 / 0.950,
  gemma-4-12B Q8_0 1.010 / 0.987, gemma-4-26B-A4B UD-IQ3_XXS 1.071 / 1.025, gemma-4-26B-A4B UD-Q4_K_M
  1.128 / 0.950. The last three prefill levers of the arc, each pod-confirmed: the split-k arm's k
  offset masked to its chunk alignment (unmasked, one chunk of gemma-3-1b's down ran 304 us against
  the whole GEMM's 152, the probe's `splitk` arm on the pod, so every split lost) with the tile and split picks folded into one wave model (`cm2_gemm_pick`; gemma-3-1b's
  down 152 -> 78 us in four l chunks, E2B's 12288-deep down 268 -> 162 in three); the flash tile's
  f16 O accumulator under a 3 ln 2 row-max bias, its mask pass gated to the edge steps and a
  `[dont_unroll]` KV loop (E4B's attention 6868 -> 4014 us a window, gemma-3-1b's 2671 -> 1844);
  and a partial last weight tile served on the fast path over the plane's last whole 128 rows with
  Q6_K's scales staged (the 26B's 2112-wide shared expert 300 -> 71 us a dispatch, its 704-wide
  experts 26.6 -> 20.5 ms a window). The three decodes at 0.950 (E4B, the two Q4_K_M files) are
  `followup_vulkan.md` item 51's dependent-dispatch gap. Provenance, direction-grade: the RunPod
  RTX PRO 4500 Blackwell 32 GB (82 SMs, driver without `VK_NV_cooperative_matrix_decode_vector`,
  the scalar decode arm), `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128 -t 16` under
  `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048 DASLLAMA_GPU_VRAM_MB=31000`, against the
  prebuilt llama.cpp b10660 Vulkan `llama-bench -ngl 99 -fa 1 -t 16 -r 3` on the same pod; the
  per-dispatch figures from `harness/vk_gemm_probe.das`'s `gemma`, `splitk` and `cm2g:k6` arms and
  the resident prefill's `DASLLAMA_GPU_PROF=1` window profile.

- **LANDED (2026-09-10) - a browser's job queue workers park instead of spinning.** The
  engine's 30 ms spin-before-park window (`g_jobque_spin_us`) was unconditional, and in Chrome
  the 8 spinning web workers cost the caller its core: Pocket TTS (the parrot page's poem, four
  chunks, wasm64, 8 workers in the pool, the M-series box) generated at 1.1x real time with the
  renderer at 680-920% CPU, against 1.4x at ~112% with the workers parked (per chunk, ms:
  prompt 820 / backbone 1850 / codec 3600 spinning, 550 / 1250 / 2900 parked; three runs each
  within 3%). Team dispatch stays on there (0.69 against 0.72 rtf off, the head 98 against 140
  ms). One worker instead of eight, parked or spinning, reads 0.7x (prompt 1370 / backbone 3080
  / codec 5800), so the pool does pay - 2.2x from one to eight - once nothing spins. The
  emscripten default was set to 0 here (`dasllama_jobque_spin_default`) - REVERSED 2026-09-16, see
  the entry above; the desktop keeps its window.
  Provenance, direction-grade: the m1 box (10 cores, the `m1.tune.json` profile); the browser
  figures are the engine's own per-synthesis timing line (`dasLLAMA tts: ... rtf`) read from the
  page's console, the wasm64 release of `examples/dasLLAMA/parrot` served locally to Chrome under
  `DASLLAMA_ALLOW_UNTUNED=1` (the reference kernel bodies, no tune sidecar, so the absolute
  figures are the fallback bodies'), the lab's own toggles flipping the arms between measures,
  the CPU readings `top` over the renderer process; the desktop figure (18x on 7 workers) is the
  same timing line under `-jit` with the box's profile, the same text through the lab's measure.

- **OWED ROWS - the Vulkan cm2 prefill's board cells.** `performance/records/zen2.json` carries
  one das/vulkan cell (Qwen3-4B Q8_0 under `DASLLAMA_COOPMAT=mm`, 2026-07-25) and no cm2 cell,
  while the Vulkan pp arc (the group-aware split-k, the hand-laid four-wide twins, the
  last-layer slice, the parallel embed) made every Vulkan prefill faster -
  the Q4_K_M 1B window 27.8 -> 22.1 ms on the `lcpp_bench --for-debug-purposes` rig,
  stage readings only. `performance/gen_bench_records.das`'s hardware stamp refuses this box
  while the remote-access daemon runs, so the mint waits for the owner's rig window. Owed: the
  zen2 vulkan cell re-minted, plus a cm2 cell (the served default on the 5060 Ti) on a 1B kq
  vehicle, both spawned by `gen_bench_records`, both walls out-of-process, direction-grade
  against the 07-25 row; done when both rows sit in `performance/records/zen2.json` and the
  site records are regenerated.

- **OWED ROWS - the parity pass's ten 1B pp512 vehicles.** The mirror pass (2026-09-03) read
  Q3_K_L at 1.08, IQ3_M at 1.06 and IQ2_XXS
  at 0.98 of the reference on the `lcpp_bench --for-debug-purposes --plen 512 --ngen 0 --reps
  12` rig bracketed by `llama-bench -p 512 -n 0 -r 6`, stage readings only; the board's rows
  for those vehicles are two-rep readings and stand until re-minted. Same refusal as above -
  the hardware stamp declines this box while the remote-access daemon runs. Owed: the ten
  vehicles' vulkan pp512 rows re-minted by `gen_bench_records`, direction-grade against the
  rows they replace; done when they sit in `performance/records/zen2.json`.

- **LANDED (2026-09-09) - the shared expert's K-quant planes are minted beside the q8
  transcode, and the whole-model resident driver reads those.** A MoE's shared expert reached
  the device only as the loader's q8 transcode of the file's Q4_K / Q6_K planes - twice the
  bytes its decode GEMVs read. The loader now keeps the gate/up/down planes in the file's own
  K-quant format beside that transcode (`wsh*_fmt`, `wshk*_offs`; `IMAGE_VERSION` 36, so every
  image re-mints once) and the whole-model resident driver places the K-quant copies. On the
  Qwen1.5-MoE-A2.7B-Chat Q4_K_M twin the device image reads 10107 -> 9756 MB and the token
  7.06 -> 6.12 ms (the `vk_rdec gpu avg/token` line of `benchmarks/lcpp_bench.das --prof
  --jobque-profiling` under `DASLLAMA_GPU_PROF=1`, RTX 5060 Ti) [direction-grade - two commits].
  The pair to read is footprint against wall clock: the device image and the token both fall,
  while the loaded model on a GPU box carries BOTH copies - the loader mints the K-quant planes
  only on a load with a GPU tier armed, so a CPU-only box's planar image is what it was, and the
  tier-armed load of the twin serves 11781 MB of weights against the CPU-only load's 11291 (the
  490 MB: three planes x layers x dim x n_ff_shexp weights in the file's format, k4 +381 MB and
  k6 +109). That growth stands until the per-op and CPU rails read the
  K-quant planes too and the q8 transcode goes,
  which `followup_vulkan 43` owns in its still-open list (the CPU chain's shared expert on the
  same K-quant planes; today the resident-vs-CPU bar carries the two forms' rounding).

- **LANDED (2026-09-10) - the expert stamps of the grid-codebook formats run a 32-deep k step;
  every other stamp keeps 64.** The cm2 tile template's k step (`BK`) is a stamp constant: the s
  and e stamps (the expert schedule's 32-row and 128-row columns, `CM2_TC_E` in the class ladders)
  of iq2xxs, iq2xs, iq2s, iq3xxs and iq3s run 32-deep, every other stamp 64
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default` carries the tile readings behind the split). On the
  RTX 5060 Ti (`benchmarks/lcpp_bench.das -jit --for-debug-purposes -r 3 -p 512 -n 128 -t 16`
  under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048`, the
  x64-gen backend, untuned-stamped; the arms' rates sit in `followup_vulkan.md` item 45):
  Qwen3.6-35B-A3B UD-IQ2_XXS pp512 1.08x with the four-wide decode twin and 1.17x without it
  (`DASLLAMA_VK_DECVEC=0`, the arm a driver without `VK_NV_cooperative_matrix_decode_vector`
  runs), tg128 1.00x on both arms; the Qwen1.5-MoE-A2.7B-Chat Q4_K_M twin (k4 expert planes)
  1.07x on the four-wide arm, where the same stamps at 32 had read behind the 64-deep ones - the
  reason the K-quants keep 64; against llama.cpp b10660 on the same box (`external`, its rates and both builds
  in item 45) the 35B's twin arm moves from 1.06x to 1.14x of its four-wide arm and the scalar arm
  from 0.86x to 1.00x of its scalar arm [direction-grade - one commit].

- **LANDED (2026-09-10) - a split role takes as many k chunks as fill the device by itself.**
  The split pick (since folded into `cm2_gemm_pick`) still lets the dispatch group (a role plus the neighbours it co-runs beside)
  decide whether k splits, but the chunk count is the SM count over the role's own workgroups,
  since a split role serializes its group through the one scratch plane anyway
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`). On a Linux RTX 5080 (84 SMs, driver 580.173,
  the scalar decode arm, `-t 8`, the bench form of the row above at `-n 32`) the 35B's shared
  expert gate and up move from two chunks of 32 workgroups to four of 64: pp512 1.019x and
  1.014x of the two-chunk arm's two reads, tg32 0.99x and 1.00x; the profiled window's
  sh_gate + sh_up 8.25 -> 5.42 ms (`DASLLAMA_GPU_PROF=1`, the same runs; the reference exe's
  rows the ratios below name are `llama-bench -p 512 -n 32 -r 5` on b10660 on the same pod,
  `external`, and `followup_vulkan.md` item 45 carries every absolute rate of this sequence). The
  36-SM card's rows are unchanged by construction: no
  stocked model's window there has a group at half the SMs whose member alone would take more
  chunks than the group (the 35B's pair fills 32 of 36 and runs whole) [direction-grade -
  one commit].

- **LANDED (2026-09-10) - the cm2 tile loop's hand unroll is one superblock per block (`UNR`: 4
  steps of 64, 8 of 32) instead of eight steps.** The decode inlines once per unrolled copy, so an
  eight-copy 64-deep stamp's code, refetched after each window's weight stream had passed the L2,
  cost a 64-workgroup GEMM 16 us of its 27 on the Linux RTX 5080 (`harness/vk_gemm_probe.das --
  cold:k6`'s flush row: 43.0 us against 26.7 warm; 3 us on the RTX 5060 Ti), and four copies run
  the m tiles 10-14% faster hot there (`harness/vk_gemm_probe.das -- cm2:k6` gate m 57.3 -> 64.5
  TFLOP/s, q/wo m 51.1 -> 55.5; the l and s tiles' sweep sits in `followup_vulkan.md` item 45,
  `ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`). Pod (the bench form of the two rows above at `-n
  32`, `-t 8`, the scalar decode arm): pp512 1.021x of the eight-copy form, tg32 1.007x; the
  32-deep expert stamps at 2 copies read below their eight-copy form with e_gate / e_up longer, at
  8 as before. RTX 5060 Ti (`-r 5`, `-t 16`, same session): the 35B twin arm within noise of the
  eight-copy form (0.997x and 0.989x on two reads), its scalar arm 0.999x, the Qwen1.5-MoE Q4_K_M
  twin 1.023x [direction-grade - one commit].

- **LANDED (2026-09-10) - the iq2xxs and iq2s scalar decode callbacks in pair form.** The callback
  derives every shared read from the pair's first element and selects the element last
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes`), so the driver's two-wide callback commons the
  pair's work. Linux RTX 5080 (the scalar decode arm, the bench form of the rows above): the skewed
  expert schedule's e+s ladder `moesk:iq2xxs` 0.357 -> 0.316 ms (gate/up) and 0.360 -> 0.320 (down),
  `moesk:iq2s` 0.428 -> 0.380 and 0.484 -> 0.432 (`harness/vk_gemm_probe.das`); the 35B pp512
  1.054x of the single-element form, tg32 0.997x (0.805x of the reference exe's 5217 there,
  `external`). The four-wide twin arm (the RTX
  5060 Ti's) never runs the scalar callback, so its rows stand [direction-grade - one commit].

- **LANDED (2026-09-10) - the iq2xs, iq3s, iq3xxs, k5 and k6 scalar decodes take the pair form
  too, the K-quants dropping their byte2 lane selects for the shift form on the way.** Linux RTX
  5080, the scalar arm, `harness/vk_gemm_probe.das -- cm2:<fmt>` hot rates: k6 gate l 85.6 -> 94.3 TFLOP/s, m 65.4 -> 70.5, s
  27.6 -> 34.6, q/wo l 71.0 -> 78.2, m 56.1 -> 60.7, the shared expert's down (64 workgroups of
  K 512) 22.6 -> 21.5 us; k5 gate l 83.9 / m 62.7, q/wo l 69.9 / m 54.0, the shared expert's
  gate/up m stamp 105 -> 81 us, its down 31 -> 25 (the k5 rows before were read at the eight-copy
  unroll, so they fold that lever in); `moesk:iq3s` e+s reads 0.326 / 0.347 ms (no earlier pod
  row). The 35B's profiled q 3106 -> 2918, k 1205 -> 1103, v 1025 -> 940, wo 1974 -> 1815, the
  shared expert's three stamps 6653 -> 6430 us; pp512 1.008x of the single-element form (`-r 3`
  against `-r 5`), tg32 1.00x (0.812x of the reference exe's 5217, `external`) [direction-grade -
  one commit].

- **LANDED (2026-09-10) - the deltanet scan runs one column per lane cluster, k and q read from
  the read-only conv plane per lane, no shared staging and no per-token barrier.** Linux RTX
  5080, Qwen3.5-0.8B-Q8_0 (18 recurrent layers): the profiled scan 8854 -> 8084 us per window,
  pp512 1.033x of the staged form (0.746x of the reference exe's 29957, `external`), tg32 within
  noise (0.985x); the 35B 0.996x (noise). The same kernel without `@readonly` on `conv` and
  `smalls` read the scan at 19004 us and pp512 at 0.68x of the staged form: the NonWritable
  decoration is what lets a token's k and q loads pass the previous token's o store
  [direction-grade - one commit].

- **LANDED (2026-09-10) - every Vulkan binding no kernel of its class writes is NonWritable: the
  `[vk_dispatch]` lens stamps `readonly` from its access classification.** Only four bindings in
  the kernel file carried the qualifier by hand; every other buffer was declared writable, so the
  driver ordered a kernel's loads behind its stores to any other buffer. Linux RTX 5080: the 35B
  pp512 1.108x of the hand-qualified form (0.896x of the reference exe's 5217, `external`), tg32
  1.028x; the 0.8B pp512 1.043x (0.778x of 29957), tg32 1.056x (0.705x of 480); the 0.8B's
  profiled conv 1685 -> 1344 us per window, gate 1091 -> 1000, up 1007 -> 916, the decode step's
  total 3685 -> 3478 us per token; the sanity logits bit-identical. A same-run float4 form of the
  scan's lane shards (a lane's 16 rows contiguous, four loads per operand) read the scan and the
  window slower, and is not kept: the reference exe's shader interleaves its rows exactly as the
  scalar form does [direction-grade - one commit].

- **LANDED (2026-09-10) - the deltanet scan computes its per-token gates once per workgroup
  into shared memory.** The recurrence paid the decay (`exp(a * softplus(g + dt))`: exp, log,
  exp) and `sigmoid(beta)` (exp and a divide) per token per lane, with two global loads; the
  ablation that replaced them by constants cut the scan 7816 -> 5156 us per 0.8B window (the
  cluster reductions' ablation 7816 -> 6585, the ds-as-literal experiment 8060 -> 7816). The
  workgroup now fills two `DN_WINDOW`-long shared arrays before the token loop and the loop reads
  them. Linux RTX 5080, Qwen3.5-0.8B-Q8_0: scan 8060 -> 5240 us, pp512 1.146x of the per-lane
  form (0.892x of the reference exe's 29957, `external`), tg32 within noise; the 35B 1.022x at
  five reps (0.915x of 5217; a three-rep read was noise), tg32 1.00x; the sanity logits
  bit-identical on both [direction-grade - one commit].

- **LANDED (2026-09-10) - the window chain's MoE router and deltanet beta/alpha GEMMs ride the
  cm2 tile as f16 GEMMs (`F16GemmCm2`, eight k chunks into the split-k scratch, the reduce
  into the target).** The scalar tiles were bound by shared-memory traffic: the router at 53 us
  per layer for 0.27 GFLOP (82 before the NonWritable lever), the 16 x 16 beta/alpha tile at 94
  us per layer for 0.13 GFLOP, against the reference exe's f16 mul_mm at 10-20 us. Linux RTX
  5080, the 35B's profiled window: ba 2828 -> 1170 us over 30 layers, router 2125 -> 1621 over
  40 (three dispatches each under the drain: the f16 convert, the GEMM, the reduce); pp512
  1.039x of the scalar-tile form at five reps (0.951x of the reference exe's 5217, `external`),
  tg32 1.007x; the sanity argmax the same token at a logit 0.02 apart (f16 rows and activations
  where f32 ones were); the 0.8B unchanged (q8 beta/alpha planes, no router). The route costs a
  MoE model an f16 mirror of every router plane - `n_expert x dim` halfwords a MoE layer, minted
  only where the shape admits the route and counted by the resident plan (`resident_router_bytes`)
  [direction-grade - one commit].

- **LANDED (2026-09-10) - the deltanet conv is channel-major: a workgroup owns 256 channels
  over 32 positions, a thread slides one channel's window with the taps in registers, the
  per-head norm crosses lanes by shuffles and warps by one shared row.** The position-major form
  (one workgroup per position, the row staged in 32 KB of shared memory) re-read every input row
  once per tap and the whole tap table once per position from L2. Linux RTX 5080: the 0.8B's
  profiled conv 1351 -> 824 us per window (75 -> 46 us per layer; the reference exe's SSM_CONV
  27-47, `external`, its `GGML_VK_PERF_LOGGER=1 llama-bench -p 512 -n 0` run), pp512 1.029x of the
  position-major form (0.919x of the reference exe's 29957, `external`), tg32 within noise; the
  35B's conv 2740 -> 1660 us over 30 layers, pp512 1.010x at five reps (0.960x of 5217), tg32
  flat; the sanity argmax the same token, its logit 0.02 apart (the norm's sum in butterfly order)
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the deltanet scan loads the next token's k, q and v into registers while
  the current token computes.** The per-token chain was latency-bound on its own row's loads (the
  9B's scan 342 us a layer against the reference exe's GATED_DELTA_NET 250: 668 ns a token); now a
  token's loads issue a token ahead and only the recurrence stays on the chain. pp512 on the pod
  (Linux RTX 5080, -r 5, -r 10 on the 0.8B): the 0.8B 1.053x of the unprefetched form (0.919x ->
  0.969x of the reference exe's 29957, `external`; its scan is a quarter of the window), the 9B
  1.002x (its scan role 8193 -> 7736 us a window), the 35B 1.013x; tg32 flat. The deltanet cells
  match the CPU oracle (6 of 6). Dead beside it, all measured on the pod by whole-model rows: a
  forced barrier between the prefill's gate and up GEMMs costs little (they already co-run), a
  hazard barrier naming this op's stage alone leaves the chained-dispatch floor and tg32 where they
  are, and the step computing its own beta/alpha dots in place of the GEMV dispatch reads slower on
  the 0.8B and the 9B (the GEMV co-runs under qkv and z for free; its profile figure was the Linux
  driver's drain at the stamp) [direction-grade - one commit].

- **LANDED (2026-09-11) - the decode GEMV's block loop takes four blocks a lane straight-line, so
  four blocks' loads are in flight before a sum waits on one.** The rolled loop issued a block's
  loads after the last block's sum, one DRAM latency a block; the guarded single step stays as the
  tail. The GEMV probe's `single` arm (`harness/vk_gemv_probe.das`, one plane a dispatch under the
  token's hazard chain) reads the chained-dispatch floor at 4.5 us on every format (0.6 MB planes,
  Linux RTX 5080) and the served planes L2-warm; its ring form reads the streaming rate unchanged
  by the unroll (k4 903 -> 897 GB/s, k5 896 -> 895, k6 902 -> 903 at 4096 x 12288), so the gain is
  the in-token ramp of a plane read once: tg32 on the pod the 9B 1.021x of the rolled loop (0.912x
  of the reference exe's 112.8, `external`; its rows are four blocks a lane at K 4096 and twelve
  at K 12288), the 0.8B 1.004x (one or two blocks a lane: the tail alone), the 35B 1.001x (the
  expert rows under one block a lane), the 27B UD-IQ4_XS 0.995x; pp512 flat. The GEMV family cell runs
  its 13 formats at 5120 wide too (four unrolled steps and a tail at every lane split) beside the
  512-wide rows the tail alone serves [direction-grade - one commit].

- **LANDED (2026-09-11) - the token command's fused add+rms+requant serves every site on every model:
  a Q8_K twin for the K-quant feeds and a row-storing twin for the consumers that read the normed row
  as floats.** The fused twin (`cls_ar_rq`) quantized Q8_0 only and never stored the row, so
  `rd_fuse_gates` kept it off any model with a K-quant consumer, off hybrids (the beta/alpha GEMV
  reads the row) and off MoE (the router reads it) - two dispatches and a barrier per site where one
  would do. Now `ArRqT` stamps `cls_ar_rqx` / `cls_ar_rqk` / `cls_ar_rqkx` beside it, each site's
  stamp (`RqStamp`) picked once every layer is registered (`rd_ensure_fused_sets`: the consumer's
  block form, the row stored where a recurrent head or a router reads it), the layer after a MoE
  layer's combine requanting on its own; the profiler keys on the stamps' recorded names (`rd_ts`)
  instead of a count per layer kind. tg32 on the pod (Linux RTX 5080, -r 5, -r 10 on the 0.8B):
  the 0.8B 1.029x of the split rail (0.79x of the reference exe's 480, `external`), the 9B 1.033x
  (0.894x of 112.8), the 35B 1.019x (1.009x of 145.2), the 27B UD-IQ4_XS 1.092x (0.88x of 47.4);
  pp512 flat. The resident hybrid parity file holds (15 of 15); the
  requant-family cell holds the three stamps to the plain twin (bit for bit) and the Q8_K oracle
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the resident decode's qkv and z GEMVs co-run: the z half of the projection
  row rides its own hazard class (`VHZ_DNZ`, the prefill's z class) and the step waits on both.** Both
  halves wrote under `VHZ_DNP`, so the rail put a write-after-write barrier between two GEMVs that
  read the same row and write disjoint halves; the per-op tier already split them (`VHG_Y1 / Y2`).
  tg32 on the pod (Linux RTX 5080, -r 5, -r 10 on the 0.8B): the 0.8B 1.046x of the shared-class
  form, the 9B 1.016x, the 35B 1.017x (0.991x of the reference exe's 145.2, `external`); pp512
  flat. The resident hybrid parity file holds on the split (15 of 15)
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the fused deltanet decode step runs the delta rule with every thread owning
  one state column's row part in registers, its norms and q.k by subgroup adds, and the beta and
  alpha rows as one GEMV dispatch.** The step read the state column-per-thread in a rolled loop
  (each load's latency exposed in turn) with three single-thread loops over the head's 128 values:
  25 us a layer on every hybrid (Linux RTX 5080), the beta/alpha pair two 32-workgroup dispatches
  with a hazard barrier between. Now: the 64 loads of a thread's row part issue straight-line
  before a sum waits on one, the state held in registers from the k.S / q.S pass through the
  update; the q and k squared sums, the raw q.k and the out-norm's sum by `subgroupAdd`;
  `RouterGemvF16` reads its rows as half pairs and lands the alpha half at a second base
  (`RouterArgs.ne_split / obase_split`). Isolated (`harness/vk_gemm_probe.das dec`, Linux RTX
  5080): the step 8.8 us at 32 or 16 heads of 128, the GEMV 4.6 / 3.0 / 4.7 us at the 9B's, 0.8B's
  and 27B's shapes; the token profile's step role on the 0.8B 456 -> 222 us a token (25.3 -> 12.3
  a layer), ba 380 -> 310. tg32 (pod, -r 5, -r 10 on the 0.8B): the 0.8B 1.054x of the rolled form
  (0.70x -> 0.74x of the reference exe's 480, `external`), the 9B 1.048x (0.81x -> 0.85x of 112.8),
  the 35B 1.068x (0.91x -> 0.975x of 145.2); pp512 flat on all three. A first form with the load
  guarded inside the unrolled loop read SLOWER than the rolled form: a guard per copy puts each
  load in its own block, so the sums wait on the loads one by one. RTX 5060 Ti four-wide arm, the
  0.8B: 1.007x. The step and router GEMV cells match their CPU oracles, the GEMV cell gains
  the two-base arm [direction-grade - one commit].

- **LANDED (2026-09-10) - the k4, k5 and iq4xs cm2 tiles stage a scale cache: the tile's 128
  rows' eight sub-block scales (the K-quants' (d x sc, dmin x mn) pairs, IQ4_XS's d x (ls - 32)),
  filled once per superblock into shared memory, the decode reading one word pair where it read
  the scale plane per element.** The reference exe's `shAscales` form (its Q4_K and Q5_K tiles
  alone carry it; it has none for IQ4_XS). Linux RTX 5080, the scalar arm, `harness/vk_gemm_probe.das
  -- cm2:k5`: gate l 83.9 -> 116.2 TFLOP/s, m 62.7 -> 99.9, q/wo l 69.9 -> 95.9, m 54.0 -> 86.7,
  the shared expert's down 25 -> 17 us; `cm2:k4`: gate l 119.5, m 102.0, q/wo l 100.5, m 87.1 -
  past the reference exe's own 93-96 on the gate shape (`external`, its `GGML_VK_PERF_LOGGER=1
  llama-bench -p 512 -n 0` run); `cm2:iq4xs`: gate l 109.4, m 102.9, q/wo l 91.4. Whole-model rows
  (`benchmarks/lcpp_bench.das -jit --for-debug-purposes -r 5 -p 512 -n 32 -t 8` under
  `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`, the pod): the 35B pp512 1.016x of the
  per-element scale read (0.970x of the day's reference row, `external`; its q5_K roles are the
  qkv, z, q and shared-expert planes), tg32 flat; the Qwen3.5-9B UD-Q5_K_XL pp512 1.175x (0.760x ->
  0.892x of the reference row), its profiled window 116 -> 99 ms (the q5_K FFN roles 18.2 / 17.9 /
  16.6 -> 13.9 / 13.7 / 13.9 ms, qkv 11.2 -> 9.6, the attention layers' FFN 16.0 -> 13.4); the
  Qwen3.8-27B UD-IQ4_XS pp512 1.142x (0.892x -> 1.014x of the reference row), tg32 flat; the
  Qwen3.8-27B UD-Q3_K_XL pp512 1.087x (0.883x -> 0.959x; its q3_K roles stage no cache), tg32 flat.
  RTX 5060 Ti, the four-wide arm: the 9B 1.028x (the twin already shared one scale extraction
  across four elements), the 35B flat. The k4, k5 and iq4xs tile cells match the CPU oracle on all
  five arms [direction-grade - one commit].

- **OPEN (narrowed) - the gemma3v encode residual after the tower flash: ~0.92x vs the
  pair.** The slab road closed in three landings: the 96 head pad (guarded AV columns,
  668 -> 486 -> 452), then the LIFTED dk72 flash (MetalTowerFlash + the per-head-contiguous
  f16 K/V restride - a strided compact K/V scatters every simdgroup tile; the adoption race
  lives in `benchmarks/attn/bench_metal_pf_fused_attn.das`'s TOWER/PORTC arms and the PR).
  Cell shape now (lcpp_bench image cells, m5): gemma-3-4b 404 ms and gemma-3-12b 407 vs the
  pair's 370/380 [direction-grade - two processes]. The
  remaining ~30 ms splits across the per-layer hc-cvt dispatches (~8 ms), the ledgered
  post-CPU-window commit slack, and race-vs-cb occupancy; next levers if reopened: fold
  the hc restride into the K/V GEMM epilogues (dual-store), a Q=16/C=128 flash variant via
  the disasm discipline, and the flash's masked ragged-npos tail (would admit the qwen
  towers, whose ~1200-row attention is small anyway).

- **LANDED - the mul_mm prefill billed ceil-32 M tiles, so an npos just past a tile boundary
  paid a whole extra tile-row of wall for rows nothing reads; the GEMV-tail dispatch peels
  npos % 32 in [1,8] onto the fixed-B mv family (measured 2026-08-24, M1 Max, the qwen
  img:pp chase; plan: `qwen_pp_plan.md`).** The dig that got here killed two prior theories:
  the "8B small-M text GEMM gap" (-6% at p341) was the cold-first-process page tax - the warm
  re-run prices das +2.1% AHEAD (never conclude from run 1, struck AGAIN); and the "4B
  wide-staging tax" was dead too - the one-process probe (`harness/image_turn_probe.das`,
  three workloads x five skip arms) put the ds-wide quantum at +0.1 ms over narrow and the
  whole in-process image tax at ~+6 ms, all in the attention family. The real structure is
  the M staircase: 8B text p320 = 551 ms (10 tile-rows), p321 = 599, p352 = 600 (released
  exe walls, the bench's own driver lines) - one
  tile-row = ~48 ms, and the image turn (321 = 10x32+1) bills it for ONE real row. The tail
  dispatches floor(npos/32) tiles + the remainder rows on the mv GEMVs; a lone row rides the
  reduction-split decode GEMV - the mv forms idle half their lanes on it (probe before/after
  across the two commits, direction-grade, tokens wall 599.2 -> 575.4 -> 566.8 ms: the
  mv-only tail -24, the r==1 reroute -8.6).
  Per-form reduction alignment gates the peel (the b4 stripe reads whole 128-quant rounds;
  kdim % 128 != 0 keeps the padded tile - dims 576/896/1152 misread otherwise,
  device-probed), and r >= 2 always takes the b4 form (b2's stripe needs % 256). The mv_b2
  store now gates on nrows - defensive: at nrows == 1 it phantom-wrote its second row, an
  unread pad row or an off-the-buffer write depending on which batch path sized y.
  Results (released exe, r=3, quiet M1 Max, fresh upstream pairs): 8B img:pp 522 -> 552
  tok/s (-7.5% -> -2.2%), 4B img:pp 942 -> 986 (-2.7%), 8B text p321 534.5 -> 566.7 (+7.8%
  ahead); p341/p512 unchanged (remainders outside the window); 30B Omni img:pp 783 (rides -
  its FFN is MoE, out of tail scope). The span-attention tax split on the finer knockouts
  (attn_qk/attn_sm/attn_av x span-size scaling): ~3 ms was a CONSTANT AV overhead - with the
  span armed the AV kernels walked every query tile to np32, dropping the causal limit
  wholesale; the bound is now tile-conditional (max(uend, causal reach) only where the tile
  holds span rows - a pure shrink over softmax-zeroed columns, the f16 fused-vs-splice
  witness reads 0 with it). The 1-row-span tax fell 3.3 -> 0.58 ms (small-media turns win);
  at the 300-row turn the remaining ~6 ms is INTRINSIC non-causal pairs - the image cell
  does not move. The stage/split figures here are `harness/image_turn_probe.das` readings
  (one process, best-of-3, the `DASLLAMA_METAL_PREFILL_SKIP` arms) - direction-grade; the
  Results line above is the record-grade board. **OPEN followups: (1) the post-encode
  context residue (~10 ms on 8B - the bench-context image eval vs the probe's in-process
  wall: gpu 577 vs 570.3 ms = ~6.7 GPU cache aftermath of the per-turn tower encode, and
  total-minus-gpu 5.6 vs 1.8 ms = ~3.8 host commit slack;
  lever = encode-ahead overlap, shared with the decode per-step-overhead entry); (2) the kq
  mul_mm sites and the MoE prefill GEMMs take no tail yet (the kq mv twins exist); (3) r in
  [9,15] via a third mv pass (~12 ms more on the shapes that hit it); (4) PRE-EXISTING: the
  batch-DECODE mv rail dispatches the same unguarded-stripe kernels with no reduction
  alignment clause - B in [2,8] on a model whose dim/qd/hidden is % 128 != 0 (576/896/1152
  class) misreads; its fix wants a failing test first, own arc; (5) the b2/b4 y binding
  carries no @span, so each peeled site pays a conservative whole-buffer barrier -
  correctness-neutral, but it confounds per-r timing comparisons.**

- **LANDED - the Metal q8 decode GEMV family ran one-row-per-simdgroup with a full-n walk,
  spilling the x vector past L1 from n=4096; the reduction-split form takes every q8 model
  (measured 2026-08-24, M1 Max, the decode-tg chase).** The dig: qwen3vl text cells were
  never on the ladder - fresh pairs showed tg -9.6% (8B) / -11% (4B) vs upstream with pp at
  parity; the per-step trace (`benchmarks/decode_step_trace.das`, -p 0) put 99% of the step
  in GPU execution (gap 0.5 ms, sched 0.1 ms); the knockout ladder (the same trace under
  `DASLLAMA_METAL_DECODE_SKIP` arms) attributed 86% of the step to the GEMV class at 309-334
  GB/s achieved vs upstream's ~352 effective; the GEMV lab
  (`benchmarks/matmul/bench_metal_gemv_kernels.das`) at new 8B shapes
  showed the production form dominated at every shape, small-n included - the old "at the
  wall" verdict had measured the form's own ceiling (the per-shape rates settle in the lab's
  own report, per benchmarks/REVIEW.md). Mechanism: one row per simdgroup walks the FULL
  reduction, so the shared x stream (n x 4B) spills L1 at n>=4096 and every weight load
  pairs with a stalled x load; the fix splits the reduction across the tg's 4 simdgroups
  (n/4 window per sg, L1-resident at every decode dim) and shares each x load across 2 rows,
  reducing cross-sg via tgmem + two barriers. Swapped: `MetalQ8Gemv` (wo, dense w2, logits,
  ASR-decoder sites), the fused `qkv_rs16/rs32` (one tg per rope pair) and `w13sw16` (one tg
  per hidden row); `MetalDnBa` (d = dt_rank 16..64) keeps the old shape for occupancy.
  Results (tg, das vs upstream; the released `lcpp_bench` exe, r=3, quiet M1 Max): 8B
  36.4 -> 39.3 (-2.3%, was -9.6%), 4B 62.3 -> 65.4, E2B
  96.2 -> 98.8 (+15% lead), E4B 52.8 -> 55.3 (tie -> +4.3% ahead), gemma-3-4b 66.2, Omni-30B
  71.2; pp cells unchanged everywhere. NOTE the fused-kernel ports measured near-zero on the
  8B (qwen3 never dispatches qkv_rs - qk_norm disqualifies fused_qkv - and w13sw's two weight
  streams already amortized x); they pay on the families that do dispatch them. **OPEN
  followups: (1) the fixed per-step submission overhead (~0.5 ms GPU-idle gap + ~0.3 ms
  encode) is now the WHOLE remaining deficit - it is why qwen-4B/gemma-3-4b sit at -6..-7%
  and the 8B at -2.3% (encode-ahead is the lever); (2) residual kernel headroom - the qkv
  class runs below the form's isolated lab rate in situ (knockout ladder vs the lab's own
  report).** E2B attribution for the ladder
  lead: the non-GEMV half (fused chains) costs das ~2.7 ms/token vs upstream's ~5 - the win is
  op-fusion overhead on the machinery-heavy E-series, not GEMM speed.

- **MOSTLY LANDED (was OPEN, narrowed by slice J) - the first Metal submission after a
  CPU-only window repaid the kernel-side driver window; the residency-set HEARTBEAT removes
  ~85% of it (measured 2026-08-23, M1 Max; probe = `harness/residency_ramp_probe.das`, the
  qwen3v Metal tower encode after controlled CPU burns, submission split via
  `das_metal_boost`'s `metal_submit_trace`; plan: `qwen3vl_plan.md` slices M/J).**
  The four-timestamp split localized the whole ramp to drv = kernelStart->kernelEnd (queue
  and GPU execution FLAT): warm 0.11 ms, 200-1000 ms burn ~1.2 ms, 3000 ms burn 17.7 ms.
  The long-window mechanism is the OS collecting a committed+requested `MTLResidencySet`
  during inactivity - the cure is a background thread re-calling `requestResidency` every
  5 ms: `metal_residency_set_heartbeat` in dasMetal (5 ms cadence, keep-alive countdown),
  kicked from `residency_flush` per served step, knob `DASLLAMA_METAL_HEARTBEAT_S` (default 180 s,
  0 = A/B rail). Probe ladder at the 3000 ms burn (drv ms, x2 each): baseline 17.7/17.8,
  heartbeat 2.95/3.01, whole-map anchor buffer 18.5/18.1, heartbeat+anchor 2.94/3.00 - the
  heartbeat takes the win alone. REFUTED by that ladder: the whole-map no-copy anchor (one
  pinned unbound buffer over the full mapping - page pre-wiring changes NOTHING in either
  arm, so neither the collect nor the residual is host-page work) and the previously
  proposed **per-buffer tracking audit** (resource count is not the lever; DEMOTED).
  Earlier refutations stand: driver wake (empty cb 0.107 ms), per-page faults, light
  pulse-trains, denormals, affinity, cb-split counts, queue identity; prefill pools stay
  tracked (untracked reds `test_metal_prefill_parity`). **OPEN residual:** ~1-3 ms drv on
  the first submission after a multi-hundred-ms window, scaling with window length,
  insensitive to residency cadence AND page pinning - the driver/GPU idle-state wake class;
  no user-space lever found (pulse trains already refuted), live with it. **Post-merge
  bench confirmation (2026-08-24, fresh rig at the merged master):** the heartbeat also
  removed the ~40 ms commit->execution slack slice J could only name inside the image turn
  (timed prefill 652 -> 612 ms) - qwen img:pp gaps halved, 4B/8B -16% -> -7%, 30B -4%
  unchanged, encode flat; the encode->prefill hand-off IS a CPU-only window.

- **OPEN - arming Metal makes the CPU q8 tower encode ~1.7x slower (measured 2026-08-23,
  M1 Max, 8B qwen3v tower: 3.26-3.45 s with MetalMode.required vs 1.90 s on the CPU leg,
  same t=8; probe = a quiet `-jit` run with the rig's tune manifest, leg = the released
  `lcpp_bench` exe).** Reproduces in the quiet probe, so it is not the bench harness. Mechanism
  unnamed - worker placement / QoS when the Metal queue is live is the suspect class.
  Costs every Metal-leg image cell ~1.4-1.6 s of encode today; the Metal tower deletes the
  arm entirely, but the mechanism matters wherever a CPU encoder coexists with Metal serving.

- **RULED AND LANDED - the spliced image prefill trailed upstream on Metal; the fused span
  eval closed it (measured 2026-08-21, M1 Max, the fused-image-span arc).** The record-grade
  gap was das/upstream **0.52-0.62x** `img:pp` across all three vision models (E2B 687 vs 1101,
  E4B 362 vs 658, 12B 140 vs 268 tok/s) while das led every CPU prefill, encode and decode
  cell. Two hypotheses refuted by per-slice probes (`eval_embd_slice_` timing +
  `metal_prefill_stage_stats`): NOT a CPU fallback (all quanta served on Metal, blob models
  exempt the `min_npos` floor, zero declines) and NOT submit/readback (gpu ~ wall, CPU-side
  ~ 4 ms/turn). Also refuted: "upstream runs one graph over the whole span" - it issues the
  SAME three decodes (one per chunk, causal-flag toggled around the image
  chunk). The measured mechanism is the **pad-64 GEMM floor**: the mul_mm M tile pads every
  quantum to 64 rows (`mp = ((npos+63)/64)*64`), so the ~5-token head and ~21-token tail each
  billed a full 64-row GEMM pass through the stack - E2B slices 58.7 / 123.2 / 54.6 ms and
  head+tail 48 % of the wall for 17 % of the positions, measured by a temporary -jit per-slice
  probe (a `ref_time_ticks` wrap in `eval_embd_slice_` + `metal_prefill_stage_stats`, both
  reverted; `harness/prefill_msweep_probe.das` is the kept instrument), while the rows slice
  alone already ran at the upstream whole-span rate. **The fix:** `eval_embd_span_` issues ONE eval with a per-query
  mask - span rows [ulo, uend) uniform, head/tail causal - through the CPU flash/blocked/
  classic arms and `AttnArgs.ulo` on Metal (DASLLAMA_VERSION 7); the three-eval splice remains
  the vulkan-leg fallback and the `set_span_fuse(false)` parity rail. Record-grade after the
  arc's second lever (the M-pad fix below): Metal `img:pp` E2B **1400.2** vs 1102.6 (1.27x),
  E4B **713.7** vs 663.1 (1.08x), 12B **268.0** vs 270.7 (parity within noise) - das leads or
  ties every Metal image cell, with the decode/encode crowns re-confirmed.
- **RULED AND LANDED - the "kquant small-M kernel deficit" was the 64-row M pad (measured and
  fixed 2026-08-21, M1 Max, the fused-image-span arc round 2).** The suspected ~27 % kq
  mul_mm throughput deficit at image-turn batch sizes was REFUTED by the M-sweep
  (`harness/prefill_msweep_probe.das`, 12B Q4_K_M metal leg): the ms/npos curve is a pure
  64-boundary STAIRCASE - npos=160 cost exactly npos=192's wall (604.6 vs 606.0 ms), 96 cost
  128's - while per-PADDED-token cost is flat 3.1-3.2 ms from 128 to 512; the knockouts put
  the GEMM family at 96 % of the wall at both M. Every prefill kernel's M grid is `mp/32`
  (weight mul_mm, kq twins, attention trio), so the `mp = ceil64(npos)` pad billed a dead
  32-row GEMM block on every short prefill; the fix is `ceil32`. Record-grade image cells
  after (released rig, refs adjacent): 12B **268.0** tok/s (upstream 270.7 - parity within
  noise), E4B **713.7** (663.1), E2B **1400.2** (1102.6) - das leads or ties every Metal
  image-prefill cell. The pad cut also reaches every Metal-served model's short prefill
  (`npos % 64` in [1, 32] - the interactive-prompt shape; pp512 cells sit at % 64 == 0, which
  is why the pad went unnoticed). Full prefill-parity arm set + the gemma2 sliding-window +
  gemma4e/qwen35 family rows green on the new pad.

- **gemma4v ViT tower (E-series): the encode is the image turn's biggest single stage on every
  CPU tier; the q8 lane and the Metal leg both landed (measured 2026-08-19, M1 Max, the gemma4v
  arc).** The tower is 16 blocks x 7 GEMMs (112 bf16 GEMMs, ~151 MMAC per patch) over 1170
  patches for a 640x480 photo (-> 130 soft tokens). Record-grade, from the released `lcpp_bench`
  exe's image cell (gemma-4-E2B Q8_0 off the image rail, `-r 5`): the **CPU row** (the q8 block-
  GEMM lane, `flavor tuned`) `img:enc` **419 ms**, `img:pp` 366 tok/s over the 155-token spliced
  prompt (0.42 s), `img:tg` 41.6 tok/s - the encode is ~ 40 % of a 32-token turn; the
  **`--accel` row** (the +AMX float-batch tier on the file's planes, `flavor accel`) `img:enc`
  **459 ms**, `img:pp` 369, `img:tg` 42.6. Stage-level, from the `g4v.*` `asr_prof_report`
  buckets of a `-jit` probe (`gemma4v_encode` on a 624x480 canvas, `DASLLAMA_CPU_PREFILL=1`,
  three reps, direction only): the exact-plane CPU lane through `matmul_bf16_batch`'s per-row
  `dot_bf16` loop costs ~ 1.9 s per encode (GEMMs ~ 90 %, ~ 92 GMAC/s aggregate - ~10x under
  the q8xq8 prefill kernels), the q8 lane ~ 0.43 s (GEMMs 238 ms, the attention core 163 ms -
  now the #2 bucket), and the Metal block loop ~ 89 ms with the stem (10 ms) and tail (2 ms) on
  the CPU. Prediction P2 (0.9-1.8 s, >= 12 % of the turn, for the exact CPU lane): the share held
  by a wide margin, the absolute landed just above the band. **Ruled and landed in the arc:** the
  q8 tower lane (the gemma4a / parakeet recipe - Q8_0 GEMM planes + per-row requant) is the CPU
  serving default, the Metal leg (`register_gemma4v_gpu`) serves the file's planes on Apple
  builds, and the lane follows the serving path (Metal tower or the accelerate tier -> exact
  planes; else q8). Left on the table: the CPU attention core (163 ms of the q8 lane's 430), the
  per-row `pf_enc_rms` at head width (256-thread groups over 64 elements, ~14k tiny dispatches
  per block pair), and the E2B decoder's own Metal prefill decline ("layers", `required` mode,
  independent of the image span) - until that is served, the gemma4v Metal leg is the only GPU
  piece of an E2B image turn and the `--ngl` image row cannot run.
- **gemma4uv vision embedder: no quantized lane; the dlim rail is owed anyway (measured
  2026-08-14, M1 Max, the vision arc's slice G).** The embedder is two GEMMs (6912->3840 patch
  projection, 3840x3840 output projection). In a product-shaped turn - gemma-4-12B Q4_K_M
  served q8 off the image rail, a 640x480 photo -> 130 soft tokens - the whole image side
  (geometry + letterbox + encode) is **under one percent of the turn** against the
  time-to-first-token and decode the cell reproduces (`img:enc` vs `img:pp`/`img:tg`). Encode alone is ~54 ms at 130 tokens
  (`img:enc`, the cell) and scales with the row count, so the 280-row geometry ceiling bounds
  what any image can cost - re-measure via the cell with a max-geometry input. **A quantized plane pair is DECLINED:** a second
  plane format cannot be worth it against 0.7%.
  [!] **Measurement provenance, because the first pass got it wrong twice:** the original
  numbers came from a one-off script that (1) took `load_model_`'s `QuantMode.fp32` default,
  dequantizing a K-quant 12B and reading 2 t/s instead of 14 - a 7x error - and (2) skipped
  `tune_gate()`, worth a further ~9%. Both defaults are stated automatically by the profiling
  rig, which is why model-level timing belongs there and not in a bench somebody writes. **The `.dlim`
  rail is NOT declined, and the timing above is the wrong instrument for it** - the rail is an
  ownership question, not a speed one. Every other model artifact loads mapped, with no
  allocation and no processing at load; the embedder's `blob : array<float>` is the second,
  array-owning shape `dasllama_audio.das` explicitly refuses to maintain. What it buys is RSS
  (mapped pages are shared and evictable across processes; a 190 MB owned bf16->f32 widening is
  neither) and zero load-time work - not the load wall. **Both are DONE (2026-08-14, the same arc's slices I and J).** The rail
  landed: the embedder stages, mints and maps like the audio towers, and the prepared image maps
  with no load-time work (mmap). Plane format settled as **per-GEMM, following each source tensor's on-disk type** -
  and the premise that the mmproj is uniformly bf16 was HALF WRONG: gemma-4's shipped "BF16"
  mmproj carries an **F32** patch embedder (26.5M params) and a **BF16** projection (14.7M), so a
  both-or-neither rule bought nothing. Per-GEMM gives 200 MB -> 170 MB with tier-1 unmoved to the
  ulp (2.098e-05 before and after) - the bit-for-bit claim about `dot_bf16`'s widen, confirmed.
  Rounding the F32 half down would cost ~0.2% relative, two orders over the tier-1 gate, so it
  stays fp32. The turn is also a rig cell now (`lcpp_bench --image`: `img:enc` 53.8 ms,
  `img:pp` 63.5 tok/s, `img:tg` 14.3 tok/s on the M1 Max), so these numbers are re-measurable
  rather than quoted. **What reopens the quantized half:** a family whose vision side is a real
  ViT (the E-series `gemma4v` tower - 16 attention blocks, not two GEMMs), or batch image serving,
  where per-image cost stops hiding behind one prefill.

- **CPU attention: promote it from the loop-crown tier to the EMITTED tier (named 2026-08-05,
  the E2B deep-clip fade probes; corrected same day after a double-check).** Attention is NOT
  kernel-less: flash prefill (the default mode) tiles with online softmax and rides the
  per-box-tuned `gemm_f32_uk_4x16` for QK^T and A*V, with full f16/q8/tq4 KV codec arms.
  What it lacks is the EMITTED tier the weight GEMMs got: with f16 KV the flash path
  packs-and-converts K/V to f32 first and then runs the fp32 GEMM - no f16-native dots
  (F16C / fp16 hw-convert loads fused into the tile), no int8 QK option, softmax outside
  the tile. The measured tier gap IS the finding: on a gb1-class ~5k-token ASR prefill the
  emitted-tier blk_ffn crosses zen2/M1 at 1.13x while the loop-crown attn crosses at 2.9x
  (16.4s vs 5.6s); the f16-KV default bought only -3% (read bytes halve, the fp32 compute
  stays). It reads fine at pp512 (small share) and hurts at depth - deep prompts and batch
  serving pay the same tax audio does, text board included. **What a fix looks like:** an
  emitted, per-box-tuned attention family - convert-in-tile f16 dots, optional int8 QK
  (requant Q once, vnni/sdot straight against q8 KV - q8 flips from slowest cache format to
  likely fastest), softmax fused into the flash walk. **Why it's a WIN, not catch-up (Boris
  2026-08-05): upstream runs the same dequant-to-f32-scratch seam - an emitted tile is
  'potentially faster' than the ref, and the deep-clip audio cards on the site are the
  ready-made scoreboard.** NOT the quant-lane arc; a kernel arc of its own, eventually.

- **Batched-tg bench row: tg at B=16 sequences is the kernel story decode can't tell at B=1
  (spotted 2026-08-04, zen2 thread ladder).** Single-sequence decode is GEMV - bandwidth-bound
  at any thread count, so das:ref tg reads as parity (t=4 zen2: 13.10 vs 12.59) while pp shows
  +45% on the same box. At B=16 every decode step is a 16-row GEMM: arithmetic intensity x16,
  weight stream amortized across the batch - the same regime pp wins in, and the serving shape
  dasllama.io actually runs. The engine side EXISTS (`eval_batch_` in `dasllama_batch.das`,
  synchronous B-row step; Metal has `metal_batch_decode_forward`); what's missing is only the
  bench row: a `--tg-batch N` mode in `lcpp_bench.das` driving B sessions per step, ref via
  `llama-batched-bench` (separate binary from `llama-bench` - check the ref checkouts build it).
  **What a fix changes:** a board column where the kernel advantage shows in decode, and the
  first honest throughput-serving number for the server story.

- **Decode work-splitting vs thread count: tg on the 3990X ladder read flat 13.1-13.3 at t=4-8
  then jumped +59% to 21.2 at t=12 (2026-08-04).** Read: the per-op split count is mismatched to
  the lane count at some t (undersplit or oversplit, depending), with CCD-spread-vs-pack pin
  placement a secondary suspect (unpinned llama-bench led das tg at t=8, 14.1 vs 13.3). Cost
  today: optics only - single-sequence tg absolutes don't drive engine choices. Why it stays
  ledgered: batched decode (see the batched-tg entry) turns each step into a B-row GEMM where the
  same split/placement question prices real serving throughput; revisit there, not as a B=1 hunt.

- **`BatchLanding.cbs` per-step buffer churn: the `@scratch` recycling never happens on the batch-decode
  pipeline path (spotted 2026-08-02, lint uplift).** `batch_step_build` builds each
  step's command-buffer list in a LOCAL `cbs` and hands it over with `L.cbs <- cbs`
  (`dasllama_metal_decode.das`, the landing stash at the end of the build) - the move drops the slot's retained buffer every step,
  so `finish_pending_step`'s capacity-retaining `clear()` recycles nothing, and on a persistent
  heap the dropped buffer is ~ncb pointers per step. **What a fix changes:** prefill's
  `g_pf_cbs` ping-pong pattern (build into the global, or `push_from` a reference) recycles both
  buffers, deletes the per-step allocation, and makes the PERF026/PERF030 nolints at that site
  unnecessary. Cost today is small (one small alloc + drop per step); the reason to do it is the
  allocation sits on the measured hot orchestrator, so it belongs to a perf pass with A/B cells,
  not a lint sweep.

- **ASR peak-memory census (2026-08-03, m1, /usr/bin/time -l around asr_bench - the
  reproducible ladder is `benchmarks/asr/mem_census.sh`): canary warm footprint
  jfk 2.71 / gb1 4.29 / hp0x2 8.35 GB; canary MINT gb1 4.62 (+0.33 over warm);
  parakeet-v3 warm hp0x2 5.61 GB.** The canary-apples arc closed the 2026-08-02 baseline's
  two findings: the 13.1 GB canary serve peak (fp32 weights + quadratic scratch) is now
  ~4.3 GB on gb1 (q8 end-to-end + per-job attention slabs), the mint spike is +0.3 GB
  (read-time tensor-at-a-time transcode), and hp0x2-length canary - previously one clip from
  swap on a 16 GB box - runs at 8.35 GB. Parakeet's hp0x2 quadratic scratch died with the
  same slab change (-0.6 GB vs its pre-slab control). REMAINING: canary's hp0x2 tail is
  ~4 GB of decoder-side prefill/KV at ~7k soft tokens - map it (KV codec residency, prefill
  panel sizing) before chasing; and whisper's mint spike (1.48/5.29) predates this arc's
  machinery, already ledgered via the streamed mint.

- **Try the borrowed plane WITHOUT its bounds check, at profiling time (spotted 2026-08-01, audio
  `.dlim` arc).** `PlaneF`/`PlaneI8` `operator []` in `dasllama_plane.das` guards every read with
  one unsigned compare. **Costs nothing today, measured:** M1, `-jit`, cache-resident
  4-accumulator sum, 67.1 M elements, 7 interleaved reps - raw pointer 15.608 ms, checked plane
  15.611 ms, and the `array<float>` indexing it replaces 20.856 ms (cv <= 0.6% on every arm). So
  the check ships. **What the removal would change:** in that microbenchmark, nothing - LLVM
  folds the compare out of a loop whose bound it can prove. Real audio kernels index with computed
  offsets (`blob[woff + i]`, layer strides, conv taps) where the bound is not provable, so the
  compare may survive there and the microbenchmark does not answer it. The A/B to run once the
  audio carriers are converted: an `unsafe`/unchecked accessor variant behind a build knob, A/B'd
  on real encode/decode cells per family, interleaved, not on a synthetic sum. **Do not remove it
  on the strength of the number above** - that number measures a loop the real kernels are not.

- **gemma-4-26B-A4B tg 0.84x - routed k4 GEMV was load-issue-bound; float4-x fix SHIPPED, +8.7% tg
  (2026-07-23).** Chapter 1, knockout (new moe_rt/moe_sh arms, decode_metal_chase nomoert/nomoesh,
  Q4_K_M @512, best-of-3, clean window): full 18.80ms; routed k4/q51 expert GEMVs 5.48ms (29%) at
  ~163 GB/s; other GEMVs (QKV/WO/router) 5.30ms; dense-shared 1.89ms; attention 1.93ms; ew 1.96ms;
  non-gemv floor 6.13ms. Classifier ~ ZERO time despite 21% byte share - the greedy spec chain
  covers it (1342 hits / 3 misses). Chapter 2, kernel-lab (benchmarks/matmul/bench_metal_moe_lab
  - exact 26B expert shapes E=128 nfe=704 dim=2816 k=8, exact-arith bit-exact contract, sel
  rotation for DRAM honesty): the barrier/bubble theory REFUTED (pipelined-no-barriers ==
  in-graph rate, +/-7%; scatter-vs-contiguous sel: nil) - the production MoeGemvK4 kernel itself
  ran **142 wGB/s** at expert shapes, load-issue-bound on 32 scalar x loads per 256-block
  (the AGX checklist's vector-width-views item, missing in the MoE twins). float4-x view =
  **321 wGB/s (2.25x)**; W13-pair fusion ties it (327) so NO driver surgery; 4-rows/sg partial
  (202). FIX SHIPPED in MetalMoeGemvK4 (kernel-only; dispatch/driver/blob unchanged): in-graph
  full 53.06 -> **57.65 t/s (+8.7%)**, gpu 18.80 -> 17.31ms/step; kernel units + fam tolerance
  cells green. Chapter 3, the "other gemv" anomaly RESOLVED as a phantom (finer noqkv/nowo
  knockout arms, post-fix @512, full 17.27ms): QKV 2.56ms is AT ceiling - the 26B is HETERO
  (global heads 512-wide: wq 8192x2816 on global layers), so real QKV bytes are ~857MB/step,
  ideal 2.60ms; the old "3x" was a byte-estimate error. WO 1.52ms vs 1.30 ideal (~85%, ~0.2ms
  slack). The residual gemv-class remainder 1.23ms = router (~0.15) + the greedy spec chain's
  classifier dispatches (~1.1ms, gemv-gated, useful work). Routed experts post-fix: 3.84ms.
  Remaining quantified levers, by size: (1) **the dispatch model - MEASURED from both sides
  (2026-07-23 evening, same-window)**: das encodes **780 dispatches/step @8 / 811 @512** (new
  metal_dispatch_call_count instrument), every one implicitly barriered on the serial encoder.
  upstream's own knobs price the structure on this exact model (tg128, +/-0.4% reps): stock 58.50
  t/s; GGML_METAL_CONCURRENCY_DISABLE **52.17 (+2.08ms/tok)**; FUSION_DISABLE 53.90 (+1.46);
  GRAPH_OPTIMIZE_DISABLE 53.96 (+1.44) - upstream-with-serial-encoder lands in OUR class, i.e. no
  kernel section loses; the whole gap is dispatch structure. Same-window das: chase full @8
  **62.3 t/s (16.05ms) - AHEAD of upstream stock 58.50** under real greedy decode; the llama-bench-
  protocol rail reads 55.75 +/-3.20 (synthetic-id feed perturbs the spec chain + the rail's tg is
  intrinsically noisy - the recorded 50.31 +/-2.2 board cell is protocol-dragged and/or tinted).
  The blueprint: a concurrent encoder +
  per-op src/dst range tracking - barrier + reset only when a new op's ranges conflict with the
  live set - plus ew-chain fusion and a graph-reorder pass that grows concurrent sets. Our shape:
  an enc_dispatch wrapper in dasllama_metal_common taking declared read/write (buffer, off, len)
  ranges per dispatch (every enc_* helper already knows its buffers), auto-barriering on conflict;
  g_skip runs must stay serial (the knockout-unbarriered caveat at dasllama_metal_decode.das:2511).
  **SESSION 1 SHIPPED (2026-07-23): the hazard-tracked concurrent encoder is in** - range tracker
  in dasllama_metal_common (exact (buffer, off, len) ranges, mutable frame buffers only; weights/
  uniforms untracked), hz_gate in every decode-path enc_* helper, undeclared dispatches take a
  conservative barrier (or PANIC under DASLLAMA_METAL_HAZARD_STRICT - the parity suite's arm13
  runs strict, so declaration coverage is test-enforced), a dispatch-counter check makes a gate
  bypass loud, DASLLAMA_METAL_HAZARD_PARANOID isolates the encoder flip. Measured on the 26B
  Q4_K_M chase (full arm, best of 3): @8 63.08 -> 65.78 t/s (+4.3%), gpu 15.81 -> 15.11ms; @512
  58.42 -> 60.84 t/s (+4.1%), gpu 17.07 -> 16.40ms; barriers 780/811 implicit -> 634/665 real.
  DEFAULT flipped ON after the family gates (llama arm13 strict, arm7 q8/tq4, gemma4moe both
  rows, qwen35/qwen35moe, gptoss, gemma4e - all green under CONCURRENT+STRICT); 12B Q4_K_M spot
  pair confirms: @8 35.05 -> 36.31 t/s (+3.6%, gpu -0.83ms), @512 32.37 -> 33.34 (+3.0%, -0.88ms).
  The barrier floor is the PROGRAM-ORDER chain depth (~19 genuine links/layer on the g4 graph:
  qkv triple, we1||we3, pre_ffn2||router_norm, post_ffn2||shared-w13sw group - everything else
  chains). **SESSION 2 PHASES A-C SHIPPED (2026-07-23 night): step-graph capture (kn_* twins
  record KNodes; graph_flush replays with the hz oracle deriving barriers, so a schedule can
  only change speed) + a shape-class schedule cache (FNV over the pso sequence, compiled once)
  with ASAP leveling over the conflict DAG + the g4 dense-shared/qwen35moe-shexp scratch split
  (own bh12s panel).** Measured 26B: @8 69.23 t/s gpu 14.40ms, @512 62.90 gpu 15.87 - cumulative
  vs serial @8 +9.7%/-1.41ms (das now well ahead of upstream stock 58.50 same-window); barriers
  634->574 (the split's 2 levels/layer; leveling found nothing else - depth 574 IS the true
  chain). 12B (dense, no branch): unchanged, as expected. 26B/qwen35moe tolerance cells
  byte-identical maxd under STRICT; llama arm1/7/13 token-exact.
  **R1 ew-FUSION MEASURED (2026-07-23): one kernel ships, one refuted - the barrier lever has a
  sub-1% ceiling on the MoE-bound 26B.** MetalPreAddRms collapses the pre_post_norm epilogue
  post_attn_rms + add_rms into one dispatch (enc_pre_add_rms_site: two register reductions, x
  staged for the trailing norm; bit-identical to the pair - reduction structure copied from
  metal_rmsnorm/add_rms, the one reordered product is IEEE-multiply-commutative). DEFAULT ON via
  DASLLAMA_METAL_FUSE; chase `nofuse` A/B arm. Correctness: gemma3 token-exact (pre_post_norm
  fires it), gemma4moe-26b tolerance maxd byte-identical to baseline. 26B Q4_K_M 3-arm interleaved
  chase (full/fuse1/nofuse - Parsec-active, read the DELTA): **fuse1 vs nofuse +0.35% @8 / +0.37%
  @512, gpu -0.05ms, barriers 574->544 (-30 = one collapsed level/layer x 30 layers)** - small but
  consistent + reusable capability (larger on dense gemma: shorter chain, 30 barriers a bigger
  fraction). The SECOND fusion, MetalRmsAdd on the g4 dense-shared post_ffn1_rms + add, was
  authored, correctness-proven, and **REFUTED** by the same run: **-1.0% at both depths and
  DETERMINISTICALLY 0 barrier reduction** (720 vs 750 disp but 544 = 544 bar) - that tail is on the
  SHARED branch, which the leveler already overlaps with the longer routed branch, so fusing rms
  (inputs ready early) with the add (needs bmoe_rt, ready late) SERIALIZES the reduction behind the
  routed branch it used to overlap. Reverted; kept as this ledger's negative control. **LESSON:
  barrier COUNT isn't the whole story - a fusion OFF the critical path can't help and can hurt by
  moving a bigger node into a later, tighter level; and the 26B's barrier lever is bounded (~sub-1%)
  because the GPU is routed-GEMV-bandwidth-bound (round-1 dig), not barrier-bound.** Implication
  for the remaining rungs R2-R5: the [metal_dispatch] macro lens stays worth it as CAPABILITY/eDSL, but the
  fusion+reorder PERF upside on this MoE model is small - the candidate deeper fusions
  (router_norm+router, swiglu+we2) hit the same norm-into-GEMV grid-wide-dep wall or need the GEMV
  kernel itself to fuse the activation (ledger-class). Still open from the plan: [tune]
  schedule axes (thin until fusion adds real choices), batch-rail unification (R4), dasMetal
  promotion of the graph/lens machinery;
  (2) the q51 w2 MoE kernel at 224 wGB/s in-lab -
  the integer-compose form (q | hbit<<4 pre-convert, replacing the select chain) TESTED + REFUTED
  2026-07-23: 226 vs 224 wGB/s (+1%), bit-exact but the dot stays issue-bound in the shift/mask
  decode regardless of compose shape; kept as the lab's w2_ic negative control, do not re-chase -
  a real q51 win needs a different decode strategy (per-thread multi-block amortization or an
  upload-time qh transpose, both ledger-class); (3) WO ~0.2ms slack; (4) MetalMoeGemvK5 carries the same
  scalar-x block - mechanical sibling of the shipped fix, prove via a lab arm first. Lab kept as
  the standing rig: variants stay as arms/negative controls.

- **[metal_dispatch] lens ROLLED OUT across the decode path (2026-07-23): 34 kernels generate
  their dispatch builders from the class; @role ratchet live.** The structure macro
  (dasllama_metal_lens.das) reads a [metal_kernel] class's @ssbo/@uniform fields and generates
  the enc_* builder - binds in @binding order, hz from @role (read/write/readwrite; weight =
  untracked static upload; alias = second view of a dual-bound buffer), @off = caller-passed
  bind offset (param interleaved after its buffer; shared names emit once - dual views ride
  two fields sharing one @off), @span = exact hz byte length (product micro-grammar), grid =
  per-WorkGroupID-dim `1|param|param/coverage` (literal coverage folds the ceil bias to the
  hand-written spelling; named coverage like g_gemv_rows keeps the +cov-1 form), tg = int
  const | "param" | "name*int". Every conversion was proven ast_dump byte-identical against
  its hand-written twin before deletion (only sanctioned diffs: dn_scan's exact-div grid
  emitted as value-identical ceil; order-free hz/bind permutations). Coverage: the simple
  single-PSO set + moe routing chain + deltanet chain + embed/gemv/w13sw/qkv_rs cores + the
  whole attn and rope-store families (14 cores) - variation points stay thin hand-written
  wrappers (fmt dispatchers kq_*/moe_gemv*, pso-parameterized ew2, dummy binds, f16 picks);
  batch rail untouched (R4). Net ~ -690 lines of hand-maintained bind/hz code. RATCHET: a
  role-less @ssbo on a lensed class is now a COMPILE ERROR (the flip immediately caught
  RopeStoreQ8's undeclared bias plane); un-lensed classes stay covered by runtime STRICT.
  Gates: decode-parity suite green under CONCURRENT+STRICT (arm13 coverage proof); six
  fam matrix gates green - 26B q8/k4 tolerance cells BYTE-IDENTICAL to the pre-refactor
  baseline (2.498179/5.1424093, 2.5177252/13.825883), gptoss/qwen35/gemma2/gemma4/gemma4e
  clean; the only reds are the tracked pre-existing metal object leak (30/36/15 by filter,
  unchanged counts). `range` is a lexer type token - the hz-length tag is @span, not @range.

- **.dlim mint abort past ~11GB: root-caused as DISK-FULL, writer fixed, red cleared (2026-07-23).**
  Not an int-width bug - the write rail is 64-bit clean end-to-end (ftello, long_fwrite -> size_t
  fwrite, long_length, uint64 offsets; the Jul-21 29GB mint of this same model and the image
  suite's 5.4GB voxtral arm are standing proof). The disk had filled: 804 GiB of .dlim images
  across ~27 identity generations back to Jul 16 - nothing ever deletes a superseded image, and
  the Jul-22 sweep's ~92GB of MTP mints ate the last headroom. ENOSPC then surfaced as the
  per-plane "offset-accounting" cascade (the quoted `0xf0` was the nbytes field - 240 bytes, the
  30-layer offs plane), and the abort path removed the tmp, hiding the space pressure. Fixed in
  the writer: `write_plane` now reports short writes honestly ("disk full?"), bails the walk on
  the first failure instead of pushing the remaining planes at a full disk, and `save_image`
  verifies the on-disk byte count after close (a buffered small-tail ENOSPC at fclose was
  silent). Swept the 115 dead pre-v5 images (659 GiB; every pre-Jul-22-13:18 file predates the
  IMAGE_VERSION=5 bump and is unloadable by construction); fam-gemma4moe re-mint verified green
  (both 26B rows' engage + tolerance cells pass; the arm's remaining red is the tracked
  pre-existing metal-object leak, 30 objects on this filter). OPEN (design, propose-first):
  .dlim GC - every IMAGE_VERSION/knob change silently orphans the previous generation (~20GB per
  big model). Candidate shapes: fold the tag into the filename and keep-one-per-(gguf, tag) on
  successful save; or utime-touch images on load and age out cold siblings at save time.

- **Q6-greedy spec-chain inversion on big pure-k6 files (2026-07-22 re-pair).** `spec_cls_capable`
  (dasllama_metal_decode.das:164) is a pure CAPABILITY test - it engages the greedy spec chain for
  any tied-k6 classifier with no BENEFICIAL condition, so a big pure-k6 file eats the spec-chain
  work where it is a net loss. Cost today: gemma4-12B Q6_K B=1 greedy runs 26.81 t/s with spec on
  vs 27.11 spec-off (~+1%; `DASLLAMA_METAL_SPEC=0` recovers). This was the board's "0.81x" cell -
  RE-PAIRED to 0.977 on / 0.988 off in a clean quiet window (the 0.81 was window-skew; spec-on had
  drifted 22.4->26.8). Fix: a size/format beneficial-gate (decline spec on pure-k6 above a
  param/layer threshold, picked at the 4B-wins / 12B-loses crossover) in spec_cls_capable or the
  :1888 engagement gate. Small inch (~+1%); deferred (Boris, 2026-07-22).

- **Small-model q8 single-stream GEMV vector-load: TESTED + REFUTED (2026-07-22), do not
  re-chase.** The hypothesis (the blob's +2 quant phase forces 4 scalar int8 loads instead of
  one byte4 vector load, costing ~7-8% at B=1) was IMPLEMENTED as Fix A (a uint16 blob view +
  2 aligned ushort loads + sign-extend unpack in MetalQ8Gemv), proven bit-exact on GPU, and
  measured in a clean A/B (distinct dll hashes): PERF-NEUTRAL-to-slightly-NEGATIVE. gemma3-1b
  B=1 master 192.1 vs Fix A 188.7 (the unpack ADDS ALU where the GEMV is not load-issue-bound);
  qwen3-4B B=1 63.40 vs 63.45 (noise). Root cause of the original "regression": the ledgered
  64.5->59.5 was WINDOW-SKEW, not a real issue-bound cost - the q8 B=1 GEMV is
  bandwidth-bound at the M1 ceiling (qwen3-4B 349 GB/s, gemma4-12B 327 GB/s, both near peak),
  so cutting weight-load issues does nothing. Contrast the gpt-oss expert-GEMV win, which cut
  X-RELOAD issues (a different axis). Reverted. The real q8 B=1 lever is the non-GEMV overhead
  (elementwise dispatch fusion + attention), not the weight-load path - see the ew-fusion
  ledger items below. Kept a byproduct: the gemv kernel-unit now covers the main vectorized
  loop (n=1120), which it never did before (committed).

- **Per-config .dlim: map-only load, BLOB-ONLY metal flavor - SHIPPED (2026-07-18).** The
  contract "no processing on load FOR THE CONFIG IT WAS BUILT FOR" holds: image v3 +
  METAL_IMAGE_TAG identity flavor; the 34B block_q8_0 blob REPLACES the planar q8 planes
  (one zero-copy MTLBuffer per plane via `metal_new_buffer_no_copy_untracked`, region byte
  offsets at bind - kernel indices stay uint32-safe); k4s/k5s ride the 16B strips and k6s
  the GPU split form; every q8 kernel is blob-addressed (the S16 scale twins collapsed),
  the fused QKV/W13 kernels bind per-segment views (kind-major layout), the fixed-B
  kernels grew the kq twins' `ys` y-row-stride uniform for fused-buffer writes, and the
  cat-blob caches + the `metal_blob_region` repack are DELETED. CPU inference on a blob
  model panics; the gathers (embed_row, dequant_q8_row, split-k6) read the blob directly;
  `load_model` picks the flavor via the registered `metal_model_servable` hook. Measured:
  1B transform 133ms / map 24ms; kq-pure transform 21ms / map 13ms; all three GPU paths
  serve with 0 declines. Rewiring residue for the ledger (re-measure the q8 cells first):
  the batch qkv site's split-K stands down (its reduce writes contiguous y - plain GemmB
  serves per-segment), the B<=4 unfused single-decode qkv/w13 cats became per-tensor
  dispatches (~2 extra dispatches/layer on those rails), and the legacy quantized-X
  prefill rail is DELETED (the `!mm` serving arms, the fused add+rms+quant/swiglu+quant/
  rope_qk kernels + PSOs, enc_gemm, and the X-quant pools - ~350 lines; the mulmm_legacy
  knob survives as the required-mode forced-decline test switch, and dasllama_metal_gemm's
  planar GEMM donor is untouched - it serves CPU-flavor models' batch offload).

- **QK-norm rope-store fusion - the f16 single-stream H-form SHIPPED (wave A chase round 2);
  the rest of the family is the residual (2026-07-17).** MetalRopeStoreHF16 folds bias +
  per-head RMS + rope + store into one threadgroup-per-head dispatch on QK-norm x f16-mirror
  single-stream decode (+~1% on the 4B board; Q6_K B=1 tied upstream exactly). Residual scope,
  build when a board shows the gap: the f32/q8_0/tq4 codec twins and the BATCH H twins
  (those paths keep the MetalQkNorm prepass + flat rope-store - batch amortizes the extra
  dispatch over B rows, so the gap is smaller there), and a norm-capable fused qkv_rs
  two-pass form for the s16 path (fused qkv_rs still stands down under qk_norm).

- **Gemma stage-1 Metal deferrals (wave B, 2026-07-17).** The gemma2 enablement chose
  correctness-first shapes; each entry names today's cost on gemma-class models only (llama/qwen
  paths untouched):
  (1) *GeGLU fused-w13 stand-down* - SINGLE-STREAM HALF SHIPPED (2026-07-22): the decode
  MetalQ8GemvW13Sw grew an `act` uniform (0 = silu, 1 = gelu using the CPU geglu4 tanh-via-exp
  identity), and the dense-FFN gate fires the fused path for `w13_q8` regardless of activation
  (gemma's geglu no longer stands it down), skipping the separate geglu ew pass - -2
  dispatches/layer. Measured: gemma4-12B Q8 B=1 20.98->22.31 (+6.3%, 0.85->0.90x), gemma3-4b
  0.92->0.95, gemma2-2b 0.93->0.96, gemma3-1b 1.22->1.24; parity green (fam-gemma3/gemma4 +
  arm1-basic swiglu regression). STILL OPEN: the batch fuse13 rail (b2/b4 PSOs) keeps the
  hardcoded silu epilogue, and prefill's legacy fused rail (`enc_swiglu_quant` has no gelu twin,
  and the deferred-W2-add trick has no post-ffn-norm slot, so `fused` stands down entirely -
  mm-path prefill, the default, is unaffected).
  (2) *pre_post_norm sites are composed, not fused* - each post-norm is a separate in-place rms
  dispatch before the residual add (+2 dispatches/layer on every path). Fix: a `post_add_rms`
  kernel (rms(branch)*w_post + x, then the next pre-norm in the same tg - the row is already
  staged for the add_rms reduction).
  (3) *Sliding chunked dispatch is not compacted* - the single-stream part dispatch still grids
  ALL context chunks; below-window chunks exit whole-tg on entry (the comb skips them via chlo).
  At gemma2's 4096 window this only bites past 4K depth; fix = dispatch chunks [chlo, nchunks)
  with a ch0 uniform. Batch shares the early-exit shape (per-row windows preclude one compact
  range).
  (4) *Spec-chain stands down on embed-scale models* - the greedy GPU-argmax chain requires
  `embed_scale == 1.0`, so every gemma (sqrt-dim scale) eats the CPU next-token poke per step.
  Fix: a scale uniform on the embed-gather kernels (enc_embed / enc_embed_k6). Revisit at the
  gemma4-12B B=1 board - this was the 4B chase's biggest single lever.
  (5) *Batch window-crossing parity has no dedicated test* - the fam-gemma2 masking row proves
  single-decode + prefill past the window; the batch part/comb twins share the masked-kernel
  code but no batch test drives depth > window. Add one when a batch harness with deep
  per-row contexts exists.
  (6) *Prefill V-from-K is two dispatches* (stage 3c, 2026-07-18) - the no-wv layers run a flat
  panel copy (MetalPfCopy) then the ones-plane MetalQkNorm in place (+2 dispatches on gemma4's
  8 global layers). Fix: a read-K-write-V weightless-RMS variant (the CPU fuses the copy into
  rms_batch). Suppress adds one tiny classifier-tail dispatch - not worth fusing.
  (7) *Batch mv + split-K rails stand down under hetero* (stage 3d, 2026-07-18) - the mul_mv
  x-staging strides (u_xs4_*) and the split-K totals (u_skt_*, and the sk sites' u_qd k-dims)
  bake ONE attention class, so hetero (gemma4) batch rides the cat-GEMV forms at B <= 4 and
  the planar GEMM without sk at B >= 5. Fix when the gemma4 batch ladder says it matters:
  per-class xs4/skt twins bound per layer, same shape as the attention uniform picks. The
  batch V-from-K copy is also nrows tiny enc_copy_row dispatches - a strided-seg copy kernel
  collapses them to one.

- **Embeddings path (spotted building `/v1/embeddings`, 2026-07-06).** Two small items, neither
  chased: (1) `embed_forward` takes approach A - reuse `forward_prefill` then re-norm every
  position - which pays **one wasted last-position classifier GEMM** (vocabxdim) per embed call,
  because `forward_prefill` always runs the final norm+classifier on the last token. Fix would be
  to split the transformer stack out of `forward_prefill_body` (a `forward_prefill_stack` helper)
  and call it directly, skipping the classifier; cost is one GEMM against a whole forward, so
  negligible until embeddings are hot. (2) The server's `/v1/embeddings` calls facade `embed`
  per input, which **creates + deletes a full KV-cache session per input** - for a batch of N
  strings that's N session allocations. Fix: a dedicated reused embed session in the server (the
  facade would need a session-taking `embed` overload, or the server reaches the public
  `embed_forward` primitive - but that breaks the facade-only invariant, so the overload is the
  clean path). Both are backlog; the server is serial and embeddings are low-frequency.
- **q8 GEMV loses to fp32 on cache-resident weights without VNNI (zen2 whisper decoder,
  2026-07-08).** Decoder-q8 stage A/B on zen2 tiny/jfk: logits GEMV 3.8x faster (76 MB -
  bandwidth-bound, the q8 win) but per-layer decoder GEMVs +7-21% SLOWER q8 (~2.3 MB mats,
  L2/L3-hot across the serial decode steps; AVX2 int8-dot + per-step requant loses to plain
  FMA when there's no memory traffic to save). Cost today: whisper-tiny zen2 decode leaves
  ~10-15 ms/clip on the table; net end-to-end neutral so nothing urgent. Possible fix: a
  per-region "stay-fp32 when the mat fits cache and the box lacks int8-dot silicon" load
  heuristic (or a box_profile knob like batch_grid_2d) - DON'T build until the M1 decoder-q8
  re-sweep says whether sdot boxes want q8 everywhere (likely yes -> knob would be x64-only).
- **Gemma-4 E-series DENSE prefill trails upstream Accelerate-BLAS on M1 (Wave G3 A/B, 2026-07-07).**
  E4B pp512 das **178.7** vs upstream **192.9 = 0.93x**; E2B pp512 das **376** vs upstream **382 = 0.98x**
  (E2B ties, E4B ~7% back). Decode tied both (bandwidth-bound: E4B 18.7/19.2, E2B 36.0/36.9).
  Root: dense prefill has no sparsity/grouping lever (unlike the MoE waves that LED upstream), so it's
  das NEON-SDOT vs upstream Accelerate-BLAS (AMX-backed) on the projection/FFN GEMMs - the larger E4B
  dim (2560 x ff 10240) favors AMX more, hence 0.93x vs E2B's 0.98x. Cost today: ~7% E4B prefill
  on M1 only; boxes without AMX exposure (zen2/SPR VNNI) already close it. Possible lever: a
  gemm-gen tuned Q8 kernel for the E-series shapes (the tune framework already exists) - don't
  build until an AMX-less box A/B says the shape actually leaves das-kernel headroom on the table.

- **Canary-Qwen ASR runs fp32 for parity; q8 decoder+encoder is the follow-up (Wave A1,
  2026-07-08).** The token-for-token gate loads the LoRA-merged Qwen3-1.7B decoder + FastConformer
  encoder at fp32. Perf A/B (M1 8T, das vs NeMo SALM greedy; `benchmarks/asr/results.md`, now git history): das
  LEADS every short/dictation clip 1.4-3x (jfk das/nemo 0.61x, LibriSpeech 0.34-0.49x) - the
  Canary-Qwen use case - but TRAILS 3.7x on the 3-min gb1, where the fp32 1.7B decoder is
  bandwidth-bound over gb1's ~2500 audio soft tokens. Fix: a q8 decoder (the existing q8 GEMV path,
  ~2x decode on the bandwidth rail) + q8 encoder, both straight ports of the parakeet/whisper q8
  machinery gated behind the fp32 parity default. Don't chase until the ASR-perf pass - but
  gb1-class long-audio is where it pays.

- **Gemma-4 E-series audio (gemma4a) encoder is fp32 SCALAR - big A/B gap (Wave A2, 2026-07-08).**
  The parity gate is fp32 encoder correctness, so the gemma4a Conformer runs a plain fp32 scalar
  forward. A/B (M1 Max 8T, das vs llama-mtmd-cli; `benchmarks/asr/results.md`, now git history): das transcribe
  6028 ms / xRT 2.89 vs mtmd-cli 1547 ms / xRT 11.3 -> **das TRAILS 3.9x**, dominated by the encoder:
  das encode 1888 ms vs mtmd 117 ms = **16x** (fp32 scalar Conformer vs upstream's bf16-weight SIMD
  GEMMs); long-context decode 21.7 vs 78 tok/s also lags. Unlike A1/parakeet/whisper (which lead or
  tie), this tower has had NO perf pass. Fix: route the gemma4a tower through the gemm-gen Q8 audio
  kernel (the same SIMD/threaded machinery parakeet/whisper towers already use) - likely the single
  biggest audio-side win on the shelf - plus the long-context decode path. Not chased mid-wave.

- **Qwen3-Omni AuT tower is fp32 scalar too; perf numbers are SOFT (Wave A3, 2026-07-08).**
  Same shape as A2: parity gate is fp32, so the shared qwen3a AuT encoder runs scalar. A/B (M1 Max
  8T, das vs llama-mtmd-cli): jfk das 3625 ms / xRT 3.03 vs mtmd 1173 / 9.4 = **das trails 3.09x**;
  jfk3 das 8263 / 3.99 vs mtmd 2079 / 15.9 = **3.97x**. Dominant gap = the fp32 scalar qwen3a tower
  (~4.8x encode), same lever as the gemma4a entry above - SIMD/Q8 the shared AuT/qwen3a encoder
  covers BOTH A2 and A3. The q8 MoE thinker (grouped prefill ~207 t/s + q8 decode) also trails upstream.
  [!] These A/B numbers are SOFT: measured with a dormant Parsec host daemon (1.6% CPU) + Spotlight
  indexing the freshly-downloaded 34 GB - a clean announced Parsec-off re-sweep would firm them (parity
  is unaffected). Not chased mid-wave.

- **ASR short-clip fixed costs (parakeet, M1 - NEXT ROUND, Boris 2026-07-06; whisper tower
  q8 postponed one session behind it).** Cost today at matched 8T: jfk das 703 ms vs cli 352
  (2.0x), LibriSpeech dictation p50 651 vs 324; long clips already 1.07-1.10x, so the short
  end is where the M1 gap lives. jfk stage profile (q8): ffn 215 / conv_module 126 /
  attn_heads 120 / attn_proj 119 / conv_sub 55 / mel 33 / decode 27 ms. Levers: mel is
  single-threaded (thread FFT/mel-dot over frames); conv_sub GEMMs stay fp32 - check their
  threading; audit every `lanes_for_work` gate at jfk-size tt (small clips may run stages
  inline that would profitably thread at 8 lanes); small-tt q8 tile shapes /
  `effective_token_block` at tt~140; per-layer requant overhead at small tt; v3 decode GEMV
  is 8198x640 per step (8x v2's rows) - q8 the joint if it shows. Gate per lever: token
  parity + jfk best-of-3 + LS p50 before/after, both boxes.

- **DONE SHIPPED + SILICON-ADJUDICATED (zen2, 2026-07-05): 2-D batch chunk space (row-units x
  token-blocks).** Landed as `batch_grid_2d` (0 = 1-D / 1 = fine grid, 16-token cells /
  2 = wave-aligned, rc*ntc = whole L-waves); the knob arms a per-dispatch auto-gate (engages only
  when the 1-D grain cap starves the admitted lanes), gen ts=4 walk factored into a shared
  body_cell so the off-path is the old walk verbatim; bit-exact all three ways (op-level test +
  512-tok GEN_IDS on both boxes). zen2 A/B (T=48, 3 interleaved reps): 135M pp512 knob2 +7.9%
  (every rep; +15% in the post-pin interleaved confirm), knob1 only +2.8% - **wave alignment
  beats amortize-the-tail chunking at one-claim-per-lane granularity, the same physics as the 1-D
  wave invariant**; 1B +3-4% (kv d=512 starves); Qwen3-0.6B null as geometry predicts (nothing
  starves at 48 lanes); T=24 control identical (gate can't engage). zen2 profile pinned
  `batch_grid_2d: 2`; M1 pin stays off (<=11 lanes can never starve - verified no-regress).
  OPEN: the SPR respin should A/B the pin at T>=48 (more shapes starve at higher lane counts;
  fine-vs-aligned may flip where claim overhead differs). Original scoping kept below.
  Our batch dispatch chunks over out-row units only; tokens loop inside each chunk. Shapes
  with few row-units starve high lane counts (135M d=576 -> ~5-36 chunks for 48 lanes; the
  Qwen3-0.6B attn_chain "deep-thin" 50% lead is the same geometry). The upstream GENERIC path
  chunks 2-D - (out-rows x tokens) grid, chunk 16, one atomic counter, all architectures -
  which is exactly why its mid/high-lane scaling holds on tiny models where our gated pool
  tops out. Our batch walk already loops tokens inside units, so
  the chunk space generalizes mechanically (matmul_chunks gains a token axis; worker chunk =
  (unit range, token-block range)); y-slices stay disjoint by construction. Ordering: linearize
  ROWS-FASTEST (concurrent workers then share the token block's activation slice in LLC and
  stream disjoint weight rows; starting the counter at the nth chunk on the first wave is
  the rank gate's natural analog on our side). **Delivery shape (Boris, 2026-07-05): a PIN per
  architecture, not a default rewrite** - the 1-D scheme already wins 2 of 3 ladder rungs
  head-to-head (avx2, vnni) and the M1/zen2 boxes; 2-D lands as a box_profile knob (default
  off, current behavior byte-identical), enabled where the per-box tune/fleet A/B proves it
  (SPR-class high-T tiny models, deep-thin attn shapes). Candidate refinement to discuss at
  implementation: an in-code auto-gate that only engages 2-D when the 1-D unit count starves
  the admitted lanes (units < k*lanes) - the knob then pins the gate rather than the mode.
  Sized: the 135M-class T>=24 residual vs upstream (its 0.90-1.0 cells) + the attn_chain lead.
- **DONE SHIPPED (audio arc, 2026-07-06): the Qwen2-Audio arc (speech->text, Boris 2026-07-05).**
  Landed token-for-token vs mtmd, then grew into the whole audio wave - whisper-proper ASR
  (6-model family sweep), Ultravox/Qwen2.5-Omni/Voxtral, Qwen3-ASR (new qwen3a chunked
  encoder), Parakeet-TDT, live-mic dictation, uniform `load_asr_model`/`transcribe` surface +
  chat-audio verbs on the facade. Arc-local perf ledgers folded in below. Original scoping kept:
  The cheapest audio-input path: Whisper-large-v3 encoder (~640M - mel frontend via the
  ALREADY-BOUND dasMinfft real FFT, the same per-frame-FFT pattern dasAudio's partitioned
  convolution reverb production-tests; 2x conv1d+GELU subsample = kernel-3 neural convs ->
  im2col over EXISTING matmuls, no new compute kernel - FFT convolution only pays at long
  kernels; N PLAIN encoder blocks = existing matmul/norm/softmax with non-causal no-cache
  attention) -> avg-pool -> linear projector -> soft tokens spliced at the `<|audio|>`
  placeholder; the decoder is our EXISTING qwen2 arch untouched (no cross-attention anywhere).
  New pieces: im2col gather, encoder forward, embedding-span prefill (driver), mmproj GGUF
  loader; oracle = the upstream mtmd tool (GGUF pairs ship). ~1 modest arc; the encoder is SHARED
  infrastructure - the same implementation unlocks Ultravox (llama-3 decoder yes have it) and
  ~80% of a Whisper-proper port later.
- **DONE SHIPPED (kq chain regrain, 2026-07-12 PM^2): kq layers re-admitted to both fused decode
  chains.** The chains run one activation image per consumed form (the per-op mm_pre_f
  contract): q8 projections read the Q8_0 image, kq projections the Q8_K image (kxq/kxs/kxbs);
  the w2-input requant moves to 256-row stage-0 groups and the wo-input requant to HEAD GROUPS
  of 256/head_size heads (Qwen3's 128 = head pairs) so every Q8_K quantize covers whole
  superblocks. Gates: dim % 256 for any kq weight, hidden % 256 for a kq down-proj, qd % 256 +
  head-size divisibility for a kq wo (all real kq models pass). Bit-exact vs the per-op path -
  test_fused_decode grew a kq arm (Qwen3-4B Q5_K_M = k5+k6+head-pairs, gemma-2-2b Q4_K_M =
  k4+k6+softcap/post-norm; ids + full logits EXACT). Measured (M1 Max steady-state): Q6 tg
  30.5 -> 31.1 (+2.1%), Q5 ~flat (33.8, within run wobble), Q4 control flat; decode_prof
  confirms attn_chain 26% + ffn_chain 63% carry the whole kq decode. Dead requant_rows_q8_bs
  deleted with the last Q8_0-bs chain arms.
- **DONE SHIPPED (kq v3 panel-scratch, 2026-07-12 PM): the k5/k6 kernel unpack lever.** The
  tile now reads a BYTE-EXPANDED panel (one byte per weight - zero unpack ALU) that the
  batch cell unpacks from the packed grp planes once per (group, token-block)
  (`unpack_kq_panel_grp`, SWAR uint64 deposit), amortizing the 5/6-bit deposit over TB=128
  tokens instead of the tile's 4; the DRAM planes KEEP the packed 160/192 B/superblock form
  the decode path streams. M1 Max e2e (Qwen3-4B, warm/quiet, 3 alternated reps): Q5 pp512
  110 -> 150-168 (upstream 131 -> das 1.14-1.28x), Q6 111 -> 140-143 (upstream 138.5 -> 1.01-1.03x),
  tg64 at v2 parity (Q5 ~33.7, Q6 ~30.5, Q4 control untouched). Iso tile: k5 65->89, k6
  58->76 GMAC/s at the probe's x16 amortization (~94/~79 effective at production x32).
  **THE LESSON (measured, do not re-learn):** pure byte-expanded DRAM planes (no scratch)
  win the same pp but cost tg -30%/-20% on Q5/Q6 - M1 Max decode is DRAM-bound at the
  model level (~90 GB/s effective), so plane bytes ARE decode time; k4 stays nibble-packed
  everywhere for the same reason. Residual headroom, ledgered: (a) k6 tile 76 vs k4 95 -
  the per-16 SIGNED sub-scale fold pays 2 scale-row sexts + 4 muls per block vs k4/k5's 1;
  (b) k5 unpack still ~6% of production tile (the broadcast+carry deposit - a generated
  NEON tbl/cmtst unpack kernel would close it).
- **DONE RESOLVED (2026-07-12 PM^2, trace-diagnosed): the das prefill "bimodality" is M1
  package DVFS, not code.** Lane-timeline traces of a fast (168 t/s) vs slow (150) Q5
  pp512 run: all 8 lanes 98-99% utilized in BOTH, identical chunk counts, and a UNIFORM
  x1.113 per-chunk slowdown flat across run-deciles - one P-cluster clock step
  (3228->2904-class), run-scoped. First-run-after-idle rides a ~3 s boost window (168);
  back-to-back runs sit at the sustained clock (150-152, +/-0.6% - exactly llama-bench's
  stability, because llama-bench's reps are always steady-state). 45 s cool-downs recover
  only partially (155-162); pmset shows no thermal warnings (ordinary sustained-load DVFS).
  E-core lane placement is EXONERATED - nothing to pin. **METHOD RULE: report the
  steady-state MEDIAN of >=3 back-to-back reps and discard the first-after-idle rep;
  best-of-N systematically picks the boost outlier.** Steady-state scoreboard (M1 Max,
  Qwen3-4B vs upstream steady): Q5 ~150 vs 131 = 1.15x, Q6 ~141 vs 138.5 = 1.02x, Q4_K_M
  ~155-158 vs 172 = 0.90x.
- **DONE RACED (zen2, 2026-07-12 PM^2, 16 affinity-pinned cores both sides, ABBA before=52a22a39b
  after=3d78ca8ef, Qwen3-4B): das WINS Q5 pp 1.84x / Q6 1.32x vs upstream; the kq v2+v3 arc itself
  is ~NEUTRAL on the maddubs lattice.** pp512 das-after/upstream: Q4 161-180 vs 168.3 (~parity -
  its one AVX2 K-quant repack), Q5 158-161 vs 87.0 (**1.84x**), Q6 128-130 vs 98.0
  (**1.32x**); tg64 ~ upstream parity all three (19.3/16.6/14.5 vs 19.25/16.87/14.69). ABBA
  before->after: Q4/Q5 pp par-to-+3%, **Q6 pp -4.6%** (135.2->129.0 median), tg within noise
  (Q4 -4%, Q5 +2%, Q6 -3%). Reading: the M1 unpack win was an sdot-lattice property - on
  Zen2's maddubs lattice the v1 unpack was never the bottleneck, and the Q5/Q6 upstream wins
  pre-date the arc (its dot-product rail is that slow). The Q6 -4.6% is the arc's one x64 cost;
  suspects (not yet attributed - needs a mid-arc leg): the v2 signed per-16 fold's separate
  lo/hi i16 chains (flush every 2 madds) vs v1's, or v3's second verbatim load per weight
  vector on a load-port-bound lattice. Candidate fix if chased: gate kqBytes per-ISA (byte
  panels NEON-only - needs a per-format panel-flag companion so the batch cell knows).
  Sized: ~5% Q6 pp on zen2 only; zen2 stays 1.32x ahead of upstream regardless. Also not yet run:
  the zen2 kq tune sweep (families ran the maddubs-mr8 fallback rows; crowns could shift a
  few %). SPR when a box respins - per Boris (2026-07-12): future profiling moves to an
  AWS box, local boxes are a bottleneck.
- **kq tune bench lacks a MoE-shaped cell row (spotted validating the mr4 crowns, 2026-07-12).**
  The mr4 tile crown gave dense pp +10-26% but gave back ~3% MoE prefill on qwen3moe-30B
  (fused expert cells average ~32 tokens per expert with d=768-class group spans - a regime
  the d=512/ntok=64 batch fixture doesn't represent; mr8's halved group count wins there).
  Adding a MoE-cell-shaped fixture to kq_tune_family and weighting the decision (or a
  per-model-class entry) recovers it. Sized: ~3% MoE pp on M1; re-check on zen2/SPR grids.
- **AMX fold pipelining (double-buffered C spill) - only if amx silicon verdict ever flips.**
  The shape to build: interleave block i-1's AVX-512 scale-fold between block i's
  TMUL ops (double-buffered thread-local C scratch) - the fold hides under
  tile latency; our emit_amx_tile serializes them, likely most of the T-independent ~1.6x
  end-to-end amx loss on SPR. Pure emission-order change (second spill alloca + reordered
  fold). NOT worth doing while the amx leg loses on frequency/bandwidth grounds anyway
  (SPR session-3 verdict: grid-resident, biased busd512 keeps the manifest); revisit on
  Granite-Rapids-class silicon or a cache-resident serving regime. A thread-local
  once-per-thread `ldtilecfg` (vs our per-call config+release) rides the same follow-up.
- **x64 intrinsic backends lack the `mm_rows` row-range GEMV core (fused-chain fallback).** The
  fused decode chains (team_parallel_stages, 2026-07-03) gate on `kernel_backend_has_rows()`;
  portable + both arm64 backends carry the core, so the EPYC (profile-pinned portable) and M1
  get the fused path - but an x64 box on auto-select (avx2-repack / acc8 / vnni tiers) falls
  back to per-op dispatches. Fix = extract each x64 family's GEMV inner loop into a
  `q8q8_rows_kernel_*` (mechanical, mirrors the arm64 extraction); worth ~the same join-tail
  savings wherever an intrinsic tier ever beats tuned-portable at decode. Cost today: none on
  the campaign boxes (both run backends that carry the core). (Spotted during the fused-dispatch
  work, 2026-07-03.)
- **Metal batched decode's skinny-M kernel valley - the P4 chase (emission arc, 2026-07-14).**
  The 3B d512 engineering curve (Parsec-on, fixed-token protocol) is 101/103/148/270 t/s at
  B=2/4/8/16 vs llama-batched-bench Metal's 134/219/250/363 - steps are GPU-bound (CPU side
  < 1.5ms), so the whole gap is skinny-M GEMM efficiency: the fixed-B batched GEMV forms
  ALU-saturate (~340-540 GMAC/s - the B=4 valley's 35.9ms step), and gemm32's M-pad-32 staging
  floor is ~51-54ms/step (~2900 GMAC/s padded) regardless of B, while the upstream mul_mv sustains
  near-weight-stream rates to ne11~8. Chase candidates, LAB ROWS FIRST (bench_metal_gemv_kernels
  grows variants): (a) x-staged batched GEMV - stage the B-row X panel slice in threadgroup
  memory per k-chunk so per-lane device x loads (the ALU-bound form's limiter) become tgmem
  reads; (b) a gemm16 twin (16-row M tile - halves the pad waste and A-staging at B <= 16);
  (c) the production 34B-blob mul_mm at M-pad-64 (f32-X, no quant dispatches - costs blob
  residency next to the decode driver's planar regions). Smaller shavings on the same row: a
  cls-specific GEMM crossover (the lab says the 128256-row classifier flips at B~4, the uniform
  policy switches at 5); per-row KV writeback + logits scatter memcpys (~2.3ms at B=16) could
  thread; llama-8B (dim 4096) declines batch at B >= 5 (the add+rms+quant row slab caps at
  3072 - an unfused rms+quant pair would re-admit it); GPU-side per-row greedy argmax for the
  batch (the single-stream spec chain already carries the kernel) if greedy batch serving ever
  matters. Sized honestly: closing to the upstream curve is worth up to ~2.1x at B=4, ~1.7x at B=8,
  ~1.34x at B=16 on M1. UPDATE (2026-07-14 late): (a) LANDED as the x-staged fixed-B forms -
  lab b2x 326-363 wGB/s (+40-88%), b4x 188-217 (~2x, above upstream's ~186 at ne11=4); driver
  same-window A/B B=4 +19.2%, B=2 +1.8% (B=2's step is residual/dispatch-bound: knockout says
  GEMV+cls is 11.7ms of the 16.7ms gpu step at B=2, 21.4 of 27.6 at B=4 - the rest is the
  per-step kernel residual + ~141 serialized dispatch tails, the encode-ahead/fusion rounds'
  territory). (b)/(c) were settled by the GemmB v2 + per-site split-K rounds (32x32 occupancy
  beats 64-wide at M-pad-32; mul_mm rail kept opt-in). NEW smaller shaving measured en route:
  the in-loop s16 scale select costs ~3% at B=2-4 (wscale-f16 0 vs 1 driver A/B) - the per-PSO
  s16 bake stays on the ranked list.
- **Fused-chain follow-ups: MoE FFN chain + norm/quantize as a stage.** The 2026-07-03 fused
  decode covers the attention block (all q8 arches, head_size % 32 == 0) and the DENSE FFN;
  gpt-oss's routed-expert FFN keeps its per-op groupn dispatches (3/layer) - a 2-stage MoE chain
  (router serial, [experts' gate+up + act + requant] -> [downs + weighted reduce]) is the same
  shape one level up. And each chain still has a serial ~3-6us norm+quantize prologue per block;
  folding it in as a tiny stage-0 (chunked scale+quantize after a serial sum-of-squares) or
  fusing norm INTO the quantize pass is the last serial glue. Sized: MoE chain ~= the dense win
  for gpt-oss decode; norm-stage ~1% at 1B. (Spotted during the fused-dispatch work, 2026-07-03.)
- **DONE (perf pass, 2026-07-02): tied classifier matmuls the Q8 disk quants (`Model.cls_q8`).**
  Tied Q8 loads of a Q8_0 embedding (every tied model we run - probed all 11) transcode
  `token_embd` twice into qblob - a classifier copy at wcls_off (repacked with the other 2D
  weights) and a LINEAR copy at emb_q8_off that embedding rows dequant from on demand (the laneq
  repack interleaves wcls in place, so row reads need their own un-repacked copy; on a no-repack
  box the two could alias - noted x64 follow-up) - and drop the fp32 table: on gemma-4-12B,
  classifier traffic 4.03GB -> 1.13GB/token and resident 4.03GB -> 2.26GB. Rows are bit-identical
  (same Q8_0 data the fblob decode used; gated by test_parity_tied_cls_q8_rows); the classifier
  quants are exactly what upstream matmuls. 8 of 9 tied-model fixtures held token-for-token
  unchanged; gemma2's "Once upon a time" flipped a near-tie under the PINNED classic+libm test
  kernels only (default kernels still matched the oracle 24/24) -> moved to the counting prompt
  like Qwen2.5/Phi, oracle-refrozen. fp32/q4 loads and non-Q8_0 embeddings keep the exact old
  path. (Spotted wave 3.)
- **DONE (2026-09-05): the UNTIED Q8 embedding keeps its Q8_0 quants too (`Model.emb_q8`).**
  `cls_q8` had coupled "tied classifier on the disk quants" with "no fp32 table", so a Q8 load
  of a model with its own `output.weight` still decoded `token_embd` to fp32 into fblob: vocab x
  dim x 4 bytes of RAM and image on every untied model (37 MB on stories15M, 262 MB on
  TinyLlama-1.1B, 394 MB on Phi-3.5-mini, 1.24 GB on the 152k-vocab Qwen3-30B-A3B - sized, not
  measured; Qwen3-4B, Llama-3.2, Qwen2.5, gemma-3 and SmolLM2 are tied and never carried it). Now
  any Q8 load of a Q8_0 `token_embd` transcodes the LINEAR copy at emb_q8_off and drops the table;
  tied models are unchanged (`cls_q8` = tied && emb_q8). Rows bit-identical to the fp32 decode
  (`test_parity_untied_emb_q8_rows`); stories15M's image 51 MB -> 26.6 MB, mint + map + token-exact
  (image suite arm `untied`); the Metal blob flavor carries the copy on TinyLlama (arm
  `metal-untied`). IMAGE_VERSION 33: every image re-mints once. Vulkan's device embed gather has
  no untied-q8 arm yet (the f32-upload arm declines, the CPU embed loop serves). (Spotted by the
  storyteller's wasm download budget.)
- **DONE (perf pass, 2026-07-02): V-from-K layers fuse the K->V copy with the weightless V-norm.**
  Decode (mm_qkv) and prefill both rmsnorm k->v out-of-place when v_norm is on (bit-identical to
  copy + in-place norm; the block's v_norm step skips those layers). (Spotted wave 3.)
- **DONE (perf pass, 2026-07-02): upstream A/Bs run (quiet box, CPU `-ngl 0`, llama-bench
  pp512/tg64 vs our matched driver, ggml-parity fast-math).** gemma-4-12B: prefill us 75-80 t/s
  vs upstream 74.4+/-0.5 (parity to +5%); decode us ~7.3 vs 8.74 (~84% - the remaining decode gap
  is the next lever). gpt-oss-20b: prefill us ~219 vs 117 (~1.9x FASTER - the grouped MoE GEMM);
  decode us ~19 vs ~39-42 (~0.47x - exactly the MXFP4->Q8 doubled expert-weight-traffic asymmetry
  quantified: the native-MXFP4/Q4_0 entry below is now the headline gpt-oss decode lever).
  POST-JOBQUE (#3361 wake propagation + batch dispatch + worker spin, same-window anchors):
  12B decode 7.3 -> 7.9 t/s vs 8.63 (gap 84% -> ~92%); gpt-oss decode 19.2 -> 22.0 vs 39.9
  (0.47x -> 0.55x) - the dispatch-latency share of the decode gap is banked; what remains on
  gpt-oss is the weight-format asymmetry. (Spotted waves 3/5.)
- **DONE (perf pass, 2026-07-02): MoE prefill runs expert-bucketed grouped GEMMs - bit-exact.**
  `ffn_moe_prefill_grouped` routes every position (one batched router GEMM + the shared
  `moe_select`), CSR-buckets the (position, slot) pairs by expert, runs one batched GEMM chain
  per touched expert off a single whole-batch requant, and reduces the parked outputs in exactly
  the decode accumulation order (k slots then shared expert) - so it is bit-identical to the
  per-position path: the batch GEMM/requant/gate kernels are bit-for-bit their single-token
  forms per row. Proven on both MoE models (all logits identical after a 300-token prefill,
  grouped vs reference) and pinned by the qwen2moe fixture running through BOTH paths
  (`set_moe_grouped_prefill` A/B). Decode unchanged (one token = no bucketing win).
  Measured (M1 Max, interleaved in-process A/B): Qwen1.5-MoE 512-tok prefill 31 -> ~270 t/s
  (~8.7x); gpt-oss-20b 256-tok prefill 27 -> ~186 t/s (~6.8x). (Spotted wave 4.)
- **DONE (perf pass, 2026-07-02): MoE decode re-quantized the same activation per expert.**
  `moe_ffn_core` now quantizes xb once per layer into dedicated `moe_xq/moe_xs` (the
  down-projections quantize s.hb into the shared xq/xs, which would clobber a hoisted image
  there) and routes every gate/up matmul through `mm_at_q8_pre`. Bit-identical (same quants).
  (Spotted wave 4.)
- **DONE (MXFP4 arc, 2026-07-02): native-MXFP4 expert stacks + repacked TBL/SDOT kernels + the
  MoE dispatch fuse.** Was: gpt-oss-20b's 4.25-bit expert stacks ran as Q8 (2x resident, 2x
  decode traffic). Now: the stacks stay as raw nibble + E8M0 planes (mxq/mxs, exact disk bits -
  the old dequant->requant amax error is gone), decoded in-register by new aarch64_neon
  intrinsics (tbl16_lo/tbl16_hi = vqtbl1q_s8 of the doubled-e2m1 LUT; sdot4_w / sdot4_laneq_w
  take the tbl result as a VALUE) through dot_mx4q8 ([tuned], row-major) and dot_mx4q8_laneq4
  (interleaved 4-row repack, the block_mxfp4x4 twin). Grouped prefill expands each touched
  expert to EXACT Q8 (lossless: q = LUT int, scale = e8m0_half), writing the interleaved form
  directly on a repack backend; short prompts route per-position (npos*k >= 8*n_expert guard).
  On top, the MoE decode dispatch fuse: region-list groupn/groupn_mx4 kernels run all k
  experts' gates (ups, downs) in ONE fork/join - 288 -> ~72 mm dispatches/token, bit-exact.
  Measured (QUIET box, 2026-07-02, same-window anchors): decode 22.0 -> **31.8 t/s** @ ctx 8
  (30.7 @ ctx 512) vs llama-bench tg64 41.1 - **0.55x -> 0.77x**; per-op profile: mm_moe 50%
  measured vs 47.9% theoretical share (the 66/66 format asymmetry is GONE), MoE mms sustain
  ~77GB/s vs the dense mms' ~99 (the remaining MoE-efficiency gap = the next lever); 12B decode
  7.98 vs anchor 8.67 (92%, unchanged - dense path untouched); resident 26 -> 13.2GB; every
  fixture token-for-token unchanged (no refreeze - the counting fixtures absorbed all
  kernel-order changes). Cost paid: gpt-oss pp512 ~186 -> ~121-149 t/s (the per-expert
  expansion) vs upstream's 119.9 - still >= parity; the native mx4 batch GEMM in the expansion
  entry below reclaims it. (Spotted wave 5; the Q4_0 halfway house was skipped - native landed
  directly.)
- **q4 has no batched prefill kernel - prefill collapses to decode rate.** The q4 path serves
  everything through the scalar fp32-activation `dot_q4`/`matmul_q4` (no q8-style token-blocked
  batch GEMM, no NEON arm, no repack backend), so a q4 prefill runs at generation speed:
  measured on SmolLM2-135M, q8 prefill 1391 t/s vs q4 prefill 70 t/s ~ its own 69 t/s decode.
  A q4 batch kernel (or the load-time q4->q8 transcode as the cheap fix) closes it.
  (Spotted tutorials wave.) **LOW PRIORITY (2026-07-02):** the path is cold - every model we
  test/ship parity for is Q8_0 on disk (plus gpt-oss MXFP4->Q8); no Q4_0 GGUF anywhere in the
  fixture set. q4 only fires when a user opts into `QuantMode q4` for footprint. Priority rises
  only if the MXFP4->Q4_0 halfway house above lands (q4 becomes the resident format of a real
  20B model). **RESOLVED DIFFERENTLY (2026-07-16):** Q4_0 *files* now serve natively on the kq
  rails under q8 mode (KqFmt.q40, #3481) - batched prefill included; the legacy tier was renamed
  `QuantMode.q4_0` and remains the requant-from-f32 footprint option, per-row kernels unchanged.
- **LOW PRIORITY: f32 projection GEMM is untiled - dot-per-token, no token block.**
  `matmul_batch` (dasllama_math.das) is the exact pre-#3315 shape the Q8 path had: weight-
  stationary nest with one horizontal-reduce `dot()` per (row, token), zero register reuse
  across rows/tokens, and no L2 token-blocking (long-prefill X re-streams from DRAM per weight
  row). The SDOT-era fix transfers verbatim since it's dtype-agnostic: a 4-row x 4-token
  register tile with float4 `mad` chains and per-tile reduces (the fp32 twin of
  `dot_q8q8_sdot4x4` - keeps W row-major, no repack, decode GEMV untouched; do NOT reuse the
  broadcast-A `gemm_f32` form, it needs a transposed W copy) plus an `effective_token_block`
  at ~1/4 the Q8 block (fp32 activations are 4x fatter). Expected kernel win ~2-3.5x (what the
  attention tile measured), ceiling below Q8 (fmla = 4 MACs/instr vs SDOT 16; 4B/weight vs
  ~1.06B). LOW because the f32 arm only fires for f32 GGUF tensors - in practice the tiny
  teaching models; attention's fp32 GEMMs already have the register tile (`gemm_f32_uk_4x16`).
  (Spotted post-#3354, 2026-07-02.)
- **DONE (perf pass, 2026-07-02): `kv_cache_off` prefix-summed per call.** `Model.kv_row_prefix`
  (filled by layout_offsets, seq_len-independent) x the LIVE seq_len at call time - the O(1)
  Model overload serves both hot call sites; the Config walking form stays as the definitional
  reference. (Spotted post-wave-3 review, per Copilot on #3346.)
- **DONE (perf pass, 2026-07-02): decode attention threads over heads - crossover measured,
  default re-set.** `attention_std_decode` maybe_parallel_fors the head loop (disjoint per-head
  rows => bit-exact vs inline, gated by test_forward), behind `g_decode_attn_par_threshold`
  (profile `runtime.decode_attn_par_threshold`). Quiet-box sweep (M1 Max, inline-vs-threaded
  interleaved at 32..2048 ctx on Llama-3.2-1B and gemma-4-12B): crossover at ~200-260K work on
  BOTH; below it threading costs <=2%, above it wins reach +74% (1B) / +89% (12B) at 2048 ctx -
  the derived 4M default was ~15x too conservative (the 12B ran inline below ctx 512). Default
  is now the measured 262144. (Spotted tune audit, 2026-07-02.)
- **LOW PRIORITY: `sample_` top-k is O(top_k x vocab) scalar selection.** Each of the top_k
  rounds rescans the whole vocab (dasllama_common.das sample_) - top_k=40 on gemma-4's 262144
  vocab is ~10M compares per sampled token. Cold today (SamplingParams defaults are greedy /
  top_k=0, and all parity fixtures are greedy), but it's the sampling path the tutorials teach.
  Fix = single-pass partial selection (bounded min-heap of size top_k, or threshold-and-count).
  (Spotted tune audit, 2026-07-02.)
- **DONE (MXFP4 arc follow-up, 2026-07-02): the mm_moe bandwidth-gap profile + the bias fold.**
  Iso-benched the exact decode dispatch shape (4x [2880 x 2880] regions, DRAM-rotating):
  the fused mx4 groupn GEMV sustains **~101 GB/s - bandwidth parity with the q8 dense kernels**
  (the "77 GB/s" in-decode reading was largely single-window wobble: same build re-measured
  90 GB/s an hour later; METHOD: only round-robin interleaved cells within one process are
  trustworthy on this box, single-window absolutes swing +/-10-15%). The pre-fuse 4x1 dispatch
  shape measures 63-72 GB/s - the dispatch fuse was worth ~30% and is confirmed load-bearing.
  Follow-up landed: **expert bias vectors fold into the groupn workers' stores** (bp/boffs on the
  groupn contract; bit-identical to the post-pass add_bias, minus its serial ~36us/layer) -
  decode 34.7 -> **35.2 t/s @ ctx 8 / 33.8 @ ctx 512** (upstream same-window anchor 41.1 ->
  0.86x). Also swept: decode-attn threshold 0-vs-262144 under the spinner at ctx 8/512 -
  a WASH at both depths (the low-ctx attention is memory-latency-bound; threading's dispatch
  cost ~ its serial cost), so the measured default stands; moe_reduce/rope threading rejected
  (~8us/layer each, below dispatch cost). What remains vs upstream is its continuous-polling
  threadpool (the bus never idles between ops) - picked up by the x64 arc's jobque work, not
  patchable here. (Profiling session, 2026-07-02.)
- **MXFP4 grouped prefill pays a per-touched-expert Q8 expansion (~120MB of traffic each, half of
  it the repack scratch copy).** `expand_mx4_region_q8` writes exact row-major Q8 then runs the
  load-time `repack_q8q8_weight` (temp copy + interleave) so the laneq batch GEMM applies. Levers,
  in effort order: (a) expand DIRECTLY into the interleaved layout (folds the repack's copy away -
  needs a backend-provided expand-repack, not a layout hardcode in common); (b) a native MXFP4
  batch GEMM (mx4 twin of the laneq 4x4 tile - halves the GEMM's weight streaming too, likely wins
  outright); (c) the `npos * k >= 8 * n_expert` tiny-batch guard is an ESTIMATED breakeven
  (4-tok-prompt ttft 1895ms -> 114ms) - sweep it when the mx4 A/B rig exists. (Spotted MXFP4 arc,
  2026-07-02.)
- **Flash-attention tile shape is a frozen compile-time constant - deferred x64 tuning axis.**
  `ATTN_FLASH_QT/KV = 64x64` (dasllama_common.das) was chosen on M1 and never swept; tile shape
  is the classic per-box cache parameter, and x64's small private L2 differs in kind from M1's
  big shared L2. QT is compile-time-coupled to the `float[64]` running max/sum fixed arrays and
  the fa_* scratch sizing, so this is a compile-time axis a la `[tuned]` (profile-keyed), not a
  runtime setter. DEFERRED until an x64 box exists to measure on - do not solve the coupling
  speculatively. (Spotted tune audit, 2026-07-02.)
- **DONE (GEMV hunt, 2026-07-03): the decode "kernel top-end gap" was chunk-count misalignment,
  not the kernel.** A 12-variant GEMV race (`bench_gemv_decode.das` + the matmul_variants decode
  cells: unroll/ILP/fma-flush/dual-group/inline-scale) proved every kernel micro-opt a wash -
  the upstream live M1 kernel (its 4x8 tier needs i8mm) is the same
  shape as `dot_q8q8_laneq4`. The real delta: njobs = `get_total_hw_jobs()*k` sized chunks to the
   7 WORKERS, but 8 lanes serve them (workers + caller - team by design, fifo via main-steal), so
  the last wave ran half-empty (28 chunks / 8 lanes = 87.5% utilization). Fix:
  `get_dispatch_lanes()` (workers + caller; knob `set_dispatch_caller_lane`, decode_prof
  `--legacy-lanes`) at all 54 dispatch sites incl. x64 (same effect there, smaller: ~2-3% at 32+
  workers). Measured: GEMV cls 109 -> 118-120 GB/s (the upstream rate exactly); decode e2e 74.3 -> 77.6 t/s
  @ ctx 8 (+4.4%, ~0.95x upstream) / 66.3 -> 67.8 @ ctx 512; mm_ffn 1.10x -> 1.04x, cls 1.19x -> 1.12x
  of upstream per-op. Suite 171/171 token-for-token (chunk splits don't move per-row math). Oversplit
  re-sweep under aligned lanes: decode e2e is a WASH across x1/x2/x4 (x8 past the knee) - the
  straggler-mitigation rationale dissolved once the split became 8-on-8, x4 default stands on the
  prefill iso numbers. (GEMV hunt session, 2026-07-03.)
- **Q8 scales stream as a separate fp32 plane - an inline-fp16 layout moves ~6% less
  weight traffic.** Packing 4 fp16 scales WITH the 128 quant bytes is 136 B per
  4-row block-group in one stream; our laneq layout reads a second fp32-plane stream (144 B total
  per group). At the now-aligned ~118-120 GB/s both sides saturate equally, so the remaining
  cls/ffn per-op gap (1.04-1.12x of upstream) is almost exactly this byte ratio (144/136 = 5.9%;
  bench: our best cls call 2448us vs upstream's 2307 = 6.1%). The bench's `inline`/`inline_u2m` cells
  already prove the single-stream kernel shape works (fp32 inline = parity, occasionally +1%).
  The win requires fp16: (a) an f16->f32 convert intrinsic in dasLLVM aarch64_neon (vcvt_f32_f16
  via LLVM fpext, x64 twin F16C `vcvtph2ps` - gate on `cpu_supports`); (b) a repack variant
  writing [4xfp16 d | 128 qs] blocks + the GEMV/batch/group3/groupn kernels reading it; (c) the
  element-offset plumbing moves from plane offsets to block strides (loader `w*_off` math, groupn
  offs contracts). Sized: dasLLVM intrinsic + codegen bump, ~4 kernel twins, loader/offset sweep,
  fixture risk LOW (same math, scales exact fp16 round-trip of what the loader already computes -
  but the quantizer would store fp16-rounded scales, so oracle fixtures need a refreeze check).
  Expected: cls 1.12x -> ~1.06x, ffn 1.04x -> ~parity; decode e2e ~+2-3%. NOT this PR - needs its
  own arc. (Spotted GEMV hunt, 2026-07-03.)
- **Audio arc: the fp32 encoder is the whole ASR cost - q8 the encoder GEMMs.** The whisper-family
  encoder runs fp32 ~ 18-19 s per 30 s chunk on M1 (~37 s for qwen2a's standard 2 chunks;
  large-v3-turbo transcribe = 0.49x realtime, almost all of it the encoder window). Fast path:
  q8 the 6 encoder GEMM families + projector at load (`quantize_weights` pattern) onto the
  generated q8q8 kernels - expect the usual ~4x (turbo -> ~2x realtime) - plus threading the
  im2col/pack loops. Tolerance-gate like flash-decode (stage witness vs fp32), token-parity
  revalidate. (Audio arc, 2026-07-05/06.)
- **Audio arc: the all-silence second chunk's soft tokens are input-independent.** Every <=30 s
  clip pads to 2 chunks and the second is all mel-floor - its 750 soft tokens are the same for
  every clip, cacheable per tower; halves qwen2a encoder cost for short clips. (Audio arc,
  2026-07-05.)
- **Audio arc: whisper decoder logits GEMV, only if it shows.** The ASR decoders are small
  (tiny ~35 MMAC/token, turbo ~140 incl. the 66 MMAC tied logits) - fp32 is fine; q8 the
  token_embd logits GEMV if profiling ever surfaces it. (Audio arc, 2026-07-06.)
- **Audio arc (correctness follow-up, not perf): honor `encode(parse_special=true)`.** The flag
  is documented-unhonored; every call site assembles specials by id + per-segment text encode
  (the chat-layer pattern). Fix belongs in the tokenizer proper; the workaround callers migrate
  after. (Audio arc, 2026-07-05.)

## Transferred entries (folded in 2026-07-29, dasLLAMA doc reorg)

Arc-local ledgers and standing verdicts that were living inside per-arc plan docs. Two of them
(`whisper_plan.md`, `qwen2_audio_plan.md`) carried their own header saying "fold into
API_REWORK.md at PR time" - this is that fold, into the ledger's new home. Source doc named per
group; wording kept.

### From `gemm_generator_plan.md`

- **AMX tile family - arc CLOSED (slice I built it, slice K's SPR session 4 adjudicated it,
  2026-07-05).** amx is 0.36-0.55x the biased-busd512 champion across all four ladder models and
  the best cell ever measured sits below the plain AVX2 maddubs tier; the bill is the kstep=1
  Q8-scale-boundary fold (~256KB of C-spill per 32-row x 32-token unit vs 64KB of weight traffic -
  codegen, frequency license, and dispatch all separately exonerated). Family stays grid-resident,
  `spr_manifest` unchanged; revisit only on Granite-Rapids-class silicon or a cache-resident
  serving regime. Conditional-only tail: an **amx kstep=2 fold** (K=64 tiles, two scale segments
  per spill) halves the C traffic but its honest ceiling is ~1.6x behind the champion.
- **smmla leg (slice J) - no fallback-chain change (M3 Air silicon, 2026-07-05).** The smmla tile
  sits at **100% of the M3's ONE-NEON-pipe MMA ceiling** (130.6 GMAC/s = 4.08 G MMA/s ~ 1/cyc,
  marshalling fully hidden) and still loses to sdot's ~2.45 issues/cyc across 4 pipes - 1 pipe x 32
  MACs < 4 pipes x 16 MACs, structurally. The leg stays grid-resident + `requires="i8mm"`-gated for
  server arm64 (Neoverse V1/V2 / Graviton3+ issue smmla 2-4/cyc, where the same stamp should flip
  the verdict - the tune framework decides per box, which is the whole point).
- **PARTLY SHIPPED - loop-hint manifest kind.** The *storage* unified: the per-app tune sidecar's
  `"kernels"` section is documented to carry "this module's stamps and the loop-hint perms"
  (`modules/dasLLVM/daslib/llvm_tune.das:40`), and `dasllama_tune.das` now reads it through
  llvm_tune (`box_profile_path` -> `tune_manifest_path`, `box_profile_stale` ->
  `tune_sidecar_stale`, the perm read -> `tune_manifest_get`). Still OPEN: llvm_tune has **no
  loop-hint value kind of its own** (no loop-stamping code anywhere in it) - `dasllama_tune.das`
  still owns the `[tuned]` grid and does the hint stamping, with 18 live `[tuned]` sites, so the
  plan's "subsume, then delete dasllama_tune's `[tuned]`/box_profile rail" is not done.
- **OPEN - per-slot perms.** Unimplemented: no per-slot perm plumbing exists in llvm_tune or
  `dasllama_math_gen.das`; the phrase appears only in plan prose and in the unrelated per-slot
  *backend* pin (`x64_arch.md`, `dasllama_math.das`). What landed instead is per-FORMAT families -
  separate `[tune]` brackets for k4 / k5 / k6 / q40 / q8q8 / q51 in `dasllama_math_gen.das`, each
  its own manifest entry. Per-slot perms remains the named escalation if a box ever flips the amx
  end-to-end verdict, or if biased-vs-plain gemv turns out to matter under an amx stamp.

### From `kv_cache_compression.md` sec.9

- **BLAS / Accelerate on macOS - RULED OUT (measured).** dasLLAMA's 8-bit `sdot` kernel beats the
  BLAS (Accelerate) path, and it is structural, not incidental: BLAS is f32-only (no `hgemm`, no
  int8 - the upstream BLAS backend routes through one `cblas_sgemm` that dequantizes any non-f32 operand into
  scratch first), while the weight matmul is memory-bound on the weight stream, so a 4x weight
  expansion to feed AMX/SME f32 throughput can never win. Decision: don't route dasLLAMA matmuls
  through BLAS. **Its "for Apple lower-precision matmul the framework is BNNS ... not pursued" tail
  is SUPERSEDED** - the AMX arc (#3562) shipped a BNNS-f16 lane in
  `dasllama/dasllama_math_accelerate.das` (bf16 planes via BNNS f16 matmul under
  `DASLLAMA_ACCEL_F16=1`, jobque strip-dispatched).

- **OPEN - k4-native deltanet planes (the "dn planes are ALWAYS q8" loader invariant aged).**
  Minted on 0.8B-4B where the dn projections were a rounding error; on Qwen3.8-27B (dense
  hybrid, 48 recurrent layers) they are ~5.6 B of the 6.7 B q8-region weights, so the
  q4k->q8 load transcode costs **+~2.8 GB of image (22.3 vs ~19.5 GB metal blob) and ~20% of
  decode's per-token weight reads** (measured via the 2026-08-17 27B bring-up: dlim plane
  sizes + the q8-region scale count). The dn shapes are why the invariant exists - beta/alpha are
  dim x dt_rank (16-64, under every kq GEMM tile's N) - so the fix is a narrow k4 GEMV
  family (`MetalDnBa`-class + the CPU dot), not a kq-tile retrofit. Pays on every lane, and
  it is step one of **the 16 GB-card game** (Boris, 2026-08-17): 27B q4k on a discrete
  16 GB GPU needs the image at ~19.5 GB *plus* partial residency (the vulkan tier's
  offload/stream lane) or a ~3.5 bpw format family (k3/IQ - unsupported today).

### From `audio_models_plan.md` (Findings; correctness, not perf)

- **RESOLVED (87ffe39fe) - the v0.3 Mistral `[INST]` template's derived close was ` [/INST]` -
  wrong for multi-turn.** Fixed: `mistral_instruct_template`'s `assistant_close` is `</s>`, like
  v7-tekken's. Spotted in audio session B while adding `ChatTemplate.assistant_close`.

### From `whisper_plan.md` ("Performance ledger - fold into API_REWORK.md at PR time")

- Measured (M1, `examples/dasLLAMA/transcribe.das`, jfk 11 s, fp32): tiny 22x, base 10x, small
  2.6x, medium 0.85x, large-v3 0.42x, **large-v3-turbo 0.49x realtime** - almost all of it the fp32
  encoder window.
- Turbo encoder = the qwen2a tower cost (fp32 ~ 18-19 s per 30 s chunk on M1) - the q8
  encoder-GEMM path from the qwen2-audio ledger applies verbatim and is the headline item
  (expected ~4x -> ~2x realtime turbo). Same lever as the "Audio arc: the fp32 encoder is the whole
  ASR cost" entry above.
- Decoder is small (tiny ~35 MMAC/token, turbo ~140 MMAC/token incl. 66 MMAC tied logits) - fp32
  fine; q8 the token_embd logits GEMV if it shows up.
- Model load: byte-wise f16->f32 tensor reads (the bin stores tensors unaligned) - turbo ~1.6 GB f16
  loads in a few seconds; an aligned-fast-path only if load time starts to bite.
- gelu LUT round-trip is per-element float work - noise next to GEMMs, don't optimize.
- Long audio: encoder windows are sequential per seek; pipelining only if long-form becomes a real
  use case.
- One WhisperSession per stream shares one loaded model; sessions are cheap (scratch + caches),
  models are not - same economics as the LLM side.

### From `qwen2_audio_plan.md` (its two duplicate "Performance ledger" sections, deduped)

- Encoder GEMMs ride the existing generated-kernel family as-is (n=1280/5120 batch shapes). A
  dedicated tune pass over encoder shapes (1500-token batch, always-prefill regime) only if the
  encoder becomes a measured bottleneck vs mtmd. *(This is the later of the doc's two takes; the
  earlier "q8 the 6 GEMM families + projector at load" phrasing is the same lever, already carried
  by the "Audio arc: the fp32 encoder is the whole ASR cost" entry above.)*
- prefill 1518 pos (1500 audio) ~ 16.4 s on M1 for the 7B q8 decoder (~93 t/s); decode ~11 t/s -
  normal 7B-on-M1 numbers, no audio-specific cost.
- mel/DFT-GEMM is noise, leave it: single-threaded `gemm_f32`, ~0.13 s per clip, and the
  Mel-as-GEMM twiddle matrices are f32 fblob consts (~410 KB) - quantizing them is noise, skip.
- 30 s chunking processes chunks serially; multi-chunk audio could pipeline encoder chunks - only
  relevant for long-audio use, not scoped.
- *(Deduped away: the all-silence-second-chunk soft-token cache appears in both sections and is
  already its own entry above - "Audio arc: the all-silence second chunk's soft tokens are
  input-independent", 2026-07-05.)*

### From `PUBLIC_BENCH_PLAN.md` (v2 backlog)

- **OPEN - v2: hosted submission server**, which also hosts / auto-fetches the pinned upstream ref
  binaries. v1 accepts community records via reviewed PR plus the result panel's "copy record"
  paste funnel; the server replaces that funnel.
- **OPEN - v2: select-two compare widget (maybe)** on the leaderboard, and publishing the
  depth/batch test variants (`pp512@d4096`, `tg128@b4`, ...) that stay internal-only under v1.

### From the prefill-driver harvest (2026-08-27)

- **OPEN - MoE k-quant twin tune families.** The moe k4/k5/k6/q51 tensor twins are gated by
  the DENSE k-quant crowns rather than raced on their own; give each its own
  `metal_tensor_race` family and crown so the gate stops proxying. Instrument:
  `metal_tensor_race` on an M5 box, one q8 and one k-quant MoE model.

### From the qwen25v Metal-tower bring-up (2026-08-29)

- **OPEN - qwen25v window-attention kernel is the naive per-thread form.** `enc_tower_win_attn`
  runs one thread per q row, scalar dots straight from device f32 (the f16-staged simdgroup
  form was reverted: its staging noise re-rolls the 32-layer chaos, and the naive form still
  lands the pair ahead - enc 106 vs 117 ms, lcpp_bench image cell vs mtmd, m5
  [direction-grade - two processes]). If the encoder profile ever shows the window
  layers, the upgrade is a simdgroup-tiled f32 form (windows are 64x80 tiles; K%16 wants pad
  to 96). Instrument: asr_prof q25v.gpu split via a skip-family knockout, npos ~1564.
- **OPEN - qwen25v full layers ride the padded slab (hs 80 -> 96).** Four of 32 layers restride
  to hs_pad 96 through the attention trio; a dk80 flash monomorph (the gemma3v dk72 recipe,
  per-head-contiguous K/V) applies if the slab ever shows at big npos.

### From the gemma4a Metal-tower bring-up (2026-08-29)

- **CLOSED (same day) - the "long-context decode gap" was the embd path's CPU PLE pre-step.**
  tg128-at-depth refuted the rail attribution (das ahead at every depth); the asr_prof turn
  buckets (`benchmarks/lcpp_bench.das --asr`) pinned 774 ms of gb1's prefill on
  ple_pre_prefill_pad - the E-series PLE model_proj GEMM
  run on CPU because only the TOKEN prefill offered the pre-step to the device gate. The embd
  path now offers it too (ple_pre_prefill_pad_gated), the CPU fallback broadcasts one gather,
  and the scalar sampler is vectorized (hargmax/hlse). gb1 cell 0.83x -> 1.00x.

### From the canary M5 baseline (2026-08-29)

- **CLOSED - canary gb1 44.6 s -> 6.5 s (2.7x AHEAD of nemo's 17.8).** Notch (a) closed by the
  Metal FastConformer driver: the 32-block loop rides one command buffer on the f32 blob
  (enc_cn_attn = full bidirectional rel-pos XL attention with online softmax, one thread per
  (q,head); enc_cn_dw = centered k9 + folded BN + SiLU; LN/GLU/SiLU/axpy/GEMM/bias reuse the
  tower kernels). gb1 encode 26.8 -> 5.9 s; cells jfk 310 ms (8.5x ahead), jfk3 895 (7.2x),
  gb1 6522; transcripts verbatim on all three; encode rel-rms vs CPU 3.1e-4; kernels-suite
  oracle+poison gates + the canary mtower cell green. Residue: the encode's CPU half (mel +
  subsample front + proj, ~5.9 s of gb1) - the gemma4a whole-chunk recipe applies if the cell
  must go further.
  Original finding (decoder half): Notch (b) is
  closed: the serving decoder is the Q8_0 re-quant (CANARY_DEC_RECIPE), cls_q8 serves and the
  blob drivers accept - jfk 1.25 s (2.12x ahead), jfk3 3.91 s (1.64x), gb1 27.7 s (0.64x),
  transcript gate verbatim both serves. What remains of gb1 is notch (a) alone.
  Original finding: Two independent notches: (a) the FastConformer-32 encoder
  is CPU f32/q8 (26.8 s of the gb1 turn) - a Metal driver is the gemma4a-recipe class of
  work; (b) the decoder cannot ride the Metal rail: the f16-sourced GGUF's TIED classifier
  serves the exact fp32 path (cls_q8 wants a Q8_0 disk embedding) and the blob drivers
  decline a tied-fp32 classifier - either an fp32-table classifier GEMV on the rail, or a
  Q8_0 re-quantized decoder artifact (provenance churn: the CANARY_DEC_RECIPE would change).
  Instrument: lcpp_bench --asr -m Canary vs canary_qwen_bench.py.

### From the CPU board sweep under the hybrid pool (2026-08-29)

- **CLOSED (same day) - MoE CPU batch pp trailed on a groupn straggler, fixed by the 32-row
  region split.** The grouped-prefill offs builder now caps a CPU sub-region at 32 rows: the
  batch-groupn dispatch chunks its region x row-group units by COUNT while a unit's cost is the
  region's row count, so one zipf-heavy expert straggled the whole barrier
  (`harness/moe_kq_probe.das`: 620 GFLOP/s zipf vs 3537 uniform vs 3929 split; the kernel
  itself was never slow - narrow D=704 costs 2%, M=32 costs 14%). Post-fix cells (das/stock-BLAS ref best): 26B-A4B 448/301 =
  1.49x (was 0.82x), gpt-oss 382/238 = 1.60x (0.99x), 30B-A3B 396/280 = 1.41x (0.91x), 35B-A3B
  410/283 = 1.45x (0.94x); sanity argmax+logit fingerprints bit-identical on all four, MoE
  family matrix cells green. GPU tiles keep whole regions (unsplit). Residue: MoE pp cv 6-9%
  under the 18-lane pool remains on both engines.
  Original finding (das/stock-BLAS
  ref, both at their best lane count: 26B-A4B 242/301 = 0.82x, 30B-A3B 254/280 = 0.91x,
  35B-A3B 266/283 = 0.94x) while the same sweep has das AHEAD on every dense Q4_K_M pp cell
  (1.10-1.28x) and at 0.99-1.12x on q8/mxfp4. Not a pool-policy artifact - the hybrid pool
  roughly doubled our own MoE pp vs 6 lanes; the gap class is the k-quant MoE batch GEMM
  (expert-grouped rows), and both engines' MoE pp turns noisy at 18 lanes (cv 2-8% vs <1%
  dense - routing imbalance across lanes). Profile (26B-A4B pp512/pp2048, --prof): the grouped
  sb_kq expert batch GEMMs are 87% of ffn wall at ~0.85 TFLOP/s vs the dense batch kernel's
  ~4.7 on the same box - NOT per-expert-M starvation (rate flat from M~32 to M~128), NOT MoE
  plumbing (route+gather+reduce ~6%), NOT q51 alone (k4 gate/up 0.87 vs q51 down 0.79); mxfp4
  MoE sits at parity, so the sb_kq expert path specifically. Remaining suspects: the narrow
  per-expert output dim (D=704 -> 88 mr=8 row-tiles; q51 mr=4) and the kq_batch_groupn
  per-region walk vs the dense flat sweep. Instrument: `harness/moe_kq_probe.das` (synthetic-k4
  bare-kernel cells at expert shapes, dense-shape control included); ref bar re-taken against
  the b10659 clean-cpu build at the arc-end re-mint.

- **The mlxfast arena trunk is a standing M5 kernel-shape intel source - watch it, adopt
  shamelessly (Boris, 2026-08-30: "if there are better shapes on M5 - we are shamelessly
  adopting").** The arena (yukon.org/mlxfast, repo Layr-Labs/mlxfast-gemma4-26b-a4b-engine,
  local clone ~/Work/mlxfast) runs a collective ratchet: every accepted submission lands on
  `main`, so the trunk is a continuously-updated record of what 26 solvers' search found on
  Apple silicon. Every figure in this entry is `external` (their arena composite scorer and
  their `mlxfast-reports` engine.md - not our instruments; the composite is a two-commit
  delta of their trunk on their box, `direction-grade`): the 2026-08 delta (+30% composite
  in 33 commits, 100% kernel work) carries three candidates for our Metal kernels:
  (1) tight-grid dispatch - launch only causally/structurally valid threadgroups instead of
  early-returning ~75% of them (their decode step waived 158k of 211k tgs; our QK/AV grids
  have the same triangular waste, their engine.md item 4); (2) identity `scale*q`
  elision (~4.4 GB/prefill of traffic on their shapes); (3) a nine-dispatch norm/residual
  fusion. External figures ground the lead only - each candidate adopts through our own race
  or knockout plus a re-minted cell. Re-check the trunk each arc; diff since the last-read
  sha before every adoption pass. License verified 2026-08-30: MIT top to bottom - harness + submissions (c) 2026
  Layr Labs, vendored mlx-swift incl. Metal kernels (c) 2023 ml-explore/Apple - so adoption
  is clean with those two attribution lines (the NAX arc's steel_attention_nax credit is the
  house pattern); model weights are not in the repo and carry their own terms.

### From the MTP depth-N arc (2026-09-02)

Instruments: `harness/mtp_ruler.das` (both engines in one run, our released `dasllama-bench` exe
first from a parent that has loaded nothing, then a stock llama.cpp `llama-server` at the ref pin
6fdd0ac89, settle 180 s, `-n 128 -r 3`, the SpecBench-4 chat corpus, greedy on the identical
rendered thinking-off prompt; one record per box and model under `performance/records/mtp/`,
`--render` prints it); the round clocks (`DASLLAMA_MTP_DEBUG=time` on the exe at measurement time,
since folded into the `mtp.draft` / `mtp.verify` / `mtp.walk` / `mtp.replay` profiler sections
under `--jobque-profiling`); `harness/batch_rows_probe.das --sameslab` (ms per same-slab step at
B rows, `-jit`). Every ours-vs-llama.cpp pair is two processes and every before/after is two
commits: direction-grade.

- **gemma-4-26B-A4B-it-Q4_K_M + the Q8_0 assistant drafter, ruler on the M5 Max (7907b133c):**
  ours off 122.7 -> depth 1 145.9 (1.19x, accept 75.2%) -> depth 2 148.1 (1.22x; p1 73.9 p2
  51.3); per prompt depth 1 144.0 / 133.4 / 160.1 / 148.6 (writing / summarization / math / qa).
  llama.cpp off 98.2 / 95.8 / 98.4 / 98.2 -> n_max 1 139.6 / 119.8 / 145.8 / 129.5 (accept 81.4 /
  70.3 / 89.6 / 76.4%) -> n_max 2 130.8 / 126.0 / 163.4 / 147.9. Same drafter, same acceptance
  shape. Direction-grade.
- **Same pair on the M4 Pro (fa0b0eac4):** ours off 59.2 -> depth 1 67.5 (1.14x, 74.6%) ->
  depth 2 55.6 (0.93x); llama.cpp off 53.1 / 51.7 / 53.2 / 53.0 -> n_max 1 67.5 / 63.4 / 72.5 /
  65.4 -> n_max 2 58.0 / 56.0 / 72.6 / 63.4. Depth is a box knob the tuner mints
  (`mtp_depth_assistant`): 1 on the M4 Pro, a tie on the M5. Direction-grade.
- **Round clocks, gemma (`--ngl 99`, SpecBench-4 chat):** M5 plain step 8.15 ms, draft chain
  0.05 ms, verify+commit 11.8 ms - the two-row verify is 1.45x a step on the MoE expert union
  (1.51x on the M4 Pro); that, not the drafter, caps the gain (#88). The Q8_0 target accepts
  76.5% vs 74.6% on Q4_K_M (`lcpp_bench --mtp-ab`, M5): the target quant is not what caps
  acceptance. The pre-norm target hidden lost to post-norm on the same rail (#95).
- **Qwen3.8-27B-Q4_K_M + its split Q8_0 NextN head, ruler on the M5 Max (497dcf10a):** ours off
  27.6 -> depth 1 33.0 (1.20x, 77.4%) -> depth 2 31.4 (1.14x; p1 83.9 p2 66.3); llama.cpp off
  25.8 / 25.4 / 24.9 / 24.6 -> n_max 1 35.1 / 33.5 / 35.0 / 32.9 -> n_max 2 34.4 / 32.7 / 36.7 /
  30.0. Their verify row is cheaper on this dense hybrid (#85). Direction-grade.
- **Same pair on the M4 Pro (f591c4c13, the k4 pair walk in):** ours off 12.7 -> depth 1 14.0
  (1.10x, 77.4%) -> depth 2 12.4 (0.97x); llama.cpp off 11.5 -> n_max 1 13.6 / 13.8 / 14.5 / 13.5
  -> n_max 2 12.3 / 12.4 / 13.9 / 11.5. On the same box before the pair walk (47422ea68): depth 1
  13.9 (1.09x), depth 2 7.9 (0.62x). Direction-grade.
- **Round clocks, Qwen3.8-27B on the M4 Pro (`--ngl 99`, `-n 64 -r 2`, plain step 78.7 ms):**
  depth 1 per round draft 7.4 ms, verify 119.0 (59.5 per row, 1.51x a step), walk+commit 1.0,
  replay 0.7, tokens/round 1.80. Depth 2 before the pair walk: draft 14.1, verify 302.5 (the third
  row on the four-column form; 3.84x a step); after it: verify 194.4, the round 4.04x -> 2.65x a
  step. Direction-grade. CPU rails on the same box (ruler `--ngl 0`): off 11.56 -> depth 1 10.66
  (0.92x, 78.6% accept), identical at `--mtp-depth 2` (#94).
- **Row ladder, gemma-4-12B-it-Q4_K_M same-slab step on the M4 Pro (`batch_rows_probe --sameslab`,
  16 steps; ms per step / x a single step, before -> after the pair walk):** 1 row 44.1 / 1.00x
  both; 2 rows 52.9 / 1.20x both (the tile); 3 rows 148.1 / 3.36x -> 83.1 / 1.88x; 4 rows 156.7 /
  3.55x -> 103.1 / 2.34x; 5 rows 166.6 / 3.78x -> 133.5 / 3.03x; 8 rows (after only, the
  rebuilt tree on the archived m4 mint) 204.7 / 4.65x. A tile pair costs 1.2 single passes and
  the step is nearly all weight streaming, so pairs add linearly out to the eighth row. The M5 Max has no
  three-row cliff: 1 / 2 / 3 / 4 / 5 / 8 rows = 20.8 / 17.3 / 24.7 / 24.6 / 42.1 / 42.7 ms = 1.00 /
  0.83 / 1.19 / 1.19 / 2.03 / 2.06x - the walk stays off there by the crown. Direction-grade.
- **Depth ladder 1..4 on the M5 Max, both carriers (2026-09-03, ruler ours-only, settle 45 s -
  the off arms drift 27.3 -> 26.0 across the four Qwen arms, so the speeds are direction-grade;
  the acceptance counts are exact; records `mtp_m5_Qwen3.8-27B_depths.json` and
  `mtp_m5_gemma-4-26B-A4B_depths.json`):** Qwen3.8-27B + Q8_0 head: depth 1 27.3 -> 33.6
  (1.23x), depth 2 26.7 -> 31.6 (1.18x), depth 3 26.3 -> 34.4 (1.31x), depth 4 26.0 -> 19.7
  (0.76x); per-position acceptance at depth 4: p1 81.3 p2 64.3 p3 34.5 p4 22.4% of rounds (depth 3:
  86.4 / 69.3 / 35.2). gemma-26B + the assistant drafter: depth 1 112.2 -> 130.4 (1.16x), depth 2
  122.5 -> 146.5 (1.20x), depth 3 122.4 -> 139.5 (1.14x), depth 4 121.5 -> 112.1 (0.92x); at depth
  4: p1 71.7 p2 47.6 p3 33.2 p4 22.5%. Tokens per round 1.77 / 2.50 / 2.90 / 3.02 (Qwen) and 1.75 /
  2.25 / 2.52 / 2.75 (gemma): the fourth draft adds a tenth to a quarter of a token. The implied
  round cost in steps (tokens per round over the speedup) is 1.44 / 2.12 / 2.21 / 3.97 on Qwen
  and 1.51 / 1.88 / 2.21 / 2.99 on gemma. Depth 3 is the best Qwen point on this box today.
  Direction-grade.
- **The sampled walk (sample-and-match, #101; `lcpp_bench --mtp-ab --mtp-temp`, `-jit` debug rail,
  M5, depth 1, `-n 128 -r 1`, SpecBench-4 chat, both arms seeded alike; a `-jit` A/B enters as
  its ratio, the arms' absolute rates stay in the run's report):** gemma-26B + the assistant
  drafter at temp 0.8: on / off 1.26x, acceptance 74.7% - the greedy rate; the sampled off arm
  sits 6% under the greedy off arm (the 262k-vocab CPU sample per token, ~0.5 ms). Per prompt 1.26
  / 1.25 / 1.31 / 1.22x at 71.6 / 77.8 / 81.4 / 68.4%. At temp 2.0 the same pair accepts 29.9%
  (42.7 / 15.5 / 71.6 / 7.6% per prompt) and the round LOSES: 0.93x, the two cold prompts 0.81x and
  0.78x - the round's cost is paid whether the draws match or not (#103). An instruct model's chat
  distributions are peaked, so at serving temperatures the draw matches the draft about as often
  as the argmax did. Qwen3.8-27B + the Q8_0 NextN head at temp 0.8: 1.28x, acceptance 70.2% (greedy
  77%); per prompt 1.26 / 1.30 / 1.40 / 1.17x at 58.0 / 71.6 / 89.6 / 64.9%. Direction-grade. Board
  rows owed for both paths (`REVIEW_MEASUREMENT.md`'s re-mint duty): no `records/<box>.json` cell
  runs a sampled decode or a CPU MTP round yet, so the sampled-decode and CPU-round cells are
  minted with the arc's records-grade re-measure.
- **Where the CPU round's time goes (Qwen3.8-27B-Q4_K_M + the Q8_0 head, M5 Max CPU rails
  `--ngl 0`, `lcpp_bench --mtp-ab --prof` `-jit` debug rail, `-n 64 -r 1`, SpecBench-4 chat; the
  `mtp.draft` / `mtp.snapshot` / `mtp.verify` / `mtp.walk` / `mtp.replay` sections; plain step
  89 ms; a `-jit` A/B enters as its ratio):** on / off 0.78x at 82.3% acceptance, 1.82 tokens per
  round. Per round in steps: the two-row verify 1.79, the recurrent-state snapshot 0.25 (151 MB
  through a scalar copy loop, 22.7 ms), the reject re-forward 0.24 (a restore plus one plain step
  on 18% of the rounds), the draft 0.09 - 2.37 steps for 1.82 tokens. The verify's `mm_gemm` is
  1.75x a step's GEMVs because the kq batch tile is four-token granular and its token tail runs
  one GEMV pass per row: two rows stream and dequantize the weights twice; the padded-tile arm
  that was timed against it and refuted is followup #104's, with its numbers. LANDED: the
  snapshot as lane-parallel memcpy, 22.7 -> 1.0 ms per round, the round 2.37 -> 2.13 steps
  (0.78x -> 0.89x); verify and replay unchanged. The reject re-forward is the other lever (#105);
  with a two-token tile the round is predicted at ~1.5 steps, 1.2x. Direction-grade.
- **Where the NextN round's time goes (`lcpp_bench --prof`, `-jit` debug rail, Qwen3.8-27B on the
  M5, prompt 0 of the corpus, 128 tokens, plain step 37 ms):** depth 1, 75 rounds: draft 3.1 ms
  per draft (0.08 step), the two-row verify 45.7 ms (1.24x a step), walk 0.3, replay 0.5; depth 3,
  54 rounds: draft 3.0 ms per draft (9 ms per round), the four-row verify 66 ms (1.79x a step),
  walk 0.6, replay 1.3. The verify is 85-92% of the round; the draft chain is 6-11%. The round's
  four-row verify costs 13% more than the batch driver's four-row same-slab step (1.59x) - the
  head layer, the serial encoder and the host prep. **The gemma round the same way (gemma-26B +
  the assistant drafter, prompt 0, plain step 8.1 ms):** depth 1, 75 rounds: the fused draft chain
  0.03 ms per draft, the two-row verify 11.4 ms (1.41x a step), walk 0.01; depth 2, 63 rounds:
  the three-row verify 13.9 ms (1.72x). The verify is 99% of the gemma round - the routed experts
  re-streamed per row plus the batch step's own overhead (the batch driver's same-slab two-row step
  reads 10.4 ms against this 8.1 ms greedy step). Direction-grade.
- **A code prompt, gemma on the M5 (c3239b46c, `benchmarks/data/code1_prompts.txt`):** ours off
  123.4 -> depth 1 159.0 (1.29x, 85.5%) -> depth 2 174.0 (1.41x; p1 86.0 p2 68.0); llama.cpp off
  99.7 -> n_max 1 140.0 (84.1%) -> n_max 2 153.7. Acceptance is a property of the text: code
  drafts a fifth better than chat. Direction-grade.
- **CPU ruler leg, Qwen3.8-27B on the M5 (4da0045f0, `--ngl 0`; llama.cpp arms only - our
  in-process CPU greedy dies on this model, #91):** llama.cpp off 9.6 / 9.4 / 9.6 / 9.6 -> n_max 1
  8.4 / 9.0 / 9.5 / 8.4. Speculation loses on their CPU rail too.
- **Context sweep on the tuned m5 mint:** the dev rail under a binary-stale sidecar served no
  `runtime.metal_tensor` crowns and read prefill at well under half the board (fixed: the runtime
  section of a binary-stale sidecar now applies, #86 step 1); the 08-30 rig exe read gemma-26B
  pp512 3861 the same day - the board row (3868) stands. The sweep's own rows are #84's,
  direction-grade on the dev rail.
- **Footprint:** the assistant drafter is a 440 MiB Q8_0 sidecar resident for the session (the
  `mtp-gdraft-load` arm asserts its size) plus its GPU workspace; a session with a head or drafter
  grows `mtp_cat` to 2 x 9 x dim floats (Qwen3.8-27B: 370 KB) and `mtp_logits_b` to 9 x vocab
  floats once (about 5 MB). Decision: taken - the round's gain rides on the drafter's presence, and
  the sidecar is a fraction of a percent of the target it drafts for.

### From the Pocket small form (2026-09-10)

Instruments: `harness/tts_synth.das` (`-jit -module-cache`, alba, the first 60 sentences of the
rig corpus, M1 Max, the box's tune profile, JIT cache warm) with the process's resident set read
through `ps -o rss` every half second, two reps per file; `harness/tts_rig.py` (alba, the 200
sentences, parakeet WER + UTMOS) per weight lane. Every pair here is two processes on one box:
direction-grade.

- **The English file, the q8 form against the small form (`pocket-tts-en-q8.gguf` 152 MB,
  `pocket-tts-en-kq.gguf` 75 MB; the one-voice `pocket-tts-en-stuart-kq.gguf` 65 MB):** the
  compiled program before the load reads 1.87 GB on both; the load adds 0.66 GB on the q8 file
  and 0.50 on the kq file; over the synthesis the q8 process holds 2.54 GB and the kq process
  2.70 - the kq lane steps up 0.33 GB at its first synthesis and stays there, the q8 lane does not
  (reps within 2 MB). RTF over the 60 sentences: q8 0.0510 / 0.0507, kq 0.0435 / 0.0434 - the
  small file decodes 15% faster and its process is 6% larger. The step is followup 129's.
- **The kq file's three lanes on the rig (the q8 file 3.91 / 4.328, its f32 lane through the f16
  file 4.32 / 4.366):** native 3.86 / 4.295, the q8 pin 3.73 / 4.295, the f32 pin 3.86 / 4.330.
  The lanes agree within the rig's own spread; the small form loses nothing the rig can hear.
  Decision: taken - the browser pages read the small forms (storywish the one-voice file, parrot
  the 19-voice one), the q8 file stays the desktop default of the served set.

### From the Vulkan batched-decode arc (2026-09-19)

Instruments: `daslang -jit benchmarks/lcpp_bench.das --npl 4` (the flat rows and the four-stream row
through the scheduler's device mode; the vulkan tier in its cm2 mode and in its KHR (mm) mode as two
arms, kv f16, the box's tuned sidecar - `DASLLAMA_ALLOW_UNTUNED=1` on the pod, whose sidecar is
untuned - `DAS_JOBQUE_THREADS=16` locally and the pod's 48 vCPUs; `DASLLAMA_GPU_PROF=1` for the token
command's stamps, which itself costs ~5% of the rate, so every pair below is profiler-on against
profiler-on or off against off), the attention ruler `harness/vk_attn_probe.das` (device timestamps
a layer), the copy ruler `harness/vk_dma_probe.das`; the reference exe llama.cpp b10660
`llama-batched-bench -m <model> -c 4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on`
on both boxes the same hour, with `GGML_VK_PERF_LOGGER=1` for its per-kernel reads. The `tg128@4`
row is the summed served rate over the scheduler step whole - sampling, detokenization and the event
list inside the clock - where the batched reference exe times the decode call alone, so every ratio
against it is conservative. The pod's RTX PRO 4500 (48 vCPUs, cm2 without decode-vector) unless
named; the local RTX 5060 Ti carries decode-vector.

- **The worker spin window on a GPU-served step (pod, Llama-3.2-1B Q8_0, 16 lanes, profiler off):**
  the scheduler's greedy step runs a team-parallel argmax a row, which wakes every worker, and each
  then spins the 30 ms window across the whole GPU step (all sixteen at 100% in `top`); on the
  48-vCPU VM that starves the driver's submission thread - the four-row command's head read 2.7 ms.
  tg128@4 summed by window: 30 ms 707, 8 ms 698, 4 ms 734, 2 ms 1065, 1 ms 1058, 500 us 1053,
  100 us 1046, 0 (park at once) 1005 - the knee is the step's own length. Shipped: 500 us while the
  resident driver serves (`jobque_spin_gpu_us`); 1B 707 -> 1067, 3B 362 -> 538, 8B 269 -> 364
  (llama.cpp 1228 / 607 / 275, `external`). The flat rows unchanged (the flat bench feeds synthetic
  ids and never samples); a `--npl 4` run loads the model at four regions, each region's context a
  quarter of the plan's, so this section's flat rows are the four-region model's - the plain
  one-region flat rows are the ones the earlier entries carry [direction-grade - two commits].
- **The decode attention at four rows (pod, profiler on, us a step over 16 layers):** the
  committed pass (a workgroup a head, two keys a step behind a subgroup reduction) 380 + 70
  combine at its rule's two splits, 327 + 102 at six, 346 + 103 at twelve - the split does not
  move it; the shipped grouped pass (a workgroup a kv head's four q heads, the accumulators in
  registers and the V words prefetched) 361 with the combine fused into the last piece. The
  ruler's per-layer floor at 32 keys unsplit is 8.3 us and a split adds 4 (the release, the
  atomic, the last piece's combine); the ruler reads 14.5 at four rows and 12.6 at one where the
  committed pass's profile read 28 and 8.5 - so the four-row form gains and the one-row form pays
  ~4 us a layer, which the transfer-queue logits repay several times over. The score FMAs, the
  softmax reductions, the K loads and the V loads each price ~2 us of an 18.5 us unsplit layer on
  the 5060 Ti, the finish 4: no single cost dominates, the chain of them does
  [direction-grade - two commits].
- **The logits' trip to host memory (the copy ruler, cached host memory):** the pod's compute-queue
  copy 4.30 GB/s at 512 KB, 2 MB, 8 MB and 32 MB alike (0.95 us a 4 KB page); the transfer queue
  11.2 / 19.4 / 24.7 / 26.8 GB/s including its submit and wait (108 us for the four-row plane
  against 487). The 5060 Ti: 6.3 / 5.7 / 5.6 / 5.5 against 5.1 / 9.8 / 12.7 / 13.8. An uncached
  host target leaves the copy at 471 us and slows the host's read to 7.8 ms; the classifier
  writing straight into host memory costs 26.7 ms. Shipped: the transfer-queue copy behind a
  compute -> transfer timeline. Pod, 1B, profiler on: the four-row step 3761 -> 3371 us, tg128@4
  1019 -> 1136, tg128 426 -> 451 [direction-grade - two commits].
- **The scoreboard after the three (pod, profiler off, tg128@4 summed, ours cm2 / ours KHR /
  llama.cpp, the llama.cpp figure `external`; then tg128 ours / theirs, theirs `external`):**
  Llama-3.2-1B Q8_0 1207 / 1206 / 1217 (0.99), flat 464 / 404; Llama-3.2-3B Q8_0 580 / 579 / 605
  (0.96), flat 194 / 184; Llama-3.1-8B Q4_K_M 379 / 379 / 361 (1.05), flat 134 / 132. The RTX
  5060 Ti the same session (cm2 with decode-vector, `-c 4096` on both sides): 1B 724 / 744 / 738
  (0.98 / 1.01), flat 258 / 242; 3B 351 / 352 / 351 (1.00), flat 107 / 104; 8B 250 / 250 / 209
  (1.20), flat 78 / 77. The 1B's four-row step under the profiler: GPU 2994 us (qkv 231, rope 65,
  attn 361, wo 210, gate 486, up 457, down 588, cls 332, the norm and requant sites 250), host 375
  (the logits' landing 285, the submit 9); llama.cpp's step 3.26 ms wall with 2.82 of kernels
  (`external`, its logger's rows): its `MUL_MAT_VEC` GEMVs 655 GB/s (`external`) to our
  `Q8GemvNT`'s 540-600 on the long-K down plane, its `FLASH_ATTN_EXT` 251 (`external`) to our
  `DaAttnT`'s 361 [direction-grade - two processes].
- **The nb-row planes' footprint, the decision `RD_NB_MAX = 8` rests on:** at nb rows every
  per-token device plane (x, xb, xq, xs, kv, q, attn, aq, as, gate, up, ffnout, xb2, cos, logits)
  is nb times its one-row size, and the partials plane is nb times. Llama-3.2-1B (dim 2048, hidden
  8192, qd 2048, kvd 512, vocab 128256, 16 layers of 32 heads at head 64), one row: x, xb, xb2 and
  ffnout 4 x 2048 x 4 B = 32 KB; xq 2048 B and xs 64 x 4 B; kv (2048 + 2 x 512) x 4 = 12 KB; q 8 KB;
  attn 8 KB; aq 2048 B and as 256 B; gate and up 32 KB each; cos one rope table row, 2 x 32 x 4 =
  256 B; logits 128256 x 4 = 513 KB; the partials 32 heads x 1 split (the pod) x 66 x 4 B = 8.4 KB
  plus the counters - 0.65 MB a row, the logits four fifths of it: 0.65 MB at nb 1, 2.6 MB at nb 4,
  5.2 MB at nb 8, against a weight pass the rows share, which streams the whole model once a step
  whatever nb. The mirror's share: `moe_gpu_binding_cap` takes `RDEC_MIR_SLACK` x 4 x regions bytes
  off the mirror's binding range (a fragment's slack rows past each region's last plane), and the
  context is negotiated inside what is left; the per-row planes cost no context. Decision: taken -
  the planes are megabytes against a weight pass shared across the rows, so the row cap is the
  command's, not memory's.

### From the Metal batched-decode arc (2026-09-20)

Instruments: `daslang -jit benchmarks/lcpp_bench.das --npl 4 --ngl 99 -r 5 --ref <llama-bench>`
on the M5 Max (the four-stream row host-cached through the scheduler and the Metal batch driver,
kv f16, `DAS_TUNE_POLICY` unset - the tuned tier - with the box's fresh rig sidecar pinned through
`DAS_TUNE_MANIFEST=performance/m5.tune.json`, the box idle - the iOS Simulator and Parsec off, a
90 s settle between cells); the flat reference rows are the pinned `llama-bench` under
`lcpp_bench`'s `--ref` spawn (`-m <gguf> -ngl 99 -t 18 -p 512 -n 128 -r 5 -o json`) and the batched
reference row is `llama-batched-bench` beside it at `-ngl 99 -t 18 -c 4096 -b 2048 -ub 512
-npp 512 -ntg 128 -npl 4 -fa on`, five runs folded. The `tg128@4` row is the summed served rate
over the scheduler step whole, the reference times its decode call alone, so every ratio reads
conservative for ours; every row here is a `-jit` script reading against a reference in another
process, so every bullet is [direction-grade - two processes] unless it says one process.

- **The scoreboard (ours / llama.cpp, tg128@4 summed; then flat tg128 ours / theirs), all Q8_0
  [direction-grade - two processes]:**
  Llama-3.2-1B 944 +/- 2 / 1031 +/- 55 (0.92), flat 323 / 317; Llama-3.2-3B 430 +/- 18 / 430 +/- 30 (1.00),
  flat 139 / 134; Llama-3.1-8B 208 +/- 2 / 199 +/- 10 (1.04), flat 67 / 64; gemma-4-E2B 441 +/- 15 /
  465 +/- 19 (0.95), flat 160 / 136 - the E2B row measured with `-p 0`: behind five pp512 reps
  in the same process it read 389 +/- 23 (0.84), a heat shadow the 1B does not cast (944 either
  way), so the batched row of a small hot model is read without a prefill row in front of it or
  after a settle. The 3B row reads a cv past 3% and stands as direction-grade until a re-run; the
  reference's E2B batched row moved between 234 and 465 across the day's runs, so that ratio's
  denominator is unsettled [direction-grade - two processes].
- **The E-series batch arm (gemma-4-E2B Q8):** per-row steps read ~90 tok/s summed at four streams
  (each stream a weight pass a token); the batched step with the PLE rows form, the Q-only shared-KV
  rows and the two-width panels reads 389 - the same step's flat row unchanged at 152 [direction-grade].
- **The deltanet batch arm (Qwen3.5-0.8B Q8, the recurrent rows form - projections as rows GEMVs,
  the conv, history, norm, scan and gate one dispatch a row against that session's mirror):**
  tg128@4 982.6 +/- 21.3 against llama.cpp's 795.7 +/- 7.2 (1.23); the flat tg128 402.1 +/- 1.1 on the
  arm's tree against 401.9 +/- 1.0 on the tree before it - the row index the five kernels gained
  costs the single row nothing. Per-row dispatch stands at ~360 small dispatches a four-row step
  on this 24-layer hybrid and still reads 2.4x the flat row; the state arena that folds them into
  one dispatch a stage is `followup_metal.md` item 22 [direction-grade - two processes].
- **The 1B's four-row step, attributed (the knockout rail `set_metal_decode_skip` under the board's
  own rep, one process, heat drifting 937 -> 865 across the ladder):** full 4267 us a step; the
  weight sites out 1952 (~2.3 ms of weights, 1.24 GB at ~540 GB/s - the memory roof); attention out
  3438 (~830 us, 52 us a layer, five times its byte roof); elementwise out 4134; the qkv sites out
  4076. The stage report (`--prof` after the batched row): GPU 3807, encode 156, handoff 70, readback
  34, setup 22 us a step. The worker spin window (30 ms against 500 us) moves nothing on this box:
  942.8 against 942.0 [direction-grade - one process].
- **The split single-pass attention serves a head of 64 (the 1B's) on the f16/f32 mirrors, where the
  batch took the chunked per-(row, head) pair [direction-grade - two commits, two processes for the
  board numbers; the forms raced interleaved in ONE process below]:** a lane owns one quad of the head, the lanes past a
  narrow head re-read a valid quad with a zero query and never store, the combine merges four subgroups
  at 128 threads whatever the head. Llama-3.2-1B Q8 tg128@4 942 -> 1024 +/- 10 against llama.cpp's
  1026 (0.92 -> 1.00); the flat tg128 unchanged at 324; the head-128 kernel cells bit-exact before
  and after; the batch parity arm's f16 and f32 rows within their bars on the new form. The q8_0 and
  tq4 mirrors keep the chunked pair at head 64 (the quant twin's lane mapping is its own). The
  two forms raced interleaved in one process at the served shape (`set_metal_attn_d` flipped
  between reps of the four-stream row on the 1B, five pairs, the first pair the warm-up): the
  split form 815-951 against the chunked pair 699-858 tok/s summed, the split form ahead in every
  pair after the first (rep 1: 950.7 against 857.7; rep 3: 920.8 against 699.1); the reps drift
  past a 3% cv in the un-settled process, so the ranking is settled and the margin is not
  [direction-grade - one process, cv past 3%].
- **The NextN carriers' batched rows, plain and self-speculative (three reps, `-p 0`):** Qwen3.5-0.8B-MTP
  Q8_0 plain tg128@4 977 +/- 4 against llama.cpp's 803 +/- 4 (1.22), flat 402 / 303; Qwen3.6-27B-MTP
  Q4_K_M plain 56.3 +/- 0.3 against 45.9 +/- 2.1 (1.23), flat 27.6 / 25.3 - the batched step lands a
  NextN model's logits on the GPU like any other's now (the gate that sent them to the CPU classifier
  is gone) and carries each row's post-norm hidden as its stream's speculative carry. The
  `--npl-mtp` arm - every stream drafting on its own chain, the four streams' eight verify rows in ONE
  pass - reads 720 +/- 11 on the 0.8B and 50.3 +/- 3.6 (cv 7%) on the 27B: below the plain row on both,
  on the bench's synthetic ids, where the drafts are accepted as often as noise and the round pays
  four draft passes (each a NextN layer plus the 248k-row classifier) and a double-width verify for
  them. The arm proves the joint verify at four streams; its served rate is the ruler's question
  (`ARCHITECTURE_MEASUREMENT.md#ruler-records` of the measurement doc) and the batched draft pass - the four drafts as rows of one
  dispatch - is the lever the ledger row names [direction-grade - synthetic ids, two processes].
- **The sidecar reaches the batched row:** the 1B's tg128@4 read 840 under the shipped class profile
  (`DASLLAMA_ALLOW_UNTUNED=1`) and 944 under the box's fresh mint in the same tree - the runtime knobs
  and the Metal crowns are part of the step, not only its provenance [direction-grade - two processes].
- **A loaded box voids the batched row first [direction-grade - two processes, a loaded box]:** with an iOS Simulator rendering (two WebContent
  processes at 98% and 48%, SimMetalHost on the GPU, load 12) the 1B batched row read 466 +/- 277 and
  the reference's own flat tg128 fell from 317 to 200 - the GPU queue is shared with the simulator's
  Metal clients, and the step's CPU half (sampler, encode, spin) loses its performance cores. The
  amortized weight stream leaves dispatch latency and CPU work as what is left per step, so four rows
  feel the load where one row hides it.
### From the close-all-gaps arc, the Metal catalog rows and the CPU hybrid step (2026-09-21)

Instruments: the released bench exe (`daspkg release --root modules/dasLLAMA/benchmarks --out
modules/dasLLAMA/performance/_rig`, its own fresh mint - noise ok, validation ok, 62 kernels - and
DAS_TUNE_MANIFEST unset) at `-p 0 -n 128 -r 5 --npl 4 --ngl 99 --ref <llama-bench>` on the idle M5
Max, the reference rows as the Metal batched-decode section's; the CPU row is the `-jit` script under
`--for-debug-purposes` (the arm's tree, not the released exe) at `--npl 4` with no `--ngl`, its
reference `llama-batched-bench` at `-ngl 0`. Every ratio is tg128@4 summed over four host-cached
streams, the step served by the batch driver (the row refuses otherwise) [direction-grade - two
processes throughout].

- **The Metal rows the E-series, shared-expert and deltanet arms unlocked, all Q8_0 (ours /
  llama.cpp, tg128@4; then flat tg128 ours / theirs):** Qwen3.5-4B 232 +/- 17 / 220 +/- 2 (1.05)
  read first, in the mint's heat shadow, then 295 +/- 12 / 248 +/- 10 (1.19) on the settled box -
  both sides moved, so the first read is the shadow, not the arm; flat 108 / 93. Qwen3-30B-A3B
  256 +/- 15 / 215 +/- 3 (1.19), flat 123 / 105. Qwen3.6-35B-A3B 232 +/- 7 / 198 +/- 0 (1.17), flat
  122 / 91. The 4B and 30B rows read a cv past 3% and stand as direction-grade until a re-run.
- **The batched-step decline census (`harness/batch_decline_census.das`: one four-stream
  host-cached row per carrier, then `batch_step_census` and the driver's declines-by-reason
  table):** every official catalog row that loads - gemma-4-E2B and E4B Q8, Qwen3VL-4B Q8,
  Qwen2.5-Omni-3B Q8, gemma-4-12B Q4_K_M, gpt-oss-20b mxfp4, Mistral-Small-24B Q4_K_M,
  Qwen3.6-27B-MTP Q4_K_M, Qwen3-30B-A3B Q4_K_M, Qwen3.8-27B Q4_K_M, Qwen3.6-35B-A3B-MTP UD-Q4_K_M -
  and the Q8 board carriers (Llama-3.2-1B/3B, Llama-3.1-8B, Qwen3.5-0.8B and its MTP twin,
  Qwen3.5-4B, Qwen3-30B-A3B, Qwen3.6-35B-A3B) served eleven device steps, no CPU-stack step, no
  decline; the one per-row step each is the row's one-stream tail, per-row by definition. The
  gemma-4-26B-A4B Q4_K_M row never stepped: its image mint declined on the map-back (two sections
  named `q51q` - the legacy top-level Q5_1 plane pair beside the per-format table's q51 slot, the
  streaming writer keyed by name), the defect the arc fixes.
- **The CPU batched stack's hybrid form (Qwen3.5-0.8B Q8, the deltanet rows form per session +
  the gated attention rows, no device driver):** tg128@4 517 +/- 8 against llama.cpp's 471 +/- 10
  at `-ngl 0` (1.10); the flat tg128 208 +/- 18 / 178 - the batched step reads 2.5x the flat row
  [direction-grade - two processes, the `-jit` script].
- **The block codecs on the partial-rope carrier (the q8_0 / tq4 rope-store kernels take `rot`):**
  the census (`harness/batch_decline_census.das -- --kv q8_0 | tq4 <gguf>`, the `-jit` script,
  the untuned tier) serves Qwen3.5-0.8B Q8 and Llama-3.2-1B Q8 batched at four streams on both
  codecs, no decline. The tq4 signs table read past its 128 floats on every 256-wide head: the
  support matrix's `fam-qwen35` row (`tests/run.das -- --suite matrix --arm fam-qwen35 --family
  qwen35`, the Metal-override generate against the CPU truth token-for-token, per KV codec) reds
  on tq4 at master and passes with the table at the kernels' 512 ceiling; the forced-feed probe
  (`harness/forced_feed_probe.das -- <gguf> --kv tq4 --steps 6` - a CPU prefill, then six steps
  fed the CPU chain's tokens on a CPU and a GPU-decode session, per-step logits maxd printed)
  localized it to the tq4 rows of every 256-wide head, gemma-2-2b Q4_K_M included, with argmax
  flips on the unfixed table and none on q8_0 [one process, the M5 Max].
- **Nine streams (the census at `--npl 9`):** every non-hybrid K-quant catalog carrier serves
  batched - gemma-4-12B Q4_K_M, Llama-3.2-1B Q4_K_M, Qwen3-30B-A3B Q4_K_M, Mistral-Small-24B,
  gemma-4-26B-A4B Q4_K_M, gemma-4-E2B Q8 - no off-lattice decline in the catalog; every hybrid
  declined `dn_state` (the four-mirror LRU evicting the rows of the step it prepared) and the Q8
  hybrid then faulted on the device (nine rows written into nine-row planes by the rows form's
  four-row tiles); with the cache grown to the batch and the planes sized to the tile
  Qwen3.5-4B Q4_K_M, Qwen3.6-35B-A3B UD-Q4_K_M and Qwen3.5-0.8B Q8 serve at nine, the 0.8B at
  twelve too.
- **The drafts as rows steps (the self-speculative batched row, `--npl-mtp`, three or five reps,
  `-p 0`):** Qwen3.5-0.8B-MTP Q8 spec @4 720 -> 881 +/- 10 against the plain row's 987 +/- 47 in the
  same session; Qwen3.6-27B-MTP Q4_K_M read spec 41.4 +/- 7.0 (cv 17%, void) beside plain 54.4 in
  one pair and spec 45.3 +/- 2.3 beside plain 41.8 +/- 0.2 in the next, the plain row itself moving
  54 -> 42 between the pairs - the box's heat under the untuned debug script [direction-grade - the
  `-jit` script, untuned, synthetic ids]. The released exe on the idle box (five reps, `-p 0`,
  its own fresh sidecar): Qwen3.5-0.8B-MTP plain 977 +/- 6 against llama.cpp's 811 (1.21), spec
  890 +/- 9 - 0.91 of the plain row where the per-stream chain read 0.74, and above the
  reference's plain row (1.10); Qwen3.6-27B-MTP plain 52.2 +/- 2.4 against 38.6 (1.35), spec
  47.5 +/- 5.8 (cv 12%) - 0.91 of plain against 0.89 before, the bar too wide to rank the two
  draft forms on this carrier; on synthetic ids every accepted draft is noise, so the spec row's
  ceiling is the plain row and the served rate is the ruler's question (`ARCHITECTURE_MEASUREMENT.md#ruler-records` of the
  measurement doc) [direction-grade - two processes].

### From the server-MTP arc, the batched pre-encoded step (2026-09-21)

- **What the pre-encoded step holds (`ARCHITECTURE_GPU_MTP_DECODE.md#batch-pre-encode`).** A pre-encoded
  batched step holds its own set of `batch_step_build`'s pooled buffers beside the step in flight, so
  the pool's peak grows by one step's set. The bytes are `batch_step_build`'s own sizing at four rows
  (the server's default stream count, the fixed-row forms' four-row pad) over each carrier's loaded
  `Config`, each buffer rounded up to the pool's power-of-two bucket. The fixed part - the rows'
  activations, the FFN pair and the logits plane - is 3.5 to 5.6 MB on the catalog's large carriers
  (gemma-4-26B-A4B 4.6 MB, Qwen3.8-27B 5.6 MB, gpt-oss-20b 4.5 MB, Mistral-Small-3.1-24B 3.5 MB,
  gemma-4-E4B 4.9 MB). A carrier the fused partD attention does not serve (a head size other than
  64 or 128, mixed head sizes, a sliding window, sinks) also holds the chunked attention's partials:
  rows x heads x (largest head size + 2) x 4 bytes per 64-key chunk of the deepest row. At each
  carrier's trained context that is gemma-4-26B-A4B 539 MB at 262144 keys (a 1 GiB bucket),
  Qwen3.8-27B 406 MB at 262144 (512 MiB), gpt-oss-20b 138 MB at 131072 (256 MiB), gemma-4-E4B
  135 MB at 131072 (256 MiB); Mistral-Small-3.1-24B rides partD and holds none. Decision: the rail
  ships off (`DASLLAMA_METAL_BATCH_PRE=1` arms it) - the E4B four-row step times the same with it
  on and off (`followup_metal.md` row 23), so the second set buys nothing until a step chained on
  the GPU's own picks lands on top of it.

### From the Vulkan batched-decode arc, the qwen and phi carriers (2026-09-20)

Instruments as the section above: `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod (RTX
PRO 4500, driver 580.173, cm2 without decode-vector, and KHR) and the local RTX 5060 Ti (driver
616.56, cm2 with decode-vector), `DASLLAMA_ALLOW_UNTUNED=1` and no `DAS_TUNE_POLICY` override on
either box (an untuned sidecar serves the reference bodies), `DAS_JOBQUE_THREADS=16`, llama.cpp
b10660's `llama-batched-bench` under the section above's command line the same hour on the same
box (every llama.cpp figure below `external`, from that exe's own table), `DASLLAMA_GPU_PROF=1`
for the stamps, the attention ruler `harness/vk_attn_probe.das` for the key split. Every ratio is
`tg128@4` summed over four device-home streams through the scheduler's device mode against the
reference's `S_TG` at `-npl 4`, conservative as before (the served step samples and detokenizes
inside the clock); every lever's before -> after pair is two commits in two processes
[direction-grade - two commits], every ratio against llama.cpp two processes [direction-grade -
two processes]. The wide twin's command buffers (one a region, one a row count) are pool handles
the recorder fills - no plane grows with them, so the ladder's footprint half is nil.

- **The baseline (master before the arc, the pod):** qwen2 0.5B 1205 against 1385 (0.87 - batched,
  a gap); qwen3 0.6B 425 against 1193 and 4B 129 against 473 (0.36 and 0.27 - not batched: the N-row
  command answered zero rows for a q/k-norm model, so the scheduler stepped the four streams a row
  at a time, four weight passes a step, below one stream's rate).
- **The q/k-norm form of the N-row command:** the per-head rms kernel was already row-indexed, and
  the fused norm+rope+store kernel gained the row in its workgroup id; 0.6B 425 -> 1044, 4B 129 ->
  427 (0.87 and 0.90). The fused kernel and the split pair round apart (the compiler contracts
  each kernel on its own - the earlier "bit-identical" claim was false), so the rows take the
  form the one-row command takes and read bit for bit. On the 5060 Ti's Windows driver the
  one-row form then read twelve logits off the CPU chain: the new row arithmetic divided by a
  push field that is zero in the one-row form behind a select, and that driver evaluates both
  arms - an integer division by zero is undefined in SPIR-V. The divisor is clamped;
  `DASLLAMA_VK_FUSE_BISECT` names such a kernel in seven runs against the CPU chain.
- **The key split follows the attended span, not the SM count (the ruler, us a layer, qwen2 0.5B
  geometry, the pod):** at 640 positions one piece 14.5 and sixteen 33; at 2048 four 20.6 and
  sixteen 33; at 4096 four to eight 27 and sixteen 34; one row or four the same at four pieces or
  fewer. The old rule recorded sixteen for one row and eleven for four; the ladder (one under 512,
  four to 3072, eight past it) cut the 0.5B's four-row attention 793 -> 446 us a step, and the
  qwen3 0.6B's 767 stays bandwidth-bound (eight kv heads of 128, the rows' K/V bytes eight times
  the 0.5B's, linear in rows). It also puts the one-row and N-row forms in one summation order,
  so the qwen3 regions cells read bit for bit on the 36-SM card where the rules had them nine and
  sixteen pieces apart. Pod: 0.5B 1356, 0.6B 1111, 4B 449 (0.98, 0.92, 0.95).
- **A residual epilogue fused into the GEMV's last workgroup does not batch (Decision: rejected,
  the tree keeps the separate residual dispatch):**
  the N-column form of `Q8GemvAr`, bit-exact in its kernel cell and the regions cells, read the
  0.5B 1076 -> 988 and the 0.6B 822 -> 784 on the 5060 Ti: the last workgroup runs the rows'
  residual steps one after another where the separate `cls_ar_rq_b` dispatch runs them in
  parallel workgroups. The fused gate-up's N form (`Q8GemvGuN`, a subgroup a column quantizes)
  batches and is bit-exact, and bought nothing measurable on either card (pod 0.5B 1331, 0.6B
  1103, 4B 454); kept for the dispatch it saves.
- **The rows' logits a row a job-queue lane (`rd_land_logits_n`):** the one-lane form passed
  2.4 MB of a 152k-vocab plane twice, 322 us of a 3037 us step on the pod; the lanes read the
  plane once. 5060 Ti: 0.5B 1068 -> 1172, 0.6B 833 -> 895. A parallel sample of the rows threw in
  a forked context and stays `followup_vulkan.md` item 76.
- **The Q8_0 N-column GEMV's column loads issue together (`Q8GemvNT`):** the qwen2 1.5B's
  four-row step on the pod read 4313 us against the one-row step's 2963, and the stamps put the
  gap in the N-column GEMVs - the down GEMV 1144 against the one-row form's 694 (n 8960, d
  1536), the qkv 340 against 229, the wo 289 against the fused 289 plus a 168 requant - where
  the fused gate-up at n 1536 sat 14 percent over its one-row twin. Each column's activation
  load sat behind its own `c < ncols` branch, so the four loads issued one after another, an L2
  latency each, per weight word. Unguarded (a column past the live ones dots the last live
  column's row again, its sum never stored), the step reads 3775: down 744, qkv 286, wo 230.
  Pod, cm2 / KHR: 1.5B 871 -> 980 / 982, 0.5B 1443 -> 1615 / 1604, 0.6B 1154 -> 1261 / 1251, 4B
  459 -> 482 / 481; the 5060 Ti 0.5B 1172 -> 1246, 1.5B 615 -> 621 (its decode-vector rail hid
  most of the serialisation). Bit for bit in the kernel cell and the regions cells.
- **The ruler on both sides of the ladder's edges (us a layer, the pod, one row / four rows):** the 0.5B
  geometry at 384 one piece 10.4 / 10.4 against four 16.5 / 16.5, at 640 14.5 / 14.5 against 16.5 /
  16.5, at 1024 18.6 / 18.6 against 16.5 / 16.5, at 1536 26.7 / 26.7 against 18.6 / 20.6 - the
  one-to-four edge sits between 640 and 1024 for this geometry; the qwen3 0.6B geometry (head 128,
  two heads a kv head) at 384 one piece 14.5 / 14.5 against four 12.4 / 18.6, at 640 20.6 / 20.6
  against 14.4 / 20.8, at 1024 28.8 / 28.8 against 14.5 / 24.6 - its edge sits at 512 for one row
  and near 640 for four. `RD_UNSPLIT_POS` stays 512: the wrong side of either edge costs two
  microseconds a layer. The wide edge: at 8192 eight pieces read 32.9 against four's 41.1 for one
  row of the 0.5B geometry (37.3 against 59.2 for the 0.6B's) and 51.3 against 44.3 for four rows
  (189 against 181); at 16384 47.3 against 71.6 for one row, 81.1 against 73.8 for four. Decision:
  taken - the one-row command keeps its wide twin from 3072, the N-row command takes the split
  form at every span past 512 and its wide twin, its recorded flag and its stamp slots go.
- **The fused gate-up N form keeps its column guard (Decision: rejected - the unguarded form):**
  the same clamp that freed the plain N-column GEMV's loads made `Q8GemvGuN` slower - the 1.5B's
  four-row gate-up 1212 -> 1252 us a step on the pod, the step 3775 -> 3847, tg128@4 980 -> 965,
  the 4B 482 -> 477 and the llama 1B 1343 -> 1325 - because its unroll is eight columns wide
  whatever the count where the plain GEMV picks a stamp by count: a dead column's two dots against
  both planes cost more than its branch. The stamp by column count is `followup_vulkan.md` item
  77's fold. The ladder caps a sliding-window layer's count at its window's span: a windowed
  carrier (gemma-3's 512, gemma-4's 1024) attends at most its window, so its local layers take
  one piece at every position where the position alone read four or eight; no windowed carrier
  sits on this arc's board, so the count is the ruler's reading, not a measured step.
- **The boards at the arc's tip, tg128@4 against llama.cpp the same hour** - the pod, cm2 / KHR:
  qwen2 0.5B 1611 / 1602 against 1373 (1.17 / 1.17), qwen2 1.5B 980 / 981 against 920 (1.07 /
  1.07 - the flat row 335 against 314), qwen3 0.6B 1267 / 1251 against 1199 (1.06 / 1.04), qwen3
  4B 483 / 481 against 476 (1.01 / 1.01), Phi-3.5-mini Q4_K_M 524 / 522 against 418 (1.25 / 1.25,
  the K-quant leaves the lever does not reach); the llama family at the same tip 1B 1347 / 1345
  against 1220 (1.10 / 1.10), 3B 613 / 612 against 606 (1.01 / 1.01), 8B 380 / 378 against 361
  (1.05 / 1.05). The
  5060 Ti, cm2 with decode-vector: 0.5B 1211 against 922 (1.31), 1.5B 624 against 578 (1.08),
  0.6B 915 against 740 (1.24), 4B 273 against 263 (1.04, under `DASLLAMA_GPU_VRAM_MB=9000`: the
  16 GB card holds 3.5 GB of desktop and the four-region mirror otherwise pages), Phi-3.5-mini
  Q4_K_M 348 against 317 (1.10 - an earlier reference row of 183 on this box was the outlier: the
  reference re-run the same hour as the CUDA rows below read 924 / 577 / 743 / 266 / 317 for the
  five, the first four within a percent of the rows above); the desktop box reads three percent
  apart run to run on the 0.5B (1246 the run before). The flat rows did not move.
- **llama.cpp's CUDA build beside the Vulkan reference on the 5060 Ti (every figure `external`;
  the same b10660 checkout built with CUDA 13.4 for sm_120, `llama-bench -ngl 99 -fa 1 -r 3` for
  pp512 / tg128 and `llama-batched-bench` under the section's command line for tg128@4; ours the
  rows above, not re-run) [direction-grade - two processes]:** pp512 / tg128 / tg128@4 - Llama-3.2-1B
  Q8_0 21185 / 262 / 857, Llama-3.2-3B Q8_0 8628 / 108 / 371, Llama-3.1-8B Q4_K_M 3630 / 82 / 245,
  Qwen2.5-0.5B Q8_0 34230 / 488 / 1412, Qwen2.5-1.5B Q8_0 14831 / 202 / 652, Qwen3-0.6B Q8_0
  26458 / 403 / 1034, Qwen3-4B Q8_0 6361 / 86 / 294, Phi-3.5-mini Q4_K_M 6503 / 144 / 312. Against
  ours at four streams: 0.5B 0.86, 1.5B 0.96, 0.6B 0.89, 4B 0.93, phi 1.12; the llama family's
  local rows are the section above's (1B 724, 3B 351, 8B 250: 0.84, 0.95, 1.02); flat, ours reads
  0.93 to 0.99 of CUDA on every carrier. CUDA's batched rows beat its own Vulkan rows by 1.06 to
  1.53 on the Q8 carriers (0.98 on phi's K-quant) on this card, so the Vulkan-reference ratios
  above overstate ours by that much against the card's best engine.
- **Phi's prefill at half the reference is the attention, not the K-quant tiles (the prefill
  profiler's role stamps, 512 tokens, us over 32 layers): q 7420, k 6858, v 4807, rope 3208,
  attn 81557, wo 6825, gate 15453, up 14034, down 17669, 162 ms whole; the reference's per-kernel
  log the same window: qkv 23.0 ms, gate-up 30.3, down 27.9, flash attention 2.65 (83 us a layer at
  head 96).** The GEMM roles read on par or ahead (the k5 and k4 decode-in-load tiles 55 and 60
  TFLOP/s on the ruler's l stamp); the flash tile admits heads of 64, 128, 256 and 512 alone, so
  phi's head of 96 runs the chunked pair at thirty times the flash cost - `followup_vulkan.md`
  item 78 [direction-grade - two processes].

### From the Vulkan batched-decode arc, the gemma dense carriers (2026-09-20)

Instruments as the two sections above - `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod
(RTX PRO 4500 Blackwell, driver 580.173, cm2 without decode-vector, and the KHR arm under
`DASLLAMA_COOPMAT=mm`), `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=16`, llama.cpp b10660's
`llama-batched-bench` under the same command line the same hour (every llama.cpp figure `external`),
`DASLLAMA_GPU_PROF=1` for the stamps. Every ratio is `tg128@4` summed over four device-home streams
against the reference's `S_TG` at `-npl 4` [direction-grade - two processes]; every lever's pair is
two commits in two processes [direction-grade - two commits].

- **The baseline (master after the qwen section, the pod, one rep each):** gemma-3-1b Q8_0 1056
  against 869 and gemma-3-4b Q8_0 499 against 445 - served by the N-row command already (no epilogue,
  no epilogue); gemma-2-2b Q8_0 195 against 616 (0.32) and gemma-4-12B Q8_0 53 against 186 (0.28) -
  not batched: the command declined the classifier epilogue (the final softcap, gemma-4's suppressed
  ids), so the four streams stepped a row at a time, below one stream's rate. The 12B's global
  layers take V from K, which the command served already; the file shares no K/V across layers
  (`gemma4.attention.shared_kv_layers` 0 on the 12B and the 31B, 20 and 18 on the E2B and E4B,
  which carry per-layer embeddings too).
- **The epilogue's rows form:** gemma-2 195 -> 687 (1.11), the 12B 53 -> 173 (0.93). The 12B's
  four-row step under the profiler, us: qkv 2440, qknrope 295,
  attn 1853, wo 1428, arrq_f 592, gu 10153, down 3976, arrq_n 594, cls 1252, epi 10, 22677 whole
  against the flat step's 20556 (gu 8404, attn 628, down_ar 5143, wo_ar 2133): the fused gate-up's
  N form and the attention carried the batch's whole overhead, the host between steps under half a
  millisecond (the wall step 22935 against a 22697 fence). Under `DASLLAMA_VK_FUSE_BISECT=4` the
  split gate, up and act requant read 3981 + 3921 + 197 = 8099 at four rows against the fused
  form's 10107, and the row 187.9 (1.01) - the fused N form was the gap.
- **The fused gate-up's N form as the plain q8 leaf's shape (`Q8GemvGuNT`, stamped at two, four
  and eight columns, a column's half-block one 16-byte load beside the weight word, a column past
  the live ones re-dotting the last live column):** the 12B's gu 10153 -> 8553 a step, the row
  171.5 -> 185.1 +/- 0.07 (0.99, three reps); gemma-2 688 -> 702; Qwen2.5-1.5B 980 -> 1019 against
  the qwen section's 920 (1.11, that section's 1.07). The eight-column guarded unroll it replaced loaded a column's block as four
  words under a guard on every column - the clamp alone the qwen section priced kept that shape,
  which is why it read slower there; the stamp is a different kernel. Against the split pair the
  stamp still reads 0.45 ms behind at this shape: the remaining room, with the host's half
  millisecond.
- **The two-head attention slab (`DaAttnT` at `G = 2` on a group of one or two heads, the same
  workgroup count):** the 12B's four-row attn 1835 -> 1729 (6%, the lever-2 tip's profile against
  the lever-3 tip's), the row 185.1 -> 186.15 +/- 0.16
  against 186.19 (1.00); gemma-2 704 -> 710. The pass is bound by its K/V reads and chunk barriers
  (the four-row attention at 57% of the mirror's byte roof), not by the dead heads' FMAs -
  `followup_vulkan.md` item 73 carries the reading. The two slabs raced in one process on the RTX
  5060 Ti (`harness/vk_attn_probe.das` with its slab arm, four rows at 640 positions, device
  timestamps a layer, the arms alternated four-head / two-head / four-head / two-head): the 12B's
  sliding shape (head 256, 16 heads, two a kv head) 40.9 / 40.9 -> 27.5 / 27.5 us a layer at one
  split and 35.8 / 36.0 -> 26.6 / 26.6 at two; the gemma-2 shape (head 256, 8 heads, two a kv
  head) 39.0 -> 26.6 at one split and 22.5 -> 18.4 at four, its best split - the two-head slab
  a quarter to a third faster a layer where the pod's step read 6%, the smaller card's pass
  nearer its FMA bound. The same race on the pod (82 SMs, the arms alternated the same way, the
  four-head rows repeating within 0.5 us across the two orders): the 12B's sliding shape 32.9 ->
  26.8 at one split, 24.7 -> 20.6 at two and 28.8 -> 18.6 at four, its best; gemma-2's shape 32.9
  -> 26.8 at one split and 18.6 -> 16.5 at three and four - a third at the best split where the
  served step moved 6%, so the layer's attention is a smaller share of that step than of the
  smaller card's.
- **The board at the arc's tip, tg128@4 (ours cm2 / ours KHR / llama.cpp, three reps ours; then flat
  tg128 ours / theirs - the reference's flat figure is the same `llama-batched-bench` run's
  `S_TG` at `-npl 1`, ours the `-npl 4` run's flat row):** gemma-2-2b Q8_0 704 / 709 / 618 (1.14 / 1.15), flat 216 / 201; gemma-3-1b
  Q8_0 1084 / 1089 / 878 (1.23 / 1.24), flat 396 / 320; gemma-3-4b Q8_0 507 / 507 / 446 (1.14), flat
  149 / 136; gemma-4-12B Q8_0 185 / 185 / 186 (0.99 at the lever-2 tip; 186.15 / - / 186.19 at the
  arc's tip, 1.00), flat 55.1 / 54.3; gemma-4-12B Q4_K_M 233 / 233 / 184 (1.26), flat 81.9 / 78.1;
  gemma-4-31B Q4_K_M 103 / 103 / 90 (1.14), flat 36.6 / 32.9. The KHR arm reads the cm2 arm's rows:
  the decode kernels are one set on both. The reference's pp512 column on gemma-2-2b and gemma-3-1b
  reads 87 to 4395 tok/s across its runs (`external`) and is no prefill reference on those two.
- **The RTX 5060 Ti at the section's tip (driver 616.56, the desktop holding 0.9 to 2.1 GB of the
  16 GB, `DAS_JOBQUE_THREADS=16`), tg128@4 ours cm2 (with decode-vector) / ours KHR
  (`DASLLAMA_COOPMAT=mm`) / llama.cpp Vulkan / llama.cpp CUDA (the same b10660 checkout built with
  CUDA 13.4, every reference figure `external` from `llama-batched-bench` under the section's
  command line the same hour; then flat tg128 ours cm2 / theirs Vulkan / CUDA):** gemma-2-2b
  Q8_0 411 / 414 / 363 / 412 (1.13 against Vulkan, 1.00 against CUDA), flat 124 / 115 / 123;
  gemma-3-1b Q8_0 734 / 738 / 564 / 810 (1.30 / 0.91), flat 246 / 208 / 236; gemma-3-4b Q8_0 294
  / 292 / 258 / 291 (1.14 / 1.01), flat 84 / 78 / 84. The 4B's first pass ran in the paging class
  (3.2 flat on both arms) with the desktop holding 2.1 GB beside its 11 GB plan; the rows above are
  its repeat on the cleared card (0.9 GB held), the plan the same 8187 MB mirror at ctx 15412 that
  served at 1.4 GB held. The gemma-4-12B files fit no four-region plan on this card - the resident
  driver declines the Q4_K_M at 15.6 GB of 12.2 to 13.3 asked (item 80 carries the message's unit)
  and the Q8_0 outright (12.1 GB of weights against the 9 GB cap), the per-op rails serve them a
  row at a time at 15 to 16 (Q4_K_M) and 14 to 15 (Q8_0) summed - and the reference pages: its
  Vulkan rows read 106 (Q4_K_M) and 46 (Q8_0) at four streams, its CUDA rows 141 and 88 - so the
  12B's board is the pod's above. Ours reads at or above CUDA's four-stream rate on the two
  carriers CUDA does not win outright, and 0.91 of it on the 1B.
- **The 12B files homed on the 5060 Ti (2026-09-21, the bench pinning its batched row's context
  before the load - `followup_vulkan.md` item 81's first rung - so the plan sizes the mirror to
  the 660 positions a region the row serves instead of the binding cap's 6238):** gemma-4-12B
  Q4_K_M 151 +/- 0.2 / 106 / 141 (1.42 against Vulkan, 1.07 against CUDA), flat 48 / 44 / 48,
  the image 7179 MB with an 866 MB mirror; gemma-4-12B Q8_0 104 +/- 0.04 / 46 / 88 (2.25 / 1.18),
  flat 30 / 21 / 26, the image 12064 MB with the same mirror - 13.2 GB on the 16 GB card, the
  desktop holding 0.9 GB, no paging flag. The reference pages both files there.
- **Our gemma-2-2b pp512 on the pod reads 6468 to 19388 across runs (three-rep cv up to 38%)
  while its tg rows hold within 1%** - `followup_vulkan.md` item 79; the gemma-3 and gemma-4 rows
  hold within 2% on pp512.

### From the Vulkan batched-decode arc, the MoE carriers (2026-09-21)

Instruments as the sections above - `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod
(RTX PRO 4500 Blackwell, driver 580.173, cm2 without decode-vector, and the KHR arm under
`DASLLAMA_COOPMAT=mm`), `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=16`,
`DASLLAMA_PARITY_FULL=1` (every carrier is large-tier), llama.cpp b10660's `llama-batched-bench`
under the same command line the same hour (every llama.cpp figure `external`). Every ratio is
`tg128@4` summed over four device-home streams against the reference's `S_TG` at `-npl 4`
[direction-grade - two processes]; the lever's pair is two commits in two processes
[direction-grade - two commits].

- **The baseline (master 1c6ce2d2f, the pod, three reps ours), tg128@4 ours / llama.cpp, then flat
  ours / theirs:** gpt-oss-20b mxfp4 196 / 234 (0.84), flat 213 / 97; Qwen3-30B-A3B Q4_K_M 159 /
  312 (0.51), flat 174 / 184; Qwen3-30B-A3B UD-IQ2_XXS 165 / 196 (0.84), flat 184 / 86;
  gemma-4-26B-A4B Q4_K_M 129 / 264 (0.49), flat 140 / 94; gemma-4-26B-A4B UD-IQ3_XXS 129 / 232
  (0.55), flat 141 / 94. The N-row command declined every MoE layer, so the four streams stepped a
  row at a time and read below one stream's rate on every carrier. Qwen3.6-35B-A3B UD-IQ2_XXS
  (the hybrid) 30 / 236 (0.13), flat 167 / 112: the recurrent layers' one device state slot goes
  home and back on every stream switch - the hybrid arc's residency change, the board's worst
  served shape.
- **The reference at decode size groups nothing by expert.** The local llama.cpp clone
  (`D:/Work/llama.cpp`, the research report in the arc's notes): Vulkan takes its mat-vec path
  for up to 8 tokens (`mul_mat_vec_max_cols` in `ggml-vulkan.cpp`) and loops one dispatch a
  token on the host, CUDA one launch with a warp per token-slot pair (`MMVQ_MAX_BATCH_SIZE` in
  `ggml-cuda.cu`) - both read N x k expert planes a step; the grouped form (a counting sort into
  per-expert buckets, one GEMM with a grid axis per expert) sits past those thresholds. So the batched
  step's gain on a MoE carrier is the attention, the router, the shared expert and the submit,
  never the experts' bytes - and the one-row block's regions form (the k experts as regions of
  one slot-mapped dispatch a plane) is already the shape the reference's Vulkan loops per token.
- **The routed block's rows form (the router's columns, the per-row top-k over the shared record
  base, the expert GEMVs as the one-row leaves over `nrows x k` regions, the act over every slot,
  the unfolded down, the combine per row; `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`), cm2 /
  KHR / llama.cpp at tg128@4, three reps ours:** gpt-oss-20b 436 +/- 7 / 434 +/- 8 / 234 (1.86);
  Qwen3-30B-A3B Q4_K_M 460 +/- 14 / 432 +/- 30 / 312 (1.47); Qwen3-30B-A3B UD-IQ2_XXS 348 +/- 5 /
  347 +/- 10 / 196 (1.77); gemma-4-26B-A4B Q4_K_M 363 +/- 4 / 366 +/- 5 / 264 (1.38);
  gemma-4-26B-A4B UD-IQ3_XXS 353 +/- 3 / 355 +/- 3 / 232 (1.52). The flat rows did not move
  (214, 174, 184, 140, 141). Every carrier reads past the reference on both arms from the one
  lever; the KHR arm reads the cm2 arm within its spread on every row (the 30B Q4_K_M's KHR
  spread of 30 is the widest on the board). At the arc's tip - the rows' own combine form on a
  batched-first run, the router's columns loop - the three cm2 rows read 428 +/- 7, 453 +/- 15 and
  358 +/- 4, each within its row's spread [direction-grade - two commits].
- **The 5060 Ti (driver 616.56, the desktop holding about 0.9 GB of the 16 GB), tg128@4 ours cm2
  / llama.cpp Vulkan / llama.cpp CUDA (the same b10660 checkout built with CUDA 13.4, `external`,
  the same hour), then flat ours / theirs Vulkan / CUDA:** gpt-oss-20b 245 +/- 7 / 135 / 293
  (1.82 against Vulkan, 0.84 against CUDA), flat 130 / 61 / 127; Qwen3-30B-A3B UD-IQ2_XXS 237 +/- 1
  / 91 / 359 (2.61 / 0.66), flat 143 / 59 / 135. gemma-4-26B-A4B UD-IQ3_XXS fits no plan on this
  card (the resident driver declines at 23.9 GB asked of 13.3, its demoted down-expert rows taking
  the served weights alone to 15.4 GB - a plan figure that tallied each routed layer's dense shared
  expert twice, the low-format N-row arc's section below; the per-op rails serve 16 summed) where the reference
  pages: 109 on Vulkan, 275 on CUDA - row 81's shape. Against CUDA the two homed carriers read
  0.66 and 0.84. The expert-bucket form (`followup_vulkan.md` item 83's neighbour: the N-column
  expert GEMV a bucket, which decodes a weight block once for every row that picked its expert)
  is the lever that room names; it would serve a fifth of gpt-oss's slots at four rows and a
  tenth of the 30B's.
- **The rows against the sessions alone:** gpt-oss bit for bit (the regions file's six cells);
  Qwen3-30B Q4_K_M within 0.062 of the peak, past the K-quant dense carrier's 0.06 bar and under
  the wide bar's 0.10 (item 75's rounding through twenty-four K-quant expert planes a token over
  forty-eight layers; the regions cells' own maxdiff lines on the pod, cm2 arm); gemma-4-26B
  Q4_K_M within the wide bar against the split one-row command and off by up to 0.13 of the peak
  on five of thirty-two compares against the fused one (the same cells run before the file pinned
  the fused forms off, the pod, cm2 arm) - the folded down sum's rounding through the 26B's
  router near-ties, item 83. The routed planes at `nb` rows cost the resident image nothing a
  reader sees: gpt-oss-20b's image reads 10914 MB with a 123 MB mirror at 660 x 4 and 220 MB of
  scratch on the pod, the planes' growth inside the scratch's rounding.

### From the Vulkan batched-decode arc, the E-series carriers (2026-09-21)

Instruments as the MoE section above (the pod's cm2 arm and the 5060 Ti, `lcpp_bench.das --npl 4`
three reps, llama.cpp b10660's `llama-batched-bench` the same hour, `external`; every ratio
`tg128@4` against the reference's `S_TG` at `-npl 4` [direction-grade - two processes]; every
lever's pair is two commits in two processes [direction-grade - two commits]); both carriers
Q8_0, `DASLLAMA_PARITY_FULL=1` for the E4B. The one-row rates did not move through the
section (the pod: E2B 198.4 -> 198.7, E4B 112.7 -> 112.5; the 5060 Ti: 122.2 -> 121.5, 66.1 -> 66.2).

- **The rows form alone (commit e43ace31d: the side input a row, the pre-step projection a row a
  column, the branch's split forms, q alone on a shared-KV layer), tg128@4 ours / llama.cpp:** the
  pod E2B 540.7 +/- 1.7 / 185.5 (2.92), E4B 335.8 +/- 0.3 / 360.2 (0.93); the 5060 Ti E2B 371.0 +/- 1.0
  / 113.1 (3.28), E4B 209.9 +/- 0.5 / 213.0 (0.99). The E4B's four-row step under the profiler read
  2.4 ms of device idle a step on the pod and 4.4 on the 5060 Ti against a dense carrier's half a
  millisecond: the batch driver ran the CPU pre-step (`ple_pre_prefill`: the gather and the
  [dim x layers*ple] host GEMM over the rows) on every step, for rows the device then gathered and
  projected again.
- **The pre-step gate (commit 0840fc4a0: the batch step skips the CPU pre-step where the decode
  gate says the armed driver projects, and runs it late on a declined step):** the pod E2B 567.7
  +/- 1.6 (3.06), E4B 357.5 +/- 0.2 (0.99); the 5060 Ti E2B 386.3 +/- 2.0 (3.42), E4B 226.5 +/- 0.7
  (1.06). The E4B's idle a step 2.4 -> 1.7 ms on the pod, 4.4 -> 2.9 on the 5060 Ti.
- **The router's columns and the fused branch's rows form (commit 00ac50e0b: `RouterGemvT` reads
  each row once over every column - the pre-step projection had streamed its 55 MB plane once a
  row - and the branch takes the fused act + requant + proj over the columns where the one-row
  branch fuses, `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`):** the pod E2B 585.0 +/- 0.2 / 185.5
  (3.15), E4B 363.4 +/- 0.8 / 360.2 (1.01); the 5060 Ti E2B 394.9 +/- 0.3 / 113.1 (3.49), E4B 234.7
  +/- 0.6 / 213.0 (1.10). Against llama.cpp CUDA on the 5060 Ti (the same checkout built with CUDA
  13.4): E2B 449.7 (0.88), E4B 247.6 (0.95). The E4B's four-row step, us, pod / 5060 Ti: pleproj
  159 -> 120 / 555 -> 233; the branch (the FFN step's rows requant, the gate, the fused act + proj)
  1465 -> 1122 / 1375 -> 1195; the step whole 10763 -> 10512 / 16647 -> 16138. The reference's
  E2B batched row reads 185 on the pod against its own flat 111.5 (1.66x, where its E4B row scales
  3.3x over 108.9) and 113 on the 5060 Ti against 64.4: the reference's four-stream E2B shape is its
  own question, and the E2B ratios stand as measured.
- **The rows at the admission's ends (the pod, the cm2 arm, three reps):** E4B at two streams 195.8
  +/- 0.2 fused against 185.8 +/- 0.1 split (`DASLLAMA_VK_FUSE=0`), the reference 120.3 (1.63); at eight
  streams 614.2 +/- 0.5 fused against 597.6 +/- 0.3 split, no reference row (its `-c 4096` holds no
  eight streams of 640). gpt-oss at two streams 304.9 +/- 2.8 against 186.3 (1.64), at eight 520.4
  +/- 4.9. The fused branch wins at both ends, so the pick does not branch on the column count.
  The E4B's footprint on the 5060 Ti: the image 7533 MB, the mirror 144 MB at 660 x 4, 167 MB of
  scratch; the E2B's 4675 + 46 + 166.
- **The rows against the sessions alone:** the E2B regions file's nine cells bit for bit on both
  boxes at every commit, the batched cells served (`test_gpu_resident_regions_e2b.das`); the fused
  branch's columns held to the one-row dispatches bit for bit (`test_vkd_q8_gemv_pleact`'s columns
  arms), the router's columns likewise (`test_vkd_router_gemv_cols`), and the gpt-oss regions file
  bit for bit over the router's new form.

### The CUDA levers on the pod (2026-09-21)

Provenance of every "ours" figure in this section: `benchmarks/lcpp_bench.das` under `-jit` on the
pod (RTX PRO 4500 Blackwell, `DASLLAMA_GPU=1`, cm2, the f16 K/V mirror, the box's tuned
`DAS_TUNE_POLICY`, `--for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4`: the flat row and the
four-stream row from one process, so the flat row runs on the four-region plan), the shapes from the
same bench under `DASLLAMA_GPU_PROF=1`; every before -> after pair is two commits [direction-grade -
two commits]. The references: llama.cpp b10660 built with CUDA on the pod
(`/workspace/llama/src-b10660/build-cuda`, nvcc 12.8, `-DCMAKE_CUDA_ARCHITECTURES=120`) and the
prebuilt Vulkan b10660 (`/workspace/llama/b10660/llama-b10660/`, the E-series section's build),
both under `llama-bench -ngl 99 -fa 1 -r 3` and `llama-batched-bench -c 4096 -b 2048 -ub 512 -npp 512
-ntg 128 -npl 1,4 -ngl 99 -fa on`, `external` [direction-grade - two processes]; the CUDA kernels under nsys
2024.6.2 (`-t cuda`, the trace grouped by kernel name and grid over the last 16 steps, a step closed
by the classifier launch) beside our `DASLLAMA_GPU_PROF=1` stamps. nsys's per-kernel records inflate
a step of about 1200 launches by about 1.5 ms (the traced step ran a fifth slower), so its sums
rank the shapes and bound them from above; the reference's tg steps launch as one CUDA graph each.

- **The rows, the pod (RTX PRO 4500), tg128@4 summed / tg128:** CUDA E4B 440.2 / 125.5 against ours
  363.0 / 112.6 (0.82 / 0.90; llama.cpp Vulkan 360.2), CUDA gpt-oss 523.3 / 234.4 against ours 425.6 /
  209.1 (0.81 / 0.89; Vulkan 234). The E4B four-row step: ours 10.5 ms of GPU time and 0.65 ms of
  host (the logits copy 252 us, four argmaxes, the scheduler tick, the fence wake) against CUDA's
  9.09 ms wall - on this card the gap is GPU time, where the 5060 Ti's was the host (the E-series
  section above).
- **The shapes, E4B at four rows, ours stamps / CUDA traced, us a step:** gate + up 3025 / 3218,
  down + wo + proj 2619 / 2263, the classifier 837 / 876, the projections 697 / about 1050, the
  attention 1354 / about 755, the add + rms + requant glue 1394 / about 1850, the per-layer-embedding
  gate 430 / 226. gpt-oss at four rows: the expert GEMVs 5348 (e_gate 1768, e_up 1791, e_down 1789 -
  the floor for reading every slot's expert whole: 16 slots x 4.15 MB x 3 planes x 24 layers at
  896 GB/s) / 3903, the attention 618 / 279, the rest within a tenth. The reference's MoE GEMV reads
  every slot's expert too; its launch puts every token's dot of one row index in one block, so an
  expert two slots share leaves DRAM once.
- **The interleaved region walk (commit 36df5105f: the one-column leaves map a subgroup to
  `rg % nreg`, `rg / nreg`, so a weight row two slots share reaches the second from cache),
  tg128@4 ours before -> after / CUDA:** gpt-oss 425.6 +/- 6.8 -> 450.3 +/- 3.9 / 523.3 (0.81 ->
  0.86), Qwen3-30B-A3B Q4_K_M 453 -> 459.0 +/- 18.9 (within the spread: 128 experts over 32 slots
  share few), Qwen1.5-MoE Q8_0 520.1 +/- 35.3 -> 526.3 +/- 39.8 (the same session, within the
  spread). The one-row rates did not move (gpt-oss 209.13 -> 209.13). Under the profiler the expert GEMVs read 5348 -> 4733 us a step (e_gate 1768 -> 1517, e_up 1791 -> 1618, e_down 1789 -> 1598; the step 9235 -> 8672), 830 us above the reference's traced 3903: the walk shares a row between slots that sit within a cache's reach, the reference's block shares it within one warp set.

- **E2B, the pod, tg128@4 summed / tg128:** CUDA 745.5 / 224.3 against ours 578.3 +/- 1.3 / 198.5 (0.78 /
  0.88; llama.cpp Vulkan 185.5) - the widest of the three. The shapes at four rows, ours stamps / CUDA
  traced, us a step: gate + up 1527 / 1591, down + wo + proj 1616 / 1187, the classifier 504 / 530, the
  projections 395 / 593, the attention 1016 / 478 (a head of 256, eight heads a kv head: two workgroups a
  row, four pieces - the chain itself, 29 us a layer against their 9.5), the glue 1209 / about 1550; the
  host 0.56 ms a step (the logits copy 266 us) against their device-to-host copy of 148.
- **The attention pieces capped to one wave (commit ee15d5217: `rd_layer_pieces` bounds a layer's count
  by the SM count over its (kv head, slab) workgroups times the plan's rows, both commands alike), the
  ruler's readings at 640 positions and four rows (`harness/vk_attn_probe.das`, us a layer):** gpt-oss's
  shape (sixteen workgroups a row) 34.9 at four pieces -> 16.4 at one, Llama-1B's (eight a row) 22.6 ->
  16.5 at two, the 30B's (eight a row, head 128) 27.3 -> 20.6 at two, E4B's (two a row) stays at four
  (22.7; 34.4 unsplit): the pass costs one workgroup's chain a wave and climbs past about thirty-two
  workgroups a layer. The rows, tg128@4 before -> after: gpt-oss 450.3 -> 461.1 +/- 4.0 (CUDA 523.3: 0.86 ->
  0.88), Llama-3.2-1B Q8_0 1369.1 +/- 1.5 -> 1432.8 +/- 2.4 in one session (flat 459.2 -> 465.0: the one-row
  command takes the plan's count too), Qwen3-30B-A3B 459.0 -> 450.2 +/- 20.5 (within the spread), E4B 362.3
  +/- 0.6 (its count unchanged). The regions files hold bit for bit through the change (gpt-oss, the 30B on
  its bar, qwen2, E2B, Qwen1.5-MoE), since both commands read one count.

- **The q8 N-column row-pair stamps armed from a reduction of 4096 (`g_q8_n2_min_n`, the E-series' down GEMVs)
  - DECLINED, measured:** tg128@4 E2B 578.3 -> 525.1 +/- 1.3, E4B 362.3 -> 343.2 +/- 0.1, Llama-3.2-1B 1419.1 ->
  1312.1 +/- 1.2 (flat rates unmoved); E2B's down stamp 941 -> 1608 us a step. The pair twin halves the activation
  re-reads and still loses on this card, so the lever stays at its off default; the E-series' down group (1616 us
  a step against the reference's 1187) waits on another form.
- **The device argmax pick (`ClsArgmaxPart` + `ClsArgmaxFin` after the epilogue, the picks-only transfer twin;
  `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue`), tg128@4 before -> after / CUDA:** E2B 578.3 -> 630.0 +/- 0.7 (745.5:
  0.78 -> 0.85), E4B 362.3 -> 382.0 +/- 0.1 (440.2: 0.87), gpt-oss 461.1 -> 476.8 +/- 4.6 (523.3: 0.91), Llama-3.2-1B
  1432.8 -> 1577.4 +/- 3.9, Qwen3-30B-A3B 450.2 -> 467.7 +/- 21.9 (within the spread); flat E2B 198.5 -> 198.8, E4B 112.5 ->
  112.4, Llama-1B 465.0 -> 464.1, gpt-oss 209.2 -> 207.8 (the two passes over a 201k vocab, on a step the flat row still
  lands logits for). The E2B four-row step under the profiler: the host's logits copy 266 -> 20 us, the idle between
  commands 1407 -> 862, the passes' `t:pick` stamp 10 us a step - and 12 on Qwen2.5-0.5B (152k vocab), 9 on Llama-1B
  (128k): flat across the vocab widths the tier serves, so the split has no width gate. The first form, one
  workgroup a row over the whole row, cost 140 us on a 262k vocab and the flat rates 2-6% (E2B 198.5 -> 193.1, gpt-oss
  209.2 -> 199.7), which is why the pass is two. The pick planes take 4 x (2 x 64 + 1) bytes a row of the plan (1 KB at
  four rows), a footprint the plan's report does not itemize. The scheduler's device-mode cell counts the picks
  landed against the rows stepped (`test_gpu_resident_regions*.das`), the kernel cell holds the passes to
  `parallel_argmax` (`test_vkd_cls_argmax`), and the regions files hold bit for bit through the change (qwen2 26/26,
  E2B 13/13, gpt-oss 8/8, the hybrid 6/6, qwen2 under `DASLLAMA_VK_KV32=1`).

### From the Vulkan hybrid rows arc (2026-09-22)

Instruments as the E-series section above: the pod (RTX PRO 4500, Linux), `daslang -jit
benchmarks/lcpp_bench.das --for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4` on the cm2 arm
and on the KHR arm under `DASLLAMA_COOPMAT=mm`, `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1 DAS_JOBQUE_THREADS=16` and no `DAS_TUNE_POLICY`
override (the untuned tier) for every carrier; llama.cpp b10660's prebuilt Vulkan
`llama-batched-bench -c 4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on` the same
hour, `external`; every ratio `tg128@4` against the reference's `S_TG` at `-npl 4`
[direction-grade - two processes]; a lever's pair is two commits in two processes
[direction-grade - two commits]. The test counts below are the same box and tier under
`dastest -jit`, cm2, `DAS_JOBQUE_THREADS=8`. No 5060 Ti row is in this section.

- **The residency (commit d14bc2e32: a deltanet state slot per mirror region, the row's slot and
  parity in its `TokMeta` row, the prefill in the selected region's slot):** the one-row rates
  unmoved - Qwen3.5-0.8B Q8_0 flat 474.4 +/- 0.7 against 470.3 at the arc's tip - and the batched
  rows still a row at a time (tg128@4 475.5, one stream's rate: the N-row command still declined
  the recurrent layers); the resident, regions and scheduler hybrid files green at the commit.
- **The N-row recurrent form and the gated q (commits dcc6852a6, ca0ac2a0e, da7d315f2), tg128@4
  ours cm2 / ours KHR / llama.cpp:** Qwen3.5-0.8B Q8_0 1553.1 +/- 1.3 / 1561.0 +/- 3.8 / 971.1
  (1.60 / 1.61), flat 470.3 / 468.6 / 381.5; Qwen3.5-9B-MTP UD-Q5_K_XL 316.1 +/- 0.4 / 315.8 +/- 0.4 /
  277.4 (1.14 / 1.14), flat 104.8 / 104.8 / 102.3 (the rows form that fed its K-quant recurrent
  heads Q8_0 blocks read 314.8 on both arms: the Q8_K feed the shipped form requants at every
  recurrent head costs nothing the ruler sees); Qwen3.6-35B-A3B UD-IQ2_XXS 440.6 +/- 2.6 / 443.4
  +/- 2.0 / 235.9 (1.87 / 1.88), flat 163.7 / 163.7 / 131.5 - against the MoE section's 30 (0.13)
  a row at a time, where the one slot went home and back on every stream switch. pp512 cm2 / KHR:
  the 0.8B 29102 / 23862, the 9B 5326 / 4223, the 35B 4583 / 4138 (the reference's four-stream
  prompt rate 29067 / 5008 / 4588). The 9B's four-row ratio is the section's tightest and its flat
  ratio 1.02: the dense K-quant 9B trunk, not the recurrent form, sets it. Its regions file
  (`test_gpu_resident_regions_hybrid_k.das`, the split bar, 4 of 4) is what found the rows form's
  Q8_0 feed and, after it, an N rail that never ensured a recurrent layer's FFN leaves.
- **The rows against the sessions alone:** the hybrid regions file's batched cells bit for bit
  through the N-row command (11 of 11), the scheduler's deltanet cells, the resident hybrid file,
  and the fused step's two-row arm against the one-row dispatch a row at a time bit for bit on both
  o forms (`test_vkd_dn_step_rows`). The regions file found two defects on the way: the rows-form
  prologue stamp stores no float row (a recurrent layer 0 reading one now takes the split
  prologue), and four rope and attention sets bound the grown `TokMeta` block at its old row width,
  every row past the first reading its position outside the binding.
- **The per-region planes' footprint (computed from the geometry, no device measurement):** a
  recurrent layer's state slots are `regions x nvh x ds x ds x 4` bytes and its ring pairs
  `regions x 2 x cd x (dconv - 1) x 4` - at four regions on the Qwen3.5-0.8B (16 heads of 128,
  cd 6144, dconv 4) 4 MB + 0.6 MB a layer over 18 recurrent layers, 83 MB; on the Qwen3.6-35B-A3B
  (32 heads of 128, cd 12288) 8 MB + 1.2 MB a layer over 30 layers, 276 MB; at `RD_NB_MAX` (eight)
  regions twice that. The rows' deltanet planes grow by the row count: the projection row
  `(cd + di) x 4`, the o row `di` bytes plus `di / 32 x 4` of scales plus `di x 4` of f32, so
  eight rows on the 35B hold 0.9 MB. Beside the K/V mirror the same regions hold (the residency
  section's sizing), the slots are within a percent of the plan.
- **The carriers that served batched without evidence (the phi3, gemma3 and mistral3 families), the
  same instruments, tg128@4 ours cm2 / ours KHR / llama.cpp:** Phi-3.5-mini Q4_K_M 578.7 +/- 0.4 /
  574.8 +/- 0.4 / 552.1 (1.05 / 1.04), flat 220.4 / 221.0 / 217.4; gemma-3-1b Q8_0 1240.1 +/- 0.3 /
  1240.3 +/- 1.2 / 845.5 (1.47 / 1.47), flat 398.3 / 395.2 / 310.5; gemma-3-4b Q8_0 542.8 +/- 0.6 /
  543.4 +/- 0.6 / 442.6 (1.23 / 1.23), flat 152.2 / 152.6 / 137.2. The regions files pin the rows:
  `test_gpu_resident_regions_gemma3.das` bit for bit (6 of 6), `test_gpu_resident_regions_phi3.das`
  on the split bar (4 of 4). Mistral-Small-3.1-24B Q4_K_M has no row and no regions file: the pod's
  volume holds no room for its 14 GB beside the stocked models (`followup_vulkan.md` item 87).

### From the M4 Metal pass (2026-09-13)

Instruments: `benchmarks/matmul/bench_metal_gemv_kernels.das` at the Qwen2.5-0.5B decode shapes
(the `05b` set, plus an n-ladder at d=4096 and n=2048 at classifier width), best of 3 rounds,
production `v0_gemv` against the lab's row-per-simdgroup forms, every arm interleaved in one
process; `benchmarks/decode_step_trace.das` on the 0.5B Q8 file, whose step wall is read outside
the benchmark process: out-of-process; the records rig's Metal pair, `benchmarks/lcpp_bench.das`
under `-jit --for-debug-purposes --ngl 99 -p 512 -n 128`: debug-jit. The M4 Pro and the M5 Max,
each on its own mint. The rows are in `followup_metal.md` sec.7.1. Every rig pair below is two
builds or two knob settings in two processes - direction-grade - and enters as its ratio, the
arms' rates left in the run's report. Qwen3-30B-A3B-Instruct-2507-Q4_K_M and
Qwen3.8-27B-Q4_K_M carry committed board rows on both boxes (`performance/records/m4.json`,
`performance/records/m5.json`); the debug-jit readings here are read beside those rows.

- **The q8 GEMV at n=896, M4 Pro (ms, weight GB/s): split form vs row-per-simdgroup:** qkv
  (d=1152) 0.0081 / 135 vs 0.0051 / 228; wo (896) 0.0068 / 126 vs 0.0044 / 208; w13 (4864)
  0.032 / 145 vs 0.019 / 262; w2 (n=4864, d=896) 0.0129 / 360 vs 0.0186 / 263; cls (151936)
  0.919 / 157 vs 0.574 / 267. The first lab pass read the split form 2-3x slower still (0.029
  ms on qkv, 0.98 on cls): the M4's clocks idle between sparse dispatches, the interleaved
  all-variant run is the honest one.
- **The same on the M5 Max:** qkv 0.0055 vs 0.0047; wo 0.0050 vs 0.0044; w13 0.0110 vs
  0.0075; w2 0.0078 vs 0.0072; cls 0.240 vs 0.255 - the row form wins or ties every short shape.
- **The crossover, both boxes:** n=1536 (d=4096) row form 1.5x on both; n=2048 at d=4096 M4
  +28% row form, M5 -9%; n=2048 at d=128256 split ahead on both (M5 0.464 vs 0.491, M4 1.169
  vs 1.217); n=3072 split ahead on both. `GEMV_SG_MAX_N = 1536`.
- **The 0.5B Q8 decode step, M4 Pro:** 4.2 ms per token (gpu 4.07, gap 0.02, sched 0.03;
  197 serial nodes) before, 3.2 after; the fusion, spec-chain, concurrent-encoder and fused-
  attention rails each moved the step under 2%. Rig tg128, the row form against the split: the
  0.5B 1.27x (0.73 -> 0.94 of llama.cpp), the 0.6B 1.23x (0.80 -> 0.97), the 0.8B 1.25x (0.97
  -> 1.21). M5: 0.99x, 1.00x, 0.99x and 1.00x on the 0.5B, 0.6B, 1.5B and Llama-1B Q8 - flat.
- **dev-W on the raced M4 sidecar (no tensor crown):** Llama-1B IQ3_XXS pp 0.93 -> 1.00,
  IQ2_XXS 0.93 -> 1.00, Q4_K_M 0.97 -> 1.02 - the same three rows the hand-crowned sidecar
  gave, so the panel is the whole gain and the tensor stamp none of it. Qwen3-30B IQ2_XXS
  serves at 0.95 / 1.09 where it declined to the CPU before.
- **The bake's footprint pair (M5 Max, `utils/dasllama-convert -f metal --stream`, the
  541 MB Llama-1B IQ3_XXS):** the image at IMAGE_VERSION 36 is 659 MB with a 32 MB dev-W plane
  over 16 sites (the q8/k4/k5/k6 mirrors), at 37 it is 2571 MB with an 1856 MB plane over 112
  sites; the mint's convert time 222 -> 1016 ms. The plane is 3.4x the model on a small
  split-scale dense file - the per-panel cap keeps a 30B's expert planes out, so the cost
  lands on the small files - and it buys the resident panel's prefill (1.05 scratch -> 1.23
  resident on the M4 rows above). Decision: the bake stays on; the cap and a per-file
  footprint knob are the ledger's if the disk cost bites.
- **The step wait, Llama-1B Q8, M4 Pro (`decode_step_trace -o`, 96 steps):** GPU 5.73 ms per
  token, wall 6.38-6.71, gap 0.61-0.98 ms per token averaged over a three-step cycle (two
  steps at gap 0, then 3.3-3.6 ms; commits at +0.1, +6.0, +13.9 ms; waits 13.7, 0, 5.9 ms).
  The same file on the M5 Max: wall 2.81 = GPU 2.80, one commit per step, gap 0.00. The 0.5B
  Q8 on the M4: gap 0.02 on three processes, 1.46 (6 ms holes) on a fourth. The attention rail,
  `taskpolicy -l 0 -t 0` and `DAS_JOBQUE_AFFINITY=2` each left the gap at 0.75-0.87. A spin on
  `GPUEndTime` before the blocking wait: 5.66 ms per token, gap 0.007, 176.5 t/s, three runs
  within 0.02 ms (llama-bench 167). Rig re-times, the spin against the blocking wait: Llama-1B
  Q8 tg 1.06x (1.00 of llama.cpp), Q4_K_M 1.08x (1.00), Qwen2.5-0.5B Q8 1.10x (1.03), 1.5B Q8
  1.07x (1.03); prefill unchanged. The adaptive form arms on the first hole (3.9 ms, both runs)
  and the later buckets read gap 0.001. The rig's own process pays the wake on every unchained
  step (IQ2_XXS 1.09x spinning over blocking) while the trace tool's does not, so the spin
  ships on by default.
- **The tall stamp on the M4 Pro (rig cells, the sidecar's `metal_tall_floor` at 100000 =
  never tall, against 64):** Llama-1B Q4_0 pp 1.06x (0.97 -> 1.03 of llama.cpp), Llama-1B Q8
  1.08x (0.98 -> 1.05), Qwen3-30B-A3B Q4_K_M 1.01x (1.02 -> 1.04; its board row is
  `performance/records/m4.json`'s), Qwen3.5-9B Q8 1.00x (even); decode unchanged. The 1-D grid
  twin, the double buffer off, the floor at 16, a re-mint and a re-baked image each left Q4_0
  within 0.2% of the never-tall arm. The M5's tall win stands (its Llama-1B Q8 prefill reads
  1.00x either way under the hand crown).

### From the CPU dedup arc (2026-09-20)

Every row here is one process, both arms interleaved, best of six after a warm-up; the M5 Max
and the zen4 (Ryzen 7 PRO 8700GE) both, `-jit`, `DAS_TUNE_MANIFEST` unset on the zen4 and the
box's rig sidecar on the M5, no `DAS_TUNE_POLICY` override. The two kernel races ran an
out-of-tree race script over the engine's own calls (`gemm_f32` / `gemm_f32_jo` fed the same
operands; `eval_` over the loaded model for the prefill row), so their readings are ratios of
the two arms, not board figures; the tokenizer rows are `lcpp_bench --tok`.

- **`gemm_f32` (row blocks outer) against `gemm_f32_jo` (column blocks outer), C += A*B at
  the deltanet chunk and ASR attention shapes, jo / ij:** M5 64x128x64 0.99, 64x64x128 1.00,
  128x128x128 0.96, 128x64x1000 0.96, 128x128x3000 0.94, 256x64x4000 0.99, 1024x64x1024
  **1.17**; zen4 the same seven 1.01, 1.01, 1.00, 1.01, 0.97, **0.92**, 1.00. Both forms stay:
  jo wins where B is wide and streamed once (the ASR scores, its callers today), ij wins the
  M5's tall square; neither is a default for the other's shapes.
- **Classic against blocked prefill attention (the two bit-identical CPU forms; flash is the
  default), the whole CPU prefill of gemma-4-E2B-it-Q8_0 at 2048 tokens, `DASLLAMA_GPU=0`,
  blocked / classic:** M5 **1.052**, zen4 0.996. The blocked
  form's K/V reuse across an 8-query block buys nothing the score-row reuse of the classic form
  does not, so the blocked arm goes and the tests' bit-exact mode is classic.
- **The BPE encode walk (`lcpp_bench --tok`, every corpus, 1 KB to 1 MB), MB/s master -> arc,
  best of two interleaved rounds:** the walk appends each codepoint's bytes into one reused
  buffer instead of building a string per codepoint. M5, Qwen3-0.6B: prose 12.1 -> 16.6, code
  11.5 -> 15.6, cjk 13.5 -> 16.9 (1.15 to 1.38 at every size); zen4, Qwen3-30B-A3B (the same
  qwen2 pretokenizer): prose 7.9 -> 12.6, code 7.8 -> 12.0, cjk 10.9 -> 13.9, digits 5.5 -> 7.3,
  longword 7.0 -> 9.7 (1.13 to 1.58). cv under 2% on every row but the 4 KB ones (8%).
- **One merge heap for both tokenizer backends - DECLINED, measured:** SPM's own heap
  (`spm_merge_heap`) and BPE's are the same forty lines, and the fold read gemma-3-1b SPM
  encode on the M5 24.0 -> 22.8 at 1 KB (prose, cv under 1%), 16.1 -> 13.7 at 4 KB code, parity
  from 64 KB up; on the zen4 gemma-4-E2B read 0.81 to 0.98 of master on every corpus and size
  (prose 7.6 -> 6.6 at 4 KB, cjk 28.8 -> 23.3 at 1 MB). A float priority in place of a double
  took back one to two points; stamping the merge as a generic per backend with the push/pop
  helpers left in `dasllama_bpe.das` took the M5's 1 KB row to 0.98 and left 4 to 16 KB at 0.93
  to 0.96. The SPM heap stays its own body, in the partition of the encode that runs it
  (`ARCHITECTURE_ENGINE_FORMATS.md#bpe-merge-heap`); the BPE side keeps the unshared form.

### From the Vulkan low-format N-row arc (2026-09-23)

Instruments as the hybrid rows section above: the pod (RTX PRO 4500, Linux), `daslang -jit
benchmarks/lcpp_bench.das --for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4` on the cm2 arm
under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1
DAS_JOBQUE_THREADS=16`, the untuned tier, every figure below the mean of the three repetitions with
its standard deviation; llama.cpp b10660's prebuilt Vulkan `llama-batched-bench -c 4096 -b 2048
-ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on` the same hour, `external`, its second run of a
file; every ratio `tg128@4` against the reference's `S_TG` at `-npl 4` [direction-grade - two
processes]; a lever's pair is two commits in two processes [direction-grade - two commits]. The
5060 Ti rows (driver 616.56, Windows, the desktop holding 1.8 GB of the 16 GB) take the same command
lines, the bench pinning its batched row's context so the plan homes four regions at 660 positions;
the local llama.cpp is the same b10660 Vulkan build. The N-column ruler is
`daslang -jit harness/vk_gemv_probe.das -- cols 5120 17408` (the 27B's FFN gate shape) on the pod
under the same environment, the untuned tier in cm2 mode, its last column an N-column dispatch's
wall over N one-column dispatches, each wall the mean of its dispatch ring [direction-grade - two
commits].

The reference's first run of a file compiles its pipelines inside the measurement: i1-IQ3_S read
57.9 cold and 67.3 warm at four streams. The carriers are the three Qwen3.8-27B files whose dense
planes sit in sub-4-bit formats: i1-IQ3_S's FFN and attention q/k/o planes are IQ3_S, its recurrent
qkv and attention v planes Q4_K and its classifier Q6_K (440 IQ3_S tensors, 65 Q4_K, 1 Q6_K);
UD-Q3_K_XL mixes IQ4_XS (156 tensors, its most common), Q8_0 (98), Q4_K through Q6_K (69) and the
eight low-bit formats Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS and IQ4_NL across the
planes; UD-IQ4_XS is IQ4_XS in bulk with the same eight mixed in. All three have 64 layers with
attention every fourth, so a position costs 64 KB of f16 mirror and a region 151 MB of DeltaNet
state.

- **The eight N-column leaves alone (commit 181398621: `KqGemv<Fmt>N` for k3, k2, iq3s, iq3xxs,
  iq4nl, iq2s, iq2xs, iq2xxs on the shell's per-column `blk_contrib`), tg128@4 ours cm2 /
  llama.cpp, then flat:** i1-IQ3_S 95.4 +/- 0.2 / 67.3 (1.42), flat 48.4 / 42.4; UD-Q3_K_XL 62.4 +/-
  0.1 / 78.0 (**0.80**), flat 43.8 / 42.0; UD-IQ4_XS 55.0 +/- 0.1 / 84.9 (**0.65**), flat 41.1 / 40.5.
  The rows form scaled 1.97x over flat on the IQ3_S file and 1.34x to 1.42x on the mixed files.
- **The ruler at that tip, an N-column dispatch at four columns over four one-column ones:** q8
  0.25, k4 0.28, k6 0.27, q40 0.26, k5 0.35, k3 0.48, k2 0.50, iq3s 0.61, iq3xxs 0.61, iq2s 0.71,
  iq2xs 0.76, iq2xxs 0.78, iq4xs 1.06, iq4nl 1.06 - the shell re-ran each block's decode for every
  column, so a codebook or grid leaf shared nothing and the one-column class at 1 column read 1.23x
  the one form.
- **Lever 1 - decode once, fold per column (commit f18cb9ffa: every leaf's `blk_decode` hands back
  the block's packed int8 halves and four fold terms, one shared `blk_fold` dots them, `mad`
  throughout):** the ruler at four columns q8 0.25, k4 0.26, k6 0.26, q40 0.26, k5 0.26, k3 0.31, k2
  0.37, iq3s 0.35, iq3xxs 0.32, iq2s 0.44, iq2xs 0.41, iq2xxs 0.42, iq4xs 0.36, iq4nl 0.36; at eight
  columns k4 0.18, iq4xs 0.22, iq3s 0.27, iq2s 0.33; the one-column rates within 1% of before
  (iq4xs 196 us against 202). The rows: i1-IQ3_S 137.1 +/- 0.3 / 67.3 (**2.04**), flat 48.8;
  UD-Q3_K_XL 123.8 +/- 0.2 / 78.0 (**1.59**), flat 44.6; UD-IQ4_XS 118.2 +/- 0.2 / 84.9 (**1.39**),
  flat 41.7 (flat tg128 48.80 +/- 0.02, 44.58 +/- 0.07, 41.66 +/- 0.07); pp512 1475 +/- 5 / 1559
  +/- 4 / 1634 +/- 7. The KHR arm the same hour: tg128@4 137.3 +/- 0.1 / 124.0 +/- 0.2 / 118.0 +/-
  0.2, pp512 1394 +/- 5 / 1377 +/- 5 / 1394 +/- 4. The kernel family, N-column and gate-up cells
  hold at the tip on both boxes: the N form's every column is the one-column class's word for word.
- **llama.cpp's CUDA build of the same b10660 checkout on the pod (`external`, a reference row and
  not a target; `-DGGML_CUDA=ON`, nvcc 12.8, the Vulkan reference's command line, second run):**
  tg128@4 125.9 / 132.6 / 125.9 (i1-IQ3_S / UD-Q3_K_XL / UD-IQ4_XS), tg128 51.0 / 50.9 / 48.8,
  pp512 1811 / 1863 / 1906 - ours at the lever-1 tip reads 1.09x / 0.93x / 0.94x of it batched and
  0.96x / 0.88x / 0.85x flat [direction-grade - two processes]. CUDA's batched rate hardly moves
  with the format (126 to 133) where ours follows the decode cost of the leaves.
- **The 5060 Ti at the lever-1 tip, ours cm2 tg128@4 / tg128 with the reference's rows beside:**
  i1-IQ3_S homes (11526 MB image, 165 MB mirror, 230 MB scratch): 89.5 +/- 0.9 / 28.3 (56.5 before
  the lever) against llama.cpp's 2.5 / 2.6 - the reference pages the 12.6 GB file on this card at
  those arguments (its first and second runs alike); UD-Q3_K_XL homes (11900 MB image): 80.9 +/- 2.4
  / 25.7 (38.9 before) against 19.6 / 7.4, paging; UD-IQ4_XS declines the resident driver - 13295 MB
  asked of 12422 MB usable, the weights alone 12899 MB with other processes holding 1835 MB - and the
  per-op rails read 8.5 / 3.4 against the reference's 2.5 / 2.0, both paging. The IQ4_XS file on 16 GB
  is `followup_vulkan.md` item 81's streamed-weights arm, not a kernel.

### From the Vulkan vision tower arc (2026-09-24)

The instrument: the pod (RTX PRO 4500 Blackwell, driver 580.173, an AMD EPYC 7443 container at 48
threads under the 62 GB cgroup cap), `daslang -jit benchmarks/lcpp_bench.das -- -m <decoder> --image
<coco 640x480> --image-mmproj <mmproj> --for-debug-purposes -r 3 -t 16` under `DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`, `DASLLAMA_COOPMAT` unset (cm2, the pod's default
mode) and `DAS_TUNE_POLICY` unset (the box default; the untuned tier is what
`DASLLAMA_ALLOW_UNTUNED=1` arms, so the kernels are the fallback bodies); the `vk` arm
`DASLLAMA_GPU=1` (the tower chain on the Vulkan tower driver, the bench's engage line reading
encodes +3 of 3), the `cpu` arm `DASLLAMA_GPU=0` (the driver declining `device` on every encode,
the CPU tower chain - the q8 lane where the family has one, the exact f32 lane on qwen25v - and the
decoder on the CPU too, so only the encode column is a tower-vs-tower reading) [direction-grade -
two processes]. The encode is one image's tower time in ms, the best of the three timed reps with
the three listed beside it (the cell prints every rep), and a pp or tg figure is the decoder's
best rep in tok/s.

- **The four families, the driver against the CPU chain, encode ms (best of three, the three
  beside):** gemma-4-E2B (gemma4v q8, 130 soft tokens) **35.2** (40.5 / 35.2 / 35.8) against 701.7
  (715.1 / 701.7 / 708.4); gemma-3-4b (gemma3v q8, 256 tokens, the fixed 896 canvas - 4096 patch
  rows through 27 blocks) **144.9** (168.1 / 144.9 / 146.1) against 14471 (14506 / 14471 / 14527);
  Qwen3-VL-4B (qwen3v q8 with the three deepstack taps, 300 tokens) **247.9** (255.3 / 247.9 /
  251.9) against 1680 (1719 / 1691 / 1680); Qwen2.5-Omni-3B (qwen25v over the halfword twin, 391
  tokens, the f32 per-window route) **174.3** (219.6 / 180.3 / 174.3) against 7161 (7161 / 7198 /
  7163) - the slotted f16 route the arc replaced read 162 (the window layers read K and V off the
  compact rows, `followup_vulkan.md` row 95's staging lever). The first rep of a `vk` row carries
  the tower's upload and pipeline builds. The captions read on both arms of every pair are the
  same text and name the two cats and the remotes - an observation of the served output, not a
  parity claim; the parity instruments are the twin cells below and the chat pairs of
  `tests/test_vision_chat.das`.
- **The image turn's decode with the resident driver's span pass hydrating the mirror's rows
  before the CPU takes the image eval, tg tok/s on the `vk` arm against the `cpu` arm:** E2B
  **189.0** against 24.4 (24.7 on the `vk` arm at the tip before the pass hydrated: the decode fell
  to the CPU after the span eval), gemma-3-4b **106.2** against 15.2 (15.2 before), Qwen3-VL-4B
  **70.3** against 14.0 (56.0 before), Omni **99.0** against 20.0 [direction-grade - two commits].
  The caption the chat cell `test_vision_chat_e2b` reads under `DASLLAMA_GPU=1` is that cell's
  own assert.
- **The pp / tg columns are the decoder's**, not the tower's: on the `cpu` arm the decoder runs on
  the CPU too, and pp reads within 10% on both arms (the image turn's prefill is the CPU's on
  either: the Vulkan resident prefill declines the span eval).
- **The driver's allocations at the largest shape the path serves - the 4096-row encode cap
  (`VT_MAX_ENCODE_ROWS`, the `shape` decline past it), which is gemma3v's fixed canvas exactly:**
  the qwen25v halfword twin uploaded whole, 32 layers x (4 x 1280^2 + 3 x 1280 x 3456) weights x 2
  bytes = 1,268,776,960 bytes, about 1.27 GB (`q25v_gemm_sizes`: the four d x d GEMMs, the gate and
  up at d x ff_pad, the down at ff_pad x d); the gemma3v Q8_0 plane uploaded whole, 27 layers x (4 x
  1152^2 + 2 x 1152 x 4352) = 414 M weights, about 414 MB of quants plus 26 MB of f16 scales (2 bytes
  a 32-weight block); the per-encode device scratch at 4096 rows, from `vt_scratch` /
  `vt_scratch_f16`'s buffer list - eleven f32 row buffers at rows x d x 4 (eight on qwen25v), two
  f32 hidden buffers at rows x ff x 4, the two f16 K/V shadows at rows x d x 2, the Q8_0 feed at
  rows x max(d, ff) x 33/32 (the f16 feed at rows x max(d, ff) x 2 on qwen25v), the padded panels at
  rows x n_head x 128 x 12 where the head is not 64 wide, the rope tables at rows x hd x 4 - reads
  about 491 MB for gemma3v (d 1152, ff 4352, 16 padded heads), about 432 MB for qwen25v (d 1280, ff
  3456, 16 padded heads) and about 507 MB for the Qwen3-VL-4B qwen3v tower (d 1024, ff 4096, the
  fused [q | k | v] rows at 3 x rows x d x 4 and the three tap stashes at 3 x rows x d x 4 beside the
  padded panels); and the qwen3v host-side tap stash `ds_xtaps` at n_ds x rows x d x 4 bytes, 3 x
  4096 x 1024 x 4 = 50,331,648 bytes, about 50 MB on the 4B. The gemma4v scratch follows the same
  list at its d and ff with no padded panels (its head is 64 wide). One resident a family, released
  by the model drop's sweep.
- **The device chain's distance from the exact CPU chain, the twin cells at the tip, x token rms
  (the CPU q8 chain's own distance beside where the family has one):** gemma4v E2B 16 blocks
  0.082 / 0.160 / 0.63 / 0.26 against 0.064 / 0.193 / 0.46 / 0.25 (cb96, cb336, green 672x336,
  cb768x384; the 1- and 2-block towers within 0.015 on both); gemma3v 27 blocks 3.26 against 2.68 (the fixed canvas; the 1- and 2-block towers 0.12
  against 0.12 / 0.13); qwen3v 4B 24 blocks 0.42 / 0.62 / 0.61 against 0.36 / 0.58 / 0.44 (cb96,
  cb448, cb640x320); qwen25v 32 blocks 0.015 / 0.076 / 0.092 against the exact chain (cb112, cb448,
  cb616x336; no q8 lane), the 5060 Ti's KHR arm 0.020 / 0.034 / 0.020. The slotted f16 window
  route read 0.16 to 0.79 on the 5060 Ti's cm2 arm and 1.53 on the pod's at 32 blocks - the
  coopmat tile's f16 staging compounds over the 28 window layers, which is why the window layers
  attend in f32 on the compact rows (`ARCHITECTURE_GPU_TOWER_VULKAN.md` 2.2aq).

  Provenance of the twin-cell figures: `test_gemma4v_vulkan_twin`, `test_gemma3v_vulkan_twin`,
  `test_qwen3v_vulkan_twin` and `test_qwen25v_vulkan_twin` (`tests/test_<family>.das`), each run
  through dastest under `-jit` with `--test-names` naming the cell, `DASLLAMA_GPU=1`,
  `DASLLAMA_IMAGE=0`, `DAS_TUNE_POLICY` unset; the box is the pod (RTX PRO 4500 Blackwell, driver
  580.173) on its cm2 arm (`DASLLAMA_COOPMAT` unset) unless the sentence names the 5060 Ti (Boris's
  box, RTX 5060 Ti 16 GB, driver 616.56), whose cm2 arm is likewise the default and whose KHR arm
  is `DASLLAMA_COOPMAT=mm`. A distance is the cell's own logged x-rms reading over one run of the
  cell; the cells log the reading on green and assert the bar.

### From the Vulkan audio tower arc (2026-09-24)

The instrument: the pod (RTX PRO 4500 Blackwell, driver 580.173, an AMD EPYC 7443 container at 48
threads under the 62 GB cgroup cap), `daslang -jit benchmarks/lcpp_bench.das -- --asr -m <catalog
substring> -r 2 --for-debug-purposes` (the rows stamp 16 threads) under `DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=8 DASLLAMA_MODELS_DIR=/workspace/models
WHISPER_CPP_MODELS=/workspace/models`, `DASLLAMA_COOPMAT` unset (cm2) and `DAS_TUNE_POLICY` unset
(the untuned tier's fallback bodies); the `vk` arm `DASLLAMA_GPU=1` (the block loops on the Vulkan
tower driver, the cell's engage line reading encodes +N over 3 transcriptions, declines +0, and
the row stamped `vulkan` - the bench stamps `vulkan` only when the drivers served every clip,
`ARCHITECTURE_MEASUREMENT.md#asr-gpu-pairs`), the `cpu` arm `DASLLAMA_GPU=0` (the driver declining `device` on every
encode, the CPU q8 chain, the decoder on the CPU too - so only the encode column is a tower-vs-tower
reading; on the `vk` arm the decoder of the LLM-backed families rides the resident driver, which is
most of the wall's drop on E2B and canary) [direction-grade - two processes]. A wall is the clip's
transcription in ms, the best of two timed reps (the bench prints only the best, so the rows
carry no spread); the encode is the clip's encoder time in ms from
the cell's encode-split transcription (one per clip, `encode_ms` of the row). The five clips are
the ASR corpus's jfk (11 s), jfk3 (33 s), gb1 (199 s), hp0 (273 s) and hp0x2 (547 s); canary's
catalog row stops at gb1.

The reference figures below are all `external`, each run on the pod: whisper.cpp at the pod's
checkout built with Vulkan, `whisper-cli -m ggml-<model>-q8_0.bin -f <clip> -t 16 -bs 1 -bo 1
-nf -nt` (`-ng` for its CPU arm); llama-mtmd-cli b10660 (Vulkan), `llama-mtmd-cli -m <model
Q8_0> --mmproj <mmproj bf16> --audio <clip> -p "Transcribe the audio." --temp 0 --jinja -ngl 99
-t 16` (E2B with `-n 256`), two reps a clip, its encode the sum of the clip's `encoding done in
N ms` lines; NeMo's `generate()` through `benchmarks/asr/canary_qwen_bench.py --device cuda` on
the CUDA box; ggml's per-op profile is the whisper-cli command above over turbo jfk under
`GGML_VK_PERF_LOGGER=1`, its figures named by ggml op (`MUL_MAT`, `FLASH_ATTN_EXT`). The das
per-dispatch and per-role readings (the chain's ms a chunk, fc1 / fc2 / q / k / v / o, a
dispatch's us) are the same bench command under `DASLLAMA_GPU_PROF=1`; a forced split names its
`DASLLAMA_CM2_SPLITK` value. The RTX 5060 Ti readings run on Boris's box (driver 616.56) as
`bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- --asr -m <catalog
substring> -r 2 --for-debug-purposes` from the repo root under `DASLLAMA_GPU_PROF=1`, the pod's
other overrides alike.

- **Whisper large-v3-turbo (q8 encoder, 32 blocks at d 1280 / ff 5120 / 64-wide heads, 1500 rows
  a 30 s chunk), the `vk` arm against the `cpu` arm, wall ms / encode ms:** jfk **419 / 109**
  against 3395 / 2998; jfk3 **1306 / 219** against 7278 / 6120; gb1 **8000 / 1070** against 31608 /
  25159; hp0 **9538 / 1527** against 45858 / 37482; hp0x2 **18616 / 2910** against 86941 / 70385 -
  the encode 23x to 27x faster on the device, and the wall's remainder is the decoder on the CPU
  (`followup_vulkan.md` row 101), 15.7 s of hp0x2's 18.6 s.
- **Whisper tiny (q8, 4 blocks at d 384), wall / encode:** jfk **70 / 11** against 181 / 122; jfk3
  **228 / 22** against 438 / 261; gb1 **1367 / 251** against 2015 / 834; hp0 **1707 / 176** against
  2484 / 1056; hp0x2 **3341 / 453** against 4907 / 2151.
- **The whisper reference exe on the pod's own Vulkan build (`external`, the whisper-cli command
  above on the q8_0 siblings, `-ng` for the CPU arm; its `encode time` is the sum over the clip's
  30 s chunks, its `total time` the transcription), against the das rows above:** turbo per-chunk
  encode 35 to 40 ms on the device (hp0x2 660 ms over 19 chunks) against das's 153 ms a chunk
  (2910 / 19) - a 4x encode gap; the transcription hp0x2 3.70 s against das's 18.6 s, the decoder
  on the device there and on the CPU here; on the CPU arm the reference reads turbo hp0x2 encode
  93.7 s / total 107.9 s against das's 70.4 s / 86.9 s. Tiny per-chunk encode 3.1 to 9.3 ms on
  the device against das's 24 ms (453 / 19), total hp0x2 1.98 s against 3.34 s; the CPU arm 2.54 s
  / 5.74 s against das's 2.15 s / 4.91 s. The reference's first clip on the device carries its
  pipeline builds (tiny jfk 7.7 s, turbo 2.3 s of `encode time`), the das rows' warmup
  transcription is outside the timed reps.
- **The whisper rows again with the decoder on the Vulkan ASR-decoder driver (the per-window
  cross-KV and every decode batch on the device; the encoder's GEMMs on the cm2 f16 feed, the conv
  stem and mel still on the CPU), the same `vk` arm, wall ms / encode ms:** turbo jfk **136 / 77**,
  jfk3 **289 / 163**, gb1 **1340 / 622**, hp0 **1857 / 995**, hp0x2 **3623 / 1830** - the wall 3x
  to 6x under the CPU-decoder rows above, and hp0x2's transcription at the reference exe's 3.70 s;
  the encode 96 ms a chunk (1830 / 19) against its 47 ms on the device - the CPU mel and stem and
  the wake between chunks. Tiny jfk **40 / 30**, jfk3 **121 / 21**, gb1 **575 / 89**, hp0 **724 /
  121**, hp0x2 **1451 / 271** against the reference's 1.98 s total on hp0x2.
- **The same rows with the conv stem on the device too (the whisper-class chain's conv seat on
  the cm2 feed; the mel the one CPU phase left), wall ms / encode ms:** turbo jfk **101 / 49**, jfk3
  **236 / 98**, gb1 **1087 / 388**, hp0 **1450 / 601**, hp0x2 **2903 / 1122** - the transcription
  under the reference exe's 3.70 s on hp0x2, the encode 59 ms a chunk (1122 / 19) against its 47 ms
  on the device and the reference's 35 to 40; tiny jfk **33 / 19**, jfk3 **97 / 20**, gb1 **487 /
  62**, hp0 **599 / 70**, hp0x2 **1170 / 120** against the reference's 1.98 s. Canary on the same
  tip with the chain's row cap replaced by the device's range, the `vk` arm over all five clips
  (`--asr-clips`), wall / encode: jfk **157 / 59**, jfk3 **433 / 140**, gb1 **1258 / 784**, hp0
  **2288 / 1035** against the NeMo CUDA reference's `generate()` at 743 / 2213 / 3443 / 9022 ms
  (`external`) - 4x
  ahead on hp0; hp0x2 at this tip still declined on the front's own row cap (20.1 s, the CPU front)
  and takes the next tip's row.
- **The next tip - gemma4a's projector tail on the blocks chain and the canary front off its row
  cap - E2B (`vk`, wall / encode):** jfk **184 / 18**, jfk3 **493 / 40**, gb1 **3194 / 190**, hp0
  **3910 / 268** against llama-mtmd-cli's Vulkan encode 21 / 54 / 223 / 327 (`external`) - at or
  under the reference on every clip; canary hp0x2 **4130 / 2577** on the device (6840 rows, the
  plane R at 375 MB) against NeMo's 13508 ms (`external`).
- **The whisper rows with the pre-LN chain's row passes folded into its f16 feed (six row passes
  a block where twelve ran), wall / encode:** turbo jfk **95 / 44**, jfk3 **226 / 88**, gb1
  **1095 / 354**, hp0 **1396 / 546**, hp0x2 **2710 / 1046** against the reference exe's 3.70 s
  on hp0x2 - the chain 42.2 ms a chunk on the device (`DASLLAMA_GPU_PROF=1`, 450 dispatches,
  was 47 ms over 642) against the reference's 37.4 ms of ops for the same chunk (`external`,
  `GGML_VK_PERF_LOGGER=1`: its `MUL_MAT` ops on the q8_0 planes 23.1 ms - 128 x 84.5 us at k
  1280, 32 x 219 us at k 5120, 32 x 165 us at m 5120 - and 14.3 ms of its other ops,
  `FLASH_ATTN_EXT` among them); ours by role under `DASLLAMA_GPU_PROF=1`: fc2 11.8, fc1 8.0, q /
  k / v / o 12.5, the attention 4.1, the row passes 3.7, the readback 1.7. Tiny jfk **32 / 3**, jfk3 **100 / 6**, gb1
  **488 / 22**, hp0 **591 / 33**, hp0x2 **1171 / 66** against the reference's 1.98 s. Qwen3-ASR
  on the same chain, wall / encode: jfk **83 / 15**, jfk3 **226 / 38**, gb1 **1427 / 213**, hp0
  **1863 / 299**, hp0x2 **4299 / 626** against llama-mtmd-cli's Vulkan encode 21 / 177 / 307 / 403
  / 831 (`external`) - under the reference on every clip (the 128-row windows' blocks 4.6 ms a
  window on the device under `DASLLAMA_GPU_PROF=1`, 254 dispatches). E2B on this tip reads jfk **186 / 18**, jfk3 **493 / 40**, gb1 **3201
  / 190**, hp0 **3921 / 268**; its hp0x2 row is the family's - 13,665 soft tokens pass the E2B
  decoder's 8192-position context, on the CPU chain alike.
- **The whisper chain's fc2 GEMM under the split-k pick (four 1280-deep chunks of its 5120 K into
  the partial planes, the reduce after), every dispatch figure under `DASLLAMA_GPU_PROF=1`:** on
  the RTX PRO 4500 the fc2 dispatch reads **260 + 16** us where it read 367 whole (8.3 + 0.5 ms a
  chunk over 32 blocks, was 11.8), the chain **39.6 - 40.5 ms** a chunk on the device where it
  read 42.2 (482 dispatches) against the reference's 37.4 (`external`); the turbo rows move inside
  noise - jfk **114** vs 116 ms and hp0x2 **3458** vs 3456 with the split off
  (`DASLLAMA_CM2_SPLITK=1`), the decoder's share of the wall. On the 36-SM RTX 5060 Ti the pick
  refuses (60 tiles fill two waves there): forced, the same site reads 467 + 20 us in two chunks
  (`DASLLAMA_CM2_SPLITK=2`) and 438 + 55 in four (`DASLLAMA_CM2_SPLITK=4`) against 477 whole.
  Qwen3-ASR's 128-row windows (seven tiles at d 896) under a forced five-way split
  (`DASLLAMA_CM2_SPLITK=5`) read the reduce as a loss - hp0 encode **314** where it read 299,
  hp0x2 **658** where it read 626, 5% - so the chain's gate splits only at `VT_SK_MIN_ROWS` = 512
  rows or more; past the gate the wave model picks the chunk count (`DASLLAMA_CM2_SPLITK` unset or
  `0`), and the tower logs the pick once per model.
- **Qwen3-ASR-0.6B (the whisper-class tower through its conv front, 18 blocks at d 896 / ff 3584,
  128-row batch sets at tile 32 - the family's chunk is short), wall / encode:** jfk **628 / 541**
  against 1319 / 684; jfk3 **1569 / 1429** against 3872 / 2017; gb1 **9244 / 7835** against 29437 /
  11608; hp0 **13881 / 11465** against 42471 / 16256; hp0x2 **28114 / 24361** against 113686 /
  32035 - the encode only 1.3x to 1.5x faster: at 128 rows an encode is one 128 x 896 batch on the
  32-wide tile, and the per-encode fixed cost (the residual upload, the meta rebuild, the readback)
  is the column; a lever for the fronts row (`followup_vulkan.md` row 99) or a batching of the
  family's chunks.
- **Canary-Qwen 2.5B (the FastConformer encoder, 32 blocks at d 1024 / ff 4096 / 128-wide heads,
  the rel table at 2 npos - 1 rows a clip), wall / encode:** jfk **505 / 407** against 2233 / 1120;
  jfk3 **1585 / 1296** against 6802 / 3084; gb1 **9500 / 9057** against 33780 / 20697 - the encode
  2.3x to 2.7x faster; the full bidirectional rel-pos attention at O(npos^2 hs) a head is the
  column at gb1's 2496 rows (the wall is the encode there: 9.1 of 9.5 s).
- **Gemma-4 E2B audio (the Conformer, 12 blocks at d 1024 / ff 4096 / 128-wide heads, the chunk-12
  windowed attention, 320-row
  and 768-row batch sets on one residency), wall / encode:** jfk **461 / 295** against 3126 / 854;
  jfk3 **1306 / 849** against 9392 / 2533; gb1 **8067 / 5017** against 62576 / 15183; hp0 **10570 /
  6904** against 86876 / 20871 - the encode 2.9x to 3.0x faster; the wall's larger drop is the
  E2B decoder on the resident driver.
- **The arc's tip on the pod (`vk`, every row stamped `vulkan`, declines 0; wall / encode over
  jfk / jfk3 / gb1 / hp0 / hp0x2):** whisper turbo **93 / 41**, **213 / 83**, **1042 / 334**,
  **1383 / 513**, **2685 / 983**; Qwen3-ASR **80 / 14**, **224 / 37**, **1430 / 217**, **1866 /
  303**, **4305 / 648** against llama-mtmd-cli's 21 / 177 / 307 / 403 / 831 (`external`) - under
  the reference on every clip; canary **156 / 59**, **431 / 137**, **1263 / 787**, **2275 /
  1022**, **4224 / 2579** against NeMo's 743 / 2213 / 3443 / 9022 / 13508 (`external`); E2B **185 /
  -**, **493 / -**, **3210 / -**, **3917 / -** (the E2B ASR row takes no hp0x2: 13665 audio
  tokens exceed its decoder's 8192-token context, the facade's own refusal). The split-k A/B at
  the tip under `DASLLAMA_GPU_PROF=1`: turbo hp0x2 **3578** with the pick (fc2 8.38 ms + the
  reduce 0.52 ms a chunk over 32 blocks, the wave model's four chunks) against **3674** with the
  split off (`DASLLAMA_CM2_SPLITK=1`, fc2 11.80 ms).
- **The whisper parity pass (the same rig, `vk`, wall / encode over jfk / jfk3 / gb1 / hp0 /
  hp0x2; the walls the best of two timed reps, the provenance above) [direction-grade - two
  processes, and the pass's tip against the arc's]:** turbo **61 / 31**, **151 / 62**,
  **771 / 248**, **981 / 371**, **1941 / 719**, the loop 1 / 2 / 8 / 12 / 23 windows a clip. The
  reference is whisper-cli with its timestamps on (`external`: whisper.cpp d09f61a, version
  1.9.4-dev, the pod's Vulkan build; `whisper-cli -m ggml-large-v3-turbo-q8_0.bin -f <clip> -t 16
  -bs 1 -bo 1 -nf`, the recipe `ARCHITECTURE_MEASUREMENT.md` 2.20 carries: each clip behind jfk in
  one process, the jfk-only process subtracted, two reps and the min - the reps' pairs sit in the
  run logs, not here): encode 36 / 70 / 342 / 444 / 886, total 101 / 252 / 1302 / 1646 / 3409, over
  1 / 2 / 10 / 13 / 26 encoder runs - so the encoder reads 1.12x to 1.38x ahead a clip (31.2 ms a
  window against 34.5) and the transcription 1.67x to 1.76x. Under `-nt` the same reference
  (`external`, the same build and flags plus `-nt`) decodes no timestamp tokens and advances a
  whole 30 s a window (19 runs on hp0x2, encode 649, total 3170), a shape the das loop does not
  run. The four levers, read on the jfk ledger (`DASLLAMA_GPU_PROF=1`): the block chain 39.3 ->
  29.0 ms - the l stamp's clamped edge path on the chunk's partial last token column (1500 rows =
  five columns and 220), lifted by the GEMM records rounded to the column (the probe's `wh` arm,
  `daslang -jit harness/vk_gemm_probe.das -- wh` on the pod's RTX PRO 4500 under `DASLLAMA_GPU=1`
  in cm2 mode, the l column at 1500 rows against 1536 - the arm's alternate is the same shape at
  the whole column: q / k / v / o 95 -> 52 us, fc1 241 -> 173, fc2 357 -> 185 whole, split4
  240 -> 158 + 16, the m column 62 / 194 / 229 at either count; ggml's `MUL_MAT` on the same
  shapes reads 84 / 166 / 221 - `external`, `GGML_VK_PERF_LOGGER=1 whisper-cli -m
  ggml-large-v3-turbo-q8_0.bin -f jfk.wav -t 16 -bs 1 -bo 1 -nf`, the same build) - the cross-KV
  chain 15.8 -> 0.7 ms (the 61 MB CPU-layout readback made lazy), the stem chain 2.1 -> 0.3 ms
  with the encoder rows handed to the decoder device to device and the post-norm folded into the
  chain, the decode step 91 -> 66 dispatches a token (80 -> 55 ledger stamps: the row passes
  carrying their Q8_0 feed; a three-row prompt batch 0.84 -> 0.77 ms on the device). The
  served-row check the pass opened with: the resident prefill's tile pick beats both forced
  columns on every model measured (pp512 tok/s, the rig's `lcpp_bench -p 512 -n 0 -r 2`, the best
  of two, the tile through `DASLLAMA_CM2_TILE` unset / 128 / 256: Llama-3.2-3B Q8 14792 / 11007 /
  12442, gemma-3-1b 34671 / 34280 / 32909, E2B 16313 / 13196 / 14920, E4B 9804 / 8918 / 9496,
  gemma-2-2b 19540 / 18044 / 17533), and the wave model picks the faster column on every 512-row
  shape the probe's `g3`, `gemma`, `tl` and default arms time.
- **The tower dedup pass's tip (the same rig, `vk`, wall / encode over the five clips, the best
  of two) [direction-grade - the dedup tip against the parity pass's]:** turbo **61 / 31**,
  **149 / 61**, **772 / 249**, **988 / 371**, **1934 / 721** against the parity pass's 61 / 31,
  151 / 62, 771 / 248, 981 / 371, 1941 / 719 - within noise; the jfk ledger the same (the stem
  0.28 ms, the blocks 29.9, the cross-KV 0.73, the decode 66 dispatches - 55 stamps - at 0.77 ms
  for the three-row batch). The pass's gate for a stamp it left alone is `harness/vk_spv_diff.das`
  over the dump before and after (the kernel files and every twin): 256 stamps byte-identical, 18
  moved, none appeared, and the moved set is the fold set - the six residual seams (`cls_ar_rq`,
  `cls_ar_rq_b`, `cls_ar_rqx`, `cls_ar_comb_rq`, `cls_ar_comb_g4_rq`, `q8_gemv_ar`) on the shared
  block pass, the two clamp arms, the seven post-add and Q8 norm stamps on the seam template, the
  decoder's attention partial on the shared reduces and its two combines on one template; the f32
  and f16 layer-norm stamps and every requant stamp stayed identical. The residual seams' own gate
  is a decode row: E2B Q8_0 flat tg128 **198.45 +/- 0.09** against the ledger's 198.5 to 198.8
  (pp512 16352 +/- 43 against 16313), the rig's `lcpp_bench -m gemma-4-E2B-it-Q8_0.gguf -p 512 -n
  128 -r 3 -t 16` under `DASLLAMA_GPU=1`, `DASLLAMA_COOPMAT` unset (cm2), `DAS_TUNE_POLICY` unset,
  three reps and their spread. The four classes the pass deleted (`TowerHeadGather`,
  `TowerZeroRows`, `TowerQ3aFinish`, `TowerGluAct`) are the four that vanish, the family twins
  holding their chains' bars on the pod: `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin`,
  `test_qwen3a_vulkan_front`, `test_encoder_blocks_vulkan` (`tests/test_audio.das`),
  `test_whisper_vulkan_twin`, `test_whisper_vulkan_wdec` (`tests/test_whisper.das`),
  `test_gemma4v_vulkan_twin`, `test_gemma3v_vulkan_twin`, `test_qwen3v_vulkan_twin`,
  `test_qwen25v_vulkan_twin`, and `tests/test_vulkan_tower_kernels.das` whole, each run through
  dastest's `--test-names` filter on the pod at the tip. The review round's fix tip (the
  layernorm and post-add stores on one `mad`, the block pass behind a buffer barrier, the fused
  bias pass on the literal tanh GELU) reads **61 / 31**, **151 / 62**, **772 / 247**, **993 / 370**,
  **1934 / 714** on the same rig - within noise again - with E2B tg128 198.67 +/- 0.32 and pp512
  16432 +/- 79, and the probe's `wh` arm, its feed now carrying the read slack and its two columns
  raced round by round, reads q / k / v / o 95 -> 52 us, fc1 240 -> 170, fc2 359 -> 184 whole,
  split4 240 -> 153 + 16, the m column 62 / 186 / 228 at either count; the differ over the fix
  tip's dump against the dedup tip's: 256 identical, 18 moved (the requant family on the one
  block-width constant, the layernorm and post-add stamps on the `mad`, the fused bias pass on
  its literal GELU), none appeared or vanished. The whisper decoder's flushed-memory bar
  (`test_whisper_vulkan_wdec_flush`, `WDEC_FLUSH_LOGIT_BAR` = 5% of the knob-off chain's peak
  logit) reads 0.11 of a 23.5 peak on tiny on the pod (the bar 1.18; the batch with its last token
  changed moves the knob-off logits by 3.29, outside it).
- **The driver's allocations at the largest shape the path serves, the 4096-row encode cap
  (`VT_MAX_ENCODE_ROWS`; whisper-class chunks stop at 1500 rows, gemma4a's at 768; canary has no
  row cap and declines `shape` only past the device's storage-buffer range):** the rel quartet
  (`vt_rel_bufs`) at a 4096-row canary encode's 2 x 4096 = 8192 rows and d 1024
  - the f32 table 8192 x 1024 x 4 = 33,554,432 bytes, its Q8_0 quants 8,388,608, their scales 8192 x
  1024 / 32 x 4 = 1,048,576, the projected rows another 33,554,432 - about 76 MB; gemma4a's quartet
  is 13 rows, under 60 KB. The per-encode device scratch follows the vision arc's list above at each
  family's d and ff with no padded panels (every audio head is 64 or 128 wide), plus the qwen25v
  hidden's zero row map (`rex_dev`, cap x 4 bytes: 16 KiB at 4096 rows): about 441 MB for
  turbo at 4096 rows (d 1280, ff 5120: eleven f32 row buffers 230 MB, two hidden buffers 168 MB, the
  f16 K/V shadows 21 MB, the Q8_0 feed 22 MB) and about 353 MB for canary and E2B (d 1024, ff 4096).
  The weights uploaded whole: turbo's Q8_0 plane 32 x (4 x 1280^2 + 2 x 1280 x 5120) = 629 M
  weights, about 629 MB of quants plus 39 MB of f16 scales; canary's and E2B's eleven records a
  block, 8 d^2 + 4 d ff = 25.2 M weights a block - 806 M weights, about 806 MB plus 50 MB of
  scales, over canary's 32 blocks and 302 M, about 302 MB plus 19 MB, over E2B's 12. One resident
  a family (`g_vt_aud`, `g_vt_g4a`, `g_vt_cn`), released by the model drop's sweep.
- **The allocations that scale with the input or the model, by formula (bytes), at two shapes:**
  the fc2 split-k partial planes, nsplit x cap x d x 4 (cap = the rows rounded up to 64) - turbo's
  four-way split over 1500 rows 4 x 1536 x 1280 x 4 = 31,457,280, the pick's ceiling of eight planes
  at 4096 rows and d 1280 167,772,160; canary's rel plane, cap x (2 cap - 1) x 4 - gb1's 2496 rows
  49,830,144, hp0x2's 6840 rows (cap 6848) 375,133,440. The ASR decoder driver, with nl layers, d,
  ta encoder positions (1500) and tmax text positions (448): the cross K/V planes kx / vx at nl x d
  x ta x 4 each and their f16 halves kxr / vxr at nl x d x ta x 2 each - turbo (nl 4, d 1280)
  30,720,000 and 15,360,000, large-v3 (nl 32) 245,760,000 and 122,880,000; the self K/V caches kc
  / vc at nl x d x tmax x 2 each - turbo 4,587,520, large-v3 36,700,160; the host readback of the
  cross K/V at 2 x nl x d x ta x 4 - turbo 61,440,000, large-v3 491,520,000; the attention
  partials pms at nchunks x heads x 8 x 2 x 4 and py at nchunks x heads x 8 x 64 x 4 (nchunks = the
  longer of ta and tmax over 256 keys: 6 chunks, 20 heads) - 7,680 and 245,760 on both; the weight
  gather uploads the decoder's Q8_0 plane whole, its quant bytes plus 2 bytes a block scale, staged
  through host arrays of the same sizes.
- **The device chain's distance from the exact f32 CPU chain at the tip, rel_l2 (the CPU q8 chain's
  own distance beside):** gemma4a E2B over jfk 0.0554 against 0.0550 (the device 0.0134 from the
  CPU q8 chain); canary over jfk's log-mel 0.0233 against 0.0220 (0.0213 from the CPU q8 chain);
  the whisper-class blocks over the synthetic mel of the Metal blocks cell, on the 5060 Ti (the pod
  stocks no f32 mmproj): qwen2audio 0.0528 against 0.0684, voxtral 0.1706 against 0.1741, omni-3b
  0.0753 against 0.0790; the 5060 Ti's gemma4a and canary readings 0.0540 / 0.0229 against 0.0560 /
  0.0220. Whisper tiny and turbo transcribe jfk to the same text on the CPU q8 chain and the device
  chain, turbo with one near-tie token of 28 flipped.

  Provenance of the twin-cell figures: `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin` and
  `test_encoder_blocks_vulkan` (`tests/test_audio.das`) and `test_whisper_vulkan_twin`
  (`tests/test_whisper.das`), each run through dastest under `-jit` with `--test-names` naming the
  cell, `DASLLAMA_GPU=1`, `DASLLAMA_IMAGE=0`, `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=8`,
  `DAS_TUNE_POLICY` unset; the box is the pod on its cm2 arm unless the sentence names the 5060 Ti
  (Boris's box, RTX 5060 Ti 16 GB, driver 616.56, cm2 arm). A distance is the cell's own logged
  rel_l2 over one run of the cell; the cells log the reading on green and assert the 1.5x bar.

### From the vulkan-lane mint arc (2026-09-29)

The instrument: Boris's box (RTX 5060 Ti 16 GB, driver 616.56, cm2 arm, the Zen2 CPU),
`bin/Release/daslang.exe -jit -module-cache .jitted_scripts/module_cache/dastest.dascache
dastest/dastest.das -- --test modules/dasLLAMA/tests/test_vulkan_mint.das --test-names
test_vulkan_mint_lanes` from the repo root under `DASLLAMA_GPU=1 DAS_LOG_LEVEL=info
DAS_JOBQUE_THREADS=8 DASLLAMA_MODELS_DIR=D:/Work/llama.cpp/models`, `DASLLAMA_COOPMAT` and
`DAS_TUNE_POLICY` unset [debug-jit - the cell's own log lines, one run]. A save wall is the
`prepared image streamed in N ms` line (the whole streaming save: the walk's uploads, the twin
streamed to the writer, every planar family transcoded off the gguf mapping), a twin's size and
mint time the `plane vkblob N MB minted off the gguf in N ms` line, the warm wall the `prepared
vulkan image mapped in N ms` line. The peak footprint is the dastest process's peak working set
(`PeakWorkingSet64`, sampled every 500 ms from a wrapper) over the whole file - three loads of the
MTP carrier, two of its K-quant twin, and the cells' generations - so it bounds the cold mint from
above and is not the mint's own peak.

- **Qwen3.5-0.8B-MTP-Q8_0, the vulkan lane minted off the gguf inside the streaming save:** the
  untrimmed lane's twin **782 MB, 195 entries, minted in 4319 ms**, the whole save **6364 ms**; the
  trimmed lane's twin the same 782 MB in 4284 ms, its save **5446 ms** (no planar family written);
  the trimmed lane mapped back warm in **2508 ms**. Peak working set of the file's process
  **6558 MB**.
- **The reserve the sizing pass hands the writer for the twin** (`vk_mint_extra_bytes`): the quant
  planes' bytes, both halves, plus an eighth for the entry pads, plus the packed emb region where
  the lane is trimmed, plus 64 MB; the twin never outgrows its planar source (a device scale row is
  the disk row's f16 half or narrower), so the surplus is the pads and the window. The file sink
  truncates it at close; the in-memory sink holds it for the model's lifetime, which is the
  `DASLLAMA_IMAGE_SAVE=0` cost of the lane.
- **The lane files** (read in the arc's first run on this box, the same design before the review
  batch; the batch moved the twin's section, not a plane's bytes - `test_vulkan_inline_bake` holds
  the minted twin to the eager bake's byte for byte): one file of **1.92 GB** (1.10 GB trimmed)
  where the planar image and its twin summed 3.03 GB.

### From the AMX bf16 tile arc (2026-09-29, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 16 lanes)

The race and the carriers are from one box and one mint: `daspkg release --root
modules/dasLLAMA/benchmarks` with `DASLLAMA_CONFIRM_MODEL` on the 1B Q8_0 and no other
environment override (noise probes cv 0.08 / 0.03 / 0.06 / 0.12 %, validation drift 1.89 %, 66
kernels), the profile it exported in force. The token-block readings are from an earlier mint
of the same day on the same box, the same winners on every family. The reference build is
`HOW_TO_GET_SIDECAR.md` section 3's (`6c84c7d5d`, `clean-cpu`).

- **The bf16 tile runs every family at the matrix unit's rate: 7.4-8.7 ms a tile, where the
  best vector seat takes 20-48.** The mint's race (`harness/gen_tune_probe.das`,
  `DAS_TUNE_MODE=tune`, `-jit`), one lane, one process, the arms interleaved, best of 8 rounds
  (an arm screened out after 2), the 2048x8192x256 streaming fixture; the sidecar's `race`
  rows, tile ms, the `amx_bf16` two-group arm against the best other arm (busd512 mr16; the
  grid formats' `grid_vbmi`; q8's bias128): k4 7.44 vs 34.50, k5 7.98 vs 23.20, k6 7.42 vs
  32.44, q40 7.43 vs 24.40, iq4xs 7.49 vs 36.72, iq4nl 7.46 vs 36.95, iq3s4 7.46 vs 37.53, k3
  7.38 vs 47.91, iq3s 8.72 vs 36.80, iq3xxs 8.50 vs 36.32, k2 7.44 vs 45.29, iq2s 8.46 vs
  39.58, iq2xs 8.18 vs 39.06, iq2xxs 8.16 vs 35.63, q8 7.43 vs 20.18. The mint crowns the bf16
  arm on all fifteen families, and the q8 crown passed the end-to-end confirm. The int8 tile
  (`amx_int8`) stores and folds its sum at every 32-weight block - the traffic the bf16 leg has
  none of.
- **End to end the minted exe holds 1.27-1.41x the reference build on the mainstream carriers,
  5.9x and 21.5x on the two low-bit ones, tg ahead on all seven.** The released `lcpp_bench`
  that mint built, `dasllama-bench.exe -m <gguf> -p 512 -n 16 -r 3 -t 16 --ref <llama-bench>
  --ref-flavor clean-cpu --ref-no-affinity`; the rig runs the reference in its own process, so
  each pair below is `direction-grade`. pp512 / tg16 tok/s, ours (the spread over 3 reps) vs the
  reference: Llama-3.2-1B Q8_0 1971.7 +- 9.7 / 82.4 +- 0.3 vs 1394.7 / 78.3; Q4_K_M 1943.8 +-
  9.4 / 108.0 +- 0.5 vs 1453.1 / 87.1; IQ4_XS 1954.0 +- 3.9 / 116.5 +- 0.8 vs 1501.1 / 85.1;
  Q3_K_L 1932.1 +- 3.9 / 103.4 +- 0.4 vs 329.5 / 80.6; IQ3_M 1952.0 +- 10.4 / 115.1 +- 2.4 vs
  90.6 / 53.4; Qwen3-4B Q4_K_M 491.1 +- 0.6 / 34.0 +- 0.1 vs 385.5 / 27.8; Qwen3-4B Q8_0 483.4
  +- 1.4 / 25.0 +- 0.06 vs 374.5 / 22.3. The grid carrier (IQ3_M) sits level with the k4 one:
  the gather-fed panel is not the bill.
- **The fat exe takes the same leg.** `daspkg release --fat x86-avx2 --root
  utils/dasllama-server` - the x86-avx2 baseline with the class clones beside it - picks the
  `x86-amx` clone on this box: `dasllama-bench.exe -m <gguf> -p 512 -n 16 -r 3 -t 16` on the
  Llama-3.2-1B Q4_K_M reads 1915.1 +- 6.9 pp512 and 107.7 +- 0.6 tg16 tok/s.
- **The walk's token block: 512 holds end to end; per kernel the best block splits by K
  length.** The kernel race, one process, one script, the candidates interleaved, 9 rounds
  after 2 dropped, the job queue at its default width, `matmul_kq_batch` on k4 through the
  `x64-gen` backend with the walk's floor set to 1 for the race, median ms (cv %): at n=2048 d=2048 ntok=2048 a block of
  128 3.635 (1.5), 256 3.187 (2.7), 512 3.532 (1.5), 1024 3.965 (0.7), 2048 3.877 (0.3); at
  n=8192 d=2048 ntok=2048 128 18.515 (0.5), 256 17.005 (0.5), 512 15.658 (0.4) - 512 is that
  shape's L2 clamp. End to end (`lcpp_bench.das` as the `-jit` script, `--for-debug-purposes`,
  `DAS_TUNE_MANIFEST` on the mint's sidecar with `runtime.q8_token_block` set, the floor at 1,
  `-p 512 -n 16 -r 3 -t 16`; one process a reading, two alternated passes, `direction-grade`),
  pp512 tok/s at 128 / 256 / 512: Llama-3.2-1B Q4_K_M 1724.8 +- 2.3 and 1730.2 +- 6.6 / 1921.3
  +- 2.7 and 1944.4 +- 2.4 / 1963.3 +- 4.2 and 1968.4 +- 4.0; Q8_0 1732.2 +- 1.7 and 1724.7 +-
  6.8 / 1949.3 +- 4.0 and 1930.4 +- 3.9 / 1970.2 +- 9.6 and 1979.4 +- 4.7. The per-shape block
  is `followup_general.md` row 190.
- **What the bf16 leg allocates.** The bf16 activation plane, once per batch call:
  2 x (ntok x n + 32) bytes - 8,388,672 at ntok 512, n 8192 (the 1B's FFN down), 9,961,536 at
  n 9728 (the 4B's). The panel, one per dispatch chunk: 2 x (form x 16 x n + 32) bytes - 524,352
  and 622,656 a lane at form 2, 8,389,632 and 9,962,496 over 16 lanes. The row-dequant panel's
  row buffer, off the emitted panels only: 4 x n bytes, 32,768 and 38,912.

### From the AMX bf16 walk follow-up (2026-09-29, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 8 cores, 16 lanes)

Every rate below is `benchmarks/lcpp_bench.das` as the `-jit` script with `--for-debug-purposes`
under `-no-module-cache`, `DAS_TUNE_POLICY` unset, served on the `x64-gen` backend, one process
a reading, the arms alternated; an arm is a copy of `dasllama/dasllama_math_gen.das` put in place
before its process starts. Each pair compares across processes and is `direction-grade`.

- **The bf16 walk's block in whole tiles under no L2 clamp: 1.065x the clamped arm's pp512 on
  the 4B carriers, the 1B level.** The clamped arm is the walk of the AMX bf16 tile arc (the 512
  floor under `effective_token_block`, 431 tokens at K 9728); the other is `bf16_token_block` -
  512, a multiple of the 32-token tile. No environment override, the sidecar beside the script
  in force (the box's mint), `-p 512 -n 16 -r 3 -t 16`, three alternated passes, pp512 tok/s
  (the spread over 3 reps), clamped then whole-tile: Qwen3-4B Q4_K_M 486.94 +- 1.19, 486.23 +-
  0.61, 488.05 +- 1.45 against 518.70 +- 1.37, 523.30 +- 0.51, 517.21 +- 0.42; Qwen3-4B Q8_0
  491.48 +- 0.46, 494.14 +- 1.13, 492.13 +- 1.60 against 524.42 +- 0.79, 522.26 +- 0.85 (the
  third pass, 511.75 +- 18.91, is past the 3% spread and void); Llama-3.2-1B Q4_K_M 1912.78 +-
  1.69, 1905.51 +- 14.95, 1900.51 +- 3.15 against 1919.13 +- 6.08, 1916.96 +- 4.10, 1911.24 +-
  9.05.
- **The race that picked the block: 512 leads every block the L2 clamp would hand the walk.** Not
  a rate of `lcpp_bench`: `DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=<lanes> bin/daslang -jit
  -no-module-cache modules/dasLLAMA/harness/token_block_race.das` (the `x86-amx` class profile
  stamps the kernels, no sidecar; `DAS_TUNE_POLICY` unset), k4 on the `x64-gen` backend at tile
  form 2 and ntok 512, the candidates interleaved in one process, 11 rounds after 2 dropped, the
  second of two passes a lane count; best ms (cv %), every arm's sampled output equal to the 512
  arm's and the 512 arm's within the bf16 envelope of the disk-order dots. The clamp's 4 MB cuts
  the block to 431 tokens at K 9728 - 416 in whole tiles - and to 256 at K 16384. K 9728 x 2560
  rows, a block of 256 / 384 / 416 / 512: 4.839 (0.2) / 4.804 (1.5) / 4.828 (0.4) / 4.478 (0.5)
  at 16 lanes, 5.323 (1.4) / 5.359 (1.1) / 5.299 (0.8) / 5.139 (0.5) at 8. K 16384 x 4096 rows,
  256 / 384 / 512: 25.561 (0.1) / 25.630 (0.2) / 24.667 (0.1) at 16 lanes, 17.977 (0.8) /
  18.950 (0.3) / 17.190 (0.2) at 8.
- **What the walk scratch holds.** The panels, one allocation a calling context: stride x slots
  + 64 bytes, the stride a panel rounded up to a cache line plus a line, the slots
  `get_dispatch_slot_bound()` - 8 under this box's default job queue. A bf16 panel is form x 16
  x n x 2 bytes, 524,288 at form 2 and n 8192 (the 1B's FFN down) and 622,592 at n 9728 (the
  4B's), so the scratch at 8 slots is 4,194,880 and 4,981,312 bytes; a byte panel is mr x n
  bytes, 4,096 at mr 8 and n 512. The q8
  walk's bias sums, where the stamped plane carries a bias: 4 x ntok x n / 32 bytes, 524,288 and
  622,592 at ntok 512. Each figure is its formula's value at the named shape
  (`walk_panels`, `q8q8_bias_sums`, `kq_walk_panel_bytes` in `dasllama/dasllama_math_gen.das`).
- **The walk scratch costs no rate and saves no bytes.** The arms are the walk that allocates a
  panel per dispatch chunk (the file of master `640826d79`) and the walk on one scratch panel
  per dispatch slot. `DASLLAMA_ALLOW_UNTUNED=1` (the `x86-amx` class profile stamps the kernels,
  no sidecar), `-p 512 -n 16 -r 5 -t 16`, four passes a model in alternating order; pp512 tok/s (the spread
  over 5 reps) and the process's peak resident set read by `/usr/bin/time -v` (`out-of-process`),
  per-chunk then scratch. Llama-3.2-1B Q4_K_M: 1806.67 +- 11.73, 1856.04 +- 4.35, 1832.40 +-
  9.71, 1804.75 +- 22.14 against 1865.93 +- 6.94, 1878.12 +- 16.26, 1834.13 +- 37.13 (one pass
  void at 1798.04 +- 193.89); peak 1,976,720 / 1,976,804 / 1,976,868 kB (the box's first process
  read 2,885,920) against 1,990,836 / 1,980,220 / 1,980,472 / 1,980,224. Llama-3.2-1B Q8_0:
  1836.54 +- 2.94, 1837.14 +- 14.53, 1853.00 +- 10.72, 1860.91 +- 12.28 against 1909.45 +- 7.58,
  1863.06 +- 13.23, 1838.20 +- 27.82, 1849.83 +- 24.48; peak 2,473,812-2,474,228 kB against
  2,477,580-2,484,108. The scratch stays mapped where the per-chunk panels were freed - 3.4 to
  14 MB more at the peak - and the rates sit inside the spread between two processes of one arm.
- **One walk behind the cells reads the rate of the two it replaced.** The arms are the q8 and
  superblock cells each with its own walk and the cells on `bf16_walk`; the flags and override
  of the entry above, Llama-3.2-1B Q8_0, four passes in alternating order, pp512 tok/s, two walks
  then one: 1901.34 +- 28.39, 1924.55 +- 14.96, 1913.96 +- 9.64, 1845.53 +- 48.54 against
  1952.21 +- 6.37, 1859.75 +- 6.32, 1887.56 +- 22.77, 1900.56 +- 7.18.

### The CPU self-speculation entry (2026-09-29, Apple M5 Max, 18 lanes, decode on the 6 fast ones)

Every pair below is one process of `benchmarks/lcpp_bench.das` as the `-jit` script, master
`640826d79`: `lcpp_bench.das -- -m <gguf> --ngl 0 --mtp-ab --chat-prompts --prompts
modules/dasLLAMA/benchmarks/data/specbench4_prompts.txt --for-debug-purposes` (`--ngl 99` for
the Metal pair), `DAS_TUNE_POLICY` unset, `DASLLAMA_ALLOW_UNTUNED=1` (the `arm-i8mm` class
profile stamps the kernels, no sidecar), depth 1, served on the `arm64-gen` backend (`portable`
under Metal). The off and on arms run in that one process; tg-real128 tok/s (the spread over 5
reps, 4 prompts), off then on, with the drafts accepted.

- **Self-speculation on the CPU loses on every carrier; on Metal it gains.** CPU:
  Qwen3.5-0.8B-MTP Q8_0 189.56 +- 3.13 / 139.41 +- 3.71 (on is 0.74x off), 1005 of 1560;
  Qwen3.5-4B-MTP Q8_0 52.17 +- 1.15 / 45.65 +- 0.53 (0.88x), 1110 of 1455; Qwen3.5-9B-MTP Q8_0
  29.66 +- 0.50 / 26.50 +- 0.49 (0.89x), 1100 of 1470; Ornith-1.5-35B-A3B Q4_K_M 58.75 +- 1.05 /
  40.44 +- 0.74 (0.69x), 1095 of 1475. Metal: Ornith-1.5-35B-A3B Q4_K_M 132.53 +- 1.69 / 143.37
  +- 0.85 (on is 1.08x off), 1080 of 1470. The server's default arms the round on a one-stream
  GPU slot and leaves it off on the CPU.

### From the AMX short-batch arc (2026-09-30, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 8 cores, 8 lanes)

The race: `DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=8 bin/daslang -jit -no-module-cache
modules/dasLLAMA/harness/tile4_race.das` at commit `60beeec07` (the `x86-amx` class profile stamps
the kernels, no sidecar; `DAS_TUNE_POLICY` unset), one process a run, two runs. A four-token step
over 1536 MB of q8 planes a shape, walked plane after plane so every weight group streams from
DRAM; every arm timed once a round in turn, 31 rounds after 3 dropped. An arm is the stamped
GEMV a token, the stamped tile4, or the vector tile at a kstep (`k`), nrsplit (`n`) and prefetch
distance in bytes (`pf`); each figure is the median ms of a pass over all the planes (cv %), run a
/ run b. Every tile arm's output is bit for bit the GEMV arm's, the stamped tile4's the `k4 n4
pf2048` arm's.

- **The tile4's shape - kstep 4, nrsplit 4, a 2048-byte prefetch - leads every candidate on
  every shape, 1.45x to 1.58x the GEMV a token.**
  K 1536 x 6144 rows, 170 planes: gemv a token 28.57 (1.4) / 28.46 (1.1); tile4 (stamped) 19.70
  (1.8) / 19.58 (1.2); k1 n4 pf0 24.01 (2.5) / 23.93 (2.0); k2 n2 pf0 25.07 (1.9) / 25.08 (1.9);
  k2 n4 pf0 24.54 (2.0) / 24.48 (2.3); k4 n4 pf0 21.68 (1.9) / 21.68 (2.2); k2 n4 pf1024 21.39
  (1.8) / 21.43 (2.0); k2 n4 pf2048 20.19 (1.7) / 20.32 (1.4); k2 n4 pf4096 20.12 (1.6) / 20.13
  (1.2); k1 n4 pf2048 20.20 (1.8) / 20.11 (1.8); k2 n2 pf2048 22.11 (1.2) / 21.94 (1.0); k4 n4
  pf2048 19.59 (1.4) / 19.54 (1.1); k4 n4 pf4096 19.76 (1.4) / 19.74 (1.2).
  K 6144 x 1536 rows, 170 planes: gemv a token 28.42 (1.4) / 28.42 (1.1); tile4 (stamped) 19.66
  (1.2) / 19.66 (1.1); k1 n4 pf0 24.21 (2.3) / 24.38 (1.9); k2 n2 pf0 25.86 (2.0) / 25.76 (2.0);
  k2 n4 pf0 24.91 (3.2) / 24.70 (2.7); k4 n4 pf0 22.42 (2.7) / 22.23 (1.9); k2 n4 pf1024 22.73
  (5.3) / 21.84 (4.8), past the 3% spread and void; k2 n4 pf2048 20.52 (2.4) / 20.52 (1.7); k2 n4
  pf4096 20.45 (2.0) / 20.55 (1.6); k1 n4 pf2048 20.56 (1.7) / 20.51 (1.6); k2 n2 pf2048 22.53
  (1.2) / 22.54 (1.2); k4 n4 pf2048 19.64 (1.3) / 19.60 (1.2); k4 n4 pf4096 20.07 (1.4) / 20.05
  (1.5).
  K 2560 x 9728 rows, 64 planes: gemv a token 29.37 (1.5) / 29.71 (1.3); tile4 (stamped) 18.70
  (1.1) / 18.95 (1.0); k1 n4 pf0 23.51 (2.4) / 23.91 (2.0); k2 n2 pf0 24.61 (2.4) / 25.10 (2.1);
  k2 n4 pf0 24.00 (2.8) / 24.16 (3.2); k4 n4 pf0 21.47 (2.2) / 21.77 (2.7); k2 n4 pf1024 20.76
  (3.9) / 21.60 (4.9), void; k2 n4 pf2048 19.61 (1.6) / 19.59 (1.5); k2 n4 pf4096 19.71 (1.5) /
  19.73 (1.5); k1 n4 pf2048 19.48 (1.5) / 19.59 (1.3); k2 n2 pf2048 21.36 (1.1) / 21.52 (1.0); k4
  n4 pf2048 18.68 (1.1) / 18.89 (0.9); k4 n4 pf4096 19.09 (1.4) / 19.42 (1.6).
  K 9728 x 2560 rows, 64 planes: gemv a token 29.57 (1.9) / 29.16 (1.6); tile4 (stamped) 18.71
  (1.2) / 18.55 (1.2); k1 n4 pf0 23.76 (2.6) / 23.60 (2.1); k2 n2 pf0 25.21 (2.5) / 25.09 (2.1);
  k2 n4 pf0 24.30 (2.6) / 24.09 (2.2); k4 n4 pf0 21.77 (2.4) / 21.47 (2.4); k2 n4 pf1024 21.32
  (5.5) / 20.76 (5.4), void; k2 n4 pf2048 19.52 (1.8) / 19.39 (2.0); k2 n4 pf4096 19.67 (1.6) /
  19.52 (1.7); k1 n4 pf2048 19.39 (1.3) / 19.31 (1.6); k2 n2 pf2048 21.63 (1.2) / 21.62 (1.1); k4
  n4 pf2048 18.66 (0.8) / 18.54 (1.0); k4 n4 pf4096 18.98 (1.5) / 18.95 (1.4).
  The prefetch is worth 1.11x to 1.17x at kstep 4 (the `k4 n4 pf0` arm against `k4 n4 pf2048`),
  and that tile with no prefetch is already 1.27x to 1.37x the GEMV a token.

The served turn: `benchmarks/lcpp_bench.das` as the `-jit` script under `-no-module-cache`,
`lcpp_bench.das -- -m <gguf> -p 64 -n 128 -r 3 --npl 4 -t 8 --for-debug-purposes`,
`DAS_TUNE_POLICY` unset, the box's sidecar beside the script (its mint at master `202427bd2`),
served on the `x64-gen` backend, one process a reading, `.jitted_scripts` removed before each
process. An arm is the tree's `dasllama/` sources put in place before its process starts, and
the tile4 of these readings is the `k2 n4 pf2048` arm of the race - they were taken before it.
Each pair compares across processes and is `direction-grade`; rates are tok/s (the spread over 3
reps), tg128@4 summed over the 4 streams. The released reading of the landed shape is the
board's (`performance/records/gnr.json`).

- **A batch shorter than the amx tile's 32 tokens on the 4-token vector tile: 1.35x master's
  tg128@4 on gemma-4-E2B Q8_0, tg128 unmoved.** Master walks such a batch one GEMV pass a token
  (`bf16_walk` runs no tile below the token step); the arm hands it to the stamped perm's
  `tile4` companion, with no activation plane and no panel built. Master then the arm, tg128@4 /
  tg128: gemma-4-E2B Q8_0 76.46 +- 0.41 / 32.50 +- 0.38 against 103.05 +- 0.56 / 32.24 +- 0.40
  (a second master process read 81.36 +- 0.37 / 32.80 +- 0.04); Qwen3VL-4B Q8_0 49.91 +- 0.09 /
  18.97 +- 0.21 against 65.32 +- 0.42 / 19.30 +- 0.03; Qwen2.5-Omni-3B Q8_0 65.45 +- 0.34 /
  24.93 +- 0.34 against 86.08 +- 0.29 / 24.89 +- 0.19; gemma-4-E4B Q8_0 41.98 +- 0.43 / 16.22 +-
  0.07 against 55.27 +- 0.05 / 16.20 +- 0.04.
- **In the served turn the prefetch is most of the gain.** The arms are `tile4_perm`'s three
  constants edited in `dasllama/dasllama_gemm_gen.das`, gemma-4-E2B Q8_0, tg128@4 at kstep /
  nrsplit / prefetch distance: 2 / 4 / 0 80.35 +- 0.40; 1 / 4 / 0 74.32 +- 0.12; 2 / 2 / 0 78.73
  +- 0.22; 4 / 4 / 0 83.73 +- 1.26; 2 / 4 / 2048 101.87 +- 0.88; 2 / 4 / 4096 103.63 +- 0.32.
  With no prefetch the vector tile reads master's rate here, where the race's planes give it
  1.27x and more: a whole step streams every layer's groups, and four dots a line leave too few
  lines in flight.
- **The batch below the tile step skipping the bf16 walk reads 1.14x the walk's tile4 tail.**
  The arms are the kernels with the `below_amx_step` test forced false at its two q8 sites (the
  batch takes `bf16_walk`, whose tail below a tile runs `tile4`, after the activation plane and
  the panel scratch are built for nothing) and the kernels as they are, gemma-4-E2B Q8_0,
  tg128@4: 90.94 +- 0.19 against 103.99 +- 0.87. The predicate is structural - a batch below the
  step runs no tile, so the plane and the panel are dead work - and this pair is its reading,
  taken in two processes.
- **The superblock families gain less: their step is less bound by the weight stream.** Master,
  then the `tile4` with no prefetch in the superblock block emitters, then with the packed
  planes' prefetch, tg128@4: gemma-4-12B Q4_K_M 22.30 +- 0.13 / 22.51 +- 0.13 / 23.33 +- 0.03;
  Qwen3.8-27B IQ4_XS 8.64 +- 0.02 / 9.71 +- 0.01 / 10.29 +- 0.02. Qwen3-30B-A3B Q4_K_M read
  40.15 +- 0.71 on the no-prefetch arm; its master reading, 37.90 +- 2.09, is past the 3% spread
  and void.
- **Refuted: an `amx_bf16` perm over the bias-128 plane.** The vnni512 class's bias-128 `vpdpbusd`
  lattice reads ahead of the amx class's at tg128@4, so the arm stamped the amx perm with
  `bias=128` (the panel un-biasing the plane, the GEMV and `tile4` riding the biased lattice).
  gemma-4-E2B Q8_0, two alternated passes, unbiased then biased, tg128@4 / tg128: 103.08 +- 0.59
  / 31.53 +- 0.13 and 103.26 +- 0.69 / 32.16 +- 0.33 against 98.00 +- 0.40 / 29.97 +- 0.20 and
  99.02 +- 0.50 / 30.29 +- 0.48 - the biased arm is 0.95x on both legs. `.jitted_scripts` was
  kept across these four processes; the perm's `bias` argument keys the biased arm's kernels
  apart.

### From the Vulkan dedup pass (2026-10-05, RunPod RTX PRO 4500 Blackwell, 16 lanes, the models network volume)

The pass folds 247 tier-A and 76 tier-B duplicate sets across the Vulkan engine, its kernel classes
and the test rigs (one commit per fold, net about -5300 lines against master 1a0729a6ef) and means
to move no served number. The readings are `lcpp_bench.das` rows on the same card in one process a
row, master then the tip right after it, `--ngl 0 -p 512 -n 128 -r 5 -t 16`, the Vulkan lane minted
once per model and shared by both trees. pp512 on this card spreads past 10% between reps on the
small models (the 1B reads +-12500 of 30600 on master), so tg128 is the column that holds a verdict
and pp512 is read against its own spread.

- **tg128 holds on every row.** Master then the tip: Llama-3.2-1B Q4_K_M 554.87 +- 0.51 / 551.73 +-
  0.55; Qwen3-4B Q8_0 146.54 +- 12.62 / 152.21 +- 0.22; gemma-4-E4B Q8_0 98.33 +- 15.27 / 112.00 +-
  0.05; gemma-4-12B Q4_K_M 82.12 +- 0.02 / 82.18 +- 0.02; Qwen3.5-9B-MTP Q5_K_XL 105.20 +- 0.03 /
  105.07 +- 0.03; Qwen3.8-27B Q4_K_M under the tq4 K/V codec 36.97 +- 0.01 / 36.96 +- 0.01 and under
  q8_0 36.98 +- 0.09 / 37.01 +- 0.01. The +-12 to +-15 spreads land on either tree at random (the
  10-rep recheck put them on master once and on the tip once) - a stall, not a tree.
- **Two folds cost the Q8_0 prefill 3.7% and came back.** pp512 read Qwen3-4B Q8_0 11948 +- 344 ->
  11423 +- 256 and gemma-4-E4B Q8_0 9661 +- 113 -> 9371 +- 107, held by a 10-rep master-tip-master-tip
  recheck (11734 / 11294 / 11688 / 11277 and 9559 / 9269 / 9496 / 9268). The per-stage GPU profile
  (`DASLLAMA_GPU_PROF=1`, Qwen3-4B Q8_0, 512 tokens) put every Q8 weight GEMM stage 3-5% longer (q
  4229 -> 4426, k 2482 -> 2560, wo 3685 -> 3816, gate 8353 -> 8799, up 8288 -> 8652, down 7872 ->
  8244 us) and the rest flat. An A/B on the tip named them: `Q8Cm2T.decode` back to its 16-bit-lane
  `unpack8` select reads 11483 +- 243 (the shift form of the F8 rule change, 1.5%); that plus
  `MmBatchT`'s two tiles back as two bodies reads 11894 +- 331 (the one coopmat accumulator array
  walked under `[unroll_full]`, 2.5% - its own 4096x4096 probe had read flat per dispatch, 147.63 ->
  147.68 us; the served shape did not agree, and the old and new tiles load the same fragments, so
  the array itself is what the driver stops keeping in registers). With both back, at 10 reps
  master-tip-master-tip: Qwen3-4B Q8_0 11765 / 11753 / 11508 / 11754, gemma-4-E4B Q8_0 9610 / 9584 /
  9575 / 9587. A probe shape is not the served shape: the served row is the gate a fold passes.
- **The tq4 mirror's control share reads as the constant's doc says on this card.** The hybrid file's
  `tq4_within` holds a tolerance row to `TQ4_CTRL_SHARE` 0.85 of its distance from the one-token-off row
  (`tests/_resident_feed.das`): the share reads 0.09-0.44 on the RTX 5060 Ti and 0.73 on the RTX PRO
  4500 (the 900-token prompt cut at 300), so the bar sits halfway from the widest reading to the row one
  off's 1.0; the model-free controls hold a device row at 0.8 of the distance and red one at 0.9, and a
  NaN reference row with no control row reds. The pod's hybrid file: 69 cells, 65 passed, 4 skipped.
- **The resident llama file's Q4_0 carrier holds the file's logit bar.** `Llama-3.2-1B-Instruct-Q4_0-local.gguf`
  (the 1B requantized, the q40 planes on the window chain's kq batch arm) runs the file's cells under
  `LOGIT_BAR_REL` 0.06 of each row's largest live |logit| with the one-step-off control past it: 14 cells,
  12 passed, 2 skipped on the pod.
- **`rows_within_control` is the one bar statistic the resident rigs read.** The per-file copies
  (`carries_within`, `head_rows_within`, `rows_bar_fails`) fold onto it: every row within `rel` of its CPU
  row's largest live |value| (`live_maxabs`, a NaN and the suppressed-id floor left out) and the control row
  past `ctrl_bars` bars, the widest miss and the tightest control logged with their rows. The cells it
  serves read the same verdicts before and after the fold (hybrid, moe, hc, the regions files 26/26 twice).
