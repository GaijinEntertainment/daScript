options gen2
require dastest/testing_boost public
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name="composite_records")]
struct CompositeRecord {
    @sql_primary_key
    partition : string
    @sql_primary_key
    ordinal : int64
    value : string
}
[sql_table(name="generated_ids")]
struct GeneratedId {
    @sql_primary_key
    id : int64
    value : string
}

[test]
def composite_primary_key_ddl_and_zero_component(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let created = db |> try_create_table(type<CompositeRecord>)
    t |> success(created |> is_none)
    if (created |> is_some) { return }
    let inserted <- db |> try_insert(
                                     CompositeRecord(partition = "alpha", ordinal = 0l,
                                                     value = "zero"))
    t |> success(!(inserted |> is_err))
    t |> equal(db |> query_scalar("SELECT ordinal FROM composite_records", type<int64>), 0l)
    let replaced <- db |> try_insert_or_replace(
                                                CompositeRecord(partition = "alpha", ordinal = 0l,
                                                                value = "replacement"))
    t |> success(!(replaced |> is_err))
    t |> equal(db |> query_scalar("SELECT value FROM composite_records", type<string>),
               "replacement")
    db |> create_table(type<GeneratedId>)
    let generated <- db |> try_insert(GeneratedId(value = "automatic"))
    t |> success(!(generated |> is_err))
    t |> equal(db |> query_scalar("SELECT id FROM generated_ids", type<int64>), 1l)
}

[test]
def row_mutations_match_every_composite_key_column(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE composite_records(partition TEXT NOT NULL," +
        "ordinal INTEGER NOT NULL,value TEXT NOT NULL,PRIMARY KEY(partition,ordinal))")
    db |> insert(CompositeRecord(partition = "alpha", ordinal = 1l, value = "first"))
    db |> insert(CompositeRecord(partition = "alpha", ordinal = 2l, value = "same partition"))
    db |> insert(CompositeRecord(partition = "beta", ordinal = 1l, value = "same ordinal"))
    let changed <- db |> try_update(
                                    CompositeRecord(partition = "alpha", ordinal = 1l,
                                                    value = "updated"))
    t |> success(!(changed |> is_err))
    t |> equal(changed |> unwrap, 1)
    let neighbour = db |> query_scalar(
        "SELECT value FROM composite_records WHERE partition='alpha' AND ordinal=2",
        type<string>)
    t |> equal(neighbour, "same partition")
    let erased <- db |> try_delete_(CompositeRecord(partition = "alpha", ordinal = 1l))
    t |> success(!(erased |> is_err))
    t |> equal(erased |> unwrap, 1)
    t |> equal(db |> query_scalar("SELECT count(*) FROM composite_records", type<int>), 2)
    let incomplete <- db |> try_delete_by_id(type<CompositeRecord>, "alpha")
    t |> success(incomplete |> is_err)
    t |> equal(db |> query_scalar("SELECT count(*) FROM composite_records", type<int>), 2)
}

[sql_table(name="numeric_first")]
struct NumericFirst {
    @sql_primary_key
    ordinal : int64
    @sql_primary_key
    partition : string
    value : string
}
[sql_table(name="without_key")]
struct WithoutKey { value : string }

[test]
def composite_integer_first_bulk_insert_and_missing_key_mutations(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<NumericFirst>)
    let rows <- [NumericFirst(ordinal = 0l, partition = "alpha", value = "zero alpha"),
        NumericFirst(ordinal = 0l, partition = "beta", value = "zero beta"),
        NumericFirst(ordinal = 1l, partition = "alpha", value = "one alpha")]
    let inserted <- db |> try_insert(rows)
    t |> success(!(inserted |> is_err))
    let ignored <- db |> try_insert_or_ignore(rows[0])
    t |> success(!(ignored |> is_err)); t |> equal(ignored |> unwrap, 0)
    let changed <- db |> try_update(
                                    NumericFirst(ordinal = 0l, partition = "missing",
                                                 value = "wrong"))
    t |> success(!(changed |> is_err)); t |> equal(changed |> unwrap, 0)
    let erased <- db |> try_delete_(NumericFirst(ordinal = 0l, partition = "missing"))
    t |> success(!(erased |> is_err)); t |> equal(erased |> unwrap, 0)
    let selected <- db |> try_delete_(rows[0])
    t |> success(!(selected |> is_err)); t |> equal(selected |> unwrap, 1)
    t |> equal(db |> query_scalar("SELECT value FROM numeric_first WHERE ordinal=0", type<string>),
               "zero beta")
    let incomplete <- db |> try_delete_by_id(type<NumericFirst>, 0l)
    t |> success(incomplete |> is_err)
    t |> equal(db |> query_scalar("SELECT count(*) FROM numeric_first", type<int>), 2)
    db |> create_table(type<WithoutKey>)
    db |> insert(WithoutKey(value = "keep"))
    let no_key <- db |> try_delete_by_id(type<WithoutKey>, 0l)
    t |> success(no_key |> is_err)
    t |> equal(db |> query_scalar("SELECT count(*) FROM without_key", type<int>), 1)
}

[sql_table(name="unique_component")]
struct UniqueComponent {
    @sql_primary_key @sql_unique
    code : string
    @sql_primary_key
    version : int64
}

[test]
def composite_components_preserve_declared_unique_and_non_null_constraints(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> create_table(type<UniqueComponent>)
    db |> insert(UniqueComponent(code = "fixed", version = 0l))
    let duplicate <- db |> try_insert(UniqueComponent(code = "fixed", version = 1l))
    t |> success(duplicate |> is_err)
    t |> success(db |> try_exec("INSERT INTO unique_component VALUES(NULL,2)") |> is_some)
    t |> success(db |> try_exec("INSERT INTO unique_component VALUES('other',NULL)") |> is_some)
    t |> equal(db |> query_scalar("SELECT count(*) FROM unique_component", type<int>), 1)
}

[sql_view(name="read_only_rows")]
struct ReadOnlyRows { value : string }
[sql_fts5(name="search_rows")]
struct SearchRows { value : string }

[test]
def scalar_delete_rejects_view_and_fts_rows_with_a_typed_error(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let view <- db |> try_delete_by_id(type<ReadOnlyRows>, 1l)
    t |> success(view |> is_err)
    let search <- db |> try_delete_by_id(type<SearchRows>, 1l)
    t |> success(search |> is_err)
    t |> equal(db |> query_scalar("SELECT count(*) FROM sqlite_master", type<int>), 0)
}
