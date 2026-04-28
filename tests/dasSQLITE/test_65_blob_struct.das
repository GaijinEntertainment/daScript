options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// ──────────────────────────────────────────────────────────────────────────
// @sql_blob — BLOB-backed columns through daslib/archive.
//
// test_64 covers the canonical SessionSnapshot round-trip + DDL=BLOB +
// adapter dedup. This file exercises the rest of the @sql_blob surface:
//   - multiple @sql_blob fields in one struct
//   - moderately nested archive shapes (nested struct, table, deep array)
//   - schema with mixed plain + @sql_blob + @sql_json fields
//   - opacity from the daslang side: select_from materializes the struct
// ──────────────────────────────────────────────────────────────────────────

struct PointSnap {
    x : int
    y : int
}

struct VectorSnap {
    head : PointSnap
    tail : PointSnap
}

[sql_table(name = "Geometry")]
struct GeometryRow {
    @sql_primary_key Id : int
    @sql_blob Origin : PointSnap
    @sql_blob Span : VectorSnap
}

[test]
def test_two_blob_fields_in_one_struct(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<GeometryRow>)
        var origin = PointSnap(x = 10, y = 20)
        var span = VectorSnap(
            head = PointSnap(x = 0, y = 0),
            tail = PointSnap(x = 100, y = 50))
        db |> insert(GeometryRow(Id = 1, Origin <- origin, Span <- span))

        let rows <- db |> select_from(type<GeometryRow>)
        t |> equal(length(rows), 1, "one row")
        t |> equal(rows[0].Origin.x, 10, "Origin.x round-trips")
        t |> equal(rows[0].Origin.y, 20, "Origin.y round-trips")
        t |> equal(rows[0].Span.head.x, 0, "Span.head.x round-trips")
        t |> equal(rows[0].Span.tail.x, 100, "Span.tail.x round-trips")
        t |> equal(rows[0].Span.tail.y, 50, "Span.tail.y round-trips")
    }
}

[test]
def test_two_blob_fields_get_distinct_storage(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<GeometryRow>)
        let origin_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Geometry') WHERE name='Origin'",
            type<string>)
        let span_type = db |> query_scalar(
            "SELECT type FROM pragma_table_info('Geometry') WHERE name='Span'",
            type<string>)
        t |> equal(origin_type, "BLOB", "Origin DDL is BLOB")
        t |> equal(span_type, "BLOB", "Span DDL is BLOB")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Container payloads — array<int> and table<string;int> through @sql_blob.
// daslib/archive serializes these positionally; the round-trip uses
// mem_archive_save / mem_archive_load.
// ──────────────────────────────────────────────────────────────────────────

struct DeepSnap {
    seq : array<int>
    bag : table<string; int>
    nested : array<PointSnap>
}

[sql_table(name = "Deep")]
struct DeepRow {
    @sql_primary_key Id : int
    @sql_blob State : DeepSnap
}

[test]
def test_blob_payload_with_array_table_nested(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<DeepRow>)
        var snap = DeepSnap(seq <- [10, 20, 30, 40])
        snap.bag |> insert("alpha", 1)
        snap.bag |> insert("beta", 2)
        snap.nested |> push(PointSnap(x = 1, y = 2))
        snap.nested |> push(PointSnap(x = 5, y = 6))

        db |> insert(DeepRow(Id = 1, State <- snap))

        let rows <- db |> select_from(type<DeepRow>)
        t |> equal(length(rows), 1, "one deep row")
        t |> equal(length(rows[0].State.seq), 4, "seq length round-trips")
        t |> equal(rows[0].State.seq[2], 30, "seq[2] round-trips")
        t |> equal(length(rows[0].State.bag), 2, "bag size round-trips")
        let bag_alpha = rows[0].State.bag?["alpha"] ?? 0
        let bag_beta = rows[0].State.bag?["beta"] ?? 0
        t |> equal(bag_alpha, 1, "bag['alpha'] round-trips")
        t |> equal(bag_beta, 2, "bag['beta'] round-trips")
        t |> equal(length(rows[0].State.nested), 2, "nested length round-trips")
        t |> equal(rows[0].State.nested[1].x, 5, "nested[1].x round-trips")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Mixed columns — @sql_blob next to @sql_json + plain scalars
// ──────────────────────────────────────────────────────────────────────────

struct PrefBlock {
    theme : string
    lang : string
}

[sql_table(name = "MixedRow")]
struct MixedRow {
    @sql_primary_key Id : int
    Plain : int
    @sql_json Prefs : PrefBlock
    @sql_blob State : PointSnap
}

[test]
def test_mixed_blob_json_plain_columns_coexist(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<MixedRow>)
        var prefs = PrefBlock(theme = "neon", lang = "ru")
        var state = PointSnap(x = 7, y = 11)
        db |> insert(MixedRow(Id = 1, Plain = 42, Prefs <- prefs, State <- state))

        let rows <- db |> select_from(type<MixedRow>)
        t |> equal(length(rows), 1, "one mixed row")
        t |> equal(rows[0].Plain, 42, "plain int unaffected by mixed adapters")
        t |> equal(rows[0].Prefs.theme, "neon", "json prefs round-trips")
        t |> equal(rows[0].State.x, 7, "blob state.x round-trips")
        t |> equal(rows[0].State.y, 11, "blob state.y round-trips")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Two rows worth of distinct blob payloads — confirms each row carries its
// own archive bytes, no aliasing across the materialization loop.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_blob_payloads_are_per_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<DeepRow>)
        var a : DeepSnap
        a.seq <- [1, 2, 3]
        var b : DeepSnap
        b.seq <- [100, 200]
        db |> insert(DeepRow(Id = 1, State <- a))
        db |> insert(DeepRow(Id = 2, State <- b))

        let rows <- db |> select_from(type<DeepRow>)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(length(rows[0].State.seq), 3, "row 1 seq size")
        t |> equal(rows[0].State.seq[0], 1, "row 1 seq[0]")
        t |> equal(length(rows[1].State.seq), 2, "row 2 seq size")
        t |> equal(rows[1].State.seq[0], 100, "row 2 seq[0]")
    }
}
