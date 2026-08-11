# Thinking-model support across architectures

How reasoning ("thinking") models work through the chat layer and the OpenAI server: the
per-family mechanisms, the reply-side matcher, and the test map. Tool calling shares the reply
pipeline (reasoning splits first, calls parse from the content half), so it is covered here too.

## The mechanisms

All per-family knowledge is ChatTemplate data declared in `dasllama_arch_*.das` /
`dasllama_common.das` (`chatml_chat`, `hermes_tools`); the engine in `dasllama_chat.das` is
family-blind.

**Prompt side:**

1. **`think_default`** — the family's own thinking default. Qwen/GLM/gpt-oss think unless told
   otherwise; gemma-4 E-series is instruct unless asked. `create_chat_renderer_` seeds the
   session's `enable_thinking` from it; the server's `enable_thinking` request field (either
   spelling: top-level or `chat_template_kwargs`) is tri-state — ABSENT leaves the family
   default in force, a present bool overrides.
2. **`think_suppress`** (thinking → off, Qwen-shaped): prefills an empty, closed thought block
   on the generation prompt (Qwen3's `enable_thinking=false`). Renders only when thinking is
   off AND the vocab has the specials.
3. **`think_gate` + `assistant_open_think`** (off → thinking, gemma-4-shaped): the gate
   (`<|think|>`) opens the system turn — rendered even when no system prompt is set — and the
   generation prompt switches to the bare `assistant_open_think` header so the model emits its
   own thought channel. gemma-4's default `assistant_open` keeps the closed empty
   `<|channel>thought\n<channel|>` prefill, so thinking-off renders the exact pre-arc tokens.
4. **`stop_nothink`** — extra stop tokens in force whenever the next turn is NOT a thinking
   turn, merged by `effective_stop_ids`. gemma-4 lists its channel markers: an instruct-mode
   E-series model rambles past its answer through a stray `<channel|>` (observed live:
   `…4.<channel|>4`), and in a non-thinking turn a channel marker is always framing noise — the
   turn is over. Anything that generates from a ChatSession reads `effective_stop_ids(chat)`,
   never `chat.stop_ids`.
5. **Arming is vocab-gated and gate-aware** (`think_turn_active`): the reply matcher, the
   alternate opener, and the stop merge all key on one predicate — toggle on, the reply markers
   resolve in the vocab (an inert declaration like Qwen2.5's shared ChatML template never arms),
   and a gate family's gate rendered on a consumed turn or renders this turn. A mid-conversation
   `set_thinking(true)` on gemma-4 therefore stays instruct-shaped (the gate cannot enter an
   already-rendered context) with the framing stops still armed.

**Reply side — the per-family matcher** (`think_mode` + `think_open`/`think_close`):

- **symmetric** (Qwen, GLM): `<think>` … `</think>`.
- **channel_switch** (gpt-oss Harmony): `<|channel|>analysis<|message|>` … reasoning … `<|end|>`
  (repeatable; `commentary` counts as reasoning) → `<|channel|>final<|message|>` … content. The
  Harmony markers are the mode's own grammar, not per-arch data.
- **asymmetric** (gemma-4): `<|channel>thought` (trailing `\n` optional) … `<channel|>` …
  content. Asymmetric brackets are the family design.
- **truncated tail**: reasoning that never closed (budget cut) classifies as reasoning.

The engine is `split_reasoning_` (whole reply) and the incremental
`make_think_stream_`/`think_feed_`/`think_finish_` (streaming; holds partial markers back across
SSE chunk boundaries). `respond_` stores history reasoning-stripped through it; `strip_think`
remains as the legacy symmetric-only helper.

**Server surface** (`utils/dasllama-server/openai_server.das`): the reasoning span rides
`reasoning_content` — on the chat-completion message, and as streaming deltas (the
DeepSeek/llama.cpp framing). Tool-capable replies split reasoning FIRST, then
`parse_tool_calls_auto` runs on the content half, so a thinking model that calls tools yields
`reasoning_content` AND `tool_calls` in one response.

## Per-family status

| family | thinking default | reply mode | tools | smallest local model |
|---|---|---|---|---|
| Qwen 2.5 (qwen2, qwen2moe) | n/a (no think vocab) | symmetric (inert) | Hermes | Qwen2.5-0.5B |
| Qwen 3 / 3.5 / 3.6 (qwen3, qwen3moe, qwen35, qwen35moe, qwen3next) | on | symmetric | Hermes | Qwen3-0.6B / Qwen3.5-0.8B |
| GLM-4 MoE (glm4moe) | on | symmetric | none declared | **none local — zen2 leg pending, below** |
| gpt-oss | on | channel_switch | none declared | gpt-oss-20b (11 GB, large-tier) |
| gemma-4 E-series | **off** (`<|think|>` gate) | asymmetric | none declared | gemma-4-E2B |
| llama, phi3, gemma2/3, mistral3 | none | none | none declared | — |

Tool formats for gpt-oss (`<|call|>` envelopes), gemma-4 (`<|tool_call>`), GLM, llama-3, and
mistral-3 are NOT declared — the server answers `tools` on those families with an honest 400.
Declaring one is follow-up work and takes the test obligations below with it.

## Test map

- `modules/dasLLAMA/tests/test_think_split.das` — model-free: every family's exact wire shape
  through the matcher, whole-string and per-chunk down to 1 byte.
- `modules/dasLLAMA/tests/test_chat.das` — render side: gemma-4 default/thinking-ON prefills
  token-for-token (`test_chat_gemma4_thinking`), Qwen think-suppress, Hermes tool blocks
  (Qwen2.5 token-for-token, Qwen3.5 marker ids).
- `utils/dasllama-server/test_openai_server_think.das` — live legs, model-gated: Qwen3-0.6B
  (reasoning_content non-streaming + streaming order + tools-with-thinking compose),
  gemma-4-E2B (off-default / thinking-ON pair), gpt-oss-20b (Harmony split;
  `DASLLAMA_PARITY_FULL=1`).

CODEREVIEW.md binds new families to this map: declaring `think_mode` or `tool_call_open` ships
the wire-shape case and the live leg in the same change.

## GLM-4 — the pending zen2 leg

No small glm4moe model exists (the family starts at GLM-4.5-Air, ~106 B), so GLM has no local
live leg here. The symmetric matcher it shares with Qwen is fully covered model-free. What
remains is the live proof on the zen2 box (which stocks a GLM-4.5 gguf): run
`test_openai_server_think.das`'s qwen3 arm pattern against the GLM model — default thinking
produces `reasoning_content`, `enable_thinking=false` suppresses via its declared
`think_suppress`, content clean of `<think>`. When that runs, record the result here; if GLM
tool calling is wanted, its format declaration comes first and brings the test obligations
with it.
