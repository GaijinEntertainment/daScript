options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: [sql_table] DDL round-trip + row CRUD (create_table / check_schema /
// column_info / insert single+bulk / update / delete_ / delete_by_id / drop_table_if_exists).

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfCars")]
struct ConfCar {
    @sql_primary_key Id : int
    @sql_unique
    Name : string
    Price : int
}

[test(description = "create_table, check_schema, column_info and drop agree on the table shape")]
def test_conf_ddl_roundtrip(t : T?) {
    t |> run("create_table + check_schema + column_info + drop") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfCar>)
            let schema_err = db |> try_check_schema(type<ConfCar>)
            t |> success(schema_err |> is_none, "check_schema matches the struct that created the table")
            let cols <- column_info(type<ConfCar>)
            t |> equal(length(cols), 3)
            t |> equal(cols[0].name, "Id")
            t |> success(cols[0].is_pk, "Id carries @sql_primary_key")
            t |> equal(cols[1].name, "Name")
            t |> success(!cols[1].is_nullable, "non-Option field maps to NOT NULL")
            db |> drop_table_if_exists(type<ConfCar>)
            db |> create_table(type<ConfCar>)   // idempotent teardown allows re-create
        }
    }
}

[test(description = "single and bulk insert, and a sql_unique constraint, behave as the contract says")]
def test_conf_insert_select(t : T?) {
    t |> run("single insert returns a fresh id; select_from reads it back") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfCar>)
            let id1 = db |> insert(ConfCar(Id = 0, Name = "Ka", Price = 100))
            let id2 = db |> insert(ConfCar(Id = 0, Name = "Kb", Price = 200))
            t |> success(id1 > 0l, "insert reports the new row's primary key")
            t |> success(id2 > id1, "ids are fresh per insert")
            let rows <- db |> select_from(type<ConfCar>)
            t |> equal(length(rows), 2)
        }
    }
    t |> run("bulk insert is one transactional unit") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfCar>)
            db |> insert([
                ConfCar(Id = 0, Name = "a", Price = 1),
                ConfCar(Id = 0, Name = "b", Price = 2),
                ConfCar(Id = 0, Name = "c", Price = 3)])
            let rows <- db |> select_from(type<ConfCar>)
            t |> equal(length(rows), 3)
        }
    }
    t |> run("@sql_unique rejects duplicate field values") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfCar>)
            db |> insert(ConfCar(Id = 0, Name = "same", Price = 1))
            let duplicate = db |> try_insert(ConfCar(Id = 0, Name = "same", Price = 2))
            t |> success(duplicate |> is_err)
            t |> equal(_sql(db |> select_from(type<ConfCar>) |> count()), 1)
        }
    }
}

[test(description = "update, delete_ and delete_by_id act by primary key")]
def test_conf_update_delete(t : T?) {
    t |> run("update / delete_ / delete_by_id by primary key") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfCar>)
            let id = int(db |> insert(ConfCar(Id = 0, Name = "T", Price = 100)))
            db |> insert(ConfCar(Id = 0, Name = "U", Price = 200))
            let n_upd = db |> update(ConfCar(Id = id, Name = "T2", Price = 150))
            t |> equal(n_upd, 1)
            var rows <- db |> select_from(type<ConfCar>)
            var found_updated = false
            for (r in rows) {
                if (r.Id == id) {
                    found_updated = r.Name == "T2" && r.Price == 150
                }
            }
            t |> success(found_updated, "update rewrites non-PK columns of the addressed row")
            let n_del = db |> delete_(ConfCar(Id = id, Name = "T2", Price = 150))
            t |> equal(n_del, 1)
            let rest <- db |> select_from(type<ConfCar>)
            t |> equal(length(rest), 1)
            let n_by_id = db |> delete_by_id(type<ConfCar>, rest[0].Id)
            t |> equal(n_by_id, 1)
            let empty_now <- db |> select_from(type<ConfCar>)
            t |> equal(length(empty_now), 0)
        }
    }
}
