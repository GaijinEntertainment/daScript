options gen2
options persistent_heap

require _common public

// Per-source XML benchmark lane. One parsed DOM (100k cars) held in [init], shared by every
// benchmark, freed in [finalize]. Functions stay named <family>_m5f.

let N = 100000

var g_doc : xml_document?
var g_root : xml_node
var g_dealers : array<Dealer>
var g_ys : array<int>

[init]
def xml_bench_init {
    let xml = fixture_xml_string(N)
    g_doc = new xml_document()
    var result : xml_parse_result
    if (!load_string(g_doc, xml, result, parse_default)) {
        panic("xml fixture parse failed")
    }
    g_root = g_doc.document_element
    g_dealers <- fixture_dealers_array()
    g_ys <- make_ints(N)
}

[finalize]
def xml_bench_fini {
    delete g_dealers
    delete g_ys
    unsafe { delete g_doc }
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
def aggregate_match_m5f(b : B?) {
        b |> run("aggregate_match", N) {
            let total = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 200)
                .aggregate(0, $(acc : int, c : Car) => acc + c.price))
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def all_match_m5f(b : B?) {
        b |> run("all_match", N) {
            let yes = _fold(unsafe(from_xml_node(g_root, type<Car>))._all(_.price < 9999))
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
}

[benchmark]
def any_match_m5f(b : B?) {
        b |> run("any_match", N) {
            let yes = _fold(unsafe(from_xml_node(g_root, type<Car>))._any(_.price > 500))
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
}

[benchmark]
def average_aggregate_m5f(b : B?) {
        b |> run("average_aggregate", N) {
            let a = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(double(_.price)).average())
            b |> accept(a)
            if (a == 0.0lf) {
                b->failNow()
            }
        }
}

[benchmark]
def bare_last_m5f(b : B?) {
        b |> run("bare_last", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>)).last())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def bare_order_where_m5f(b : B?) {
        b |> run("bare_order_where", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)
                                       ._order_by(_.price)
                                       .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def chained_select_collapse_m5f(b : B?) {
        b |> run("chained_select_collapse", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _select(_.brand) |> _select(_ + 1) |> distinct() |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def chained_where_m5f(b : B?) {
        b |> run("chained_where", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)
                                   ._where(_.year >= 2015)
                                   .count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def contains_match_m5f(b : B?) {
        b |> run("contains_match", N) {
            let yes = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.id).contains(50000))
            b |> accept(yes)
            if (!yes) {
                b->failNow()
            }
        }
}

[benchmark]
def count_aggregate_m5f(b : B?) {
        b |> run("count_aggregate", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_by_count_m5f(b : B?) {
        b |> run("distinct_by_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._distinct_by(_.brand).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_by_order_take_m5f(b : B?) {
        b |> run("distinct_by_order_take", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._distinct_by(_.dealer_id)._order_by(_.price).take(10).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def distinct_by_order_to_array_m5f(b : B?) {
        b |> run("distinct_by_order_to_array", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._distinct_by(_.dealer_id)._order_by(_.price).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def distinct_count_m5f(b : B?) {
        b |> run("distinct_count", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.brand).distinct().to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def distinct_count_pred_m5f(b : B?) {
        b |> run("distinct_count_pred", N) {
            unsafe {
                let c = _fold(from_xml_node(g_root, type<Car>) |> _distinct_by(_.brand) |> count($(c) => c.year > 2009))
                b |> accept(c)
                if (c == 0) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def distinct_take_m5f(b : B?) {
        b |> run("distinct_take", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._select(_.brand).distinct().take(3).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def element_at_match_m5f(b : B?) {
        b |> run("element_at_match", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).element_at(100))
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def first_match_m5f(b : B?) {
        b |> run("first_match", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).first())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def first_or_default_match_m5f(b : B?) {
    let sentinel = Car(id = -1, name = "none", price = 0, brand = 0, year = 0, dealer_id = 0)


        b |> run("first_or_default_match", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).first_or_default(sentinel))
            b |> accept(row)
            if (row.id == -1) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_average_m5f(b : B?) {
        b |> run("groupby_average", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          AvgPrice = _._1 |> select($(c : Car) => c.price) |> average()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_count_m5f(b : B?) {
        b |> run("groupby_count", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_first_m5f(b : B?) {
        b |> run("groupby_first", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          FirstCar = _._1 |> first()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_count_m5f(b : B?) {
        b |> run("groupby_having_count", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._having(_._1 |> length >= 5)
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_hidden_sum_m5f(b : B?) {
        b |> run("groupby_having_hidden_sum", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._having(_._1 |> select($(c : Car) => c.price) |> sum > 50000)
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_having_post_where_m5f(b : B?) {
        b |> run("groupby_having_post_where", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                ._where(_.Total > 9000000)
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_max_m5f(b : B?) {
        b |> run("groupby_max", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          MaxPrice = _._1 |> select($(c : Car) => c.price) |> max()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_min_m5f(b : B?) {
        b |> run("groupby_min", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          MinPrice = _._1 |> select($(c : Car) => c.price) |> min()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_multi_reducer_m5f(b : B?) {
        b |> run("groupby_multi_reducer", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          N = _._1 |> length,
                                          TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum(),
                                          MaxPrice   = _._1 |> select($(c : Car) => c.price) |> max()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_select_order_m5f(b : B?) {
        b |> run("groupby_select_order", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          Total = _._1 |> select($(c : Car) => c.price) |> sum()))
                                ._order_by(_.Total)
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_select_sum_m5f(b : B?) {
        b |> run("groupby_select_sum", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._select(_.price)
                                ._group_by(_ % 100)
                                ._select((K = _._0, S = _._1 |> sum()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_sum_m5f(b : B?) {
        b |> run("groupby_sum", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_where_count_m5f(b : B?) {
        b |> run("groupby_where_count", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._where(_.price > 500)
                                ._group_by(_.brand)
                                ._select((Brand = _._0, N = _._1 |> length))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def groupby_where_sum_m5f(b : B?) {
        b |> run("groupby_where_sum", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>))
                                ._where(_.price > 500)
                                ._group_by(_.brand)
                                ._select((Brand = _._0,
                                          TotalPrice = _._1 |> select($(c : Car) => c.price) |> sum()))
                                .to_array())
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_count_m5f(b : B?) {
        b |> run("join_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _join(g_dealers,
                                                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                                                          $(c : Car, d : Dealer) => (c.name, d.name))
                                                                 |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def join_groupby_count_m5f(b : B?) {
        b |> run("join_groupby_count", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _join(g_dealers,
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
def join_groupby_to_array_m5f(b : B?) {
        b |> run("join_groupby_to_array", N) {
            let groups <- _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _join(g_dealers,
                                          $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                          $(c : Car, _d : Dealer) => (Brand = c.brand, Price = c.price))
                                 |> _group_by(_.Brand)
                                 |> _select((Brand = _._0,
                                             Total = _._1 |> select($(t : tuple<Brand : int; Price : int>) => t.Price) |> sum())))
            b |> accept(groups)
            if (empty(groups)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_select_m5f(b : B?) {
        b |> run("join_select", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _join(g_dealers,
                                        $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                        $(c : Car, d : Dealer) => (CarName = c.name, DealerId = d.id))
                               |> _select(_.CarName))
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def join_where_count_m5f(b : B?) {
        b |> run("join_where_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _join(g_dealers,
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
def last_match_m5f(b : B?) {
        b |> run("last_match", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).last())
            b |> accept(row)
            if (row.price == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def long_count_aggregate_m5f(b : B?) {
        b |> run("long_count_aggregate", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).long_count())
            b |> accept(c)
            if (c == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def max_aggregate_m5f(b : B?) {
        b |> run("max_aggregate", N) {
            let m = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price).max())
            b |> accept(m)
            if (m == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def min_aggregate_m5f(b : B?) {
        b |> run("min_aggregate", N) {
            let m = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price).min())
            b |> accept(m)
            if (m > 999) {
                b->failNow()
            }
        }
}

[benchmark]
def order_by_multi_key_m5f(b : B?) {
        b |> run("order_by_multi_key", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)
                                       ._order_by_keys((_.brand, _.price), 0u)
                                       .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def order_distinct_take_m5f(b : B?) {
        b |> run("order_distinct_take", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._select(_.brand)._order_by(_).distinct().take(5).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def order_reverse_normalized_m5f(b : B?) {
        b |> run("order_reverse_normalized", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._order_by(_.price).reverse().take(10).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def order_take_desc_m5f(b : B?) {
        b |> run("order_take_desc", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._order_by_descending(_.price).take(10).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def reverse_distinct_by_m5f(b : B?) {
        b |> run("reverse_distinct_by", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>).reverse()._distinct_by(_.brand).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def reverse_take_m5f(b : B?) {
        b |> run("reverse_take", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>).reverse().take(10).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def reverse_take_select_m5f(b : B?) {
        b |> run("reverse_take_select", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>).reverse().take(10)._select(_.name).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def select_count_m5f(b : B?) {
        b |> run("select_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price * 2).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_m5f(b : B?) {
        b |> run("select_where", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_count_m5f(b : B?) {
        b |> run("select_where_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price * 2)._where(_ > 1000).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_order_take_m5f(b : B?) {
        b |> run("select_where_order_take", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)
                                       ._order_by(_.price)
                                       .take(10)
                                       .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def select_where_sum_m5f(b : B?) {
        b |> run("select_where_sum", N) {
            let s = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price * 2)._where(_ > 1000).sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def single_match_m5f(b : B?) {
        b |> run("single_match", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.id == 42).single())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def skip_take_m5f(b : B?) {
        b |> run("skip_take", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>)).skip(1000).take(100).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def skip_while_match_m5f(b : B?) {
        b |> run("skip_while_match", N) {
            let total = _fold(unsafe(from_xml_node(g_root, type<Car>))._skip_while(_.id < 50000).count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sort_first_m5f(b : B?) {
        b |> run("sort_first", N) {
            let row = _fold(unsafe(from_xml_node(g_root, type<Car>))._order_by(_.price).first())
            b |> accept(row)
            if (row.id == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sort_take_m5f(b : B?) {
        b |> run("sort_take", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._order_by(_.price).take(10).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def sort_take_select_m5f(b : B?) {
        b |> run("sort_take_select", N) {
            unsafe {
                let rows <- _fold(from_xml_node(g_root, type<Car>)._order_by(_.price).take(10)._select(_.name).to_array())
                b |> accept(rows)
                if (empty(rows)) {
                    b->failNow()
                }
            }
        }
}

[benchmark]
def sum_aggregate_m5f(b : B?) {
        b |> run("sum_aggregate", N) {
            let s = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price).sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def sum_where_m5f(b : B?) {
        b |> run("sum_where", N) {
            let s = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)._select(_.price).sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_count_m5f(b : B?) {
        b |> run("take_count", N) {
            let rows <- _fold(unsafe(from_xml_node(g_root, type<Car>)).take(1000).to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def take_count_filtered_m5f(b : B?) {
        b |> run("take_count_filtered", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500).take(1000).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_sum_aggregate_m5f(b : B?) {
        b |> run("take_sum_aggregate", N) {
            let s = _fold(unsafe(from_xml_node(g_root, type<Car>))._select(_.price).take(1000).sum())
            b |> accept(s)
            if (s == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_where_count_m5f(b : B?) {
        b |> run("take_where_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>)).take(1000)._where(_.price > 500).count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def take_while_match_m5f(b : B?) {
        b |> run("take_while_match", N) {
            let total = _fold(unsafe(from_xml_node(g_root, type<Car>))._take_while(_.id < 50000).count())
            b |> accept(total)
            if (total == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def to_array_filter_m5f(b : B?) {
        b |> run("to_array_filter", N) {
            let prices <- _fold(unsafe(from_xml_node(g_root, type<Car>))._where(_.price > 500)._select(_.price).to_array())
            b |> accept(prices)
            if (empty(prices)) {
                b->failNow()
            }
        }
}

[benchmark]
def where_join_count_m5f(b : B?) {
        b |> run("where_join_count", N) {
            let c = _fold(unsafe(from_xml_node(g_root, type<Car>)) |> _where(_.price > 500)
                           |> _join(g_dealers,
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
def zip_count_pred_m5f(b : B?) {
        b |> run("zip_count_pred", N) {
            let c = _fold(zip(unsafe(from_xml_node(g_root, type<Car>)), g_ys)
                          ._select(int64(_._0.price) * int64(_._1))
                          .count($(v) => v > 50000l))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_dot_product_m5f(b : B?) {
        b |> run("zip_dot_product", N) {
            let s = _fold(zip(unsafe(from_xml_node(g_root, type<Car>)), g_ys)
                          ._select(int64(_._0.price) * int64(_._1))
                          .sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_dot_product_3arg_m5f(b : B?) {
        b |> run("zip_dot_product_3arg", N) {
            let s = _fold(zip(unsafe(from_xml_node(g_root, type<Car>)), g_ys,
                              $(c : Car; y : int) => int64(c.price) * int64(y)).sum())
            b |> accept(s)
            if (s == 0l) {
                b->failNow()
            }
        }
}

[benchmark]
def zip_reverse_to_array_m5f(b : B?) {
        b |> run("zip_reverse_to_array", N) {
            let rows <- _fold(zip(unsafe(from_xml_node(g_root, type<Car>)), g_ys)
                              .reverse()
                              .to_array())
            b |> accept(rows)
            if (empty(rows)) {
                b->failNow()
            }
        }
}

[benchmark]
def cross_join_m5f(b : B?) {
    let xml = fixture_xml_string(1000)
    let dealers <- fixture_dealers_array()
    parse_xml(xml) $(doc, ok) {
        if (!ok) {
            b->failNow()
            return
        }
        b |> run("cross_join", 1000) {
            let rows <- cross_join_to_array(unsafe(from_xml_node(doc.document_element, type<Car>)), dealers,
                                            $(ca : Car, de : Dealer) => (CarId = ca.id, DealerId = de.id))
            let c = length(rows)
            b |> accept(c)
            if (c != 1000 * DEALER_COUNT) {
                b->failNow()
            }
        }
    }
}
