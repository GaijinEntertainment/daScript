# JSON in daslang

Read this skill before writing or editing `.das` code that produces, consumes, or navigates JSON. The companion tutorial is [tutorials/language/30_json.das](tutorials/language/30_json.das); read it for runnable examples of every pattern below.

## Pick the right tool first

Reach for the simplest tool that fits the data. The order of preference is fixed:

| Situation | Use | Why |
|---|---|---|
| You have a daslang struct/array/value and want a JSON string, or you have a JSON string and a struct to fill | **`sprint_json(v, pretty)` / `sscan_json(json, var v)`** | Zero ceremony, no `JsonValue?` allocation, respects all field annotations (`@rename`, `@optional`, `@embed`, `@unescape`, `@enum_as_int`). Handles structs, classes, arrays, tables, tuples, variants, enums, bitfields, vector types, pointers, all basic types |
| You're navigating arbitrary JSON whose shape isn't known at compile time, or you need a `JsonValue?` tree to mutate | **`JV(x)` / `from_JV(js, type<T>)` from `daslib/json_boost`** | Generic reflection on structs/tables/arrays/tuples/variants/vectors/enums; safe-access ops `?.`, `?[]`, `??`; `is`/`as` on the underlying variant |
| The generic `JV` / `from_JV` doesn't know your custom type | **Add a `def JV(x : MyType)` / `def from_JV(...)` overload** | Function-overload dispatch picks it up automatically — no macro, no annotation |
| Building a JSON object by hand from a `table<string; JsonValue?>` | **Last resort** — only when neither sprint nor JV applies | Verbose; loses field-annotation support; the structure is invisible to types |

If you find yourself reaching for the last row, ask first whether the JSON shape can be modeled as a struct or variant — almost always it can, and `sprint_json` will be cleaner.

## `require`

```das
require daslib/json_boost
```

`json_boost` re-exports `daslib/json` publicly — never `require` both. The `sprint_json` / `sscan_json` builtins are exposed by the runtime; no extra require is needed for them.

## `sprint_json` / `sscan_json`

The default for any "value ↔ JSON string" task.

```das
let s = sprint_json(value, /*pretty=*/false)   // compact
let s = sprint_json(value, /*pretty=*/true)    // human-readable, multi-line

var dst : MyStruct
let ok = sscan_json(json_str, dst)             // returns bool
```

Notes:
- Supports the full set: structs, classes (deref via `*ptr`), arrays, tables, tuples, variants (key = active variant name), enums (string by default; int with `@enum_as_int`), bitfields, `floatN`/`intN` vectors, pointers, all primitives.
- `sscan_json` returns `bool` — false on parse error. Missing fields keep their existing/default values; **unknown keys are silently ignored**, so validate required fields yourself if needed.
- Round-trip: `sscan_json(sprint_json(x, false), var y)` reproduces `x` for any supported shape.

## Field annotations

These are read by both `sprint_json`/`sscan_json` and the generic `JV`/`from_JV`:

| Annotation | Effect |
|---|---|
| `@optional` | Skip the field on output if it equals the default/empty value |
| `@embed` | Emit a `string` field as raw JSON (no surrounding quotes — caller's responsibility to provide valid JSON) |
| `@unescape` | Don't escape characters in the string output |
| `@enum_as_int` | Serialize an enum as its integer value instead of its string name |
| `@rename = "json_key"` | Map this field to a different JSON key — the standard fix when the JSON key collides with a daslang keyword (`type`, `class`) |

Idiomatic: prefix the daslang field with `_` and use `@rename` (e.g. `@rename = "type" _type : string`).

## `JV` / `from_JV` (json_boost)

Use when working with `JsonValue?` trees — reading arbitrary JSON, building one tree to merge into another, or implementing a thin adapter that doesn't have the destination struct on hand.

```das
let p = Player(name = "Hero", hp = 100)
var js = JV(p)                              // struct → JsonValue?

var error : string
var parsed = read_json(json_str, error)
let p2 = from_JV(parsed, type<Player>)      // JsonValue? → struct
```

`JV` accepts: any primitive, vectors, tables, arrays, tuples, variants, enums, structs/classes (via reflection). `JV(a, b, c, ...)` with multiple args (up to 10) builds an array — useful for one-line literals like `JV(1920, 1080)`.

`from_JV` second argument is a type witness (`type<T>`); an optional third argument is the default if the value is null/missing/wrong-typed.

## Custom-type extension

When the generic reflection can't (or shouldn't) handle your type, add an overload. No macro, no annotation — function-overload resolution does the rest.

Pattern (see [daslib/dap.das](daslib/dap.das) `JV(data : Variable)` and [daslib/refactor.das](daslib/refactor.das) `JV(li : LineInfo)`):

```das
def JV(data : MyType) : JsonValue? {
    var inscope tab <- {
        "field1" => JV(data.field1),
        "field2" => JV(data.field2)
    }
    return JV(tab)
}

def from_JV(v : JsonValue const explicit?; ent : MyType; defV : MyType = MyType()) : MyType {
    return MyType(
        field1 = v?.field1 ?? defV.field1,
        field2 = v?.field2 ?? defV.field2
    )
}
```

For `JV` the return type **must** be `JsonValue?`. For `from_JV` the second argument is a value of the type (not `type<T>`) and the third is the default — match the signatures of the primitive overloads in [daslib/json_boost.das](daslib/json_boost.das).

## Safe access on `JsonValue?`

`json_boost` overloads the safe-access operators so they never crash:

```das
let name        = js?.user?.name ?? "unknown"          // ?. for object keys
let first_score = js?["user"]?["scores"]?[0] ?? 0      // ?[] for keys / array indices
let safe_val    = (nothing : JsonValue?)?.foo ?? "ok"  // null pointer is fine
```

**Gotcha:** `js?.value` reads the *struct field* `JsonValue.value` (the underlying `JsValue` variant), **not** a JSON key called `"value"`. For a JSON key, use `js?["value"]`.

## `is` / `as` on `JsonValue?`

Checks the underlying `JsValue` variant — works through the pointer.

```das
if (js is _string)  { let s = js as _string }
if (js is _longint) { let n = js as _longint }
```

The seven variant cases:

| Case | Type |
|---|---|
| `_object`  | `table<string; JsonValue?>` |
| `_array`   | `array<JsonValue?>` |
| `_string`  | `string` |
| `_number`  | `double` (floating point) |
| `_longint` | `int64` (integer) |
| `_bool`    | `bool` |
| `_null`    | `void?` |

`is`/`as` on the wrong case crashes — always guard with `is` first when the shape is uncertain.

## `%json~ ... %%` reader macro

Embed a compile-time JSON literal:

```das
var settings = %json~
{
    "resolution": [1920, 1080],
    "fullscreen": true,
    "title": "My Game"
}
%%
```

Runs through `read_json` at compile time; the result is a runtime `JsonValue?`. Useful for fixtures, defaults, and tests.

## Parsing & errors

```das
var error : string
var js = read_json(text, error)
if (js == null) { /* error string is set */ }
```

- `read_json(text, var error)` — returns `JsonValue?` or `null` on failure; takes either `string` or `array<uint8>`.
- `write_json(js)` — serialize a `JsonValue?`. Null pointer writes as `"null"`.
- `try_fixing_broken_json(text)` — repair LLM output: fixes string concatenation (`"a" + "b"` → `"ab"`), trailing commas, double-quoted nesting. Run before `read_json` when consuming model-generated JSON.

## Writer settings

Each setter returns the previous value — save/restore for scoped changes.

```das
let prev = set_no_trailing_zeros(true)
defer() { set_no_trailing_zeros(prev) }
// ... write_json calls in here drop trailing zeros ...
```

- `set_no_trailing_zeros(bool)` — drop `.000000` from round floats (e.g. `1` instead of `1.000000`).
- `set_no_empty_arrays(bool)` — skip empty array fields in objects.
- `set_allow_duplicate_keys(bool)` — accept duplicate keys in `read_json` (last wins).

## Manual construction (last resort)

When the JSON shape really is dynamic — mixed-type maps, schema decided at runtime — build the tree directly:

```das
var inscope tab : table<string; JsonValue?>
tab |> insert("name", JV("Alice"))
tab |> insert("age",  JV(30))
var obj = JV(tab)
```

Don't reach for this until you've ruled out modeling the data as a struct/variant. Field annotations (`@rename`, `@optional`, etc.) don't apply here — every key is hand-written.

## Common gotchas

- **Class instances**: `sprint_json` takes a value; deref the smart pointer with `sprint_json(*classPtr, false)`. `JV(*classPtr)` likewise.
- **Unknown keys are ignored** by `sscan_json` and `from_JV` — they aren't an error. Validate required fields explicitly if missing data is meaningful.
- **`@rename` for keyword keys**: JSON keys like `"type"` and `"class"` collide with daslang keywords. Name the field `_type` / `_class` and add `@rename = "type"` / `@rename = "class"`.
- **Cross-context JSON**: a `JsonValue?` allocated in one context's heap is invalid in another. If you parse on the main thread and consume on a worker, clone the strings (`clone_string`) on receive and reparse, or send the raw text instead.
- **`var inscope` for the residual smart_ptrs only** — `JsonValue?` is a raw pointer (gc_node), pass-by-value is just a pointer copy. Don't use `var inscope` on `JsonValue?`.

## Reference

- Tutorial — read this first for runnable examples of every pattern: [tutorials/language/30_json.das](tutorials/language/30_json.das)
- daslib sources: [daslib/json.das](daslib/json.das), [daslib/json_boost.das](daslib/json_boost.das)
- Real custom `JV` overloads: [daslib/dap.das](daslib/dap.das), [daslib/refactor.das](daslib/refactor.das)
- Real `sprint_json` users: [utils/daspkg/lockfile.das](utils/daspkg/lockfile.das), [daslib/debug.das](daslib/debug.das), [utils/mcp/protocol.das](utils/mcp/protocol.das)
