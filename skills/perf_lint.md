# Performance lint (`PERF*`)

Read this before running the lint suite or putting any code under a hot-path contract.
`daslib/perf_lint.das` is a `[lint_macro]` AST pass; findings are reported as error
code 31208.

There is no per-rule catalog here: every finding's error text states the pattern and the
rewrite on its own. A finding you cannot act on from its message alone is a defect of the
message — fix the text in `daslib/perf_lint.das`, don't document around it.

## Running it, and turning rules off

```bash
bin/daslang utils/lint/main.das -- file.das [dir ...] \
  [--quiet] [--silent] [--perf-only|--style-only|--paranoid-only] \
  [--disable CODE,...] [--enable CODE,...] [--workers N]
```

The MCP `lint` tool runs the same three passes. `--enable` is a whitelist (only the listed rules
run); on overlap `--disable` wins.

Four ways to silence a rule, narrowest first:

1. `// nolint:PERFnnn` on the line, with a reason.
2. A per-module `options` knob, where the rule has one.
3. `.lint_config` — a `[rules]` table entry (`PERF007 = false`) turns a rule off, or on
   (`STYLE005 = true`), tree-wide.
4. `DAS_LINT_DISABLE=PERF028,...` for one run, no source edit.

## Hot-path contracts (PERF026-028)

These three are **annotation-gated**: nothing is checked until a function declares a contract.
From each annotated root the scan follows **direct** calls transitively, so a sink several frames
deep is still reported.

| Annotation | Bans |
|---|---|
| `[hot_path]` | all three |
| `[no_alloc]` | heap traffic (PERF026) |
| `[no_env]` | environment lookups (PERF027) |
| `[no_io]` | console / file I/O (PERF028) |
| `[cold_path]` | *prunes the walk* — a one-time init or opt-in leg reached from a hot function |

Combine on one line: `[no_alloc, no_env, no_io]`. Two stacked annotation blocks do not parse.

**Declaring a contract is free.** The five annotations are compiler-registered metadata markers,
and code under contract does not `require daslib/perf_lint` — a plain build pays nothing and stays
silent. The trade is that a plain build no longer fails on a violation; lint does, everywhere lint
runs.

### What counts as heap traffic

Detection is structural, not a list of names. Every array/table heap operation bottoms out in a
`__builtin_array_*` / `__builtin_table_*` extern, so the rule matches the prefix (minus the
lock/probe forms) — `push` / `reserve` / `resize` / `erase` / `insert` / `delete` are covered
without naming one generic, and a newly added builtin is caught by default. On top of that:
`new` (including `new Foo(f = v)`, which lowers to a move-to-heap), `delete`, string
interpolation, lambda capture frames, table indexing (`t[k]` inserts on read), and any builtin
returning a freshly allocated string.

### `@scratch` — declaring a reused buffer

A buffer sized to the current step's geometry and reused is not an accident. Say so once, at the
buffer, rather than with a `nolint` per call site (which buries the next real finding and does not
survive the code moving):

```das
struct Session {
    @scratch attq : array<float>     // reused per step; sizing it is the owner's strategy
    logits : array<float>            // unmarked: sizing this on a hot path is reported
}

// a helper that sizes a caller's buffer marks the PARAMETER — the call site cannot see
// which field arrived, because the destination comes in by reference
def overwrite_resize(@scratch var a : array<numT>; need : int64) { ... }
```

A sizing call whose destination reaches a `@scratch` declaration is not descended into — a struct
field, a by-ref helper parameter, or a module global (`var @scratch g : array<T>`; the annotation
goes AFTER `var`, before it is a syntax error). The mark covers the sizing set
(`resize`/`reserve`/`push*`/`emplace`/`insert`/`erase`/`pop`/`clear`) and **table indexing**
(`t[k]` on a `@scratch` table is the pool / residency-cache shape), and it follows local reference
bindings (`var lst & = pool.free_bufs[b]` carries the mark).

The name is the property, not the lint: a reuse promise is exactly the precondition a compiler
would need to keep capacity across a shrink, skip the zero-fill, or hoist a loop-invariant
sizing out. None of that is implemented yet.

### `@exact_size` — declaring an input-scaled buffer

The sibling contract for buffers whose size follows the input (a clip's frames, an image's
pixels, a vocabulary) — the class that crosses `max_unreserved_size` in a single grow and panics
only when a big enough input arrives. Same placement rules as `@scratch` (field, global, local,
by-ref parameter; after `var`); the two stack: `@scratch @exact_size buf : array<float>`. On an
`@exact_size` array every `resize`/`resize_no_init` must follow a `reserve` or `ensure_capacity`
of the same receiver earlier in the same function (PERF032); a helper that reserves internally is
transparent, and a by-ref helper marks its own parameter so the `resize` inside is held to it:

```das
struct EncoderState {
    @exact_size x : array<float>     // [T x d]: T is the clip length
}
def reserve_resize(@exact_size var a : array<numT>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}
```

Not a runtime flag: the annotation changes nothing about how the array grows — it makes the
reserve-first spelling a checked contract at every site that touches the buffer.

### Escape hatches, in order of preference

1. `[cold_path]` on the callee — the honest fix when the leg genuinely runs once (lazy init, PSO
   compile, opt-in bookkeeping, a check behind a debug flag).
2. `@scratch` on the destination when it is a reused buffer.
3. `// nolint:PERF026` **with a reason**, honored anywhere along the chain — anchor line, sink
   line, or any call site between. The sink often bottoms out in `daslib/builtin.das`, whose lines
   are nobody's to annotate, so the call site that owns the decision is the honest place.
4. `DAS_LINT_DISABLE=PERF028` for a whole run. With all three codes disabled the closure walk is
   skipped entirely, so this buys compile time and not just quiet.

### What the scan deliberately does not report

- **Anything under a `panic(...)` argument** — a panic is fatal, so its interpolated message is on
  the abort path by construction.
- **Macro-generated subtrees.** A rewritten stub stamps the caller's line onto its splice, so its
  own machinery would otherwise be blamed on every call site.
- **Indirect calls.** An invoke through a function pointer or lambda cannot be resolved
  statically — annotate the implementations it reaches instead. Taking a function's address is
  not calling it.
