options gen2
options persistent_heap

require _common public
require _sql_families public
require postgres
require postgres/postgres_boost public

// PostgreSQL engine lane: connects to DAS_PG_CONNINFO (default: the local dev
// server on port 5499), builds fixtures in a throwaway schema, drops it on exit.
// SELF-SKIPS when no server is reachable — every [benchmark] returns without
// running, so the results collector shows an em-dash column instead of failing.
// Numbers include same-box TCP round-trip cost; that IS the honest client-side
// view of a networked engine. Needs the dasPostgreSQL package installed — see
// README.md and .das_package.

def private conninfo : string {
    if (has_env_variable("DAS_PG_CONNINFO")) return get_env_variable("DAS_PG_CONNINFO")
    return "host=localhost port=5499 user=postgres dbname=postgres"
}

var g_db : postgres_boost::SqlRunner
var g_pg_ok = false
var g_schema = ""

[init]
def postgres_bench_init {
    var r = try_open_postgres(conninfo())
    if (r |> is_err) {
        to_log(LOG_WARNING, "postgres lane skipped: {r |> unwrap_err}\n")
        return
    }
    g_db <- move_unwrap(r)
    g_schema = "bench_{PQbackendPID(g_db.conn)}"
    g_db |> exec("DROP SCHEMA IF EXISTS {sql_quote_id(g_schema)} CASCADE")
    g_db |> exec("CREATE SCHEMA {sql_quote_id(g_schema)}")
    g_db |> exec("SET search_path TO {sql_quote_id(g_schema)}")
    fixture_db(g_db, N)
    fixture_db_brands(g_db, N)
    g_pg_ok = true
}

[finalize]
def postgres_bench_fini {
    if (g_pg_ok) {
        g_db |> exec("DROP SCHEMA {sql_quote_id(g_schema)} CASCADE")
    }
    delete g_db
}

[benchmark]
def aggregate_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_aggregate_match(b, g_db)
}

[benchmark]
def all_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_all_match(b, g_db)
}

[benchmark]
def any_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_any_match(b, g_db)
}

[benchmark]
def average_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_average_aggregate(b, g_db)
}

[benchmark]
def bare_order_where_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_bare_order_where(b, g_db)
}

[benchmark]
def chained_where_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_chained_where(b, g_db)
}

[benchmark]
def contains_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_contains_match(b, g_db)
}

[benchmark]
def count_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_count_aggregate(b, g_db)
}

[benchmark]
def distinct_by_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_by_count(b, g_db)
}

[benchmark]
def distinct_by_order_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_by_order_take(b, g_db)
}

[benchmark]
def distinct_by_order_to_array_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_by_order_to_array(b, g_db)
}

[benchmark]
def distinct_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_count(b, g_db)
}

[benchmark]
def distinct_count_pred_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_count_pred(b, g_db)
}

[benchmark]
def distinct_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_distinct_take(b, g_db)
}

[benchmark]
def element_at_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_element_at_match(b, g_db)
}

[benchmark]
def first_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_first_match(b, g_db)
}

[benchmark]
def first_or_default_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_first_or_default_match(b, g_db)
}

[benchmark]
def groupby_average_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_average(b, g_db)
}

[benchmark]
def groupby_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_count(b, g_db)
}

[benchmark]
def groupby_first_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_first(b, g_db)
}

[benchmark]
def groupby_having_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_having_count(b, g_db)
}

[benchmark]
def groupby_having_hidden_sum_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_having_hidden_sum(b, g_db)
}

[benchmark]
def groupby_having_post_where_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_having_post_where(b, g_db)
}

[benchmark]
def groupby_max_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_max(b, g_db)
}

[benchmark]
def groupby_min_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_min(b, g_db)
}

[benchmark]
def groupby_multi_reducer_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_multi_reducer(b, g_db)
}

[benchmark]
def groupby_select_order_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_select_order(b, g_db)
}

[benchmark]
def groupby_select_sum_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_select_sum(b, g_db)
}

[benchmark]
def groupby_sum_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_sum(b, g_db)
}

[benchmark]
def groupby_where_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_where_count(b, g_db)
}

[benchmark]
def groupby_where_sum_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_groupby_where_sum(b, g_db)
}

[benchmark]
def join_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_join_count(b, g_db)
}

[benchmark]
def join_groupby_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_join_groupby_count(b, g_db)
}

[benchmark]
def join_groupby_to_array_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_join_groupby_to_array(b, g_db)
}

[benchmark]
def join_select_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_join_select(b, g_db)
}

[benchmark]
def join_where_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_join_where_count(b, g_db)
}

[benchmark]
def last_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_last_match(b, g_db)
}

[benchmark]
def long_count_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_long_count_aggregate(b, g_db)
}

[benchmark]
def max_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_max_aggregate(b, g_db)
}

[benchmark]
def min_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_min_aggregate(b, g_db)
}

[benchmark]
def order_by_multi_key_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_order_by_multi_key(b, g_db)
}

[benchmark]
def order_distinct_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_order_distinct_take(b, g_db)
}

[benchmark]
def order_reverse_normalized_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_order_reverse_normalized(b, g_db)
}

[benchmark]
def order_take_desc_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_order_take_desc(b, g_db)
}

[benchmark]
def reverse_distinct_by_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_reverse_distinct_by(b, g_db)
}

[benchmark]
def reverse_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_reverse_take(b, g_db)
}

[benchmark]
def reverse_take_select_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_reverse_take_select(b, g_db)
}

[benchmark]
def select_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_select_count(b, g_db)
}

[benchmark]
def select_where_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_select_where(b, g_db)
}

[benchmark]
def select_where_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_select_where_count(b, g_db)
}

[benchmark]
def select_where_order_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_select_where_order_take(b, g_db)
}

[benchmark]
def select_where_sum_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_select_where_sum(b, g_db)
}

[benchmark]
def single_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_single_match(b, g_db)
}

[benchmark]
def skip_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_skip_take(b, g_db)
}

[benchmark]
def skip_while_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_skip_while_match(b, g_db)
}

[benchmark]
def sort_first_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_sort_first(b, g_db)
}

[benchmark]
def sort_take_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_sort_take(b, g_db)
}

[benchmark]
def sort_take_select_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_sort_take_select(b, g_db)
}

[benchmark]
def sum_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_sum_aggregate(b, g_db)
}

[benchmark]
def sum_where_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_sum_where(b, g_db)
}

[benchmark]
def take_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_take_count(b, g_db)
}

[benchmark]
def take_count_filtered_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_take_count_filtered(b, g_db)
}

[benchmark]
def take_sum_aggregate_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_take_sum_aggregate(b, g_db)
}

[benchmark]
def take_where_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_take_where_count(b, g_db)
}

[benchmark]
def take_while_match_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_take_while_match(b, g_db)
}

[benchmark]
def to_array_filter_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_to_array_filter(b, g_db)
}

[benchmark]
def where_join_count_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_where_join_count(b, g_db)
}

[benchmark]
def zip_count_pred_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_zip_count_pred(b, g_db)
}

[benchmark]
def zip_dot_product_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_zip_dot_product(b, g_db)
}

[benchmark]
def zip_dot_product_3arg_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_zip_dot_product_3arg(b, g_db)
}

[benchmark]
def cross_join_postgres(b : B?) {
    if (!g_pg_ok) {
        return
    }
    bench_cross_join(b, g_db)
}

