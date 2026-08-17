# JSON

```das
require daslib/json_boost        // re-exports daslib/json publicly — never require both
```

`sprint_json` / `sscan_json` are runtime builtins and need no require at all.

## Pick the simplest tool that fits

| Situation | Use |
|---|---|
| A daslang value ↔ a JSON string | `sprint_json(v, pretty)` / `sscan_json(json, var v)` |
| Arbitrary JSON whose shape is unknown, or a tree to mutate | `JV(x)` / `from_JV(js, type<T>)` |
| A one-off object with no struct on hand | `JV((key1 = val1, key2 = val2))` — the named-tuple form |
| The generic walker doesn't know your type | add a `def JV(x : MyType)` / `def from_JV(…)` overload |
| Genuinely dynamic shape — runtime keys, conditional inserts | build a `table<string; JsonValue?>` by hand |

The last row is a genuine last resort — it loses every field annotation and makes the structure
invisible to the type system. Almost any shape can be modeled as a struct or variant first.

## sprint_json / sscan_json

```das
let s = sprint_json(value, /*pretty=*/true)
var dst = MyStruct()
let ok = sscan_json(json_str, dst)          // bool; false on parse error
```

Covers structs, classes (deref the pointer: `sprint_json(*ptr, false)`), arrays, tables, tuples,
variants (key = the active field name), enums, bitfields, vector types, pointers, and all
primitives. Round-trips for every supported shape. **Unknown keys are silently ignored** — validate
required fields yourself if missing data is meaningful.

| Annotation | Effect |
|---|---|
| `@optional` | Skip the field on output when it equals the default/empty value |
| `@embed` | Emit a `string` field as raw JSON (the caller must supply valid JSON) |
| `@unescape` | Do not escape characters in the string output |
| `@enum_as_int` | Serialize an enum as its integer value instead of its name |
| `@rename = "json_key"` | Map to a different JSON key |

`@rename` is the standard fix when a JSON key collides with a daslang keyword: name the field
`_type` and add `@rename = "type"`. The same annotations are read by `JV` / `from_JV`.

**Array-element defaults are the one sharp edge.** "Missing fields keep their defaults" holds for
the top-level struct **you** construct. Elements of a nested `array<Struct>` that `sscan_json`
allocates itself are **zero-filled — the element type's field initializers are not applied** — so a
`rotation : float4 = float4(0,0,0,1)` comes back all-zero for any element whose JSON omits the key,
silently. When array elements carry meaningful non-zero defaults, parse with `read_json` +
`from_JV(js, type<T>)` instead: `from_JV` honors element field defaults.

## JV / from_JV and JsonValue?

```das
var js = JV(Player(name = "Hero", hp = 100))     // value → tree

var error : string
var parsed = read_json(json_str, error)          // null on failure, error set
let p2 = from_JV(parsed, type<Player>)           // tree → value
```

`JV` accepts primitives, vectors, arrays, tables, tuples, variants, enums, and structs/classes by
reflection; `JV(a, b, c, …)` (up to 10 arguments) builds an array. `from_JV`'s second argument is a
type witness, and an optional third is the fallback when the value is null, missing, or wrong-typed.

A custom type is handled by adding overloads — no macro, no annotation, overload resolution does
the rest. `JV` must return `JsonValue?`; `from_JV` takes a *value* of the type as its second
argument and the default as its third:

```das
def JV(data : MyType) : JsonValue? {
    return JV({ "field1" => JV(data.field1), "field2" => JV(data.field2) })
}

def from_JV(v : JsonValue const explicit?; ent : MyType; defV : MyType = MyType()) : MyType {
    return MyType(field1 = v?.field1 ?? defV.field1, field2 = v?.field2 ?? defV.field2)
}
```

## Navigating a tree

```das
let name  = js?.user?.name ?? "unknown"           // ?. walks object keys
let score = js?["user"]?["scores"]?[0] ?? 0       // ?[] takes keys and array indices
```

The safe operators never crash, including on a null pointer. **`js?.value` reads the struct field
`JsonValue.value`** (the underlying `JsValue` variant), *not* a JSON key named `"value"` — for that
key write `js?["value"]`.

`is` / `as` test the underlying variant through the pointer; `as` on the wrong case crashes, so
guard with `is` whenever the shape is uncertain.

| Case | Type | | Case | Type |
|---|---|---|---|---|
| `_object` | `table<string; JsonValue?>` | | `_number` | `double` |
| `_array` | `array<JsonValue?>` | | `_longint` | `int64` |
| `_string` | `string` | | `_bool` | `bool` |
| | | | `_null` | `void?` |

## Parsing, writing, embedding

- `read_json(text, var error)` takes a `string` or an `array<uint8>`; `write_json(js)` serializes
  (a null pointer writes as `"null"`).
- `try_fixing_broken_json(text)` repairs model-generated output — concatenation (`"a" + "b"`),
  trailing commas, double-quoted nesting — run it before `read_json` on such input.
- Writer settings each return the previous value, so save and restore for a scoped change:
  `set_no_trailing_zeros`, `set_no_empty_arrays`, `set_allow_duplicate_keys`.
- `%json~ … %%` embeds a compile-time literal, parsed at compile time into a runtime `JsonValue?`.
- `from_json(jv, type<Row>)` is a lazy typed row source over a JSON **array** — see queries.md.

## Building without a struct

```das
return JV((kind = meta.kind, rendered = false, payload = invoke(meta.serializer)))
// → {"kind": "…", "rendered": false, "payload": {…}}

var tab : table<string; JsonValue?>          // the dynamic-shape fallback
tab |> insert("name", JV("Alice"))
var obj = JV(tab)
```

The named-tuple constructor produces a `tuple<name:T1; …>` and `JV` walks its named fields into an
object. Limits versus `JV(struct)`: **no `@optional` / `@rename` / `@embed`**, since annotations
attach to real struct fields only, so every key is always emitted under its daslang name. It is
all-or-nothing — for conditional keys, declare a small struct with `@optional`.

The hand-built table takes a plain `var`, **never `var inscope`** — `inscope` asks for a scope-exit
finalize, and deleting a container of raw pointers frees the pointees (`error[31009] … requires
unsafe`). `JsonValue?` is a garbage-collected raw pointer, so passing it by value is a pointer copy.

**Cross-context:** a `JsonValue?` allocated in one context's heap is invalid in another. Send the
raw text across a thread boundary and re-parse, rather than the tree.
