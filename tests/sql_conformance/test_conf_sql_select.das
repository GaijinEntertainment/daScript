options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Contract area: the `_sql` read chain — _where translation with captured-var binds,
// _order_by, take/skip, distinct, _distinct_by (provider-lowered: bare-aggregate GROUP BY
// or DISTINCT ON per caps.distinct_on), renamed-projection aliases + ordering over them,
// _first/_first_opt terminals, and the _sql_text analyzer surface (placeholder style is
// provider-rendered, so only shape is asserted).

require dastest/testing_boost public
require _conformance_provider
require strings

[sql_table(name = "ConfNums")]
struct ConfNum {
    @sql_primary_key Id : int
    Val : int
    Tag : string
}

def private seed(db) {
    db |> create_table(type<ConfNum>)
    db |> insert([
        ConfNum(Id = 0, Val = 10, Tag = "a"),
        ConfNum(Id = 0, Val = 20, Tag = "b"),
        ConfNum(Id = 0, Val = 30, Tag = "a"),
        ConfNum(Id = 0, Val = 40, Tag = "c"),
        ConfNum(Id = 0, Val = 40, Tag = "a")])
}

[test]
def test_conf_where_binds(t : T?) {
    t |> run("captured-var predicate binds and filters") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let cutoff = 25
            let rows <- _sql(db |> select_from(type<ConfNum>) |> _where(_.Val > cutoff))
            t |> equal(length(rows), 3)
            let both <- _sql(db |> select_from(type<ConfNum>)
                |> _where(_.Val > cutoff && _.Tag == "a"))
            t |> equal(length(both), 2)
        }
    }
    t |> run("two _where calls compose with AND") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let rows <- _sql(db |> select_from(type<ConfNum>)
                |> _where(_.Val >= 20)
                |> _where(_.Tag == "a"))
            t |> equal(length(rows), 2)
        }
    }
    t |> run("string predicates: starts_with / contains") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let rows <- _sql(db |> select_from(type<ConfNum>) |> _where(_.Tag |> starts_with("a")))
            t |> equal(length(rows), 3)
        }
    }
}

[test]
def test_conf_order_page_distinct(t : T?) {
    t |> run("_order_by + skip/take paginate deterministically") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let page <- _sql(db |> select_from(type<ConfNum>)
                |> _order_by(_.Val)
                |> skip(1) |> take(2))
            t |> equal(length(page), 2)
            t |> equal(page[0].Val, 20)
            t |> equal(page[1].Val, 30)
        }
    }
    t |> run("_order_by_descending") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let top <- _sql(db |> select_from(type<ConfNum>)
                |> _order_by_descending(_.Val)
                |> take(1))
            t |> equal(top[0].Val, 40)
        }
    }
    t |> run("distinct over a single-column projection") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let tags <- _sql(db |> select_from(type<ConfNum>) |> _select(_.Tag) |> distinct())
            t |> equal(length(tags), 3)
        }
    }
}

[test]
def test_conf_terminals(t : T?) {
    t |> run("_first / _first_opt") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let first_a = _sql(db |> select_from(type<ConfNum>)
                |> _where(_.Tag == "a") |> _order_by(_.Val) |> _first())
            t |> equal(first_a.Val, 10)
            let missing = _sql(db |> select_from(type<ConfNum>)
                |> _where(_.Val > 1000) |> _first_opt())
            t |> success(missing |> is_none, "_first_opt on an empty result is none")
        }
    }
}

[test]
def test_conf_distinct_by(t : T?) {
    t |> run("_distinct_by picks the first row per key in pk order") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let rows <- _sql(db |> select_from(type<ConfNum>) |> _distinct_by(_.Tag) |> _order_by(_.Val))
            t |> equal(length(rows), 3)
            t |> equal(rows[0].Val, 10)
            t |> equal(rows[0].Tag, "a")
            t |> equal(rows[1].Val, 20)
            t |> equal(rows[1].Tag, "b")
            t |> equal(rows[2].Val, 40)
            t |> equal(rows[2].Tag, "c")
        }
    }
    t |> run("reverse() |> _distinct_by picks the last row per key") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let rows <- _sql(db |> select_from(type<ConfNum>)
                |> reverse() |> _distinct_by(_.Tag) |> _order_by((_.Val, _.Tag)))
            t |> equal(length(rows), 3)
            t |> equal(rows[0].Val, 20)     // last "b" row
            t |> equal(rows[1].Val, 40)     // last "a" row is (40, "a")
            t |> equal(rows[1].Tag, "a")
            t |> equal(rows[2].Val, 40)     // last "c" row is (40, "c")
            t |> equal(rows[2].Tag, "c")
        }
    }
    t |> run("_distinct_by count / predicate count / column aggregate") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let n = _sql(db |> select_from(type<ConfNum>) |> _distinct_by(_.Tag) |> count())
            t |> equal(n, 3)
            let n2 = _sql(db |> select_from(type<ConfNum>) |> _distinct_by(_.Tag) |> _count(_.Val >= 20))
            t |> equal(n2, 2)               // first-per-tag rows are (10,a) (20,b) (40,c)
            let total = _sql(db |> select_from(type<ConfNum>) |> _distinct_by(_.Tag) |> _select(_.Val) |> sum())
            t |> equal(total, 70)
        }
    }
    t |> run("_group_by |> _select(key, first()) returns one whole row per group") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let groups <- _sql(db |> select_from(type<ConfNum>)
                |> _group_by(_.Tag)
                |> _select((Tag = _._0, First = _._1 |> first())))
            t |> equal(length(groups), 3)
            let byTag <- {for (g in groups); g.Tag => g.First.Val}
            t |> equal(byTag?["a"] ?? -1, 10)
            t |> equal(byTag?["b"] ?? -1, 20)
            t |> equal(byTag?["c"] ?? -1, 40)
        }
    }
}

[test]
def test_conf_projection_alias_order(t : T?) {
    t |> run("_order_by over a renamed projection alias actually orders") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let rows <- _sql(db |> select_from(type<ConfNum>)
                |> _select((Name = _.Tag, Cost = _.Val))
                |> _order_by_descending(_.Cost) |> take(2))
            t |> equal(length(rows), 2)
            t |> equal(rows[0].Cost, 40)
            t |> equal(rows[1].Cost, 40)
        }
    }
    t |> run("grouped aggregate orders by its projection alias") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let groups <- _sql(db |> select_from(type<ConfNum>)
                |> _group_by(_.Tag)
                |> _select((Tag = _._0, Total = _._1 |> select($(c : ConfNum) => c.Val) |> sum()))
                |> _order_by_descending(_.Total))
            t |> equal(length(groups), 3)
            t |> equal(groups[0].Tag, "a")  // a: 10+30+40 = 80
            t |> equal(groups[0].Total, 80)
            t |> equal(groups[1].Total, 40) // c
            t |> equal(groups[2].Total, 20) // b
        }
    }
}

[test]
def test_conf_sql_text_shape(t : T?) {
    t |> run("_sql_text emits SELECT/WHERE with quoted identifiers") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let cutoff = 5
            let sql = _sql_text(db |> select_from(type<ConfNum>) |> _where(_.Val > cutoff))
            t |> success(sql |> starts_with("SELECT "), "starts with SELECT")
            t |> success(find(sql, "FROM \"ConfNums\"") >= 0, "table name is quoted")
            t |> success(find(sql, "WHERE") >= 0, "predicate renders a WHERE clause")
            t |> success(find(sql, "{cutoff}") < 0, "captured var is bound, not inlined")
        }
    }
}
