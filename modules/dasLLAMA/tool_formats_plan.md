# Tool formats arc — ToolMode for every family but GLM

**Endgoal (one PR):** when it merges, every supported family except GLM-4 serves `tools`
through the OpenAI endpoint — gpt-oss, gemma-4, mistral3, llama — with tests demonstrating
each per the CODEREVIEW.md recognition-test rule. GLM keeps its THINKING.md remote-leg record
(zen2). The Hermes/Qwen family is the already-working reference.

## The design: ToolMode as template data (the ThinkMode pattern)

The current tool surface is six strings on ChatTemplate that encode exactly one convention
(Hermes: JSON defs in the system prose, a symmetric call-marker pair, results on user turns).
Each remaining family breaks at least one of those assumptions, so the tool format becomes a
mode + per-mode render/parse in the chat layer, exactly as ThinkMode did for reasoning:

```
enum ToolMode { none; hermes; harmony; gemma4; mistral; llama_json }
```

Per-mode responsibilities (all dispatched in `dasllama_chat.das`; arch files stay declarative):

1. **Defs render** — where and how the tool definitions enter the prompt.
2. **Call parse** — extract calls from the generated reply, composing AFTER the reasoning
   split (reasoning first, calls from the content half — the shipped pipeline).
3. **Call replay** — `render_assistant_calls` for stateless history.
4. **Results render** — `add_tool_results` framing.

The server's `parse_tool_calls_auto` becomes mode-dispatched; the OpenAI normalization
(`tool_calls`, `finish_reason`) is already format-agnostic past extraction.

## Per-family wire formats (verify each against the GGUF template + llama.cpp before coding)

| family | defs | call | results | notes |
|---|---|---|---|---|
| **harmony** (gpt-oss) | DEVELOPER turn, TypeScript-style `functions` namespace block — NOT a JSON list | `commentary` channel with recipient header `to=functions.NAME` (+ optional `<|constrain|>json`), body = JSON args, ends at `<|call|>` (already our stop) | tool message back on the matching recipient framing | the channel splitter already parses these headers (recipient-qualified commentary routes to reasoning today — with tools declared it becomes `tool_calls`); highest infrastructure reuse |
| **gemma4** | system-turn block (verify exact shape) | asymmetric `<|tool_call>` … `<tool_call|>` | family's response tags | interplay with thinking is specified in THINKING.md: thoughts inside an in-flight tool chain survive history-strip, and tool-response continuation PREFILLS the thought opener — the asymmetric matcher must accept the opener from either side (the case left unhandled in the thinking arc) |
| **mistral** | `[AVAILABLE_TOOLS]`…`[/AVAILABLE_TOOLS]` (before the last user turn, not the system block) | `[TOOL_CALLS]` + JSON ARRAY, no closing marker (ends at EOS) | `[TOOL_RESULTS]`…`[/TOOL_RESULTS]` | real control tokens in the v0.3/tekken vocabs; closest fit to the marker model |
| **llama_json** (llama-3.x) | JSON defs in the system prompt | the WHOLE reply is a bare JSON object (`{"name":…,"parameters":…}`) — no markers; `<|python_tag|>` only for builtins (out of scope) | `ipython` role turn | parse = strict whole-content JSON sniff (a call iff the full content parses as an object with name+parameters); weakest small-model reliability — see the grammar section |

## Tests (the CODEREVIEW rule, applied four times)

- **Model-free**: per-mode parse pins (call extraction, replay round-trip, reasoning+tools
  compose for harmony/gemma4) — `tests/test_chat.das` render pins + parse cases wherever a
  wire shape is expressible without a model.
- **Live legs** in `utils/dasllama-server/test_openai_server_think.das` (or a sibling
  `_tools` file if it outgrows): gpt-oss-20b (PARITY_FULL tier, local), gemma-4-E2B (local,
  under tier), llama-3.2-3B-Q4 (local, under tier), mistral — **fetch
  Mistral-7B-Instruct-v0.3 Q4_K_M (~4.4 GB, under tier)**; the local v0.3 Q8 is over the
  6 GiB tier and Small-24B far over.
- Each live leg: declare one obvious tool, greedy, assert `finish_reason="tool_calls"` +
  parsed name/args; harmony additionally asserts reasoning_content coexists (the compose).
- `tool_choice:"none"` and the honest-400-for-GLM stay covered by the existing tests.

## The grammar question (audited 2026-08-11)

**llama.cpp**: full GBNF engine + `json_schema_to_grammar` + **lazy grammars** (trigger
tokens/patterns arm the constraint mid-generation — the tool-call mechanism: free until
`<tool_call>`-class trigger fires, then schema-constrained JSON; forced from step 0 for
`tool_choice:"required"`). Their schema→grammar half is the fragile part — 2026 issue tail:
combined-grammar build fails above ~58 tools / cumulative schema size, `maxLength >= 2000`
emits unparseable GBNF, empty-object schemas break the whole request, PCRE shorthands
unsupported.

**dasLLAMA**: nothing — the sampler is a flat logits → cutoffs → argmax/CDF pipeline
(`dasllama_sampling.das`), no per-step constraint hook.

**Decision: formats first; grammar is NOT in this PR.** Three of four families emit their
trained formats reliably unconstrained (the shipped Qwen legs prove the pattern at 0.6B
greedy; llama.cpp itself shipped tool calling before lazy grammars existed). Grammar is an
engine-layer feature with its own design surface (incremental token-level matching across
UTF-8-partial pieces, candidate filtering vs full-vocab masking, penalty/cutoff interplay,
decode hot-path cost) and llama.cpp's bug tail shows the schema→grammar half alone is an arc.
Gating four working formats on it inverts the value order.

**Follow-up arc (separate, after this PR): constrained decoding.**
- Milestone 1: a JSON-only constrained sampler (state-machine over candidate tokens, no
  schema compilation — the 80/20): upgrades the llama leg to deterministic-parseable,
  implements `response_format: json_object`, and gives `tool_choice:"required"` an honest
  core for llama_json mode.
- Milestone 2 (only if demanded): schema-aware constraint + lazy triggers per ToolMode
  (the trigger points are exactly the mode's call markers — keep them data so this slots in).
  General GBNF is explicitly out of scope until a consumer needs it.
- The concrete trigger for starting the arc: the llama-3.2-3B live leg flaking at greedy, or
  a real `tool_choice:"required"` / structured-output consumer.

## Follow-up discussion before the PR: dasllama-server chat preview

The control page's chat panel speaks the same OpenAI surface this arc extends — before the PR
opens, discuss what the preview should DO with the new fields: render `reasoning_content`
(collapsed thinking block? streamed inline?), display declared tools and emitted
`tool_calls`/results turns, and whether the preview grows a way to answer a call (a canned
tool-result box) so a tool round-trip is demonstrable from the page. Scope decision is Boris's;
the outcome lands either in this PR or as its own follow-up arc.

## Order of work inside the PR

1. ToolMode enum + dispatch skeleton; migrate Hermes onto it (zero-behavior-change commit,
   pinned by the existing token-for-token tests).
2. harmony (most reuse, biggest value) → gemma4 (asymmetric pair + the thinking interplay)
   → mistral (cleanest marker fit) → llama_json (sniff parse, lenient live assert).
3. Server: mode-dispatched parse; `tool_choice` handling unchanged ("required" still
   warns-as-auto until the constrained-decoding arc).
4. Docs: THINKING.md tool section update, both READMEs, das2rst grouping for any new facade
   verb (the docs gate re-runs — the think_drain lesson).

## Chat-layer wiring design (settled; implement next)

**Defs placement per mode** (all in the first-turn machinery): hermes = system prose (as
shipped); harmony = canonical system body (identity + `Reasoning: medium` + channel declaration
+ commentary-routing line; no live date — noted deviation) then a DEVELOPER turn
`# Instructions\n\n{system}\n\n# Tools\n\n{namespace}`; gemma4 = `<|tool>decl<tool|>` blocks
spliced after the system content (markers + `<|"|>` are vocab specials — marker-split);
mistral + llama_json = defs block prefixed into the FIRST user turn (deviation: canonical
mistral puts them before the LAST user turn — identical for single-turn requests; note it).

**Replay per mode** (render_assistant_calls_): harmony = ` to=functions.NAME` + `<|channel|>`
`commentary json` + `<|message|>` + args, closed by `<|call|>` (NOT the derived `<|end|>` —
per-mode close suppression); gemma4 = `<|tool_call>` DSL blocks, turn stays OPEN (a
`turn_open` session flag; results continue the same model turn); mistral = `[TOOL_CALLS]`
special + JSON array text + `</s>`; llama_json = the bare JSON object + `<|eot_id|>`.

**Results per mode** (add_tool_results_): hermes = user-framed `<tool_response>` (as shipped);
harmony = own turns `<|start|>functions.NAME to=assistant<|channel|>commentary<|message|>` +
JSON-quoted result + `<|end|>` (names from a `last_call_names` session field, positional);
gemma4 = `<|tool_response>response:NAME{...}<tool_response|>` in the open model turn, then
generation continues in-turn (no assistant_open); mistral = `[TOOL_RESULTS]` framing;
llama_json = an `ipython`-header turn with the string-results-JSON-quoted quirk.

**Server**: finish-path dispatch — harmony+tools = `harmony_parse` (one walk supersedes
split-then-parse); gemma4 = think-split then `gemma4_parse`; mistral/llama_json = think-split
(inert) then their parsers; hermes = as shipped.
