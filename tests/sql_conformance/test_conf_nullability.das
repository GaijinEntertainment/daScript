options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: Option<T> nullability — DDL NULL-ability, bind/read of some/none,
// and the is_some / is_none / unwrap_or predicate translations.

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfUsers")]
struct ConfUser {
    @sql_primary_key Id : int
    Name : string
    @safe_when_uninitialized Nick : Option<string>
    @safe_when_uninitialized Age : Option<int>
}

def private seed(db) {
    db |> create_table(type<ConfUser>)
    db |> insert([
        ConfUser(Id = 0, Name = "a", Nick = some("ace"), Age = some(30)),
        ConfUser(Id = 0, Name = "b", Nick = none(type<string>), Age = some(40)),
        ConfUser(Id = 0, Name = "c", Nick = none(type<string>), Age = none(type<int>))])
}

[test(description = "some and none option values survive an insert and select round-trip", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_option_roundtrip(t : T?) {
    t |> run("some/none survive an insert + select_from round-trip") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            var rows <- db |> select_from(type<ConfUser>)
            t |> equal(length(rows), 3)
            for (r in rows) {
                if (r.Name == "a") {
                    t |> success(r.Nick |> is_some, "stored some() reads back some")
                    t |> equal(r.Nick |> unwrap, "ace")
                }
                if (r.Name == "c") {
                    t |> success(r.Age |> is_none, "stored none() reads back none")
                }
            }
        }
    }
}

[test(description = "is_some, is_none and unwrap_or translate to the NULL tests and COALESCE")]
def test_conf_null_predicates(t : T?) {
    t |> run("is_some / is_none translate to IS [NOT] NULL") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let with_nick <- _sql(db |> select_from(type<ConfUser>) |> _where(_.Nick |> is_some))
            t |> equal(length(with_nick), 1)
            let no_nick <- _sql(db |> select_from(type<ConfUser>)
                |> _where(_.Nick |> is_none) |> _order_by(_.Name))
            t |> equal(length(no_nick), 2)
            t |> equal(no_nick[0].Name, "b")
        }
    }
    t |> run("unwrap_or translates to COALESCE") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let grown <- _sql(db |> select_from(type<ConfUser>)
                |> _where(_.Age |> unwrap_or(0) >= 40) |> _order_by(_.Name))
            t |> equal(length(grown), 1)
            t |> equal(grown[0].Name, "b")
        }
    }
}
