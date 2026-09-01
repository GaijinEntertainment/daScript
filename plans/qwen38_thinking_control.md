# How much Qwen 3.8 thinks, and who controls it

Research memo. Written 2026-09-01. Read-only research; nothing in any checkout was modified.

Evidence base:
- Qwen's own files, downloaded fresh from Hugging Face into `/tmp` during this session:
  `Qwen/Qwen3.8-27B` `chat_template.jinja`, `README.md`, `tokenizer_config.json`,
  `generation_config.json`, and `Qwen/Qwen3.8-Flash-Next` `chat_template.jinja`, `README.md`.
- The local llama.cpp checkout `D:\Work\llama.cpp`, at HEAD `6c84c7d5d` ("model: add
  Qwen3.8-Flash-Next (qwen4exp) (#27742)"). All `llama.cpp` line numbers below are that commit.
- The local daslang checkout `D:\Work\daScript` (module `modules/dasLLAMA`, server
  `utils/dasllama-server`).
- Public docs and papers, cited inline.

---

## Summary in one paragraph

Qwen 3.8 has exactly one first-party knob for the *amount* of thinking, and it is a prompt
knob, not a budget: `reasoning_effort`, with three legal values `xhigh` (the default), `medium`,
and `low`. The chat template turns that value into one English sentence which it prepends to
the system turn; the model was trained to respond to that sentence. There is a second knob,
`enable_thinking`, but that is on/off, not an amount. Qwen ships **no** token budget, **no**
`thinking_budget` template variable, **no** budget-forcing text, and **no** `/think` `/no_think`
soft switch in the 3.8 template. Everything that actually *caps* the thinking length is bolted
on by the serving stack: llama.cpp has a full reasoning-budget sampler, vLLM has
`thinking_token_budget`, SGLang has a `thinking_budget` that is reported broken. Both
Qwen3.8-27B and Qwen3.8-Flash-Next (`qwen4exp`) ship a byte-identical chat template, so
everything here applies to both without change.

---

## (a) The mechanisms that actually exist

### A1. `enable_thinking` — a trained-in on/off switch, not an amount

Template spelling (`Qwen/Qwen3.8-27B/chat_template.jinja`, lines 163-170):

```jinja
{%- if add_generation_prompt %}
    {{- '<|im_start|>assistant\n' }}
    {%- if enable_thinking is defined and enable_thinking is false %}
        {{- '<think>\n\n</think>\n\n' }}
    {%- else %}
        {{- '<think>\n' }}
    {%- endif %}
{%- endif %}
```

Two things to notice.

1. With thinking **on** (the default), the template itself writes the opening `<think>\n` into
   the generation prompt. The model never emits `<think>`; it starts already inside the thought
   block and only has to emit `</think>` to leave it. This matters for any budget mechanism —
   see A6 and B3.
2. With thinking **off**, the template writes an already-closed empty block
   `<think>\n\n</think>\n\n`. This is the same "prefill an empty closed block" trick Qwen3 used.

API spelling, per the model card (`README.md` lines 296, 459):

```python
extra_body={"chat_template_kwargs": {"enable_thinking": False}}
```

The card adds (line 466): on Qwen Cloud the field is top-level `"enable_thinking": False`, not
wrapped in `chat_template_kwargs`.

Source: <https://huggingface.co/Qwen/Qwen3.8-27B> and the raw template at
<https://huggingface.co/Qwen/Qwen3.8-27B/raw/main/chat_template.jinja>.

### A2. `reasoning_effort` — the amount knob. It is a system-prompt sentence.

This is the only first-party control over *how much* the model thinks. Template lines 45-56,
verbatim:

```jinja
{%- set reasoning_instructions = '' %}
{%- if enable_thinking is undefined or enable_thinking is true %}
    {%- set resolved_reasoning_effort = reasoning_effort|default('xhigh') %}
    {%- if resolved_reasoning_effort not in ('xhigh', 'medium', 'low') %}
        {{- raise_exception('Unexpected reasoning effort ' ~ reasoning_effort ~ '. Supported types are xhigh (default), medium, and low.') }}
    {%- endif %}
    {%- if resolved_reasoning_effort == 'xhigh' %}
        {%- set reasoning_instructions = 'Reasoning effort is set to xhigh. Please think carefully through the task, validate key assumptions, consider plausible alternatives, and prioritize correctness, consistency, and clarity in the final answer.' %}
    {%- elif resolved_reasoning_effort == 'low' %}
        {%- set reasoning_instructions = 'Reasoning effort is set to low. Keep your thinking brief and focused, moving directly to the conclusion without unnecessary elaboration.' %}
    {%- endif %}
{%- endif %}
```

Five facts follow directly from that code, and they are the most useful things in this memo:

1. **The whole mechanism is one sentence of English prepended to the system turn.** There is no
   special token, no logits change, no counter. The `reasoning_instructions` string is spliced
   in ahead of the user's system content (template lines 58-86) — and if the request carries no
   system message at all, the template *creates* a system turn holding only that sentence
   (lines 81-85).
2. **`medium` injects nothing.** Only `xhigh` and `low` set the string; `medium` leaves it
   empty. So a request with `reasoning_effort="medium"` renders to a prompt that is
   byte-identical to a request with thinking on and no effort field. `medium` is the neutral
   baseline; `xhigh` and `low` are the two deviations from it.
3. **The default is the expensive end.** `reasoning_effort|default('xhigh')` — omitting the
   field gets you the "think carefully, validate assumptions, consider alternatives" sentence.
4. **Only three values are legal.** `low`, `medium`, `xhigh`. Anything else — including
   OpenAI's own standard `"high"` — makes the template raise, and the request fails. This is a
   sharp trap for an OpenAI-compatible front end, because `high` is the value most clients send.
5. Because it is only a prompt sentence, it is **advisory**. The model was post-trained to obey
   it, but nothing enforces it, and there is no upper bound on the trace length at any level.

Model card wording (`README.md` lines 258-262):

> Qwen3.8 comes with official support for `reasoning_effort`, which can be used to adjust
> reasoning depth and control cost:
> - `xhigh` (default): for complex tasks demanding thorough analysis
> - `medium`: balancing accuracy and speed
> - `low`: efficient reasoning optimizing for speed and cost

The card also carries an explicit warning against assuming lower effort is always faster
end-to-end (`README.md` line 267):

> In multi-turn agentic tasks, lower reasoning effort does not always reduce overall task
> completion time. Although it may produce faster per-turn responses, it can also lead to
> insufficient analysis, more failures, and repeated retries, which may increase total latency
> and token consumption.

On the wire it is a **top-level OpenAI field**, not a template kwarg, in Qwen's own example
(`README.md` line 300):

```python
completion = client.chat.completions.create(
    model="Qwen/Qwen3.8-27B",
    messages=messages,
    extra_body={"chat_template_kwargs": {"enable_thinking": True, "preserve_thinking": True}},
    reasoning_effort="xhigh",  # xhigh by default; supported levels are xhigh, medium, and low
    ...
)
```

vLLM's own recipe page for this model agrees on all of the above:
<https://recipes.vllm.ai/Qwen/Qwen3.8-27B>.

### A3. `preserve_thinking` — history retention, which is an amount knob at second hand

Template lines 111-119:

```jinja
{%- set reasoning_content = '' %}
{%- if message.reasoning_content is string %}
    {%- set reasoning_content = message.reasoning_content %}
{%- endif %}
{%- set reasoning_content = reasoning_content|trim %}
{%- if preserve_thinking is undefined or preserve_thinking is true or loop.index0 > ns.last_query_index %}
    {{- '<|im_start|>' + message.role + '\n<think>\n' + reasoning_content + '\n</think>\n\n' + content }}
{%- else %}
    {{- '<|im_start|>' + message.role + '\n' + content }}
{%- endif %}
```

Defaults to true. Note the retention is driven by a `reasoning_content` field on each historical
assistant message — so a client that strips reasoning before storing history silently gets
`preserve_thinking` behaviour with empty thoughts (an empty `<think>\n\n</think>` per turn),
which is neither of the two documented modes. The card claims retention improves KV-cache reuse
and agent consistency (`README.md` line 472). Turning it off shortens the prompt but the card
warns it costs decision consistency in agent loops.

Note the `or loop.index0 > ns.last_query_index` clause: thinking is *always* kept for assistant
messages after the last real user query, i.e. within the current tool-call chain, regardless of
the flag.

### A4. What Qwen does *not* ship

Checked directly against the downloaded files, all negative:

- **No `thinking_budget` / `reasoning_budget` template variable.** `grep -n "budget" chat_template.jinja`
  returns nothing.
- **No budget-forcing text.** The "Considering the limited time by the user, I have to give the
  solution based on the thinking directly now." style closer is **not** Qwen's; it is a serving
  convention (see A6).
- **No `/think` or `/no_think` soft switch.** Those were Qwen3-generation features documented on
  the Qwen3 model cards (<https://huggingface.co/Qwen/Qwen3-8B>). `grep -n "no_think\|/think"`
  finds no handling in the 3.8 template and no mention in either 3.8 model card. Whether the 3.8
  weights still react to those strings as plain prompt text is untested — see (c).
- **No special reasoning-length token.** Nothing in the vocab named for effort or budget.

### A5. Token-level facts you need for any sampler-side work

From `Qwen/Qwen3.8-27B/tokenizer_config.json`, `added_tokens_decoder`:

| token | id | `special` flag |
|---|---|---|
| `<|im_start|>` | 248045 | true |
| `<|im_end|>` | 248046 | true |
| `<tool_call>` | 248058 | false |
| `</tool_call>` | 248059 | false |
| `<think>` | **248068** | false |
| `</think>` | **248069** | false |

`<think>` and `</think>` are **single vocabulary entries**. Forcing an early end of thinking is
therefore a one-token force, not a multi-token string force. (`special: false` here only means
they are not stripped by `skip_special_tokens`; they are still atomic added tokens.)

`generation_config.json`: `temperature 1.0`, `top_k 20`, `top_p 0.95`, `eos_token_id [248046, 248044]`.

Recommended sampling per the card (`README.md` lines 252-253) — worth quoting because it differs
between modes:
- Thinking: `temperature=1.0, top_p=0.95, top_k=20, min_p=0.0, presence_penalty=0.0, repetition_penalty=1.0`
- Non-thinking: `temperature=0.7, top_p=0.80, top_k=20, min_p=0.0, presence_penalty=1.5, repetition_penalty=1.0`

And the agentic sizing guidance (`README.md` lines 507-510), which is the closest Qwen comes to a
budget: *"Reasoning Content: Set the maximum output length to 262,144 tokens. Final Response: Set
the maximum output length to 131,072 tokens."* That is a recommendation to size two separate
caps, not a mechanism.

### A5b. Qwen3.8-Flash-Next / `qwen4exp` is the same story

`diff` of the two downloaded templates after trailing-whitespace normalisation: **identical**.
The Flash-Next card says the same thing in prose (`README.md` line 350): *"Qwen3.8-Flash-Next
supports controlling thinking behavior via `enable_thinking`, `preserve_thinking`, and
`reasoning_effort`."* Same three effort levels, same `xhigh` default (line 384).

The llama.cpp commit `6c84c7d5d` that added `qwen4exp` is purely architectural — hyper-connections,
gated delta net, MoE, PLE n-gram embeddings, UINT64 GGUF arrays. It touches no chat, template, or
reasoning code. `grep -rn "xhigh"` across the whole llama.cpp tree hits exactly one line, the help
text of `--reasoning-effort` (`common/arg.cpp:3711`). There is no Qwen3.8 template fixture under
`models/templates/` (newest Qwen there is `Qwen3.5-4B.jinja`) and no Qwen3.8 test in
`tests/test-chat.cpp`.

### A6. What serving stacks bolt on

#### llama.cpp — a real reasoning-budget sampler (the most complete implementation seen)

`D:\Work\llama.cpp`, HEAD `6c84c7d5d`.

**The sampler.** `common/reasoning-budget.h:10-16` declares a five-state machine:

```
IDLE -> COUNTING -> WAITING_UTF8 -> FORCING -> DONE
```

- `IDLE`: passthrough, watching for the start sequence.
- `COUNTING`: decrement per token, watching for a natural end sequence.
- `WAITING_UTF8`: budget spent, but let the current multi-byte character finish.
- `FORCING`: emit the forced sequence token by token. `common/reasoning-budget.cpp:166-186` is
  the whole enforcement — every logit except the one forced token is set to `-INFINITY`.
- `DONE`: passthrough. It **re-arms** on a new start tag (`reasoning-budget.cpp:147-161`),
  because some models open several `<think>` blocks in one reply.

**The forced sequence** is `reasoning_budget_message` tokens followed by the first end tag
(`tools/server/server-schema.cpp:415-427`). That is exactly the "Considering the limited time…"
pattern: you supply the sentence, the server prepends it to `</think>` and forces the lot.

**CLI flags** (`common/arg.cpp`):

| line | flag | meaning |
|---|---|---|
| 3680 | `--reasoning-format none\|deepseek\|deepseek-legacy` | where thoughts land in the response |
| 3691 | `-rea, --reasoning on\|off\|auto` | sets `enable_thinking` template kwarg |
| 3709 | `--reasoning-effort LEVEL` | passes the string straight into the jinja context |
| 3721 | `--reasoning-budget N` | `-1` unlimited, `0` immediate end, `N` token budget |
| 3729 | `--reasoning-budget-message MESSAGE` | text injected before the end tag on exhaustion |
| 3736 | `--reasoning-preserve` / `--no-reasoning-preserve` | sets the `preserve_reasoning` kwarg |

Environment aliases exist for each (`LLAMA_ARG_REASONING`, `LLAMA_ARG_REASONING_EFFORT`,
`LLAMA_ARG_THINK_BUDGET`, `LLAMA_ARG_THINK_BUDGET_MESSAGE`).

**Per-request HTTP fields** (`tools/server/server-common.cpp:1313-1378`):

- `chat_template_kwargs: {"enable_thinking": bool, ...}` — merged over the server defaults.
- `reasoning_effort: "<level>"` — the OpenAI field. `"none"` is special-cased to
  `enable_thinking = false` (line 1326-1328); any other non-empty string is forwarded as the
  template kwarg (line 1330).
- `reasoning_budget_tokens`, alias `thinking_budget_tokens` (line 1365-1366), falling back to the
  server default.
- `reasoning_budget_message` (line 1375).
- `reasoning_control: bool` (line 1376) — arms the sampler so it can be forced later at runtime.

**Runtime early-stop.** `POST /v1/chat/completions/control` with
`{"id": <completion id>, "action": "reasoning_end"}` forces the in-flight completion out of its
thought block mid-stream (`tools/server/server-context.cpp:2431-2447`, documented at
`tools/server/README.md:1454`). It requires `reasoning_control: true` on the original request.
This is a "stop thinking, answer now" button for a UI.

**Two llama.cpp findings that bear on Qwen 3.8 specifically:**

1. **The budget arms correctly despite the template pre-filling `<think>`.** Recall from A1 that
   the model never emits `<think>` — the template does. llama.cpp handles this: the generation
   prompt is re-tokenized into `prefill_tokens` (`common/sampling.cpp:279-292`) and those tokens
   are fed through `llama_sampler_accept` on the budget sampler at construction
   (`common/sampling.cpp:318-322`), so the `<think>` in the prompt moves the state machine
   `IDLE -> COUNTING`. Any stack that rolls its own budget must do the same or the counter never
   starts.
2. **`--reasoning-preserve` does not work on Qwen 3.8.** It writes the template kwarg
   `preserve_reasoning` (`common/arg.cpp:3742,3745`); the Qwen 3.8 template reads
   `preserve_thinking` (template line 116). The names differ, so the flag is inert here. The
   working spelling is `chat_template_kwargs: {"preserve_thinking": false}`.

**How llama.cpp routes the Qwen 3.8 template.** There is no Qwen3.8 branch. The template
contains `<tool_call>`, `<function=` and `<parameter=` (its tool block, template line 68), which
is the Qwen3-Coder detector at `common/chat.cpp:3596-3602`. Inside
`common_chat_params_init_qwen3_coder`, `supports_reasoning` is set by finding `<think>` in the
template source (`common/chat.cpp:1172`), which succeeds, so
(`common/chat.cpp:1183-1186`):

```cpp
data.thinking_start_tag = "<think>";
data.thinking_end_tags = { "</think>", "<tool_call>" };
```

Both of those flow into the budget sampler. So `--reasoning-budget` works on Qwen 3.8 out of the
box, provided `--jinja` is on. Whether `reasoning_effort` is offered to the client is decided by
a capability probe that renders the template with `reasoning_effort = "low"` and checks whether
the variable was read (`common/jinja/caps.cpp:526-532`); Qwen 3.8 reads it, so it probes as
supported.

#### vLLM

- Sampling parameter `thinking_token_budget` — per-request reasoning token limit.
- `--reasoning-parser qwen3` is required for Qwen 3.8, because the template opens every assistant
  turn with `<think>` and without the parser the whole thought block lands in `content`
  (<https://recipes.vllm.ai/Qwen/Qwen3.8-27B>).
- `--reasoning-config '{"reasoning_start_str": "<think>", "reasoning_end_str": "I have to give the
  solution based on the reasoning directly now.</think>"}'` — this is where the "limited time"
  style closer comes from. The docs say putting transitional language in `reasoning_end_str`
  makes the termination "more natural".
- `--default-chat-template-kwargs '{"enable_thinking": false}'` for a server-wide default;
  per-request `chat_template_kwargs` always wins.
- The docs list Qwen3, DeepSeek and Nemotron3 as the families supporting a thinking budget.
- Source: <https://docs.vllm.ai/en/latest/features/reasoning_outputs/>.

#### SGLang

Exposes `chat_template_kwargs` and `separate_reasoning: true` (which populates
`reasoning_content`), plus a `thinking_budget`. The budget is reported **not enforced** on this
model generation: sgl-project/sglang issue #25536, "thinking_budget not enforced for Qwen3.6 —
reasoning consumes all max_tokens", where `thinking_budget: 200` still produced roughly 1400
reasoning tokens and left nothing for the answer
(<https://github.com/sgl-project/sglang/issues/25536>). Treat SGLang's budget as unreliable.

#### Ollama

Reported in the model discussions to replace the template with a generic one, which silently
disables `reasoning_effort` entirely
(<https://huggingface.co/Qwen/Qwen3.8-27B/discussions/113>).

### A7. Independent write-ups on quantization and on banning reconsideration phrases

This is directly relevant to us because we run this model quantized.

**Quantization makes reasoning traces longer.** Two 2026 papers:

- *"Quantized Reasoning Models Think They Need to Think Longer, but They Do Not"*
  (<https://arxiv.org/abs/2606.00206>). Abstract, verbatim in the load-bearing part: *"aggressive
  PTQ reduces accuracy while increasing chain-of-thought (CoT) length… in up to 52% of the
  quantized models' failures, models reach the right answer in intermediate reasoning steps but do
  not output it as a final answer… Positions with high KL divergence correlate strongly with high
  next-token entropy, and at these positions quantized models disproportionately sample
  overthinking markers such as "wait", "but", and "alternatively". We show that simply introducing
  a training-free logit penalty on a curated set of overthinking markers can reduce CoT length by
  12--23% while preserving or improving accuracy across 5 models (1.5B-32B parameters), 3
  quantization methods, and 5 benchmarks… Overthinking errors produced by quantized models are
  particularly reduced by up to 58%."*
- *"Quantization Inflates Reasoning: Token Inflation as a Hidden Cost of Low-Bit Reasoning Models"*
  (<https://arxiv.org/abs/2606.25519>). INT4/INT3 can preserve accuracy while inflating reasoning
  token count enough to cancel the per-token speedup, with more intermediate steps and more
  semantic repetition in the trace.

The practical reading: a Q4_K_M Qwen 3.8 is expected to think *longer* than the bf16 model at the
same `reasoning_effort`, and the extra length is disproportionately made of "wait" / "but" /
"alternatively" continuations sampled at high-entropy positions. A logit penalty on those markers
is the paper's own remedy and it is cheap.

**Banning reconsideration phrases outright.** *"Wait, We Don't Need to 'Wait'! Removing Thinking
Tokens Improves Reasoning Efficiency"* (NoWait), <https://arxiv.org/abs/2506.08343> — suppresses
explicit self-reflection tokens (`Wait`, `Hmm`) at decode time and reports a 27%-51% shorter
chain of thought across five R1-style model series and ten benchmarks with utility preserved.

**Rollback and resampling** is the Antislop sampler
(<https://github.com/sam-paech/auto-antislop>, summary at
<https://www.emergentmind.com/topics/antislop-sampler>). Unlike a logit ban it works on
*multi-token phrases*: it keeps the inference trace, and when a banned pattern completes it
backtracks to the token where the pattern began, downweights the offending continuation, and
resamples. The suppression is soft — `p_new = p_old * 10^(-10s)` with ban strength `s` in `[0,1]`,
`s=1` being a hard ban. It scales to 8000+ patterns (direct token banning caps out near 2000) and
accepts regexes. The cost is real: 69%-96% output slowdown in the bad cases. This is the only
mechanism in the set that can ban a phrase like `", but wait"` precisely — a plain logit ban on
the `wait` token cannot distinguish `", but wait"` from a legitimate `wait`.

For comparison, llama.cpp itself has **no** backtracking sampler (`grep -rni "antislop\|backtrack"`
over `src/ common/ tools/` finds only unrelated tokenizer and unicode code). It has DRY
(`common/sampling.cpp:353`) and `logit_bias`, which are enough for the NoWait-style single-token
ban but not for phrase-level rollback.

---

## (b) What we could expose as "quick" / "normal" / "high"

Our stack, for reference: chat rendering is `modules/dasLLAMA/dasllama/dasllama_chat.das` over
per-family `ChatTemplate` data declared in `dasllama_arch_*.das` /
`dasllama_common.das:1120-1150`; sampling is
`modules/dasLLAMA/dasllama/dasllama_sampling.das:23-32` (`SamplingParams`: temp, top_k, top_p,
min_p, repetition/presence/frequency penalties — no logit bias, no forced tokens, no phrase ban);
the OpenAI server is `utils/dasllama-server/openai_server.das`. The design overview is
`modules/dasLLAMA/THINKING.md`. We already carry a Qwen3.8-27B GGUF spec
(`modules/dasLLAMA/performance/model_specs.das:346-351`, `Qwen3.8-27B-UD-Q4_K_M.gguf`,
`serve_ctx = 262144`) but there is **no** `dasllama_arch_qwen38.das` yet, and `chatml_chat`
(`dasllama_common.das:4639-4653`) does not prefill `<think>` on the assistant open, which the
Qwen 3.8 template does.

The answer to the question is **yes**, and there are three independent levers. I would build them
in this order.

### B1. Ship `reasoning_effort` as the primary knob (cheap, first-party, no sampler work)

Map the three rungs straight onto Qwen's three levels:

| our rung | `reasoning_effort` | what the model sees |
|---|---|---|
| quick | `low` | the "keep your thinking brief and focused" sentence |
| normal | `medium` | no sentence at all (the neutral prompt) |
| high | `xhigh` | the "think carefully, validate key assumptions" sentence |

Concrete steps:

1. **Add an effort field to `ChatTemplate`.** The cleanest shape given our existing data-driven
   design is a three-slot string set — e.g. `effort_low : string`, `effort_high : string`, with
   empty meaning "inject nothing", which is literally how Qwen encodes `medium`. Render it into
   the system turn ahead of the user's system content, matching template lines 58-86, including
   the case where there is no user system prompt and we must synthesize a system turn holding only
   the sentence.
2. **Add a `reasoning_effort` field to the server.** Add `"reasoning_effort"` to `CHAT_FIELDS`
   (`utils/dasllama-server/openai_server.das:1556-1559`) — today it is silently warned about as an
   unsupported field. Also accept it inside `chat_template_kwargs` (the loop at line 2521-2527
   currently warns on every key but `enable_thinking`).
3. **Decide the out-of-range policy, and do not copy Qwen's.** Qwen's template hard-fails on
   `"high"`, which is the single most common value an OpenAI client sends. We should map
   `high` -> `xhigh` and `minimal` -> `low`, accept `none` -> thinking off (llama.cpp's rule,
   `server-common.cpp:1326`), and reject the rest with a 400 naming the legal set. This is a
   deliberate divergence from llama.cpp, which passes the string through and lets the template
   blow up.
4. **Keep it a per-family declaration, not a Qwen special case** — some families have no effort
   sentence at all and should render nothing regardless of the request.

Caveat to state in the UI: this is a *request*, not a cap. It has no worst case.

### B2. Add a hard token budget in the sampler (the only real cap)

Because `<think>` and `</think>` are single tokens (A5), our version is much simpler than
llama.cpp's general string-sequence machine.

Concrete steps:

1. **Add to `SamplingParams`:** `reason_budget : int64` (`-1` off, `0` = end immediately,
   `N` = token cap), plus the forced-token list and the optional closer message.
2. **Count inside the block.** The generation loop knows whether it is inside the thought span —
   `dasllama_chat.das` already has the reply-side matcher (`ThinkStream` /
   `make_think_stream_`/`think_feed_`, `dasllama_chat.das:1054-1057`) for exactly this. Arm the
   counter when the block opens. **Critical**: with the Qwen 3.8 shape the block is opened by the
   *prompt*, not by a sampled token, so the counter has to be armed at prompt-render time when
   `think_open` was prefilled — this is the same bug llama.cpp avoids by feeding the prefill
   tokens through the sampler (`common/sampling.cpp:318-322`).
3. **Force on exhaustion.** Set every logit but the forced token to `-inf` for as many steps as
   the forced sequence is long. If a closer message is configured, force its tokens first and
   `</think>` (248069) last — this is what makes the transition read naturally rather than as an
   abrupt cut. Mirror llama.cpp's UTF-8 grace state (`reasoning-budget.h:14`) so we do not sever
   a multi-byte character.
4. **Re-arm on a second `<think>`** (`reasoning-budget.cpp:147-161`) — the model can open more
   than one block per reply.
5. **Server field:** accept `reasoning_budget_tokens` with alias `thinking_budget_tokens`
   (llama.cpp's spelling, `server-common.cpp:1365-1366`) plus `reasoning_budget_message`.

Suggested rung defaults, to be measured, not assumed: quick 512-1024, normal 4096-8192, high
unlimited. The community reports 22k reasoning tokens on a single SVG prompt at `xhigh` and
~60k per turn in agent use, so "unlimited" for the high rung needs a ceiling from `max_tokens`
regardless (<https://huggingface.co/Qwen/Qwen3.8-27B/discussions/113>).

### B3. A "stop thinking now" button (small, high perceived value)

llama.cpp's `POST /v1/chat/completions/control` with `action: "reasoning_end"`
(`server-context.cpp:2431-2447`) is about thirty lines of work on top of B2: arm the budget
machinery for the request, then let an out-of-band request drive the state machine straight to
FORCING. For an interactive UI watching a model think for four minutes this is worth more than
either of the above.

### B4. Optional, quantization-motivated: an overthinking-marker penalty

We serve Q4_K_M, and A7 says that specifically inflates traces with `wait` / `but` /
`alternatively`. A single-token logit penalty is a small addition to `SamplingParams` (we have
no `logit_bias` at all today) and the paper reports 12-23% shorter traces at equal or better
accuracy. Reasonable shape: a per-rung penalty, off at `high`, mild at `normal`, strong at
`quick`.

Phrase-level bans (`", but wait"`) need rollback and resampling, which we do not have and which
costs 69-96% throughput in the reference implementation. **Do not build that.** The single-token
penalty gets most of the benefit at no throughput cost.

### B5. Prerequisite work regardless of which rungs we ship

- **`dasllama_arch_qwen38.das` does not exist.** Qwen 3.8 is ChatML-shaped so `chatml_chat` is
  the right base, but three things differ from our Qwen3/3.5 wiring: (i) the generation prompt
  must prefill `<think>\n` when thinking is on — our `chatml_chat` does not
  (`dasllama_common.das:4642-4643`), and `ChatTemplate` already has the field for it,
  `assistant_open_think`, added for gemma-4 (`dasllama_common.das:1135`); (ii) the tool block is
  Qwen3-Coder XML (`<tool_call><function=…><parameter=…>`), **not** the Hermes JSON our
  `dasllama_arch_qwen35.das:44` installs; (iii) the effort sentence from B1.
- **History retention conflicts with Qwen's default.** `THINKING.md` records that our `respond_`
  stores history **reasoning-stripped**. Qwen 3.8 defaults to `preserve_thinking = true` and
  renders `<think>\n{reasoning_content}\n</think>` for every historical assistant turn. Stripping
  leaves us rendering empty thought blocks — neither documented mode. We should either carry
  `reasoning_content` in history and honour `preserve_thinking`, or render the non-preserving
  branch (template line 119) deliberately. Right now we do neither on purpose.

---

## (c) What is uncertain or contradictory

1. **Nobody publishes what the effort levels actually cost.** Qwen gives three adjectives. The
   only numbers found are community anecdotes and they disagree wildly: one report has `medium`
   cutting time by about a third with no measurable quality loss, another has `medium` moving
   thinking tokens only from 106 to 91 on a short prompt while still cutting latency by a third,
   a third has `xhigh` at 22,276 reasoning tokens and 21 minutes on one prompt, another claims
   ~60k tokens per agent turn
   (<https://huggingface.co/Qwen/Qwen3.8-27B/discussions/113>,
   <https://huggingface.co/Qwen/Qwen3.8-27B/discussions/97>). These are different prompts on
   different hardware. **We should measure our own three rungs before publishing them.**

2. **Qwen contradicts itself on whether lower effort is cheaper.** The card promotes `low` as
   "optimizing for speed and cost" and then warns that in multi-turn agent work lower effort can
   *increase* total latency and token consumption through retries (`README.md:267`). Both can be
   true; it means a per-turn benchmark will overstate the benefit of the quick rung for agents.

3. **`medium` being prompt-identical to "no instruction" is my reading of the template, not a
   documented claim.** It follows directly from lines 45-56 (the string is only set for `xhigh`
   and `low`), but Qwen never says it. It implies the model's *trained* default behaviour — what
   it does with no effort sentence — is the `medium` behaviour, and that `xhigh` being the
   template default means the shipped default is deliberately above the trained baseline. That
   inference is untested.

4. **Whether `/think` and `/no_think` still work is untested.** They are absent from the 3.8
   template and both 3.8 cards. The 3.8 weights may still react to them as prompt text, since
   they were trained into earlier Qwen generations, but there is no evidence either way and
   nothing in the template gives them meaning. Do not build on them.

5. **The `thinking_budget` name collides across stacks and does not mean the same thing.**
   vLLM calls it `thinking_token_budget`; llama.cpp calls it `reasoning_budget_tokens` with a
   `thinking_budget_tokens` alias; SGLang calls it `thinking_budget` and reportedly does not
   enforce it. A client that sends "the" thinking budget will silently get nothing on two of the
   three. If we implement B2, accept every spelling.

6. **Qwen's own agentic sizing advice assumes a feature most stacks lack.** "Set reasoning to
   262,144 and the final response to 131,072" presupposes *separate* caps for reasoning and
   answer. Neither our server nor llama.cpp's has two caps; there is one `max_tokens`. This is
   the failure mode SGLang issue #25536 describes — reasoning eats the whole budget and the
   answer gets zero. **A budget mechanism (B2) is what makes a single `max_tokens` safe.**

7. **The llama.cpp `preserve_reasoning` / `preserve_thinking` name mismatch** (A6) is stated
   from reading the code, not from running it. `common/arg.cpp:3742` writes `preserve_reasoning`;
   the Qwen 3.8 template reads `preserve_thinking` at line 116. I found no llama.cpp code that
   translates between the two. Worth a five-minute empirical check before relying on it, and
   worth an upstream issue if confirmed.

8. **The quantization/trace-length papers are recent and I read abstracts, not full method
   sections.** The 12-23% and 27-51% figures are the papers' own headline claims on their own
   benchmarks, not measured on Qwen 3.8 or on our quant. The direction of the effect is
   well-corroborated by two independent papers; the magnitudes are not ours until we measure.

9. **Not checked:** whether Qwen 3.8 was RL-trained on the effort sentences specifically (the
   card says "official support", which implies it, but no technical report was located), and
   whether the `qwen4exp` Flash-Next weights behave identically to the dense 27B at the same
   effort level despite the identical template.

---

## Sources

- <https://huggingface.co/Qwen/Qwen3.8-27B> — model card
- <https://huggingface.co/Qwen/Qwen3.8-27B/raw/main/chat_template.jinja> — the template
- <https://huggingface.co/Qwen/Qwen3.8-27B/raw/main/tokenizer_config.json>
- <https://huggingface.co/Qwen/Qwen3.8-27B/raw/main/generation_config.json>
- <https://huggingface.co/Qwen/Qwen3.8-Flash-Next> — Flash-Next / `qwen4exp` model card
- <https://huggingface.co/Qwen/Qwen3.8-27B/discussions/113> — "This model cannot stop thinking"
- <https://huggingface.co/Qwen/Qwen3.8-27B/discussions/97> — "A crazy thinking model"
- <https://recipes.vllm.ai/Qwen/Qwen3.8-27B> — vLLM recipe
- <https://docs.vllm.ai/en/latest/features/reasoning_outputs/> — vLLM reasoning outputs
- <https://github.com/sgl-project/sglang/issues/25536> — SGLang thinking_budget not enforced
- <https://huggingface.co/Qwen/Qwen3-8B> — Qwen3 card, for the `/think` `/no_think` history
- <https://arxiv.org/abs/2606.00206> — Quantized Reasoning Models Think They Need to Think Longer
- <https://arxiv.org/abs/2606.25519> — Quantization Inflates Reasoning
- <https://arxiv.org/abs/2506.08343> — NoWait
- <https://github.com/sam-paech/auto-antislop> and <https://www.emergentmind.com/topics/antislop-sampler> — Antislop rollback sampler
- `D:\Work\llama.cpp` @ `6c84c7d5d` — `common/arg.cpp`, `common/common.h`,
  `common/reasoning-budget.{h,cpp}`, `common/sampling.cpp`, `common/chat.cpp`,
  `common/jinja/caps.cpp`, `tools/server/server-common.cpp`, `tools/server/server-schema.cpp`,
  `tools/server/server-context.cpp`, `tools/server/README.md`
- `D:\Work\daScript` — `modules/dasLLAMA/THINKING.md`,
  `modules/dasLLAMA/dasllama/dasllama_common.das`, `dasllama_chat.das`,
  `dasllama_sampling.das`, `dasllama_arch_qwen35.das`,
  `modules/dasLLAMA/performance/model_specs.das`, `utils/dasllama-server/openai_server.das`
