# flatten shader example harness

Demonstrates [`daslib/flatten`](../../daslib/flatten.das) driving a real **callless,
branchless shader-graph backend** — a local testing copy of EdenSpark's
`engine.render.shader_dsl`.

The backend compiles a pixel shader into a pure dataflow graph of primitive
"opcodes" (`add`, `mul`, `lerp`, `sat`, `dot_f3`, …). It has **no control flow**:
no `if`, no loops, no user function calls, no `?:`. flatten is the front-half that
makes those expressible — it inlines helper calls, lowers `if/else` to predicated
`?:` selects, and unrolls fixed-count loops, handing the backend branchless,
call-free code.

```
./run.sh                 # regression: compile every shader, report pass/fail
./run.sh cel_shading     # print one shader's opcode graph
./capability/check.sh    # capability: control-flow shaders lower through flatten
```

## Layout

| Path | What |
|---|---|
| `shaders/user_shaders/`, `shaders/user_shaders_2d/` | The real EdenSpark sample shaders (3D + 2D post-fx), **unedited**. |
| `shaders/features/` | Feature fixtures: loop-unroll source forms (array-literal / parallel multi-iterator / `urange`) that compile **only** because flatten unrolls them, plus `angle_spin.shader` exercising the native `radians`/`degrees` → `mul` lowering. flatten is load-bearing, not transparent. |
| `backend/shader_dsl_primitives.das` | I/O contract structs (`PbrInput`/`PbrOutput`) + the leaf primitives. The component-wise math ops are the native daslang `math` builtins (`require math public`); only the engine-specific leaves (`tex2d`/`noise`/`frac`/`remap`/`one_minus`/`fresnel`/`unpack_normal`) remain `[hint]` stubs. |
| `backend/shader_dsl_boost.das` | The validator + graph IR builder + the `[pixel_shader]` annotation. **This is where flatten is wired in.** |
| `backend/shader_graph_ir_builder.das` | Print-only stand-in for the engine's native `sg_ir_*` graph sink — dumps human-readable opcodes. |
| `backend/shader_dsl.das` | Re-exporter: `require engine.render.shader_dsl` pulls in the two modules above. |
| `flatten_shaders.das_project` | Module resolver aliasing `engine.render.shader_dsl` → `backend/`, so the real shaders compile with **zero edits**. |

## How flatten plugs in

The `[pixel_shader]` annotation's `patch` hook calls `flatten_function(func, HINT_WHITELIST)`
*before* validation (`backend/shader_dsl_boost.das`). `HINT_WHITELIST` is the set of leaf
primitive names — flatten keeps those calls as leaves and inlines/lowers everything else.
The leaves are the native `math` builtins (mapped to opcodes by name via `NATIVE_OPCODE`)
plus the engine-specific `[hint]` stubs. `radians`/`degrees` carry no opcode: `flatten_opt`
lowers each to a `mul` (`radians(x) → x * K`), and an adjacent const factor merges
(`degrees(x) * C → x * (K*C)`). The backend then walks the branchless result. For the
capability tier it also maps `ExprOp3 → Select` (and comparison ops → mask nodes) to
consume flatten's `?:` output.

## Two tiers

- **Regression** — the real `user_shaders*` are already 100% branchless dataflow (there
  is no control-flow primitive in the DSL *yet* — flatten is what adds one). flatten must
  be **transparent** on them: the emitted graph is the same one the backend produces
  without flatten. The `shaders/features/` fixtures additionally exercise the loop-unroll
  source forms (`for…in` array literal, parallel `(a, b in xs, ys)`, `urange`) — there
  flatten is load-bearing, but the same compile-clean check covers them. `./run.sh`
  checks every shader compiles clean and emits a graph.
- **Capability** *(`capability/check.sh`)* — shaders written with constructs the
  backend rejects today, made to compile by flatten. This is the feature flatten
  exists to deliver:
  - `cap_control.shader` uses an `if/else`; flatten lowers it to `?:`, which the
    backend consumes as `select` nodes (it bans `ExprIfThenElse` directly).
  - `cap_helper.shader` factors logic into a helper function; flatten inlines it to
    the **identical opcode graph** as the hand-inlined twin `cap_inlined.shader`.

> The backend here is a faithful testing copy, not the shipping engine module; it
> prints opcodes instead of feeding the native shader-graph compiler. It will be
> re-synced as the engine's `engine.render.shader_dsl` evolves (the engine side adds
> the `ExprOp3 → Select` opcode to consume flatten's `?:`).
