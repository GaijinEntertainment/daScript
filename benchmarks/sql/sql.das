options gen2
options persistent_heap

require _common public

// Per-source SQL benchmark lane. One in-memory DB (cars + dealers + brands) opened once (global
// init + [init] fixtures), shared by every benchmark, closed in [finalize]. Functions stay named
// <family>_m1 so the results collector pivots (family x source) unchanged. See benchmarks/README.md.

let N = 100000

var g_db : SqlRunner <- open_sqlite(":memory:")
var g_ys : array<int>

[init]
def sql_bench_init {
    g_db |> exec("PRAGMA foreign_keys = ON")
    fixture_db(g_db, N)
    fixture_db_brands(g_db, N)
    g_ys <- make_ints(N)
}

[finalize]
def sql_bench_fini {
    delete g_ys
    delete g_db
}

def make_ints(n : int) : array<int> {
    var a : array<int>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = i
    }
    return <- a
}

[benchmark]
def aggregate_match_m1(b : B?) {
        b |> run("aggregate_match", N) {
            let total = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 200)
                                |> _select(_.price) |> sum())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def all_match_m1(b : B?) {
        b |> run("all_match", N) {
            let bad = _fold(g_db |> select_from(type<Car>) |> _where(_.price >= 9999) |> count())
            b |> accept(bad)
            if (bad != 0) {
                b->failNow()
            }
        }
}

[benchmark]
def any_match_m1(b : B?) {
        b |> run("any_match", N) {
            let opt = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> _first_opt())
            b |> accept(opt)
            if (!is_some(opt)) {
                b->failNow()
            }
        }
}

[benchmark]
def average_aggregate_m1(b : B?) {
        b |> run("average_aggregate", N) {
            let a = _fold(g_db |> select_from(type<Car>) |> _select(_.price) |> average())
            b |> accept(a)
            if (a == 0.0lf) {
                b->failNow()
            }
        }
}

[benchmark]
def bare_order_where_m1(b : B?) {
        b |> run("bare_order_where", N) {
            let rows <- _fold(g_db |> select_from(type<Car>)
                                |> _where(_.price > 500)
                                |> _order_by(_.price))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def chained_where_m1(b : B?) {
        b |> run("chained_where", N) {
            let c = _fold(g_db |> select_from(type<Car>)
                            |> _where(_.price > 500)
                            |> _where(_.year >= 2015)
                            |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def contains_match_m1(b : B?) {
        b |> run("contains_match", N) {
            // SQL doesn't have a direct CONTAINS for arbitrary values; use _any with _where.
            let opt = _fold(g_db |> select_from(type<Car>) |> _where(_.id == 50000) |> _first_opt())
            b |> accept(opt)
            if (!is_some(opt)) {
                b->failNow()
            }
        }
}

[benchmark]
def count_aggregate_m1(b : B?) {
        b |> run("count_aggregate", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_by_count_m1(b : B?) {
        b |> run("distinct_by_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _distinct_by(_.brand) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_by_order_take_m1(b : B?) {
        b |> run("distinct_by_order_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>)
                                |> _distinct_by(_.dealer_id)
                                |> _order_by(_.price)
                                |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_by_order_to_array_m1(b : B?) {
        b |> run("distinct_by_order_to_array", N) {
            let rows <- _fold(g_db |> select_from(type<Car>)
                                |> _distinct_by(_.dealer_id)
                                |> _order_by(_.price))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_count_m1(b : B?) {
        b |> run("distinct_count", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _select(_.brand) |> distinct())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_count_pred_m1(b : B?) {
        b |> run("distinct_count_pred", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _distinct_by(_.brand) |> _count(_.year > 2009))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_take_m1(b : B?) {
        b |> run("distinct_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _select(_.brand) |> distinct() |> take(3))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def element_at_match_m1(b : B?) {
        b |> run("element_at_match", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500)
                              |> skip(100) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def first_match_m1(b : B?) {
        b |> run("first_match", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def first_or_default_match_m1(b : B?) {
        b |> run("first_or_default_match", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_average_m1(b : B?) {
        b |> run("groupby_average", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              AvgPrice = _._1 |> select($(c : Car) => c.price) |> average())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_count_m1(b : B?) {
        b |> run("groupby_count", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_first_m1(b : B?) {
        b |> run("groupby_first", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              FirstCar = _._1 |> first())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_count_m1(b : B?) {
        b |> run("groupby_having_count", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _having(_._1 |> length >= 5)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_hidden_sum_m1(b : B?) {
        b |> run("groupby_having_hidden_sum", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _having(_._1 |> select($(c : Car) => c.price) |> sum > 50000)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_post_where_m1(b : B?) {
        b |> run("groupby_having_post_where", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                  |> _where(_.Total > 9000000))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_max_m1(b : B?) {
        b |> run("groupby_max", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              MaxPrice = _._1 |> select($(c : Car) => c.price) |> max())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_min_m1(b : B?) {
        b |> run("groupby_min", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              MinPrice = _._1 |> select($(c : Car) => c.price) |> min())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_multi_reducer_m1(b : B?) {
        b |> run("groupby_multi_reducer", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              N = _._1 |> length,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum(),
                                              MaxPrice   = _._1 |> select($(c : Car) => c.price) |> max())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_select_order_m1(b : B?) {
        b |> run("groupby_select_order", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                  |> _order_by(_.Total))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_select_sum_m1(b : B?) {
        b |> run("groupby_select_sum", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.price % 100)
                                  |> _select((K = _._0, S = _._1 |> select($(c : Car) => c.price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_sum_m1(b : B?) {
        b |> run("groupby_sum", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_where_count_m1(b : B?) {
        b |> run("groupby_where_count", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _where(_.price > 500)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0, N = _._1 |> length)))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_where_sum_m1(b : B?) {
        b |> run("groupby_where_sum", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                                  |> _where(_.price > 500)
                                  |> _group_by(_.brand)
                                  |> _select((Brand = _._0,
                                              TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_count_m1(b : B?) {
        b |> run("join_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _join(g_db |> select_from(type<Dealer>),
                                                               $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                               $(c : Car, d : Dealer) => (CarName = c.name, DealerName = d.name))
                                                      |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def join_groupby_count_m1(b : B?) {
        b |> run("join_groupby_count", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                |> _join(g_db |> select_from(type<Dealer>),
                         $(c : Car, d : Dealer) => c.dealer_id == d.id,
                         $(c : Car, d : Dealer) => (Brand = c.brand, DealerId = d.id))
                |> _group_by(_.Brand)
                |> _select((Brand = _._0, N = _._1 |> count())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_groupby_to_array_m1(b : B?) {
        b |> run("join_groupby_to_array", N) {
            let groups <- _fold(g_db |> select_from(type<Car>)
                |> _join(g_db |> select_from(type<Dealer>),
                         $(c : Car, d : Dealer) => c.dealer_id == d.id,
                         $(c : Car, d : Dealer) => (Brand = c.brand, Price = c.price))
                |> _group_by(_.Brand)
                |> _select((Brand = _._0,
                            Total = _._1 |> _select(_.Price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_select_m1(b : B?) {
        b |> run("join_select", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _join(g_db |> select_from(type<Dealer>),
                                                                   $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                   $(c : Car, d : Dealer) => (CarName = c.name)))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_where_count_m1(b : B?) {
        b |> run("join_where_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _join(g_db |> select_from(type<Dealer>),
                                                                $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                $(c : Car, d : Dealer) => (CarPrice = c.price, DealerId = d.id))
                                                      |> _where(_.CarPrice > 500)
                                                      |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def last_match_m1(b : B?) {
        b |> run("last_match", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500)
                              |> _order_by_descending(_.id) |> _first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def long_count_aggregate_m1(b : B?) {
        b |> run("long_count_aggregate", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> long_count())
            b |> accept(c)
            if (c == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def max_aggregate_m1(b : B?) {
        b |> run("max_aggregate", N) {
            let m = _fold(g_db |> select_from(type<Car>) |> _select(_.price) |> max())
            b |> accept(m)
            if (m == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def min_aggregate_m1(b : B?) {
        b |> run("min_aggregate", N) {
            let m = _fold(g_db |> select_from(type<Car>) |> _select(_.price) |> min())
            b |> accept(m)
            if (m > 999) {
                b->failNow()
            }
        }
}

[benchmark]
def order_by_multi_key_m1(b : B?) {
        b |> run("order_by_multi_key", N) {
            let rows <- _fold(g_db |> select_from(type<Car>)
                                |> _where(_.price > 500)
                                |> _order_by_keys((_.brand, _.price), 0u))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def order_distinct_take_m1(b : B?) {
        b |> run("order_distinct_take", N) {
            let rows <- _fold(g_db |> select_from(type<Brand>) |> _select((value = _.value)) |> distinct() |> _order_by(_.value) |> take(5))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def order_reverse_normalized_m1(b : B?) {
        b |> run("order_reverse_normalized", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by_descending(_.price) |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def order_take_desc_m1(b : B?) {
        b |> run("order_take_desc", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by_descending(_.price) |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def reverse_distinct_by_m1(b : B?) {
        b |> run("reverse_distinct_by", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> reverse() |> _distinct_by(_.brand))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def reverse_take_m1(b : B?) {
        b |> run("reverse_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by_descending(_.id) |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def reverse_take_select_m1(b : B?) {
        b |> run("reverse_take_select", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by_descending(_.id) |> take(10) |> _select(_.name))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def select_count_m1(b : B?) {
        b |> run("select_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_m1(b : B?) {
        b |> run("select_where", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_count_m1(b : B?) {
        b |> run("select_where_count", N) {
            // SQL form folds projection into the WHERE filter — the engine evaluates
            // ``price * 2 > T`` per row and counts matches.
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price * 2 > 1000) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_order_take_m1(b : B?) {
        b |> run("select_where_order_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>)
                                |> _where(_.price > 500)
                                |> _order_by(_.price)
                                |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_sum_m1(b : B?) {
        b |> run("select_where_sum", N) {
            let s = g_db |> query_scalar("SELECT SUM(price * 2) FROM Cars WHERE price * 2 > {1000}", type<int>)
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def single_match_m1(b : B?) {
        b |> run("single_match", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _where(_.id == 42) |> _first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def skip_take_m1(b : B?) {
        b |> run("skip_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> skip(1000) |> take(100))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def skip_while_match_m1(b : B?) {
        b |> run("skip_while_match", N) {
            let total = _fold(g_db |> select_from(type<Car>) |> _where(_.id >= 50000) |> count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sort_first_m1(b : B?) {
        b |> run("sort_first", N) {
            let row = _fold(g_db |> select_from(type<Car>) |> _order_by(_.price) |> _first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sort_take_m1(b : B?) {
        b |> run("sort_take", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by(_.price) |> take(10))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def sort_take_select_m1(b : B?) {
        b |> run("sort_take_select", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> _order_by(_.price) |> take(10) |> _select(_.name))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def sum_aggregate_m1(b : B?) {
        b |> run("sum_aggregate", N) {
            let s = _fold(g_db |> select_from(type<Car>) |> _select(_.price) |> sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sum_where_m1(b : B?) {
        b |> run("sum_where", N) {
            let s = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> _select(_.price) |> sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_count_m1(b : B?) {
        b |> run("take_count", N) {
            let rows <- _fold(g_db |> select_from(type<Car>) |> take(1000))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def take_count_filtered_m1(b : B?) {
        b |> run("take_count_filtered", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> take(1000) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_sum_aggregate_m1(b : B?) {
        b |> run("take_sum_aggregate", N) {
            let s = _fold(g_db |> select_from(type<Car>) |> _select(_.price) |> take(1000) |> sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_where_count_m1(b : B?) {
        b |> run("take_where_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> take(1000) |> _where(_.price > 500) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_while_match_m1(b : B?) {
        b |> run("take_while_match", N) {
            let total = _fold(g_db |> select_from(type<Car>) |> _where(_.id < 50000) |> count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def to_array_filter_m1(b : B?) {
        b |> run("to_array_filter", N) {
            let prices <- _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500) |> _select(_.price))
            b |> accept(prices)
            if (empty(prices)) {
                b->failNow()
            }
        }
}

[benchmark]
def where_join_count_m1(b : B?) {
        b |> run("where_join_count", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price > 500)
                                                       |> _join(g_db |> select_from(type<Dealer>),
                                                                $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                $(c : Car, d : Dealer) => (CarPrice = c.price, DealerId = d.id))
                                                       |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_count_pred_m1(b : B?) {
        b |> run("zip_count_pred", N) {
            let c = _fold(g_db |> select_from(type<Car>) |> _where(_.price * _.year > 1000000) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_dot_product_m1(b : B?) {
        b |> run("zip_dot_product", N) {
            let s = _fold(g_db |> select_from(type<Car>) |> _select(int64(_.price) * int64(_.year)) |> sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_dot_product_3arg_m1(b : B?) {
        b |> run("zip_dot_product_3arg", N) {
            let s = _fold(g_db |> select_from(type<Car>) |> _select(int64(_.price) * int64(_.year)) |> sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def cross_join_m1(b : B?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, 1000)
        b |> run("cross_join", 1000) {
            let rows <- _sql(db |> select_from(type<Car>)
                                |> _cross_join(db |> select_from(type<Dealer>),
                                               $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id)))
            let c = length(rows)
            b |> accept(c)
            if (c != 1000 * DEALER_COUNT) {
                b->failNow()
            }
        }
    }
}
