# dasLLAMA tutorials arc — round 2 (branch bbatkin/dasllama-tutorial-cleanup)

Continues the committed resync + tutorials 10/11 + facade `parse_calls` + server `marker_parse`
dedup. Six items, each greenlit in discussion 2026-08-11. Series design rule going forward:
the page is the precise, verified source for agent re-narrators — machine-legible consistency
over narration-mode tuning; genuine precision defects are bugs, individual readers' preferred
narration order is not.

## 1. Tutorial 00 — revision round 5 (RST only; the .das is untouched)

From two readers' feedback (chat + v4 rewrite ledgered in the session). All edits keep the
page's voice, structure, and section order — поправить, not переписать.

Reader-1 items (navigation aids):

- **1a** `One token through the model`: elided `forward` skeleton code block right after "The
  heart of the file is one function" — full signature, `...` elisions, one line per step with
  terse trailing comments (RMSNorm / position / append K,V / score, softmax, blend /
  residual #1 / SwiGLU / residual #2 / final norm + classifier). `.. das-doc: fragment`.
- **1b** `Logits`: inline the 8-line `most_likely` at its mention.
- **1c** KV-cache paragraph: 1–2 sentences on `kv_heads` / `kv_multiple` — larger models share
  one cached K/V head among several query heads; stories15M has `kv_heads == heads` so the
  divisor is 1. The code-alongside reader stalls exactly at that line otherwise.
- **1d** `The transformer at a glance`: config-at-a-glance table — dim 288 (width), layers 6
  (depth), heads 6, head_size 48 = dim/heads, hidden 768 (~8/3·dim), vocab 32,000, seq_len
  (read the true value from the checkpoint header at execution). Each row names the
  size-independent idea; the table is the "this model's settings" anchor.
- **1e** `Where the weights come from`: +2 paragraphs — the memory story. ~60 MB of floats are
  never copied: the OS lends the file's bytes, typed views lend windows into them, matmul
  reads straight off the disk cache; the only per-token copy is one 288-float row; each view
  lives only inside its block, so nothing dangles over the borrow. Discipline: a sentence
  about daslang rather than about the bytes gets cut (no `$()`/`#` teaching). Plants the mmap
  seed tutorial 05's prepared-image table pays off.

Reader-2-derived items (mode-independent precision defects):

- **1f** Embedding join flip: introduce the learned table BEFORE naming the embedding — "the
  checkpoint holds a learned table: one row per vocabulary token, 288 floats each; a token's
  row is its embedding". Kills the "32,000-row table appears mid-sentence" stall that started
  the reader's whole cascade.
- **1g** Grid-two-ways thread, stated backward (no forward promises): at the classifier, the
  payoff — the embedding table was a grid we *indexed into*; the classifier uses a grid the
  other way, *multiplied by* — and for stories15M it is the same grid, indexed at the start,
  multiplied at the end.
- **1h** "vocabulary row" tie: at both classifier mentions, first use reads "every row of the
  embedding table — one per vocabulary token". The compound was never earned.
- **1i** "memory" de-collision: the KV cache owns the word. Residual stream loses "working
  memory" (pick a non-colliding role word at execution); prefill's "builds up its memory of
  them" is reworded to the record language of 1j.
- **1j** Record-vs-prediction paragraph at the prefill teaching point: one pass produces two
  outputs about two different tokens — the deposit into the cache (what this token is — kept)
  and the logits (what follows — discarded during prefill, wanted during generation).
  Dissolves "why run tokens we already know".
- **1k** Settings-vs-laws sentence at the FIRST bare structural number: a setting of this
  checkpoint, not a law; real models turn the same dials higher. The shamans paragraph stays
  where it is.

Verify: doc-verify page green, Sphinx zero warnings, term audit over the changed spans (one
word = one object; no term before its referent).

## 2. Scheduler promotion

`utils/dasllama-server/llm_scheduler.das` → `modules/dasLLAMA/dasllama/dasllama_scheduler.das`
(module renamed `dasllama_scheduler`, stays facade-only). Server switches to
`require dasllama/dasllama_scheduler`. `test_llm_scheduler.das` →
`modules/dasLLAMA/tests/test_scheduler.das`. Register the module in BOTH `.das_module` and the
CMake file lists. Not folded into the `dasllama` umbrella — layer 3 stays an explicit require.

Verify: moved test green, `test_openai_server` green, server compiles, no `llm_scheduler`
references remain outside history.

## 3. Tutorial 12 — voice activity (Silero VAD)

`12_voice_activity.das` + RST + toctree + nav (11 gains Next → 12). Zero-download: weights are
checked in; the default run SYNTHESIZES a test signal (tone/noise/silence spans) so the
tutorial runs with no arguments; a WAV argument replaces it. Sections: (1) `vad_probs` speech
curve; (2) `vad_segments` + `VadOpts` hysteresis (thr .5 / neg .35 / min-speech 250 ms);
(3) streaming `vad_chunk_prob`, the live-mic shape; (4) VAD-gated transcription — runs only
when an ASR model path is given, skips politely otherwise.

Verify: live no-arg run + WAV run; lint/format; RST gates.

## 4. Tutorial 13 — serving (the capstone)

`13_serving.das` + RST + toctree + nav (12 → 13; 13 ends the series). Section 1 shows the raw
mechanism once — paged pool (`create_kv_pool`, page_rows=16 so a modest shared prefix donates
full pages), renderer-built prompts, ONE hand-rolled `eval_batch` step. Sections 2+ drive the
real `dasllama_scheduler`: `create_scheduler` / `submit` / `scheduler_step` / events; the
batched-vs-sequential wall-clock A/B for 4 conversations; prefix cache working underneath;
the stateless replay seam in ~10 lines; closes with "same module, more streams" →
`utils/dasllama-server`. Teaching model SmolLM2-135M.

Verify: live run with honest printed numbers; lint/format; RST gates.

## 5. Tutorial 08 — the splice section

One "under the hood" section: the same audio turn done directly — tower soft tokens +
`generate_embd` (~15 lines), closing the facade docstring's "use it directly for custom
multimodal prompts" pointer. RST section to match. Verify: compile gate; live run if the
omni mmproj + a WAV are on disk, otherwise noted honestly.

## 6. Program-root gate

New `modules/dasLLAMA/tests/test_program_roots.das` (line-based, `test_facade_docs` style)
over `tutorials/dasLLAMA/*.das`, `examples/dasLLAMA/*.das`, `utils/dasllama-server/*.das`:

- a file that requires `dasllama/` and exports `main` must carry `options stack = 524288`;
- a file that additionally calls `load_model(` / `load_asr_model(` / `load_audio_tower(`
  must declare prefill intent: `allow_cpu_prefill(` or `set_metal_mode(`.

Tutorial 00 is exempt automatically (no dasllama require). Verify: gate green on the tree;
negative control — strip one line in a scratch copy, gate must go red.

## Wrap

Format + lint every touched file; doc-verify the dasLLAMA page family; Sphinx clean; suites
(tool_formats, facade_docs, think_split, scheduler, server) green; commits at stopping
points. Preflight and PR are a separate ask.
