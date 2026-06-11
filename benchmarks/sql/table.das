options gen2
options persistent_heap

require _common public

// Per-source table benchmark lane (m7): the same Car rows keyed by id in a table<int; Car>, chains in
// each_kv form (`_.key` / `_.value.<field>`) so the kv usage-pruner picks the cheapest iterator set.
// Table slot order is unspecified — guards stay order-insensitive. Functions stay named <family>_m7.

let N = 100000

typedef CarKV = tuple<key : int; value : Car>
typedef DKV = tuple<key : int; value : Dealer>

var g_t : table<int; Car>
var g_dealers : array<Dealer>
var g_dealer_t : table<int; Dealer>   // dealers keyed by id — the join_probe srcB
var g_dealer_kv : array<DKV>          // same rows pre-materialized in slot order — the build-side baseline

[init]
def table_bench_init {
    g_t <- fixture_table(N)
    g_dealers <- fixture_dealers_array()
    for (d in g_dealers) {
        g_dealer_t |> insert(d.id, d)
    }
    for (k, v in keys(g_dealer_t), values(g_dealer_t)) {
        g_dealer_kv |> push((key = k, value = v))
    }
}

[finalize]
def table_bench_fini {
    delete g_t
    delete g_dealers
    delete g_dealer_t
    delete g_dealer_kv
}

[benchmark]
def aggregate_match_m7(b : B?) {
    b |> run("aggregate_match", N) {
        let total = _fold(unsafe(each_kv(g_t))._where(_.value.price > 200)
            .aggregate(0, $(acc : int, c : CarKV) => acc + c.value.price))
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def all_match_m7(b : B?) {
    b |> run("all_match", N) {
        let yes = _fold(unsafe(each_kv(g_t))._all(_.value.price < 9999))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def any_match_m7(b : B?) {
    b |> run("any_match", N) {
        let yes = _fold(unsafe(each_kv(g_t))._any(_.value.price > 500))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def average_aggregate_m7(b : B?) {
    b |> run("average_aggregate", N) {
        let a = _fold(unsafe(each_kv(g_t))._select(double(_.value.price)).average())
        b |> accept(a)
        if (a == 0.0lf) {
            b->failNow()
        }
    }
}

[benchmark]
def bare_last_m7(b : B?) {
    b |> run("bare_last", N) {
        let row = _fold(unsafe(each_kv(g_t)).last())
        b |> accept(row)
        if (row.key == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def bare_order_where_m7(b : B?) {
    b |> run("bare_order_where", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)
                                   ._order_by(_.value.price)
                                   .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def chained_select_collapse_m7(b : B?) {
    b |> run("chained_select_collapse", N) {
        let c = _fold(unsafe(each_kv(g_t)) |> _select(_.value.brand) |> _select(_ + 1) |> distinct() |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def chained_where_m7(b : B?) {
    b |> run("chained_where", N) {
        let c = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)
                               ._where(_.value.year >= 2015)
                               .count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def contains_match_m7(b : B?) {
    b |> run("contains_match", N) {
        let yes = _fold(unsafe(each_kv(g_t))._select(_.key).contains(50000))
        b |> accept(yes)
        if (!yes) {
            b->failNow()
        }
    }
}

[benchmark]
def count_aggregate_m7(b : B?) {
    b |> run("count_aggregate", N) {
        let c = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_by_count_m7(b : B?) {
    b |> run("distinct_by_count", N) {
        let c = _fold(unsafe(each_kv(g_t))._distinct_by(_.value.brand).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_by_order_take_m7(b : B?) {
    b |> run("distinct_by_order_take", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._distinct_by(_.value.dealer_id)._order_by(_.value.price).take(10).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_by_order_to_array_m7(b : B?) {
    b |> run("distinct_by_order_to_array", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._distinct_by(_.value.dealer_id)._order_by(_.value.price).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_count_m7(b : B?) {
    b |> run("distinct_count", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._select(_.value.brand).distinct().to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def distinct_count_pred_m7(b : B?) {
    b |> run("distinct_count_pred", N) {
        unsafe {
            let c = _fold(each_kv(g_t) |> _distinct_by(_.value.brand) |> count($(c) => c.value.year > 2009))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_take_m7(b : B?) {
    b |> run("distinct_take", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._select(_.value.brand).distinct().take(3).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def element_at_match_m7(b : B?) {
    b |> run("element_at_match", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).element_at(100))
        b |> accept(row)
        if (row.key == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def first_match_m7(b : B?) {
    b |> run("first_match", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).first())
        b |> accept(row)
        if (row.value.price <= 500) {
            b->failNow()
        }
    }
}

[benchmark]
def first_or_default_match_m7(b : B?) {
    let sentinel : CarKV = (key = -1, value = Car(id = -1, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0))
    b |> run("first_or_default_match", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).first_or_default(sentinel))
        b |> accept(row)
        if (row.key == -1) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_count_m7(b : B?) {
    b |> run("groupby_count", N) {
        let groups <- _fold(unsafe(each_kv(g_t))
                            ._group_by(_.value.brand)
                            ._select((Brand = _._0, N = _._1 |> length))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_sum_m7(b : B?) {
    b |> run("groupby_sum", N) {
        let groups <- _fold(unsafe(each_kv(g_t))
                            ._group_by(_.value.brand)
                            ._select((Brand = _._0,
                                      TotalPrice = _._1 |> select($(c : CarKV) => c.value.price) |> sum()))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def join_count_m7(b : B?) {
    b |> run("join_count", N) {
        let c = _fold(unsafe(each_kv(g_t)) |> _join(g_dealers,
                                                    $(c : CarKV, d : Dealer) => c.value.dealer_id == d.id,
                                                    $(c : CarKV, d : Dealer) => (c.value.name, d.name))
                                           |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def join_probe_m7(b : B?) {
    // srcB is a table joined on its bare key → fused key probe, no internal join hash
    b |> run("join_probe", N) {
        let c = _fold(unsafe(each_kv(g_t)) |> _join(unsafe(each_kv(g_dealer_t)),
                                $(c : CarKV, d : DKV) => c.value.dealer_id == d.key,
                                $(c : CarKV, d : DKV) => (CarPrice = c.value.price, DealerName = d.value.name))
                       |> _where(_.CarPrice > 500)
                       |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def join_probe_build_m7(b : B?) {
    // build-side baseline: identical rows from the pre-materialized kv array — the join hashes srcB
    b |> run("join_probe_build", N) {
        let c = _fold(unsafe(each_kv(g_t)) |> _join(g_dealer_kv,
                                $(c : CarKV, d : DKV) => c.value.dealer_id == d.key,
                                $(c : CarKV, d : DKV) => (CarPrice = c.value.price, DealerName = d.value.name))
                       |> _where(_.CarPrice > 500)
                       |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def join_where_count_m7(b : B?) {
    b |> run("join_where_count", N) {
        let c = _fold(unsafe(each_kv(g_t)) |> _join(g_dealers,
                                $(c : CarKV, d : Dealer) => c.value.dealer_id == d.id,
                                $(c : CarKV, d : Dealer) => (CarPrice = c.value.price, DealerId = d.id))
                       |> _where(_.CarPrice > 500)
                       |> count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def last_match_m7(b : B?) {
    b |> run("last_match", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).last())
        b |> accept(row)
        if (row.value.price <= 500) {
            b->failNow()
        }
    }
}

[benchmark]
def long_count_aggregate_m7(b : B?) {
    b |> run("long_count_aggregate", N) {
        let c = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).long_count())
        b |> accept(c)
        if (c == 0l) {
            b->failNow()
        }
    }
}

[benchmark]
def max_aggregate_m7(b : B?) {
    b |> run("max_aggregate", N) {
        let m = _fold(unsafe(each_kv(g_t))._select(_.value.price).max())
        b |> accept(m)
        if (m == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def min_aggregate_m7(b : B?) {
    b |> run("min_aggregate", N) {
        let m = _fold(unsafe(each_kv(g_t))._select(_.value.price).min())
        b |> accept(m)
        if (m > 999) {
            b->failNow()
        }
    }
}

[benchmark]
def order_by_multi_key_m7(b : B?) {
    b |> run("order_by_multi_key", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)
                                   ._order_by_keys((_.value.brand, _.value.price), 0u)
                                   .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def order_distinct_take_m7(b : B?) {
    b |> run("order_distinct_take", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._select(_.value.brand)._order_by(_).distinct().take(5).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def order_take_desc_m7(b : B?) {
    b |> run("order_take_desc", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._order_by_descending(_.value.price).take(10).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

// Point-lookup pair: the fused probe (key-equality where + first_or_default → `g_t?[k]`, O(1) total —
// per-element ns reads ~0) vs the same query forced onto the linear scan via a second always-true
// `where` (collapses to a compound `&&` predicate, which the probe matcher correctly declines).
[benchmark]
def point_lookup_m7(b : B?) {
    b |> run("point_lookup", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.key == N / 2).first_or_default(default<CarKV>))
        b |> accept(row)
        if (row.key == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def point_lookup_scan_m7(b : B?) {
    b |> run("point_lookup_scan", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.key == N / 2)._where(_.value.price >= 0)
            .first_or_default(default<CarKV>))
        b |> accept(row)
        if (row.key == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def reverse_take_m7(b : B?) {
    b |> run("reverse_take", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t).reverse().take(10).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def select_count_m7(b : B?) {
    b |> run("select_count", N) {
        let c = _fold(unsafe(each_kv(g_t))._select(_.value.price * 2).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_m7(b : B?) {
    b |> run("select_where", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_count_m7(b : B?) {
    b |> run("select_where_count", N) {
        let c = _fold(unsafe(each_kv(g_t))._select(_.value.price * 2)._where(_ > 1000).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_order_take_m7(b : B?) {
    b |> run("select_where_order_take", N) {
        let rows <- _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)
                                   ._order_by(_.value.price)
                                   .take(10)
                                   .to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def select_where_sum_m7(b : B?) {
    b |> run("select_where_sum", N) {
        let s = _fold(unsafe(each_kv(g_t))._select(_.value.price * 2)._where(_ > 1000).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def single_match_m7(b : B?) {
    b |> run("single_match", N) {
        let row = _fold(unsafe(each_kv(g_t))._where(_.key == 42).single())
        b |> accept(row)
        if (row.key != 42) {
            b->failNow()
        }
    }
}

[benchmark]
def skip_take_m7(b : B?) {
    b |> run("skip_take", N) {
        let rows <- _fold(unsafe(each_kv(g_t)).skip(1000).take(100).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

// skip_while / take_while measure the gated walk: slot order is unspecified, so the predicates are
// chosen to be uniformly false (skip nothing) / uniformly true (take everything) — full deterministic walks.
[benchmark]
def skip_while_match_m7(b : B?) {
    b |> run("skip_while_match", N) {
        let total = _fold(unsafe(each_kv(g_t))._skip_while(_.key < 0).count())
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_first_m7(b : B?) {
    b |> run("sort_first", N) {
        let row = _fold(unsafe(each_kv(g_t))._order_by(_.value.price).first())
        b |> accept(row)
        if (row.key == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_take_m7(b : B?) {
    b |> run("sort_take", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._order_by(_.value.price).take(10).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def sort_take_select_m7(b : B?) {
    b |> run("sort_take_select", N) {
        unsafe {
            let rows <- _fold(each_kv(g_t)._order_by(_.value.price).take(10)._select(_.value.name).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def sum_aggregate_m7(b : B?) {
    b |> run("sum_aggregate", N) {
        let s = _fold(unsafe(each_kv(g_t))._select(_.value.price).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def sum_where_m7(b : B?) {
    b |> run("sum_where", N) {
        let s = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)._select(_.value.price).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_m7(b : B?) {
    b |> run("take_count", N) {
        let rows <- _fold(unsafe(each_kv(g_t)).take(1000).to_array())
        b |> accept(rows)
        if (empty(rows)) {
            b->failNow()
        }
    }
}

[benchmark]
def take_count_filtered_m7(b : B?) {
    b |> run("take_count_filtered", N) {
        let c = _fold(unsafe(each_kv(g_t))._where(_.value.price > 500).take(1000).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_sum_aggregate_m7(b : B?) {
    b |> run("take_sum_aggregate", N) {
        let s = _fold(unsafe(each_kv(g_t))._select(_.value.price).take(1000).sum())
        b |> accept(s)
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_where_count_m7(b : B?) {
    b |> run("take_where_count", N) {
        let c = _fold(unsafe(each_kv(g_t)).take(1000)._where(_.value.price > 500).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def take_while_match_m7(b : B?) {
    b |> run("take_while_match", N) {
        let total = _fold(unsafe(each_kv(g_t))._take_while(_.key > 0).count())
        b |> accept(total)
        if (total == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def to_array_filter_m7(b : B?) {
    b |> run("to_array_filter", N) {
        let prices <- _fold(unsafe(each_kv(g_t))._where(_.value.price > 500)._select(_.value.price).to_array())
        b |> accept(prices)
        if (empty(prices)) {
            b->failNow()
        }
    }
}
