options gen2

require daslib/json_boost
require dastest/testing_boost public
require daslib/linq_boost public

// The from_json generator (the for-loop / tier-2 source builder) and read_json_field (the field-pruned
// single-key read), exercised directly — edge cases the fused _fold lane never reaches: empty array,
// non-array value, null source.

struct Car {
    id    : int
    name  : string
    price : int = -1
}

def mkjson(n : int) : JsonValue? =>
    JV([for (i in range(n)); JV(Car(id = i + 1, name = "Car{i}", price = i * 10))])

[test(description = "a for loop over from_json visits every row of the array in order")]
def test_for_loop(t : T?) {
    let jv = mkjson(5)
    var ids : array<int>
    var sumPrice = 0
    for (c in from_json(jv, type<Car>)) {
        ids |> push(c.id)
        sumPrice += c.price
    }
    t |> equal(ids, [1, 2, 3, 4, 5])
    t |> equal(sumPrice, 0 + 10 + 20 + 30 + 40)
}

[test(description = "an array comprehension over from_json builds from the rows")]
def test_comprehension(t : T?) {
    let jv = mkjson(4)
    let names <- [for (c in from_json(jv, type<Car>)); c.name]
    t |> equal(names, ["Car0", "Car1", "Car2", "Car3"])
}

[test(description = "from_json over an empty JSON array yields no rows")]
def test_empty_array(t : T?) {
    var empty : array<JsonValue?>
    let jv = JV(empty)
    var n = 0
    for (_c in from_json(jv, type<Car>)) {
        n++
    }
    t |> equal(n, 0)
}

[test(description = "from_json over a JSON value that is not an array yields no rows")]
def test_non_array(t : T?) {
    // a JSON string (not an array) yields nothing
    let jv = JV("not an array")
    var n = 0
    for (_c in from_json(jv, type<Car>)) {
        n++
    }
    t |> equal(n, 0)
}

[test(description = "from_json over a null source yields no rows")]
def test_null_source(t : T?) {
    let jv : JsonValue? = null
    var n = 0
    for (_c in from_json(jv, type<Car>)) {
        n++
    }
    t |> equal(n, 0)
}

[test(description = "read_json_field returns the present value and keeps the supplied default for a missing key")]
def test_read_json_field(t : T?) {
    let jv = mkjson(3)
    var first : JsonValue const?
    for (e in jv.value as _array) {
        first = e
        break
    }
    // present keys
    t |> equal(read_json_field(first, "id", -99), 1)
    t |> equal(read_json_field(first, "name", "?"), "Car0")
    // missing key keeps the supplied default
    t |> equal(read_json_field(first, "nope", 42), 42)
    t |> equal(read_json_field(first, "missing", "fallback"), "fallback")
}

// Option-B contract: from_json is a flat-object query source (like from_xml_node) — it reads top-level
// fields BY NAME and never honors a custom whole-row from_JV(Row) override. The escape hatch for override
// semantics is to materialize the raw array with an explicit from_JV first. These tests pin both halves.

struct OverrideRow {
    name : string
    tag  : int
}

// A user override that always returns a sentinel, distinct from any by-name materialize.
[unused_argument(v, ent)]
def from_JV(v : JsonValue const explicit?; ent : OverrideRow; defV : OverrideRow = OverrideRow()) : OverrideRow =>
    OverrideRow(name = "OVERRIDDEN", tag = 999)

def mkjson_override() : JsonValue? =>
    JV([JV(OverrideRow(name = "real0", tag = 0)), JV(OverrideRow(name = "real1", tag = 1))])

[test(description = "from_json reads rows by field name and does not call a from_JV override")]
def test_from_json_ignores_override(t : T?) {
    let jv = mkjson_override()
    // from_json reads top-level fields by name — the from_JV(OverrideRow) override is NOT invoked.
    let rows <- [for (r in from_json(jv, type<OverrideRow>)); r]
    t |> equal(length(rows), 2)
    t |> equal(rows[0].name, "real0")
    t |> equal(rows[0].tag, 0)
    t |> equal(rows[1].name, "real1")
    t |> equal(rows[1].tag, 1)
}

[test(description = "the fused from_json lane also reads whole rows by name and ignores a from_JV override")]
def test_fused_lane_ignores_override(t : T?) {
    let jv = mkjson_override()
    // The fused JsonAdapter materializes whole rows by name too — override still ignored.
    let rows <- _fold(unsafe(from_json(jv, type<OverrideRow>))._where(_.tag >= 0).to_array())
    t |> equal(length(rows), 2)
    t |> equal(rows[0].name, "real0")
    t |> equal(rows[1].name, "real1")
}

[test(description = "an explicit from_JV over the raw array does call the override")]
def test_array_escape_honors_override(t : T?) {
    let jv = mkjson_override()
    // The escape hatch: an explicit from_JV(e, type<Row>) over the raw array DOES honor the override.
    let esc <- [for (e in jv.value as _array); from_JV(e, type<OverrideRow>)]
    t |> equal(length(esc), 2)
    t |> equal(esc[0].name, "OVERRIDDEN")
    t |> equal(esc[0].tag, 999)
    t |> equal(esc[1].name, "OVERRIDDEN")
}
