options gen2
options rtti
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/json_boost
require daslib/strings_boost
require daslib/rtti
require strings
require daslib/json

// ============================================================
//  sprint_json: basic types
// ============================================================

[test(description = "sprint_json writes every scalar type and escapes strings")]
def test_sprint_basic_types(t : T?) {
    t |> run("int") @(t : T?) {
        t |> equal(sprint_json(42, false), "42")
    }
    t |> run("uint") @(t : T?) {
        t |> equal(sprint_json(0xFFu, false), "255")
    }
    t |> run("int64") @(t : T?) {
        t |> equal(sprint_json(123456789l, false), "123456789")
    }
    t |> run("float") @(t : T?) {
        // sprint_json formats floats in its own way
        let s = sprint_json(3.14, false)
        t |> equal(s != "", true)
    }
    t |> run("double") @(t : T?) {
        let s = sprint_json(3.14lf, false)
        t |> equal(s != "", true)
    }
    t |> run("bool true") @(t : T?) {
        t |> equal(sprint_json(true, false), "true")
    }
    t |> run("bool false") @(t : T?) {
        t |> equal(sprint_json(false, false), "false")
    }
    t |> run("string") @(t : T?) {
        t |> equal(sprint_json("hello", false), "\"hello\"")
    }
    t |> run("string with escapes") @(t : T?) {
        t |> equal(sprint_json("a\nb", false), "\"a\\nb\"")
    }
}

let private pos_inf = unsafe(reinterpret<double>(0x7FF0000000000000ul))
let private quiet_nan = unsafe(reinterpret<double>(0x7FF8000000000000ul))

[test(description = "sprint_json writes NaN, Infinity and -Infinity for non-finite reals")]
def test_sprint_non_finite_reals(t : T?) {
    for ((value, text) in [(pos_inf, "Infinity"), (-pos_inf, "-Infinity"),
            (quiet_nan, "NaN"), (1.5lf, "1.5")]) {
        t |> equal(sprint_json(value, false), text)
        t |> equal(sprint_json(float(value), false), text)
    }
}

[test(description = "sprint_json writes non-finite real vector components in object, key and array form")]
def test_sprint_non_finite_vectors(t : T?) {
    let inf = float(pos_inf)
    let nan = float(quiet_nan)
    t |> equal(sprint_json(float2(inf, -inf), false),
        "\{\"x\":Infinity,\"y\":-Infinity\}")
    t |> equal(sprint_json(float3(inf, -inf, nan), false),
        "\{\"x\":Infinity,\"y\":-Infinity,\"z\":NaN\}")
    t |> equal(sprint_json(float4(inf, -inf, nan, 1.5), false),
        "\{\"x\":Infinity,\"y\":-Infinity,\"z\":NaN,\"w\":1.5\}")
    let keyed <- { float2(inf, -inf) => 7 }
    t |> equal(sprint_json(keyed, false),
        "\{\"[Infinity,-Infinity]\":7\}")
    t |> equal(sprint_json([[nan, inf], [-inf, 1.5]], false),
        "[[NaN,Infinity],[-Infinity,1.5]]")
}

// ============================================================
//  sprint_json: containers
// ============================================================

[test(description = "sprint_json writes arrays, tables and tuples including the empty ones")]
def test_sprint_containers(t : T?) {
    // NOTE: assertions use parsed JSON instead of exact string matching
    // due to JIT bug with sprint_json compact mode — see test_sprint_jit_compact_bug.
    t |> run("array") @(t : T?) {
        let json = sprint_json([1, 2, 3], false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js != null, true)
    }
    t |> run("empty array") @(t : T?) {
        let a : array<int>
        let json = sprint_json(a, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js != null, true)
    }
    t |> run("table") @(t : T?) {
        let tab <- { "x" => 1 }
        let json = sprint_json(tab, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.x ?? -1, 1)
    }
    t |> run("empty table") @(t : T?) {
        let tab : table<string; int>
        let json = sprint_json(tab, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js != null, true)
    }
    t |> run("tuple") @(t : T?) {
        let s = sprint_json((42, 3.14), false)
        // tuples serialize with _0, _1 keys
        t |> equal(s != "", true)
    }
}

// ============================================================
//  sprint_json: structs
// ============================================================

struct SimpleStruct {
    x : int
    name : string
}

[test(description = "sprint_json writes a struct as a JSON object of its fields")]
def test_sprint_struct(t : T?) {
    t |> run("basic struct") @(t : T?) {
        let s = SimpleStruct(x = 42, name = "hello")
        let json = sprint_json(s, false)
        // verify fields are present
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.x ?? 0, 42)
        t |> equal(js?.name ?? "", "hello")
    }
}

// ============================================================
//  sprint_json: enums
// ============================================================

enum Color {
    red
    green
    blue
}

[test(description = "sprint_json writes an enum as its name string")]
def test_sprint_enum(t : T?) {
    t |> run("enum as string") @(t : T?) {
        t |> equal(sprint_json(Color.green, false), "\"green\"")
    }
}

// ============================================================
//  sprint_json: variants
// ============================================================

variant TestPayload {
    code : int
    msg : string
}

[test(description = "sprint_json writes the active alternative of a variant")]
def test_sprint_variant(t : T?) {
    t |> run("variant int") @(t : T?) {
        let v = TestPayload(code = 99)
        let json = sprint_json(v, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.code ?? 0, 99)
    }
    t |> run("variant string") @(t : T?) {
        let v = TestPayload(msg = "hi")
        let json = sprint_json(v, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.msg ?? "", "hi")
    }
}

// ============================================================
//  sprint_json: classes
// ============================================================

class TestAnimal {
    species : string
    legs : int
}

[test(description = "sprint_json writes a class instance as an object of its fields")]
def test_sprint_class(t : T?) {
    t |> run("class fields") @(t : T?) {
        var a = new TestAnimal(species = "cat", legs = 4)
        let json = sprint_json(*a, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.species ?? "", "cat")
        t |> equal(js?.legs ?? 0, 4)
        unsafe { delete a; }
    }
}

// ============================================================
//  sprint_json: callables (class methods, lambdas)
//  regression: a class method / lambda field is a function pointer with no
//  JSON value, and JsonWriter emitted "name": with nothing after it, producing
//  invalid JSON that read_json rejected and that dropped the whole object.
//  Now function/block values serialize to null.
// ============================================================

class MethodClass {
    hp : int
    def heal() {}
}

struct LambdaFieldStruct {
    x : int
    fn : lambda<(a : int) : int>
}

[test(description = "sprint_json skips method fields and keeps the JSON valid for lambda fields")]
def test_sprint_callables(t : T?) {
    t |> run("class method field is skipped, json stays valid") @(t : T?) {
        var a = new MethodClass(hp = 7)
        let json = sprint_json(a, false)        // pointer form, as reported
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")                   // was "{\"hp\":7,\"heal\":}" — invalid
        t |> equal(js?.hp ?? 0, 7)              // sibling data field survives
        t |> equal(find(json, "heal") < 0, true) // method omitted entirely, not "heal":null
        unsafe { delete a; }
    }
    t |> run("struct with initialized lambda field is valid json") @(t : T?) {
        var b = LambdaFieldStruct(x = 3)
        b.fn <- @(a : int) : int => a + 1
        let json = sprint_json(b, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.x ?? 0, 3)
        unsafe { delete b.fn; }
    }
    t |> run("standalone lambda is valid json") @(t : T?) {
        var lam <- @(a : int) : int => a * 2
        let json = sprint_json(lam, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")                   // {"__lambda":null,"__finalize":null}
        unsafe { delete lam; }
    }
}

// ============================================================
//  sprint_json: null pointer
// ============================================================

[test(description = "sprint_json writes a null void pointer as null")]
def test_sprint_null(t : T?) {
    t |> run("null void pointer") @(t : T?) {
        var p : void?
        t |> equal(sprint_json(p, false), "null")
    }
}

// ============================================================
//  sprint_json: human-readable formatting
// ============================================================

[test(description = "pretty output contains newlines and compact output does not"), no_jit]
def test_sprint_pretty(t : T?) {
    t |> run("pretty has newlines") @(t : T?) {
        let s = SimpleStruct(x = 1, name = "a")
        let json = sprint_json(s, true)
        // human-readable format has newlines
        t |> equal(find(json, "\n") >= 0, true)
    }
    t |> run("compact has no newlines") @(t : T?) {
        let s = SimpleStruct(x = 1, name = "a")
        let json = sprint_json(s, false)
        // NOTE: Under JIT, sprint_json(struct, false) may produce pretty-printed
        // output for certain struct layouts.  SimpleStruct appears unaffected,
        // but if this fails under JIT, it is the same known bug documented in
        // test_sprint_jit_compact_bug below.
        t |> equal(find(json, "\n") >= 0, false)
    }
}

// ============================================================
//  @optional annotation
// ============================================================

struct OptionalFields {
    name : string
    @optional opt_bool : bool
    @optional opt_int : int
    @optional opt_float : float
    @optional opt_string : string
    @optional opt_array : array<int>
    @optional opt_table : table<string; int>
    @optional opt_ptr : void?
}

[test(description = "an optional field is omitted at its default and written otherwise")]
def test_annotation_optional(t : T?) {
    t |> run("default values are omitted") @(t : T?) {
        let s <- OptionalFields(uninitialized name = "test")
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        // name is present
        t |> equal(js?.name ?? "", "test")
        // all optional fields with defaults should be absent
        t |> equal(find(json, "opt_bool") < 0, true)
        t |> equal(find(json, "opt_int") < 0, true)
        t |> equal(find(json, "opt_float") < 0, true)
        t |> equal(find(json, "opt_string") < 0, true)
        t |> equal(find(json, "opt_array") < 0, true)
        t |> equal(find(json, "opt_table") < 0, true)
        t |> equal(find(json, "opt_ptr") < 0, true)
    }
    t |> run("non-default values are present") @(t : T?) {
        let s <- OptionalFields(uninitialized
            name = "test",
            opt_bool = true,
            opt_int = 42,
            opt_float = 1.0,
            opt_string = "hi",
            opt_array = [1],
            opt_table <- { "k" => 1 }
        )
        let json = sprint_json(s, false)
        // all optional fields with non-default values should be present
        t |> equal(find(json, "opt_bool") >= 0, true)
        t |> equal(find(json, "opt_int") >= 0, true)
        t |> equal(find(json, "opt_float") >= 0, true)
        t |> equal(find(json, "opt_string") >= 0, true)
        t |> equal(find(json, "opt_array") >= 0, true)
        t |> equal(find(json, "opt_table") >= 0, true)
    }
}

// ============================================================
//  @enum_as_int annotation
// ============================================================

struct EnumFields {
    normal : Color
    @enum_as_int as_int : Color
}

[test(description = "an enum_as_int field is written as an integer where a plain enum is written as a string")]
def test_annotation_enum_as_int(t : T?) {
    t |> run("normal enum is string") @(t : T?) {
        let s = EnumFields(normal = Color.green, as_int = Color.blue)
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.normal ?? "", "green")
    }
    t |> run("enum_as_int is integer") @(t : T?) {
        let s = EnumFields(normal = Color.red, as_int = Color.blue)
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.as_int ?? -1, 2)     // blue = 2
    }
    t |> run("enum_as_int zero") @(t : T?) {
        let s = EnumFields(normal = Color.red, as_int = Color.red)
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.as_int ?? -1, 0)     // red = 0
    }
}

// ============================================================
//  @embed annotation
// ============================================================

struct EmbedFields {
    name : string
    @embed data : string
}

[test(description = "an embed field is written as raw JSON and a string is re-parsed")]
def test_annotation_embed(t : T?) {
    // NOTE: @embed assertions use parsed JSON (read_json + type checks) instead of
    // exact substring matching because of a JIT bug: sprint_json(struct, false)
    // ignores humanReadable=false for some struct layouts under JIT, producing
    // pretty-printed output with newlines instead of compact JSON.
    // See test_sprint_jit_compact_bug below.
    t |> run("embedded array") @(t : T?) {
        let s = EmbedFields(name = "test", data = "[1,2,3]")
        let json = sprint_json(s, false)
        // data should be embedded as raw JSON, not quoted
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        // verify data is an array [1,2,3], not a string "[1,2,3]"
        t |> equal(js?.data is _array, true)
    }
    t |> run("embedded object") @(t : T?) {
        let s = EmbedFields(name = "test", data = "\{\"x\":1\}")
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        // verify data is an object with key "x", not a string
        t |> equal(js?.data?.x ?? -1, 1)
    }
    t |> run("embedded string is reparsed") @(t : T?) {
        let s = EmbedFields(name = "test", data = "[1,2,3]")
        let json = sprint_json(s, false)
        // the entire output should be valid JSON
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js != null, true)
    }
}

// ============================================================
//  @unescape annotation
// ============================================================

struct UnescapeFields {
    normal : string
    @unescape raw : string
}

[test(description = "an unescape field keeps the raw string where a plain one escapes backslashes")]
def test_annotation_unescape(t : T?) {
    t |> run("normal string escapes backslash") @(t : T?) {
        let s = UnescapeFields(normal = "C:\\Users\\test", raw = "C:\\Users\\test")
        let json = sprint_json(s, false)
        // normal field should have escaped backslashes: C:\\Users\\test
        t |> equal(find(json, "C:\\\\Users\\\\test") >= 0, true)
    }
    t |> run("unescape preserves raw") @(t : T?) {
        let s = UnescapeFields(normal = "hello", raw = "C:\\Users\\test")
        let json = sprint_json(s, false)
        // raw field should NOT have double backslashes — just single: C:\Users\test
        // Note: @unescape produces non-standard JSON (unescaped backslashes), so
        // read_json cannot parse these values.  We use value-only substring matching
        // which is format-independent (unaffected by JIT pretty-printing spaces).
        t |> equal(find(json, "C:\\Users\\test") >= 0, true)
    }
}

// ============================================================
//  @rename annotation
// ============================================================

struct RenameFields {
    @rename = "type" _type : string
    @rename = "class" _class : int
    normal : bool
}

[test(description = "a rename annotation changes the key in sprint_json, JV and from_JV")]
def test_annotation_rename(t : T?) {
    t |> run("renamed fields in sprint_json") @(t : T?) {
        let s = RenameFields(_type = "widget", _class = 5, normal = true)
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        // field appears as "type" not "_type"
        let type_val = js?["type"] ?? ""
        t |> equal(type_val, "widget")
        // field appears as "class" not "_class"
        let class_val = js?["class"] ?? 0
        t |> equal(class_val, 5)
        // normal field keeps its name
        t |> equal(js?.normal ?? false, true)
    }
    t |> run("renamed fields in JV") @(t : T?) {
        var s = RenameFields(_type = "box", _class = 3, normal = false)
        var js = JV(s)
        // JV also respects @rename
        let type_val = js?["type"] ?? ""
        t |> equal(type_val, "box")
        let class_val = js?["class"] ?? 0
        t |> equal(class_val, 3)
    }
    t |> run("from_JV with rename") @(t : T?) {
        var error : string
        var js = read_json("\{ \"type\": \"item\", \"class\": 7, \"normal\": true \}", error)
        t |> equal(error, "")
        let s = from_JV(js, type<RenameFields>)
        t |> equal(s._type, "item")
        t |> equal(s._class, 7)
        t |> equal(s.normal, true)
    }
}

// ============================================================
//  @rename without string value (bare annotation)
// ============================================================

struct BareRenameFields {
    @rename _bare : int         // no string value — strips leading underscore, key is "bare"
    normal : string
}

[test(description = "a bare rename strips the leading underscore in sprint_json, JV and from_JV")]
def test_annotation_rename_bare(t : T?) {
    t |> run("bare @rename strips underscore in sprint_json") @(t : T?) {
        let s = BareRenameFields(_bare = 42, normal = "ok")
        let json = sprint_json(s, false)
        // Validate via parsed JSON — bare @rename strips leading underscore
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        t |> equal(js?.bare ?? -1, 42)
    }
    t |> run("bare @rename strips underscore in JV") @(t : T?) {
        var s = BareRenameFields(_bare = 99, normal = "ok")
        var js = JV(s)
        // key is "bare" (underscore stripped)
        t |> equal(js?.bare ?? 0, 99)
    }
    t |> run("bare @rename from_JV") @(t : T?) {
        var error : string
        var js = read_json("\{ \"bare\": 7, \"normal\": \"yes\" \}", error)
        t |> equal(error, "")
        let s = from_JV(js, type<BareRenameFields>)
        t |> equal(s._bare, 7)
        t |> equal(s.normal, "yes")
    }
}

// ============================================================
//  Combined annotations
// ============================================================

enum TestLevel {
    low
    medium
    high
}

struct CombinedAnnotations {
    @rename = "label" _label : string
    @optional @enum_as_int pri : TestLevel
    @embed payload : string
    @unescape raw : string
}

[test(description = "annotations combine on one struct and optional does not skip an enum field")]
def test_combined_annotations(t : T?) {
    // NOTE: @embed assertion uses parsed JSON instead of substring match
    // due to JIT bug with sprint_json compact mode — see test_sprint_jit_compact_bug.
    t |> run("all annotations together") @(t : T?) {
        let s = CombinedAnnotations(
            _label = "test",
            pri = TestLevel.high,
            payload = "[1,2]",
            raw = "a\\b"
        )
        let json = sprint_json(s, false)
        var error : string
        var js = read_json(json, error)
        t |> equal(error, "")
        // @rename: "label" not "_label"
        t |> equal(js?["label"] ?? "", "test")
        // @enum_as_int: high = 2
        t |> equal(js?.pri ?? -1, 2)
        // @embed: payload is a raw JSON array, not a quoted string
        t |> equal(js?.payload is _array, true)
    }
    t |> run("optional on enum does not skip (enum type not checked)") @(t : T?) {
        let s = CombinedAnnotations(
            _label = "test",
            pri = TestLevel.low,        // low = 0
            payload = "[]",
            raw = ""
        )
        let json = sprint_json(s, false)
        // @optional does not recognize enum types in the C++ implementation,
        // so the field is always present even when the value is 0/default
        t |> equal(find(json, "\"pri\"") >= 0, true)
    }
}

[test(description = "compact output contains no newlines")]
def test_sprint_jit_compact_bug(t : T?) {
    t |> run("compact mode should not contain newlines") @(t : T?) {
        let s = CombinedAnnotations(
            _label = "test",
            pri = TestLevel.high,
            payload = "[1,2]",
            raw = "a\\b"
        )
        let json = sprint_json(s, false)
        let has_newlines = find(json, "\n") >= 0
        t |> equal(has_newlines, false)
    }
}

// ============================================================
//  sprint_json_at — addr+TypeInfo entry point
//
//  Equivalent to sprint_json but takes (addr, ti) directly instead
//  of routing through an `any` arg + SimNode_CallBase::types[].
//  Lets call sites that have only a raw `void?` address (e.g. from
//  the dasImgui widget dispatcher) serialize without a per-call-site
//  daslang thunk.
// ============================================================

var private g_int = 42
var private g_uint = 0xFFu
var private g_int64 = 123456789l
var private g_bool_t = true
var private g_bool_f = false
var private g_string = "hello"
var private g_escape = "a\nb"

struct private Pair {
    a : int
    b : float
}
var private g_pair = Pair(a = 7, b = 3.5)

var private g_array : array<int>
var private g_table : table<string; int>

[init]
def fill_array_and_table() {
    g_array |> push(1)
    g_array |> push(2)
    g_array |> push(3)
    g_table |> insert("x", 10)
    g_table |> insert("y", 20)
}

[test(description = "sprint_json_at writes the same text as sprint_json for scalars, strings, structs, arrays and tables")]
def test_sprint_json_at_matches_sprint_json(tt : T?) {
    tt |> run("int") @(t : T?) {
        let a = sprint_json(g_int, false)
        let b = sprint_json_at(unsafe(addr(g_int)), typeinfo rtti_typeinfo(g_int), false)
        t |> equal(a, b)
    }
    tt |> run("uint") @(t : T?) {
        let a = sprint_json(g_uint, false)
        let b = sprint_json_at(unsafe(addr(g_uint)), typeinfo rtti_typeinfo(g_uint), false)
        t |> equal(a, b)
    }
    tt |> run("int64") @(t : T?) {
        let a = sprint_json(g_int64, false)
        let b = sprint_json_at(unsafe(addr(g_int64)), typeinfo rtti_typeinfo(g_int64), false)
        t |> equal(a, b)
    }
    tt |> run("bool true") @(t : T?) {
        let a = sprint_json(g_bool_t, false)
        let b = sprint_json_at(unsafe(addr(g_bool_t)), typeinfo rtti_typeinfo(g_bool_t), false)
        t |> equal(a, b)
    }
    tt |> run("bool false") @(t : T?) {
        let a = sprint_json(g_bool_f, false)
        let b = sprint_json_at(unsafe(addr(g_bool_f)), typeinfo rtti_typeinfo(g_bool_f), false)
        t |> equal(a, b)
    }
    tt |> run("string") @(t : T?) {
        let a = sprint_json(g_string, false)
        let b = sprint_json_at(unsafe(addr(g_string)), typeinfo rtti_typeinfo(g_string), false)
        t |> equal(a, b)
    }
    tt |> run("string with escapes") @(t : T?) {
        let a = sprint_json(g_escape, false)
        let b = sprint_json_at(unsafe(addr(g_escape)), typeinfo rtti_typeinfo(g_escape), false)
        t |> equal(a, b)
    }
    tt |> run("struct") @(t : T?) {
        let a = sprint_json(g_pair, false)
        let b = sprint_json_at(unsafe(addr(g_pair)), typeinfo rtti_typeinfo(g_pair), false)
        t |> equal(a, b)
    }
    tt |> run("array<int>") @(t : T?) {
        let a = sprint_json(g_array, false)
        let b = sprint_json_at(unsafe(addr(g_array)), typeinfo rtti_typeinfo(g_array), false)
        t |> equal(a, b)
    }
    tt |> run("table<string;int>") @(t : T?) {
        let a = sprint_json(g_table, false)
        let b = sprint_json_at(unsafe(addr(g_table)), typeinfo rtti_typeinfo(g_table), false)
        t |> equal(a, b)
    }
}

[test(description = "sprint_json_at writes a non-empty compact form and a different human-readable form")]
def test_sprint_json_at_human_readable(t : T?) {
    let compact = sprint_json_at(unsafe(addr(g_pair)), typeinfo rtti_typeinfo(g_pair), false)
    let human = sprint_json_at(unsafe(addr(g_pair)), typeinfo rtti_typeinfo(g_pair), true)
    t |> equal(empty(compact), false)
    t |> equal(empty(human), false)
    // Human-readable form contains newlines; compact form doesn't.
    t |> equal(find(human, "\n") >= 0, true)
    t |> equal(find(compact, "\n") < 0, true)
}

// ============================================================
//  Enum-as-flags JSON serialization.
//
//  An OR-combined enum value (e.g. ImGui's `OpenOnArrow | DefaultOpen`)
//  matches no single enumerator. The reflective JSON writer used to emit
//  NOTHING for such a value, producing invalid JSON ({"flags": ,...}) that
//  dropped the entire surrounding object on parse. It now emits the number,
//  which round-trips (scanEnum already accepts a bare integer).
// ============================================================

enum private TreeFlag {
    none       = 0
    on_arrow   = 1
    default_on = 2
    framed     = 4
}

var private g_single   = TreeFlag.default_on
var private g_none     : TreeFlag
var private g_combined : TreeFlag

struct private WidgetState {
    flags  : TreeFlag
    opened : bool
}
var private g_widget : WidgetState

[init]
def setup() {
    // on_arrow | default_on == 3 — no single enumerator equals 3. Plain enums don't
    // define `|` (ImGui's flag enums get it from their C++ binding); build the combined
    // value the way it actually arrives in memory — an OR'd int reinterpreted as the enum.
    g_combined = unsafe(reinterpret<TreeFlag>(int(TreeFlag.on_arrow) | int(TreeFlag.default_on)))
    g_widget.flags = g_combined
    g_widget.opened = true
}

[test(description = "an OR-combined enum value is written as a number that round-trips instead of emitting nothing")]
def test_enum_flags_serialize(tt : T?) {
    tt |> run("single value serializes to its name") @(t : T?) {
        let s = sprint_json_at(unsafe(addr(g_single)), typeinfo rtti_typeinfo(g_single), false)
        t |> equal(s, "\"default_on\"")
    }
    tt |> run("zero value serializes to its name") @(t : T?) {
        let s = sprint_json_at(unsafe(addr(g_none)), typeinfo rtti_typeinfo(g_none), false)
        t |> equal(s, "\"none\"")
    }
    tt |> run("combined value serializes to the number, not empty") @(t : T?) {
        let s = sprint_json_at(unsafe(addr(g_combined)), typeinfo rtti_typeinfo(g_combined), false)
        t |> equal(s, "3")
    }
    tt |> run("combined value round-trips through sscan") @(t : T?) {
        var back : TreeFlag
        let ok = sscan_json_at("3", unsafe(addr(back)), typeinfo rtti_typeinfo(back))
        t |> equal(ok, true)
        t |> equal(int(back), 3)
    }
    tt |> run("struct with a combined enum field stays parseable") @(t : T?) {
        // The regression: a combined flags field used to blank out, leaving
        // {"flags": ,"opened":true} — invalid, so the whole object dropped.
        let s = sprint_json_at(unsafe(addr(g_widget)), typeinfo rtti_typeinfo(g_widget), false)
        var err : string
        let parsed = read_json(s, err)
        t |> equal(parsed != null, true)
        t |> equal(parsed?["opened"] ?? false, true)
        t |> equal(parsed?["flags"] ?? 0, 3)
    }
}
