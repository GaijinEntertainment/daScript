# DECS — daslang Entity Component System

Read this skill before writing or editing any `.das` code that uses `decs`, `decs_boost`, or the `from_decs` linq bridge. The companion tutorial is [tutorials/language/34_decs.das](tutorials/language/34_decs.das) — runnable examples of every pattern below. Module sources: [daslib/decs.das](daslib/decs.das), [daslib/decs_boost.das](daslib/decs_boost.das), [daslib/decs_state.das](daslib/decs_state.das).

## What DECS is

A pure-daslang ECS runtime with archetypal storage. Entities are an `EntityId` (id + generation). Components are name-keyed typed values. Entities with the same set of component names share an **archetype** — a SoA bucket where each component is stored in its own contiguous `array<uint8>` indexed by entity slot. Queries iterate archetype-at-a-time, so the cost is proportional to matching archetypes, not entity count.

All mutations (`create_entity`, `delete_entity`, `update_entity`) are **deferred** — they're queued as lambdas in a global action list and applied on `commit()`. The two bulk creators (`create_entities`, `create_entities\`T`) are the exception: they're immediate. None of these may be called from inside a query.

## `require`

```das
require daslib/decs_boost
```

`decs_boost` re-exports `decs` publicly — **never `require` both**. `decs_boost` adds the `query` / `find_query` / `from_decs` call macros, the `[decs_template]` structure macro, the `[decs(stage = ...)]` function macro, and the `REQUIRE` / `REQUIRE_NOT` block annotations. `decs_state` is a separate optional module that wires DECS into the DAP debugger; require it only when you want live archetype/request inspection.

## Creating entities

`create_entity` queues a deferred build. The block receives the future `EntityId` and a mutable `ComponentMap`. Use `:=` to set components — the operator is overloaded for every workhorse type and dispatches to `make_component`, which captures the daslang type info needed to clone, serialize, dump, and GC-mark the value.

```das
let player = create_entity() @(eid, cmp) {
    cmp.name := "hero"
    cmp.hp   := 100
    cmp.pos  := float3(0, 0, 0)
}
commit()  // entity now visible to queries
```

Returns an `EntityId` you can keep — generation is set on creation, so the id is valid for `update_entity` / `delete_entity` / `query(eid, ...)` even before `commit()` runs (the queries themselves see nothing until commit).

`set(cmp, "name", value)` and `cmp |> set("name", value)` are the longhand forms; `cmp.name := value` is the dot/`:=` shorthand. For an existing component slot they overwrite in place; type mismatch panics. The names "eid" and the implicit `eid` component are managed by the runtime — don't set "eid" yourself.

## Queries — the workhorse

```das
query() $(name : string; hp : int; pos : float3) {
    print("{name}: hp={hp} pos={pos}\n")
}
```

Each block argument is a component name + type. The macro expands to `for_each_archetype(req_hash, req_factory) $(arch) { for (...) { ... } }` — one outer pass per matching archetype, one inner per-entity loop pulling each component as a temp `array<T>` view. Cost stays archetype-proportional even when only a handful of archetypes match.

**Mutation:** `var name : Type&` makes that component writable. `&` alone (no `var`) is read-only-by-ref.

```das
query() $(var pos : float3&; vel : float3) {
    pos += vel
}
```

**Single-entity:** pass an `EntityId` first.

```das
query(eid) $(tag : string; val : int) {
    print("Found: tag={tag} val={val}\n")
}
```

**find_query:** stops on the first `return true`. Same shape as `query` but no `eid` form. Default fallthrough is `false`.

```das
let found = find_query() $(idx : int) {
    return true if (idx == 7)
    return false
}
```

## REQUIRE / REQUIRE_NOT

Filter without binding. Block-level annotations on the `$ ...` block, before the parameter list:

```das
query <| $ [REQUIRE(weapon)] (name : string; hp : int) {
    print("{name} hp={hp}\n")
}

query <| $ [REQUIRE_NOT(shield)] (name : string) {
    print("{name}\n")
}
```

`REQUIRE(a, b, c)` and `REQUIRE_NOT(a, b)` both take comma-separated bare names. They contribute to the request hash so the cache works.

## Defaults and optionals

A parameter with a default value applies that default when the entity lacks the component. **Default-valued parameters must be `const`** — no `var`, no `&`. The macro routes them through `get_default_ro`, which falls back to a `repeat_ref` iterator yielding the default.

```das
query() $(name : string; alpha : float = 0.5) {
    print("{name}: alpha={alpha}\n")
}
```

**Optional pointer parameters** are different — annotate the argument as `[optional]` and declare it as a pointer:

```das
query() $(name : string; @optional shield : bool?) {
    if (shield != null) { ... }
}
```

`@optional` arguments cannot have default values and must be pointer types. Internally these route to `get_optional`, returning `null` for entities that lack the component. Don't confuse the two forms — the default-value form is for "use X when missing", the optional-pointer form is for "tell me whether it was missing".

## `[decs_template]`

A struct annotated with `[decs_template]` becomes a bundle of components. The macro generates four functions:

- `apply_decs_template(cmp, src)` — sets each field on a `ComponentMap` with prefix `StructName_`
- `remove_decs_template(cmp, src)` — removes those components
- `apply_decs_template_arch(arch, eidx, eid, src, cache)` — direct archetype write with cached indices
- `create_entities\`T(count, blk)` — bulk creator using the cache, no per-entity ComponentMap

```das
[decs_template]
struct Particle {
    pos  : float3
    vel  : float3
    life : int
}

create_entity() @(eid, cmp) {
    apply_decs_template(cmp, Particle(pos = float3(0), vel = float3(1, 0, 0), life = 100))
}
commit()

query() $(var p : Particle) {        // expands to particle_pos, particle_vel, particle_life
    p.pos += p.vel
    p.life -= 1
}
```

When a template struct appears as a query parameter the macro expands the parameter into one binding per field (with the `StructName_` prefix). It also auto-fills `REQUIRE` with all the field names — you don't need to add a `[REQUIRE(...)]` block.

**Custom prefix:** `[decs_template(prefix = "particle")]` overrides the default. `[decs_template(prefix)]` (no value) yields an empty prefix — components are named exactly the field names.

**Kaboom rule:** inside a template-using query, the struct *variable* (e.g. `p`) does not exist as a value — only `p.field` is rewritten. Trying to use `p` itself (assigning, passing to a function) emits a static error: "decs_template variables can only be accessed by fields". This is intentional; the struct is a compile-time fiction.

## `update_entity`

Modifies, adds, or removes components. The block receives a mutable `ComponentMap` snapshot of the entity. If the component set changes, the entity moves to a different archetype on commit.

```das
update_entity(eid) @(eid, cmp) {
    var hp = 0
    hp = get(cmp, "hp", hp)             // read existing
    cmp |> set("hp", hp - 25)           // overwrite
    cmp.enraged := true                 // add
}
commit()

update_entity(eid) @(eid, cmp) {
    cmp |> remove("enraged")            // remove
}
commit()
```

`get(cmp, name, default)` reads with type inference from the default. `cmp |> set(name, value)` writes (insert or overwrite); type mismatch panics. `cmp |> remove(name)` removes by name. `cmp |> has(name)` checks presence without inserting.

## Stage functions

`[decs(stage = name)]` turns a function into a query that runs when you call `decs_stage("name")`. The function body becomes the query body; the function arguments become the query parameters. `decs_stage` calls `commit()` before and after running every registered function for that stage.

```das
[decs(stage = simulate)]
def simulate_particles(var p : Particle) {
    p.pos += p.vel
    p.life -= 1
}

for (_step in range(3)) {
    decs_stage("simulate")
}
```

`REQUIRE` / `REQUIRE_NOT` work as annotation arguments here too:

```das
[decs(stage = update_ai, REQUIRE = ai_turret)]
def update_ai(eid : EntityId; var turret : Turret; pos : float3) { ... }
```

## Bulk creation

Two flavors. Both are immediate (no commit needed) and both panic if called from inside a query.

**`create_entities(count, blk)`** — ComponentMap-based, all entities must share the archetype shape. The first entity's components establish the archetype; subsequent entities are checked against that hash. Pre-resizes component arrays once for the whole batch.

```das
create_entities(100) $(eid : EntityId; i : int; var cmp : ComponentMap) {
    apply_decs_template(cmp, Particle(pos = float3(float(i), 0, 0), vel = float3(0, 1, 0), life = 100))
}
```

**`create_entities\`T(count, blk)`** — the macro-generated form, ~7× faster at scale. The block receives a pre-allocated mutable struct instance — just write fields. The macro caches archetype indices on first call and writes directly into archetype storage on subsequent ones.

```das
create_entities`Particle(1000) $(eid : EntityId; i : int; var p : Particle) {
    p.pos  = float3(float(i), 0, 0)
    p.vel  = float3(0, 1, 0)
    p.life = 100
}
```

Use the backtick form for any hot-path bulk creation. Use the ComponentMap form only when the per-entity component set varies (mixed templates, runtime-decided fields).

## `from_decs` — the linq bridge

`from_decs` materializes a query as `iterator<tuple<...>>` you can feed into a linq chain. It runs the query immediately, pushes each match as a tuple into a backing array, and returns that array as a sequence. The block receives the same parameter list as `query` — same default/optional rules — but its body is empty (the macro fills it in).

```das
require daslib/linq_boost
require daslib/decs_boost

let qq = (
    from_decs($(index : int; text : string) {})
        ._where(_.index < 2)
        .reverse()
        .to_array()
)
```

Tuple field names match the query parameter names — you can write `_.index`, `_.text`, etc. inside `_<op>` shorthands.

`from_decs` is a materializing terminal — it evaluates the entire query into an `array<tuple>` up front. That's the right shape when you want sort/filter/take semantics on the query results, but it forfeits the archetype-pass-with-tight-inner-loop performance of plain `query`. For pure side-effects (mutate components, accumulate a sum, count matches), keep using `query`. Reach for `from_decs` when the consumer is genuinely a linq chain (`_where` + `_order_by` + `take` + `to_array`) or when you want the result as a value to return / pass / persist.

## Component access on archetypes

Inside `for_each_archetype` (which is what `query` expands to), the component arrays are visible directly:

| Function | Returns | Behavior on missing component |
|---|---|---|
| `get(arch, name, type<T>)` | mutable `array<T -const -& -#>` view | panics |
| `get_ro(arch, name, type<T>)` | const `array<T -const -& -#> const` view | panics |
| `get_default_ro(arch, name, default)` | `iterator<T const&>` | repeats default for `arch.size` items |
| `get_optional(arch, name, type<T?>)` | `iterator<T?>` | repeats `null` for `arch.size` items |

These are the primitives the query macro lowers to. Use them directly only when writing macros or low-level archetype iteration. For everything else, write `query()`.

`find_component_index(arch, name)` returns the index in `arch.components`, and `set_direct_at(arch, idx, eidx, value)` writes by pre-resolved index — used by `apply_decs_template_arch` and the bulk-creator macros. `set_direct(arch, name, eidx, value)` does the lookup per call.

## Single-entity reads outside a query

`get_component(eid, name, default)` returns the component value (typed from the default) or the default if the entity is dead or lacks the component. No archetype traversal.

```das
let hp  = get_component(hero, "hp", 0)            // returns int
let pos = get_component(hero, "pos", float3(0))    // returns float3
let dead = get_component(deleted_eid, "hp", -999)  // -999
```

`is_alive(eid)` — `false` for `INVALID_ENTITY_ID`, dead entities, or stale generation IDs. `entity_count()` returns total alive entities across all archetypes.

## Serialization

DECS state round-trips through `daslib/archive`:

```das
require daslib/archive

var data <- mem_archive_save(decsState)
restart()
mem_archive_load(data, decsState)
```

Components serialize through the type-info `serializer` callback that `make_component` set up — no per-component registration is needed. Stride mismatches on load panic with a clear "stride mismatch" message.

## Common gotchas

- **`commit()` / `restart()` / bulk creators / `before_gc` / `after_gc` cannot be called from inside a query.** They check `insideQuery != 0` and panic. If you need to schedule work seen by a query, queue it on `update_entity` / `delete_entity` / `create_entity` (all deferred — safe inside queries) and let the next commit apply it.
- **Don't set `"eid"` yourself.** The runtime injects an `eid` component automatically. Explicit `cmp.eid := someEid` only happens in serialization paths.
- **Two `[]` lookups on a hash table inside a single expression invalidate references.** This applies to `decsState.archetypeLookup` and `decsState.entityLookup` if you ever index them by hand — bind the lookup to a local first.
- **`query(eid)` returns nothing if the eid is dead** — the loop simply does not run. There is no "returned false" signal. Use `is_alive(eid)` first if you need to distinguish.
- **Default-valued parameter or `@optional` pointer — pick one.** The validator rejects either combined incorrectly: optional with a default → "optional argument {name} can't have a default value"; optional non-pointer → "optional argument {name} must be a pointer"; default + `var` or `&` → "argument {name} has default value, it can't be & or var".
- **Template-struct query parameters expand by field — the variable itself is not a value.** Touching `p` instead of `p.field` triggers a `static_assert_false` at compile time. If you want a real struct, copy out: `var pp = Particle(pos = p.pos, vel = p.vel, life = p.life)`.
- **`create_entities` requires identical archetypes for every entity in the batch.** A divergent component set on entity `i` panics with "all entities must have the same components". Use `create_entity` in a loop if shapes vary, or split into per-archetype batches.
- **Threaded contexts.** `decsState` is a per-context module-level `var` — DECS is not thread-safe across contexts. If you fork a thread context, run a separate DECS world there or marshal data through messages.

## Reference

- Tutorial — read first: [tutorials/language/34_decs.das](tutorials/language/34_decs.das)
- daslib sources: [daslib/decs.das](daslib/decs.das), [daslib/decs_boost.das](daslib/decs_boost.das), [daslib/decs_state.das](daslib/decs_state.das)
- Linq operators reference: [skills/linq.md](skills/linq.md)
