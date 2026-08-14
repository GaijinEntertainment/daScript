options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Contract area: joins (_join / _left_join with Option-flowing right side) and
// subquery predicates (_in / _not_in / _any / _none).

require dastest/testing_boost public
require _conformance_provider

[sql_table(name = "ConfCust")]
struct ConfCust {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "ConfOrder")]
struct ConfOrder {
    @sql_primary_key Id : int
    CustId : int
    @sql_column = "amount_value"
    Amount : int
}

def private seed(db) {
    db |> create_table(type<ConfCust>)
    db |> create_table(type<ConfOrder>)
    db |> insert([
        ConfCust(Id = 1, Name = "alice"),
        ConfCust(Id = 2, Name = "bob"),
        ConfCust(Id = 3, Name = "carol")])
    db |> insert([
        ConfOrder(Id = 0, CustId = 1, Amount = 100),
        ConfOrder(Id = 0, CustId = 1, Amount = 250),
        ConfOrder(Id = 0, CustId = 2, Amount = 50)])
}

[test]
def test_conf_join(t : T?) {   // nolint:STYLE038 flat list of independent join-contract arms
    t |> run("inner _join matches pairs") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let pairs <- _sql(db |> select_from(type<ConfCust>)
                |> _join(db |> select_from(type<ConfOrder>),
                         $(c : ConfCust, o : ConfOrder) => c.Id == o.CustId,
                         $(c : ConfCust, o : ConfOrder) => (Who = c.Name, HowMuch = o.Amount))
                |> _order_by((_.Who, _.HowMuch)))
            t |> equal(length(pairs), 3)
            t |> equal(pairs[0].Who, "alice")
            t |> equal(pairs[0].HowMuch, 100)
            t |> equal(pairs[2].Who, "bob")
        }
    }
    t |> run("_join may return one exact source row") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let orders : array<ConfOrder> <- _sql(db |> select_from(type<ConfCust>)
                |> _join(db |> select_from(type<ConfOrder>),
                         $(c : ConfCust, o : ConfOrder) => c.Id == o.CustId,
                         $(c : ConfCust, o : ConfOrder) => o)
                |> _where(_.Amount >= 100)
                |> _order_by(_.Amount))
            t |> equal(length(orders), 2)
            t |> equal(orders[0].CustId, 1)
            t |> equal(orders[0].Amount, 100)
            t |> equal(orders[1].Amount, 250)
        }
    }
    t |> run("_aggregate consumes a whole-source join projection") @(tt : T?) {
        with_conf_db() $(db) {
            seed(db)
            let stats = _sql(db |> select_from(type<ConfCust>)
                |> _join(db |> select_from(type<ConfOrder>),
                         $(c : ConfCust, o : ConfOrder) => c.Id == o.CustId,
                         $(c : ConfCust, o : ConfOrder) => o)
                |> _where(_.Amount >= 100)
                |> _aggregate($(_rows) => (
                    N = _rows |> count,
                    Total = _rows |> _select(_.Amount) |> sum,
                    First = _rows |> _select(_.Amount) |> min,
                    Last = _rows |> _select(_.Amount) |> max)))
            tt |> equal(stats.N, 2)
            tt |> equal(stats.Total, 350)
            tt |> equal(stats.First, 100)
            tt |> equal(stats.Last, 250)
        }
    }
    t |> run("whole-source join results may feed another join") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let orders : array<ConfOrder> <- _sql(db |> select_from(type<ConfCust>)
                |> _join(db |> select_from(type<ConfOrder>),
                         $(c : ConfCust, o : ConfOrder) => c.Id == o.CustId,
                         $(c : ConfCust, o : ConfOrder) => o)
                |> _join(db |> select_from(type<ConfOrder>),
                         $(left : ConfOrder, right : ConfOrder) => left.Id == right.Id,
                         $(left : ConfOrder, right : ConfOrder) => right)
                |> _order_by(_.Amount))
            t |> equal(length(orders), 3)
            t |> equal(orders[0].Amount, 50)
            t |> equal(orders[1].Amount, 100)
            t |> equal(orders[2].Amount, 250)
        }
    }
    t |> run("_left_join flows unmatched right side as none") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let pairs <- _sql(db |> select_from(type<ConfCust>)
                |> _left_join(db |> select_from(type<ConfOrder>),
                              $(c : ConfCust, o : ConfOrder) => c.Id == o.CustId,
                              $(c : ConfCust, o : Option<ConfOrder>) => (Who = c.Name, Got = o |> is_some))
                |> _order_by(_.Who))
            t |> equal(length(pairs), 4)   // alice x2, bob x1, carol x1 (unmatched)
            var carol_unmatched = false
            for (p in pairs) {
                if (p.Who == "carol") {
                    carol_unmatched = !p.Got
                }
            }
            t |> success(carol_unmatched, "unmatched right side surfaces as none")
        }
    }
}

[test]
def test_conf_subqueries(t : T?) {
    t |> run("_in / _not_in over a one-column subquery") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let with_orders <- _sql(db |> select_from(type<ConfCust>)
                |> _where(_.Id |> _in(db |> select_from(type<ConfOrder>) |> _select(_.CustId)))
                |> _order_by(_.Name))
            t |> equal(length(with_orders), 2)
            t |> equal(with_orders[0].Name, "alice")
            let without <- _sql(db |> select_from(type<ConfCust>)
                |> _where(_.Id |> _not_in(db |> select_from(type<ConfOrder>) |> _select(_.CustId))))
            t |> equal(length(without), 1)
            t |> equal(without[0].Name, "carol")
        }
    }
    t |> run("_any / _none guard predicates (EXISTS / NOT EXISTS)") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            // Uncorrelated guards: the predicate references the SUBQUERY's row via `_`.
            let when_big_exists <- _sql(db |> select_from(type<ConfCust>)
                |> _where((db |> select_from(type<ConfOrder>))._any(_.Amount > 200)))
            t |> equal(length(when_big_exists), 3)
            let when_huge_exists <- _sql(db |> select_from(type<ConfCust>)
                |> _where((db |> select_from(type<ConfOrder>))._any(_.Amount > 1000)))
            t |> equal(length(when_huge_exists), 0)
            let while_no_huge <- _sql(db |> select_from(type<ConfCust>)
                |> _where((db |> select_from(type<ConfOrder>))._none(_.Amount > 1000)))
            t |> equal(length(while_no_huge), 3)
        }
    }
}
