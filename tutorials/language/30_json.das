// Tutorial 30: JSON
//
// This tutorial covers:
//   - Parsing JSON strings with read_json
//   - Building JSON values with JV constructors
//   - Writing JSON back to strings with write_json
//   - Safe access operators: ?[], ?., ??
//   - is / as variant checks on JSON values
//   - Struct serialization with JV and from_JV
//   - Enum serialization
//   - Modifying JSON values
//   - Annotations: rename, optional, embed
//   - The %json~ reader macro for inline JSON
//   - Writer settings: no_trailing_zeros, no_empty_arrays
//   - Broken JSON repair with try_fixing_broken_json
//
// requires:  daslib/json, daslib/json_boost
// Run: daslang.exe tutorials/language/30_json.das

options gen2
options persistent_heap

require daslib/json_boost

// === Parsing JSON ===
// read_json takes a string and returns a JsonValue? pointer.
// If parsing fails, the error string is set and null is returned.

def parsing_basics() {
    print("=== Parsing basics ===\n")

    var error : string
    var js = read_json("\{ \"name\": \"Alice\", \"age\": 30, \"active\": true \}", error)
    if (js != null) {
        print("parsed OK\n")
    } else {
        print("parse error: {error}\n")
    }

    // JsValue is a variant with these cases:
    //   _object  — table<string; JsonValue?>
    //   _array   — array<JsonValue?>
    //   _string  — string
    //   _number  — double (floating point)
    //   _longint — int64  (integer)
    //   _bool    — bool
    //   _null    — void?
    print("is object: {js is _object}\n")
    // output: is object: true
}

// === Building JSON with JV ===
// JV() constructors wrap native values into JsonValue?.
// JVNull() creates a JSON null.

def building_json() {
    print("\n=== Building JSON ===\n")

    var str_val = JV("hello")        // _string
    var int_val = JV(42)             // _longint
    var flt_val = JV(3.14)           // _number
    var bool_val = JV(true)          // _bool
    var null_val = JVNull()          // _null

    print("string: {write_json(str_val)}\n")
    // output: string: "hello"
    print("int: {write_json(int_val)}\n")
    // output: int: 42
    print("float: {write_json(flt_val)}\n")
    // output: float: 3.14
    print("bool: {write_json(bool_val)}\n")
    // output: bool: true
    print("null: {write_json(null_val)}\n")
    // output: null: null

    // Multi-value JV creates an array:
    var arr = JV(1, "two", true)
    print("array: {write_json(arr)}\n")
    // output: array: [1,"two",true]
}

// === Writing JSON ===
// write_json converts a JsonValue? back to a string.
// It handles null pointers gracefully.

def writing_json() {
    print("\n=== Writing JSON ===\n")

    var error : string
    var js = read_json("[1, 2, 3]", error)
    print("compact: {write_json(js)}\n")
    // output: compact: [1,2,3]

    // null pointer writes as "null"
    var empty : JsonValue?
    print("null ptr: {write_json(empty)}\n")
    // output: null ptr: null
}

// === Safe access operators ===
// json_boost provides ?., ?[], and ?? for convenient access.
// These never crash — they return null/default on missing keys.

def safe_access() {
    print("\n=== Safe access ===\n")

    var error : string
    var js = read_json("\{ \"user\": \{ \"name\": \"Bob\", \"scores\": [10, 20, 30] \} \}", error)

    // ?. accesses object fields by name
    let name = js?.user?.name ?? "unknown"
    print("name: {name}\n")
    // output: name: Bob

    // ?[] accesses by string key or integer index
    let first_score = js?["user"]?["scores"]?[0] ?? 0
    print("first score: {first_score}\n")
    // output: first score: 10

    // ?? provides a default when the value is null or missing
    let missing = js?.user?.email ?? "no email"
    print("missing: {missing}\n")
    // output: missing: no email

    // Chained access on deeply missing paths returns default
    let deep_missing = js?.a?.b?.c ?? -1
    print("deep missing: {deep_missing}\n")
    // output: deep missing: -1

    // null pointer is safe
    var nothing : JsonValue?
    let safe_val = nothing?.foo ?? "safe"
    print("null ?.: {safe_val}\n")
    // output: null ?.: safe
}

// === is / as variant checks ===
// json_boost rewrites is/as on JsonValue? to check the underlying
// JsValue variant. This works even through the pointer.

def variant_checks() {
    print("\n=== Variant checks ===\n")

    var js = JV("hello")
    print("is string: {js is _string}\n")
    // output: is string: true
    print("is number: {js is _number}\n")
    // output: is number: false
    print("as string: {js as _string}\n")
    // output: as string: hello

    var jn = JV(42)
    print("is longint: {jn is _longint}\n")
    // output: is longint: true
}

// === Struct serialization ===
// json_boost provides generic JV() and from_JV() that convert
// daScript structs to/from JSON objects using compile-time reflection.

struct Player {
    name : string
    hp : int
    speed : float
}

def struct_serialization() {
    print("\n=== Struct serialization ===\n")

    // struct → JSON
    let p = Player(name = "Hero", hp = 100, speed = 5.5)
    var js = JV(p)
    print("json: {write_json(js)}\n")
    // output: json: {"name":"Hero","hp":100,"speed":5.5}

    // JSON → struct
    var error : string
    var parsed = read_json("\{ \"name\": \"Mage\", \"hp\": 80, \"speed\": 3.2 \}", error)
    var p2 = from_JV(parsed, type<Player>)
    print("name: {p2.name}, hp: {p2.hp}\n")
    // output: name: Mage, hp: 80

    // Roundtrip
    var rt = from_JV(JV(p), type<Player>)
    print("roundtrip name: {rt.name}\n")
    // output: roundtrip name: Hero
}

// === Enum serialization ===
// Enums serialize as strings by default.

enum Weapon {
    sword
    bow
    staff
}

def enum_serialization() {
    print("\n=== Enum serialization ===\n")

    var js = JV(Weapon.bow)
    print("weapon json: {write_json(js)}\n")
    // output: weapon json: "bow"

    var w = from_JV(JV("staff"), type<Weapon>)
    print("weapon: {w}\n")
    // output: weapon: staff
}

// === Modifying JSON values ===
// You can manipulate JSON trees by building them incrementally.

def modifying_json() {
    print("\n=== Modifying JSON ===\n")

    // Build an object by hand
    var tab : table<string; JsonValue?>
    tab |> insert("name", JV("Alice"))
    tab |> insert("age", JV(30))
    var obj = JV(tab)
    print("built: {write_json(obj)}\n")

    // Build an array by hand
    var items : array<JsonValue?>
    items |> push(JV(1))
    items |> push(JV("two"))
    items |> push(JV(true))
    var arr = JV(items)
    print("array: {write_json(arr)}\n")
    // output: array: [1,"two",true]
}

// === Reader macro %json~...%% ===
// The %json~ reader macro lets you embed JSON directly in daScript code.
// It parses at compile time and creates a JsonValue? at runtime.

def reader_macro_demo() {
    print("\n=== Reader macro ===\n")

    var settings = %json~
    {
        "resolution": [1920, 1080],
        "fullscreen": true,
        "title": "My Game"
    }
    %%

    let title = settings?.title ?? "untitled"
    let fs = settings?.fullscreen ?? false
    let width = settings?["resolution"]?[0] ?? 0
    print("title: {title}, fullscreen: {fs}, width: {width}\n")
    // output: title: My Game, fullscreen: true, width: 1920
}

// === Writer settings ===
// Global settings control write_json behavior:
//   set_no_trailing_zeros(true) — omit .000000 from round floats
//   set_no_empty_arrays(true)   — skip empty array fields in objects

def writer_settings() {
    print("\n=== Writer settings ===\n")

    let old_tz = set_no_trailing_zeros(true)

    var js = JV(1.0lf)
    print("no trailing zeros: {write_json(js)}\n")
    // output: no trailing zeros: 1

    var js2 = JV(3.14lf)
    print("with fraction: {write_json(js2)}\n")
    // output: with fraction: 3.14

    set_no_trailing_zeros(old_tz)
}

// === Tables and arrays ===
// JV and from_JV work with table<string;T> and array<T>.

def collections_demo() {
    print("\n=== Collections ===\n")

    // array roundtrip
    var arr <- array<int>(10, 20, 30)
    var js_arr = JV(arr)
    print("array: {write_json(js_arr)}\n")
    // output: array: [10,20,30]

    var back = from_JV(js_arr, type<array<int>>)
    print("back[1]: {back[1]}\n")
    // output: back[1]: 20

    // table roundtrip
    var tab <- { "x" => 1, "y" => 2 }
    var js_tab = JV(tab)
    print("table: {write_json(js_tab)}\n")
}

// === Broken JSON repair ===
// try_fixing_broken_json attempts to fix common issues from LLM output:
//   - String concatenation: "hello" + "world" → "helloworld"
//   - Trailing commas: [1, 2, ] → [1, 2]
//   - Nested quotes: "she said "hi"" → "she said \"hi\""

def broken_json_demo() {
    print("\n=== Broken JSON repair ===\n")

    let bad = "\{ \"msg\": \"hello\", \}"
    let fixed = try_fixing_broken_json(bad)
    var error : string
    var js = read_json(fixed, error)
    if (js != null) {
        let msg = js?.msg ?? ""
        print("repaired: {msg}\n")
        // output: repaired: hello
    }
}

// === Tuples and variants ===
// Tuples serialize as objects with "_0", "_1", ... keys.
// Variants serialize with the active variant name as key.

variant Shape {
    circle : float
    rect : float2
}

def tuples_and_variants() {
    print("\n=== Tuples and variants ===\n")

    // Tuple
    let t = (42, "hello", true)
    var js_t = JV(t)
    print("tuple: {write_json(js_t)}\n")
    // output: tuple: {"_0":42,"_1":"hello","_2":true}

    // Variant
    let s = Shape(circle = 5.0)
    var js_s = JV(s)
    print("variant: {write_json(js_s)}\n")
    // output: variant: {"circle":5}
}

// === Vector types ===
// float2/3/4 and int2/3/4 serialize as objects with x/y/z/w keys.

def vectors_demo() {
    print("\n=== Vectors ===\n")

    var js = JV(float3(1.0, 2.0, 3.0))
    print("float3: {write_json(js)}\n")
    // output: float3: {"x":1,"y":2,"z":3}

    var v = from_JV(js, type<float3>)
    print("back: {v}\n")
    // output: back: (1, 2, 3)
}

[export]
def main() {
    parsing_basics()
    building_json()
    writing_json()
    safe_access()
    variant_checks()
    struct_serialization()
    enum_serialization()
    modifying_json()
    reader_macro_demo()
    writer_settings()
    collections_demo()
    broken_json_demo()
    tuples_and_variants()
    vectors_demo()
    return true
}
