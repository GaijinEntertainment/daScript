# dasLLAMA TTS family: Kokoro + KittenTTS on a permissive daslang front end

Goal: dasLLAMA speaks. A new model flavor - StyleTTS2-lineage text-to-speech - lands as a
family from day one: KittenTTS nano (15M) and mini (80M) plus Kokoro-82M, all driven by one
das-native G2P front end with no GPL anywhere in the shipped pipeline, structured exactly the
way the ASR family is (types floor -> shared block home -> facade -> one file per family).
The front end does not merely match espeak-ng - the rig scores every change, and the targets
in phase 5 are set to beat it.

Context: route decided over 2026-08-28/29 (memory: `dasllama_tts_route_arc.md`). The
LLM-codec route (Qwen3-TTS, Gepard+NanoCodec) was surveyed, A/B'd at the codec level, and
parked: Qwen3-TTS stays the multilingual/cloning fallback, but Boris ruled the StyleTTS2
lineage the direction once the G2P fidelity experiment proved a permissive front end is a
drop-in. espeak-ng (GPL-3.0) is dev-time oracle ONLY, never shipped, never linked.

## Design rulings (Boris)

- 2026-08-28: KittenTTS/espeak/GPL dropped as a runtime dependency "for sure"; GPL in a
  separate repository acceptable in principle but simpler/cheaper preferred.
- 2026-08-29: "the more i look at those dictionaries, the more i think kitten and better yet
  kokoro are the direction" - StyleTTS2 lineage with a permissive das-native G2P.
- 2026-08-29: small statistical tagger over heuristics (averaged perceptron; heuristics keep
  a permanent quality ceiling on heteronyms).
- 2026-08-30: bring BOTH Kokoro and Kitten from the start - family with selection from day
  one. `dasllama_tts.das` facade + per-family `dasllama_kokoro.das` / `dasllama_kitten.das`.
  Use silver-lexicon provenance (accepted knowingly - see Licensing). Actively try to
  SURPASS espeak/misaki on phoneme quality where possible.
- 2026-09-02: ONE PR, long arc - the phases below are checkpoints inside it, not PRs. Work
  on the M1 Max (the oracles live there). Base is master fffb3b370; master merges into the
  branch once PR #3924 (CPU kernel parity) lands. Kokoro's first GGUF carries a curated
  roster, but the FULL voice set and both Kitten checkpoints are downloaded at the pinned
  revisions from day one, so the full GGUF is one converter flag away. Nothing generated is
  committed - GGUFs and packed data live in the model store, not `models/`.

## Evidence base (box-local, M1 Max)

All numbers below were measured 2026-08-29 in `~/Work/tts-ab/g2p/` (REPORT.md, 604 lines;
17 rerunnable scripts; 3,400 WAVs; 14 JSON artifacts; 200 category-tagged sentences in
`data/sentences.json` with expected spoken forms). The codec A/B lives in `~/Work/tts-ab/`
(REPORT.md) and the parked NanoCodec port study in `~/Work/tts-ab/PORT_PLAN.md`. These
directories are the oracles for phase 1 and the seed of the phase-4/5 rig - do not delete.

Winning front end ("arm E") = misaki's lexicon + POS rules, with CMUdict -> g2p_en-GRU as
the OOV fallback (all Apache-2.0 / BSD-2):

| model | espeak-reference WER | arm E WER | p | UTMOS delta |
|---|---|---|---|---|
| Kokoro-82M | 3.13% | 3.18% | 0.86 | 0.000 (ref-vs-itself noise is +0.09pp) |
| kitten-mini | 3.77% | 4.00% | 0.79 | -0.002 |
| kitten-nano | 3.50% | 4.22% | 0.20 | +0.012 |

Heteronyms (38 annotated sentences, both readings must appear): misaki-rules arms 27/38,
CMUdict+homograph-list 14/38, espeak-ng itself 11/38. The permissive front end already beats
what both models were trained on.

Hard-won facts phase 1 must not rediscover:

- **misaki with no fallback silently DELETES OOV words** (`KPipeline` passes `unk=''`;
  Kokoro's `KModel.forward` filters None phonemes; KittenTTS's `TextCleaner` drops unknown
  symbols). 56/2,171 word tokens vanished from audio; +1.3-2.0pp WER, all in the OOV bucket.
  A fallback chain is mandatory, and a mispronounced word beats a missing one on every model.
- **CMUdict alone cannot reach `ɐ` and `ᵻ`** (each in ~1/3 of sentences) and loses UTMOS on
  both Kittens - misaki's lexicon is the base, CMUdict is only the fallback.
- **The POS tagger is load-bearing**: without it heteronyms fall 71% -> 5% and the article
  "a" is read as the letter name (misaki's DT rule stops firing).
- **Stress goes immediately before the stressed vowel** in both espeak and misaki notation,
  and CMUdict marks stress on the vowel digit - so NO syllabifier is needed anywhere.
- **Kokoro's decoder is stochastic**: `torch.rand` initial sine phase + `torch.randn_like`
  noise excitation (`kokoro/istftnet.py:150,205,253`), ~9% of signal RMS run to run.
- **Style vector indexing differs**: KittenTTS indexes its per-voice style by the length of
  the NORMALIZED TEXT; Kokoro indexes its 510-row voice pack by PHONEME COUNT
  (`pack[len(ps)-1]`).
- **kitten-*-0.8 checkpoints carry per-voice `speed_priors`** in `config.json`
  (0.8 for `expr-voice-5-m`); omitting them puts the model outside its shipped operating
  point.
- Calibrated symbol maps (settled by hill-climbing NED on 4,000 held-out words; tables in
  `~/Work/tts-ab/g2p/results/calibration.json`):
  ARPAbet->espeak-IPA word NED 0.0890; ARPAbet->misaki 0.0705; misaki->espeak-IPA 0.0941
  (`ɜɹ->ɜːɹ, ɑ->ɑː, ɔ->ɔː, u->uː, əɹ->ɚ` + flap). The misaki->espeak floor 0.0941 is pure
  notation - agreement below it measures spelling, not pronunciation.
- Reference model revisions (pin these in the converters; all three sit in the g2p
  experiment's hub cache `~/Work/tts-ab/g2p/.hf/hub/`, Kokoro with its full 54-voice roster):
  `KittenML/kitten-tts-nano-0.8` @ `7a1db645b1f3ab9420761d87428e042b9cec3f26`,
  `KittenML/kitten-tts-mini-0.8` @ `c02725660cea441db4c383af69f1f26f5cd00947`,
  `hexgrad/Kokoro-82M` @ `f3ff3571791e39611d31c381e3a41a3af07b4987`.
- Kokoro's `config.json` vocabulary includes misaki's invented symbols `A I O Q W Y ᵊ T` -
  proof its training labels were misaki output; arm D (misaki+espeak-fallback) is Kokoro's
  reference, arm A (raw espeak via phonemizer, en-us, with_stress) is KittenTTS's.

## File charters (mirror of ARCHITECTURE_MEDIA.md sec 1.7)

| file | owns |
|---|---|
| `dasllama/dasllama_tts_types.das` | the floor: `TtsCaps` (voices, sample rate, langs, cloning=no), voice descriptors, synth request/result types. Family files require this, never each other. |
| `dasllama/dasllama_tts_blocks.das` | the StyleTTS2-lineage block home (tower analogue): AdaIN1d, AdaLayerNorm, LSTM/BiLSTM wrappers over the VAD cell, Conv1d + weight-norm-folded readers, ConvTranspose1d, harmonic source (sine gen + noise), ISTFT/overlap-add, snake/leaky/tanh activations, the prosody predictor shapes, the seeded-RNG rail. One home: a family file re-implementing one of these is a defect. |
| `dasllama/dasllama_tts.das` | the facade: `load_tts_model(path)`, `caps`, `create_session`, `synthesize(m, s, text, voice, speed) : array<float>` (+ sentence-chunked streaming form), model-kind dispatch by GGUF metadata. Returns f32 PCM + rate; requires NO `audio` module - the REVIEW_PLACEMENT.md rule that no engine file but `dasllama_audio_io` requires `audio` stands; file output is the caller's job through `dasllama_audio_io`. |
| `dasllama/dasllama_kitten.das` | KittenTTS family: config, weight map (ONNX-derived names), assembly, `speed_priors`, style-by-text-length, the calibrated misaki->espeak-IPA symbol map + KittenTTS `TextCleaner` token table (~180 symbols). Serves nano and mini from one code path. |
| `dasllama/dasllama_kokoro.das` | Kokoro family: config, weight map, assembly, voice packs (510 x style rows per voice), style-by-phoneme-count, deterministic noise (own RNG, seed in the session; oracle mode reads captured noise). |
| `dasllama/dasllama_textnorm.das` | text normalization: numbers, ordinals, years/decades, currency, fractions, units, abbreviations, clock times. Ported from KittenTTS's Apache `normalize_text` WITH its six defects fixed (below). No `num2words` (LGPL) anywhere. |
| `dasllama/dasllama_postag.das` | averaged-perceptron PTB tagger: greedy left-to-right, features = word/prefix/suffix/shape + two previous tags + neighbors; weights from `harness/train_postag.py` on UD English-EWT `xpos` (PTB tagset = exactly what misaki's POS-conditioned entries key on). Includes the tokenizer (punctuation/contraction splitting) since misaki derives punctuation phonemes from PTB punctuation tags. |
| `dasllama/dasllama_g2p.das` | the misaki port: gold+silver lexicon lookup (POS-conditioned entries first), function-word rules (a/an/the/to weak forms, plural/verb -s, 's), fallback chain lexicon -> CMUdict -> GRU seq2seq, stress placement. Emits misaki-inventory phoneme strings; per-model symbol mapping lives in the family file. |

Registration: new `.das` files under `dasllama/` follow the existing module rows
(`.das_module` + CMakeLists - memory: `dasllama_common_split_rules.md`). ARCHITECTURE_MEDIA.md
gains a 1.7c TTS section with these charters in the same PR that creates the files.

## Data, converters, formats

`harness/` scripts (Silero-converter pattern: pinned revision in, deterministic file out):

- `harness/convert_kitten.py` - ONNX initializers -> GGUF f32/f16. Fold weight-norm
  (`weight_g`/`weight_v` -> one tensor; the g2p report verified folding bit-exact on Kokoro,
  379dB). Embed the 8 voices from `voices.npz` as `voice.<name>` tensors and `speed_priors`
  as metadata. One GGUF per size: `kitten-nano.gguf`, `kitten-mini.gguf`.
- `harness/convert_kokoro.py` - safetensors + per-voice pack `.pt` -> `kokoro-82m.gguf`;
  weight-norm folded; voices as `voice.<name>` [510, style_dim] tensors. The converter takes
  the full roster (all 54 voice `.pt` files at the pinned revision are downloaded); the first
  shipped GGUF embeds a curated subset - af_heart + ~8 spanning gender/accent - by a flag,
  and the full-roster GGUF is the same command without it.
- `harness/build_g2p_data.py` - misaki `us_gold.json` (90,201 entries, 790 POS-conditioned)
  + `us_silver.json` (93,361) + CMUdict 0.7a (123,455 words / 133,737 prons, via NLTK
  distribution) + the calibrated ARPAbet->misaki map -> one packed `tts_g2p.bin`: sorted
  string table + offsets (binary search or open-addressed hash at load; NOT JSON parsing at
  startup - 7MB of JSON is a startup-time defect). Also packs the g2p_en GRU (3.34MB f32)
  with its ARPAbet output mapped through the same table.
- `harness/train_postag.py` - UD English-EWT train split, `xpos` column -> averaged
  perceptron weights, pruned (drop features that never discriminate on the decisions the
  lexicon actually consults; unpruned NLTK-style is 5.7MB, target <= 3MB) -> `tts_postag.bin`.
  Emits test-split accuracy into the file header; the das loader asserts it.
- `harness/capture_kokoro_noise.py` - runs the PyTorch reference once per parity sentence
  with `torch.rand`/`torch.randn_like` intercepted, dumps the draws -> `tests` fixture, so
  das-vs-torch parity is provable despite the stochastic decoder.

Nothing generated lands in git (Boris, 2026-09-02): the three GGUFs, `tts_g2p.bin` and
`tts_postag.bin` all go to the model store beside the other dasLLAMA models
(`~/Work/llama.cpp/models`, resolved the way the ASR tests resolve theirs), each with its
license sidecar; the repo carries only converters, sources of truth for the data, and tests: kitten (Apache-2.0), Kokoro (Apache-2.0), misaki lexicons (Apache-2.0), CMUdict
(BSD-2, CMU copyright line), g2p_en GRU (Apache-2.0), UD-EWT-trained tagger (weights ours;
CC BY-SA attribution for the training data in THIRD_PARTY_NOTICES.md). Update
`THIRD_PARTY_NOTICES.md` in the first PR that ships any of these.

## Licensing posture (explicit, because it was the whole point)

- Shipped: everything Apache-2.0 / BSD-2 / MIT. No espeak-ng, no `phonemizer`, no
  `espeakng-loader`, no `num2words` (LGPL) at build or runtime.
- Dev-time oracles (never shipped): espeak-ng + phonemizer (arm A reference), PyTorch
  reference implementations, onnxruntime, NeMo Parakeet (WER scoring), UTMOS.
- **Silver-lexicon provenance - accepted knowingly (Boris, 2026-08-30):** misaki's
  `us_silver` (93,361 entries, resolved 7.4% of corpus tokens) reproduces from espeak-ng
  output character-for-character in 91% of sampled entries (97% within 0.15 NED). The file
  itself is Apache-2.0 and is data; we ship it as misaki ships it. The gold tier is NOT
  espeak-derived (33% match). If this call is ever revisited, the fallback chain already
  covers silver's role at a measured cost (OOV WER rises toward the arm-B numbers).

## Kernel inventory

Existing (cite, don't rewrite):

- Conv1d + bias, kernel-3 style loops: `dasllama_vad.das` (SV_C1..C4), Parakeet conformer
  convs (`dasllama_parakeet.das`); depthwise variants in the ASR path.
- LSTM cell: `dasllama_vad.das` (`lstm_ih/hh/b`, h/c state) - BiLSTM = two passes + concat.
- GEMM/GEMV: `dasllama_math*` via the tower wrappers; LayerNorm rows: `dasllama_tower.das`.
- STFT-as-matmul precedent: VAD's `stft` basis ([258][256]) - ISTFT gets the transposed
  treatment (basis matmul + overlap-add); dasMinfft exists if a real FFT ever wins.
- Sentence chunking, UTF-8 walking: `daslib` string surface (byte-view rules apply - see
  `skills/daslang/references/strings.md`; PERF031).

New in `dasllama_tts_blocks.das`:

- ConvTranspose1d (stride-2/­stride-N upsample; weight-norm folded at convert).
- AdaIN1d / AdaLayerNorm (style-conditioned scale+shift - trivial given norm rows).
- Harmonic source module (f0 -> sine harmonics + noise mix; Kokoro istftnet + Kitten twin).
- ISTFT/overlap-add with window compensation.
- Snake / LeakyReLU / tanh as needed by the exact graphs (enumerate from the ONNX dump and
  the Kokoro module list in phase 2/3 step 0 - do not assume).
- Seeded RNG (xorshift/PCG) for the noise rail: session-seeded at runtime, fixture-fed in
  oracle mode.

Perf posture: JIT always (`-jit`, memory: `feedback_dasllama_always_jit.md`), AOT
compile-gate only. CPU first; these are 15-82M conv/LSTM nets - real-time is not in doubt,
so no [tune]/Metal work in the initial arcs (Metal is a later, measured decision; the codec
A/B showed PyTorch-CPU RTF ~0.1 for 114M - ours must simply not embarrass that class).

## Phases

The arc is one PR (`bbatkin/dasllama-tts`). Each phase is a checkpoint inside it with its own
oracle and exit gate; a phase commits at its gate, never before. Tests live in the module's
test dir (memory: `feedback_test_dir_per_module.md`); every claim below that can regress
gets a dastest that fails without the code (branch-test rule).

### Phase 1 - the front end (textnorm + postag + g2p)

Steps:
1. `dasllama_textnorm.das` - port KittenTTS `normalize_text` semantics; fix the six defects
   as failing-first tests: (a) crash on `etc.,` (`int('')` on a bare-comma number match);
   (b) `3:12:45` h:mm:ss + missing space (`secondslast`); (c) fractions defined-not-wired
   (`1/2 cup` -> "one two cup"); (d) units defined-not-wired (`kg/GB/ms/GHz` left as
   letters); (e) `°C` stripped; (f) decades (`1960s` -> "nineteen sixtys"). Idempotence
   test over the 200-sentence corpus.
2. `dasllama_postag.das` - perceptron + tokenizer; gate: UD-EWT test-split tag accuracy
   >= 96.5% AND the g2p heteronym gate below (the tagger only matters through it).
3. `dasllama_g2p.das` + `harness/build_g2p_data.py` - lexicon, rules, fallback chain,
   stress. Port misaki's `en.py` rules (0.03MB of Python - small); function words; the
   letter-name rule for single letters/acronyms.
4. Per-model mapping tables (misaki->espeak-IPA for Kitten; identity for Kokoro) as data in
   the family files (phase 2/3 consume them; land the tables now, tested against
   calibration.json).

Oracle: the python arm E in `~/Work/tts-ab/g2p/scripts/` (phonemize.py) - regenerate its
phoneme JSON, compare token streams.

Exit gates:
- Token-identical with python arm E on >= 99% of the 200-sentence set AND a fresh
  2,000-sentence sweep (mine LJSpeech + Harvard lists 7+; the 1% budget is for
  tokenizer-boundary ties, each diff hand-classified).
- Heteronym set: >= 27/38 (parity with misaki+spaCy; espeak is 11/38).
- The six textnorm defect tests green; no `num2words`, no espeak symbol in any error path.
- Startup: g2p data load < 50ms on the M1 (packed binary, no JSON).

### Phase 2 - KittenTTS family (first bring-up: deterministic ONNX, smaller graph)

Steps:
0. Dump the ONNX graphs (nano + mini) - enumerate every op, shape, and initializer; write
   the block list into the PR (this is the real op inventory; the plan's list above is the
   expectation, the dump is the truth).
1. `harness/convert_kitten.py` -> GGUFs; loader in `dasllama_kitten.das` reading through
   `parse_gguf_meta` (bypass `resolve_arch` - TTS kinds get their own dispatch in the
   facade, precedent: the NanoCodec port study found `general.architecture` panics
   otherwise).
2. `dasllama_tts_blocks.das` - implement the dumped op set; per-stage activation-dump rail
   (mirror the VAD bring-up).
3. Assembly + `dasllama_tts_types.das` + minimal facade (`load`/`synthesize`, no streaming
   yet).

Oracle: onnxruntime on the same GGUF-source checkpoints; per-stage dumps compared stage by
stage, then end-to-end.

Exit gates:
- End-to-end waveform max-abs-diff <= 1e-4 vs onnxruntime on 20 sentences x 2 voices x both
  sizes (fp32), including `speed_priors` applied.
- The 200-sentence rig through das-Kitten-nano: WER within 0.3pp and UTMOS within 0.02 of
  the python arm-E numbers (4.22% / 4.035) - proves front end + engine compose.
- dastest suite: per-block unit tests against fixture tensors + one end-to-end golden.
- JIT RTF < 0.5 nano, < 1.0 mini on the M1 (generous first gate; tighten later).

### Phase 3 - Kokoro family

Steps:
0. Enumerate the module list from the safetensors + `kokoro` package source (text encoder,
   predictor, decoder istftnet); confirm against the block home; add only what's missing.
1. `harness/convert_kokoro.py` + `harness/capture_kokoro_noise.py`.
2. `dasllama_kokoro.das` - assembly, voice packs, style-by-phoneme-count, noise rail.

Oracle: PyTorch reference with captured noise (bit-comparable); free-running UTMOS band.

Exit gates:
- Captured-noise parity <= 1e-4 end-to-end on 20 sentences x 3 voices.
- Free-running (own RNG): UTMOS on the 200-set within Kokoro's own run-to-run band
  (+-0.01 of 4.499) and WER within 0.3pp of 3.18%.
- Voice pack correctness: per-voice golden fixtures (style row selection is the easy bug:
  off-by-one on phoneme count).

### Phase 4 - facade, product surface, rig

1. Facade completion: sentence-chunked streaming (chunk by textnorm sentence boundaries,
   <= 400 chars per chunk - upstream Kitten convention), voice/model listing, session reuse.
2. `dasllama_audio_io.das` gains the PCM->WAV write helper (it is the one file allowed to
   require `audio` - that REVIEW_PLACEMENT.md rule is untouched).
3. `utils/dasllama-server/openai_server.das`: `/v1/audio/speech` (closes the
   `followup_general.md` TTS capability row) - model+voice+speed+format; wav/pcm first, no
   mp3 (decline politely).
4. CLI verb on the dasllama tool (`tts "text" --model --voice --out`), tutorial-free for
   now.
5. **The rig**: port the g2p experiment's scoring loop into a repeatable harness
   (`harness/tts_rig.py` driving the das binary + NeMo-Parakeet WER + UTMOS, both dev-time)
   over `data/sentences.json`. This is the number-maker for phase 5 and the regression net
   for everything.

Exit gates: rig runs one command -> table; the server endpoint passes a curl-level dastest
(dashv skill applies); REVIEW.das/lint/format green.

### Phase 5 - the surpass program (open-ended, rig-gated)

Standing rule: no lexicon/rule change lands without a rig delta attached.

Targets (all measured against the espeak-reference columns of the 2026-08-29 tables):
- Heteronyms: > 27/38 (mine the 11 exactly-one misses; each is a rules/entry candidate).
- OOV-category WER at or below espeak's own: close nano E 9.34% -> A 8.56%, kokoro E
  12.06% -> D 10.90%. Seed material: the experiment's 52 dropped words + its worst-15
  tables (Kierkegaard/Saoirse/Nguyen class) become minted gold entries in
  `harness/build_g2p_data.py`'s local-additions file.
- Overall WER <= the espeak reference on all three models (mini already ties at p=0.79).
- Textnorm strictly better than both upstream normalizers (the six fixes already clear
  espeak's own number handling - keep adding failing-first cases from rig transcripts).
- Letter/acronym handling: single letters, ALL-CAPS runs, mixed alphanumerics (GHz case).

Backlog (each its own decision): proper-noun pack from Wiktionary IPA (CC BY-SA -
attribution+share-alike on the data file; separate ruling before use), homograph growth
past misaki's 790, UK dialect (gb tiers exist in misaki), per-language expansion (Kokoro
has non-English voices - out of scope until English is surpassed).

## Risks

- ConvTranspose1d and ISTFT are genuinely new kernels - budget bring-up time; the
  activation-dump rail is the debugging tool, not print.
- LSTM throughput in JIT at 82M scale: Parakeet precedent says fine; verify with the RTF
  gate before polishing.
- The ONNX dump may surface ops this plan did not list (e.g. GRU instead of LSTM somewhere,
  or attention in Kitten's text encoder) - phase 2 step 0 exists precisely to re-plan cheap.
- Kokoro parity is only provable under captured noise; free-running quality is gated by
  band, not bits - resist chasing bit-parity there.
- Lexicon memory/startup: packed format from day one; the 50ms gate is the tripwire.
- Style-index off-by-ones (text-length vs phoneme-count) produce plausible-but-wrong
  prosody - golden fixtures per model, not ear-checks.
- tests/mcp master red (memory: `mcp_tests_master_red.md`) is pre-existing - don't let TTS
  PRs adopt it.

## Open questions (settle at arc start, none block phase 1)

- Voice roster for Kokoro's first GGUF (curated ~8 vs all).
- the server's `/v1/audio/speech` in phase 4 vs its own arc (Boris leaned "phase 4 closes the
  capability row"; confirm at the time).
- Model file hosting/distribution beyond the local store (same answer as the other dasLLAMA
  models when they get one).
- Whether `dasllama_postag`/`dasllama_textnorm` should eventually promote to daslib (they
  are English-text utilities with no model dependency) - start module-local, promote only
  on a second consumer.
