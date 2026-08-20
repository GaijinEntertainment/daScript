# JSON

```das
require daslib/json_boost        // re-exports daslib/json publicly — never require both
```

`sprint_json` / `sscan_json` are builtins — no require.

## Pick the simplest tool that fits

| Situation | Use |
|---|---|
| A daslang value ↔ a JSON string | `sprint_json(v, pretty)` / `sscan_json(json, var v)` |
| Unknown shape, or a tree to mutate | `JV(x)` / `from_JV(js, type<T>)` |
| A one-off object, no struct on hand | `JV((key1 = val1, key2 = val2))` — named tuple |
| The generic walker doesn't know your type | a `def JV(x : MyType)` / `def from_JV(…)` overload |
| Runtime keys, conditional inserts | a hand-built `table<string; JsonValue?>` — last resort, loses every field annotation |

## sprint_json / sscan_json

```das
let s = sprint_json(value, /*pretty=*/true)
var dst = MyStruct()
let ok = sscan_json(json_str, dst)          // bool; false on parse error
```

Round-trips structs, classes (deref the pointer: `sprint_json(*ptr, false)`), arrays, tables,
tuples, variants (key = the active field name), enums, bitfields, vector types, pointers, and all
primitives. **Unknown keys are silently ignored** — validate required fields yourself.

| Annotation | Effect |
|---|---|
| `@optional` | Skip the field on output when it equals the default/empty value |
| `@embed` | Emit a `string` field as raw JSON (the caller must supply valid JSON) |
| `@unescape` | Do not escape characters in the string output |
| `@enum_as_int` | Serialize an enum as its integer value instead of its name |
| `@rename = "json_key"` | Map to a different JSON key — the fix for a key colliding with a daslang keyword: field `_type` plus `@rename = "type"` |

`JV` / `from_JV` read the same annotations.

**Defaults hold only for the top-level struct you construct.** Elements of a nested `array<Struct>`
that `sscan_json` allocates are **zero-filled — field initializers are not applied** — so
`rotation : float4 = float4(0,0,0,1)` comes back all-zero, silently, for any element whose JSON
omits the key. For meaningful non-zero element defaults parse with `read_json` +
`from_JV(js, type<T>)`, which honors them.

## JV / from_JV and JsonValue?

```das
var js = JV(Player(name = "Hero", hp = 100))     // value → tree

var error : string
var parsed = read_json(json_str, error)          // null on failure, error set
let p2 = from_JV(parsed, type<Player>)           // tree → value
```

`JV` accepts primitives, vectors, arrays, tables, tuples, variants, enums, and structs/classes by
reflection; `JV(a, b, c, …)` (up to 10 arguments) builds an array. `from_JV`'s second argument is a
type witness; an optional third is the fallback when the value is null, missing, or wrong-typed.

Custom types are handled by overloads:

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

The safe operators never crash, even on a null pointer. **`js?.value` reads the struct field
`JsonValue.value`** (the underlying `JsValue` variant), *not* a JSON key named `"value"` — for that
key write `js?["value"]`.

`is` / `as` test the underlying variant through the pointer; `as` on the wrong case crashes —
guard with `is`.

Cases: `_object` (`table<string; JsonValue?>`), `_array` (`array<JsonValue?>`), `_string`
(`string`), `_number` (`double`), `_longint` (`int64`), `_bool` (`bool`), `_null` (`void?`).

## Parsing, writing, embedding

- `read_json(text, var error)` takes a `string` or an `array<uint8>`; `write_json(js)` serializes
  (a null pointer writes as `"null"`).
- `try_fixing_broken_json(text)` repairs model-generated output before `read_json` — concatenation
  (`"a" + "b"`), trailing commas, double-quoted nesting.
- Writer settings return the previous value (save and restore for a scoped change):
  `set_no_trailing_zeros`, `set_no_empty_arrays`, `set_allow_duplicate_keys`.
- `%json~ … %%` parses a literal at compile time into a runtime `JsonValue?`.
- `from_json(jv, type<Row>)` is a lazy typed row source over a JSON **array** — see queries.md.

## Building without a struct

```das
return JV((kind = meta.kind, rendered = false, payload = invoke(meta.serializer)))
// → {"kind": "…", "rendered": false, "payload": {…}}

var tab : table<string; JsonValue?>
tab |> insert("name", JV("Alice"))
var obj = JV(tab)
```

The named-tuple form has **no `@optional` / `@rename` / `@embed`**: every key is always emitted
under its daslang name. For conditional keys, declare a small struct with `@optional`.

The hand-built table takes a plain `var`, **never `var inscope`** — the scope-exit finalize deletes
a container of raw pointers, which frees the pointees (`error[31009] … requires unsafe`).
`JsonValue?` is a garbage-collected raw pointer; passing it by value copies the pointer.

**Cross-context:** a `JsonValue?` allocated in one context's heap is invalid in another. Send the
raw text across a thread boundary and re-parse, not the tree.
