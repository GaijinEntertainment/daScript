// Tutorial 57: TOML
//
// This tutorial covers:
//   - Parsing TOML 1.0 documents with read_toml
//   - The TOML -> JsValue type mapping (tables, arrays, scalars, datetimes)
//   - Querying the result with json_boost accessors: ?., ?[], ??
//   - Nested tables and dotted keys
//   - Arrays and arrays-of-tables
//   - Inline tables
//   - Integers in decimal / hex / octal / binary
//   - Floats including inf and nan
//   - Date-times preserved as RFC-3339 strings
//   - Error handling for malformed input
//   - Deserializing straight into a struct with from_JV
//
// requires:  daslib/toml, daslib/json_boost
// Run: daslang.exe tutorials/language/57_toml.das

options gen2

require daslib/toml
require daslib/json_boost

// === Parsing basics ===
// read_toml takes a TOML string and an out error string, and returns a
// JsonValue? — the SAME tree shape daslib/json produces. A TOML document
// is always a table, so a successful parse is always an _object.

def parsing_basics() {
    print("=== Parsing basics ===\n")

    var error : string
    var doc = read_toml("title = \"daslang\"
version = 3
stable = true", error)

    if (doc == null) {
        print("parse error: {error}\n")
        return
    }
    print("is object: {doc is _object}\n")
    // output: is object: true

    // Top-level keys are accessed exactly like JSON, via json_boost's ?. and ??
    print("title: {doc?.title ?? "?"}\n")
    // output: title: daslang
    print("version: {doc?.version ?? 0}\n")
    // output: version: 3
    print("stable: {doc?.stable ?? false}\n")
    // output: stable: true
}

// === Type mapping ===
// Each TOML value maps to one JsValue case:
//   table / inline table     -> _object
//   array / array-of-tables  -> _array
//   string (all four forms)  -> _string  (already unescaped)
//   integer (any base)       -> _longint (int64)
//   float (incl inf / nan)   -> _number  (double)
//   boolean                  -> _bool
//   date-time / date / time  -> _string  (RFC-3339 lexical form preserved)

def type_mapping() {
    print("\n=== Type mapping ===\n")

    var error : string
    var doc = read_toml("s = \"text\"
i = 42
f = 3.14
b = false
when = 2026-06-12T08:30:00Z", error)

    print("s   is string : {doc?.s is _string}\n")    // true
    print("i   is longint: {doc?.i is _longint}\n")   // true
    print("f   is number : {doc?.f is _number}\n")    // true
    print("b   is bool   : {doc?.b is _bool}\n")      // true
    print("when is string: {doc?.when is _string}\n") // true — datetime kept as text
}

// === Nested tables and dotted keys ===
// [section] headers and dotted keys both build nested _object tables.
// Reach into them with chained ?. or ?[] just like JSON.

def nested_tables() {
    print("\n=== Nested tables ===\n")

    var error : string
    var doc = read_toml("[server]
host = \"localhost\"
port = 8080

[server.tls]
enabled = true

[owner.contact]
email = \"boris@example.com\"", error)

    print("host: {doc?.server?.host ?? "?"}\n")
    // output: host: localhost
    print("port: {doc?["server"]?["port"] ?? 0}\n")
    // output: port: 8080
    print("tls : {doc?.server?.tls?.enabled ?? false}\n")
    // output: tls : true
    print("mail: {doc?.owner?.contact?.email ?? "?"}\n")
    // output: mail: boris@example.com

    // Missing paths fall through to the default — never a crash.
    print("missing: {doc?.server?.timeout ?? -1}\n")
    // output: missing: -1
}

// === Arrays and arrays-of-tables ===
// A bare array becomes an _array of scalars. [[name]] repeated builds an
// _array of _object tables — one element per [[name]] block.

def arrays() {
    print("\n=== Arrays ===\n")

    var error : string
    var doc = read_toml("ports = [8000, 8001, 8002]

[[user]]
name = \"alice\"
admin = true

[[user]]
name = \"bob\"
admin = false", error)

    // Scalar array — index with ?[]
    print("ports[1]: {doc?.ports?[1] ?? 0}\n")
    // output: ports[1]: 8001

    // Array-of-tables — index, then field
    print("user[0].name : {doc?.user?[0]?.name ?? "?"}\n")
    // output: user[0].name : alice
    print("user[1].admin: {doc?.user?[1]?.admin ?? true}\n")
    // output: user[1].admin: false

    // Walk the array of tables
    let users = doc?.user
    if (users != null && users is _array) {
        for (u in users as _array) {
            print("  - {u?.name ?? "?"} (admin={u?.admin ?? false})\n")
        }
    }
}

// === Inline tables ===
// { k = v, ... } on one line is an inline table — also an _object.

def inline_tables() {
    print("\n=== Inline tables ===\n")

    var error : string
    var doc = read_toml("point = \{ x = 1, y = 2 \}
origin = \{ pos = \{ x = 0, y = 0 \}, tag = \"O\" \}", error)

    print("point.x: {doc?.point?.x ?? 0}\n")
    // output: point.x: 1
    print("origin.tag: {doc?.origin?.tag ?? "?"}\n")
    // output: origin.tag: O
    print("origin.pos.y: {doc?.origin?.pos?.y ?? -1}\n")
    // output: origin.pos.y: 0
}

// === Integer bases ===
// TOML accepts decimal, 0x hex, 0o octal, and 0b binary integers, plus
// underscore digit separators. All decode to the same _longint value.

def integer_bases() {
    print("\n=== Integer bases ===\n")

    var error : string
    var doc = read_toml("dec = 1_000
hex = 0xFF
oct = 0o755
bin = 0b1010", error)

    print("dec: {doc?.dec ?? 0}\n")   // 1000
    print("hex: {doc?.hex ?? 0}\n")   // 255
    print("oct: {doc?.oct ?? 0}\n")   // 493
    print("bin: {doc?.bin ?? 0}\n")   // 10
}

// === Floats including inf / nan ===
// Exponents, +/-inf and nan are all valid TOML floats -> _number.

def floats_special() {
    print("\n=== Floats ===\n")

    var error : string
    var doc = read_toml("avogadro = 6.022e23
neg = -2.5
huge = inf
nope = nan", error)

    print("avogadro: {doc?.avogadro ?? 0.0}\n")
    print("neg: {doc?.neg ?? 0.0}\n")
    print("huge is number: {doc?.huge is _number}\n")  // true
    print("nope is number: {doc?.nope is _number}\n")  // true
}

// === Date-times ===
// JSON has no native date type, so TOML offset-datetime, local-datetime,
// local-date and local-time are preserved verbatim as _string. You keep
// the exact lexical form the document used.

def datetimes() {
    print("\n=== Date-times ===\n")

    var error : string
    var doc = read_toml("created = 2026-06-12T08:30:00Z
day = 2026-06-12
clock = 08:30:00", error)

    print("created: {doc?.created ?? "?"}\n")
    // output: created: 2026-06-12T08:30:00Z
    print("day: {doc?.day ?? "?"}\n")
    // output: day: 2026-06-12
    print("clock: {doc?.clock ?? "?"}\n")
    // output: clock: 08:30:00
}

// === Error handling ===
// read_toml is fail-fast: any malformed input returns null and fills the
// error out-param with a descriptive message — same contract as read_json.

def error_handling() {
    print("\n=== Error handling ===\n")

    var error : string
    var doc = read_toml("key = = broken", error)
    print("doc is null: {doc == null}\n")
    // output: doc is null: true
    print("error non-empty: {!empty(error)}\n")
    // output: error non-empty: true
}

// === Deserializing into a struct ===
// Because read_toml yields the same JsonValue? tree as JSON, every
// json_boost helper works on it — including from_JV, which fills a struct
// by compile-time reflection. This turns a config file into typed data.

struct ServerConfig {
    host : string
    port : int
    workers : int
}

def struct_deserialize() {
    print("\n=== Deserialize into a struct ===\n")

    var error : string
    var doc = read_toml("host = \"0.0.0.0\"
port = 9090
workers = 4", error)

    let cfg = from_JV(doc, type<ServerConfig>)
    print("host={cfg.host} port={cfg.port} workers={cfg.workers}\n")
    // output: host=0.0.0.0 port=9090 workers=4
}

[export]
def main() {
    parsing_basics()
    type_mapping()
    nested_tables()
    arrays()
    inline_tables()
    integer_bases()
    floats_special()
    datetimes()
    error_handling()
    struct_deserialize()
}
