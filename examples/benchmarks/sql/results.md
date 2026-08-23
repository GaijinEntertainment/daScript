# Four-engine SQL benchmarks - SQLite / DuckDB / PostgreSQL / Array

The same `_fold` query families (mirroring `benchmarks/sql`) run over one `Car`
schema (n = 100 000 cars, 100 dealers, 5 brands) against four engines; cells are
ns/op, ` - ` = absent lane. The tables between the `BENCH:TABLES` markers are
machine-generated (see "How to re-run"); all other text is hand-edited.

Engine lanes (each in its own file, fixtures built once in `[init]`; the sweep
runs one process per file so lanes never contaminate each other):

- **SQLite** - in-memory DB via the in-tree `sqlite/sqlite_boost` provider.
- **DuckDB** - in-memory DB via [dasDuckDB](https://github.com/borisbat/dasDuckDB);
  columnar/analytical engine, so aggregate-heavy families land very differently
  than row stores.
- **PostgreSQL** - a live server via
  [dasPostgreSQL](https://github.com/borisbat/dasPostgreSQL) (`DAS_PG_CONNINFO`,
  default `host=localhost port=5499 user=postgres dbname=postgres`). Numbers
  include same-box TCP round-trip cost - that IS the honest client-side view of
  a networked engine; don't read them as engine-core speed.
- **Array** - no database at all: the same chains as fused linq folds over an
  in-memory `array<Car>`. The baseline the engines are paying their overhead
  against.

## Missing cells

A PostgreSQL ` - ` in *every* row just means no server was reachable (the lane
self-skips). The six formerly-dashed DuckDB/PostgreSQL families
(`distinct_by_*`, `distinct_count_pred`, `groupby_first`,
`groupby_select_order`, `reverse_distinct_by`) were a dialect gap this suite
exposed; it's closed - `_distinct_by` now lowers via the provider's
`caps.distinct_on` (`DISTINCT ON` on DuckDB/PostgreSQL, SQLite keeps its
bare-aggregate `GROUP BY`), and renamed projection entries declare
`AS "<alias>"` so ordering over them resolves on strict engines.

**Current tables: AMD Ryzen Threadripper 3990X (Windows, Release build), single sweep,
local PostgreSQL server on the same box.** Absolute cells move with the capture box - 
the cross-engine ratios are the signal, not the second decimal.

<!-- BENCH:TABLES BEGIN -->
*Generated 2026-08-22 by `examples/benchmarks/sql/_update_results.das` - ns/op; ` - ` = absent lane (e.g. no PostgreSQL server was reachable). Edit the prose around the markers, not the tables.*

## INTERP

| Benchmark | SQLite | DuckDB | PostgreSQL | Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 48.5 | 9.2 | 146.3 | 8.7 |
| `all_match` | 38.3 | 4.8 | 76.6 | 4.8 |
| `any_match` | 0.1 | 7.2 | 2.1 | 0.0 |
| `average_aggregate` | 42.5 | 5.6 | 179.2 | 9.4 |
| `bare_order_where` | 438.4 | 670.4 | 1161.9 | 184.6 |
| `chained_where` | 48.4 | 9.1 | 88.7 | 8.8 |
| `contains_match` | 0.1 | 6.7 | 1.9 | 3.6 |
| `count_aggregate` | 41.2 | 7.6 | 92.3 | 6.2 |
| `cross_join` | 23781.2 | 25652.6 | 56016.4 | 5508.4 |
| `distinct_by_count` | 64.1 | 26.8 | 155.7 | 24.4 |
| `distinct_by_order_take` | 398.8 | 125.8 | 681.3 | 31.3 |
| `distinct_by_order_to_array` | 398.0 | 127.9 | 869.7 | 31.5 |
| `distinct_count` | 63.9 | 32.0 | 167.4 | 24.7 |
| `distinct_count_pred` | 411.9 | 67.3 | 976.0 | 25.9 |
| `distinct_take` | 0.1 | 30.7 | 176.3 | 0.0 |
| `element_at_match` | 0.1 | 15.5 | 1.9 | 0.0 |
| `first_match` | 0.1 | 6.6 | 1.8 | 0.0 |
| `first_or_default_match` | 0.1 | 7.2 | 1.8 | 0.0 |
| `groupby_average` | 289.8 | 12.0 | 275.2 | 42.6 |
| `groupby_count` | 244.8 | 10.6 | 181.9 | 31.6 |
| `groupby_first` | 422.5 | 133.8 | 704.4 | 28.8 |
| `groupby_having_count` | 255.6 | 10.1 | 189.4 | 31.8 |
| `groupby_having_hidden_sum` | 298.1 | 14.0 | 287.0 | 35.9 |
| `groupby_having_post_where` | 302.1 | 12.9 | 255.4 | 32.5 |
| `groupby_max` | 299.1 | 10.2 | 180.3 | 40.7 |
| `groupby_min` | 302.0 | 12.4 | 181.7 | 39.8 |
| `groupby_multi_reducer` | 351.4 | 13.6 | 283.3 | 45.5 |
| `groupby_select_order` | 288.3 | 13.8 | 261.3 | 31.4 |
| `groupby_select_sum` | 303.1 | 44.0 | 318.4 | 50.7 |
| `groupby_sum` | 295.7 | 11.9 | 224.8 | 31.0 |
| `groupby_where_count` | 131.0 | 12.4 | 124.1 | 21.6 |
| `groupby_where_sum` | 143.9 | 11.9 | 172.2 | 21.0 |
| `join_count` | 57.7 | 11.8 | 219.9 | 57.3 |
| `join_groupby_count` | 280.2 | 21.8 | 310.9 | 99.1 |
| `join_groupby_to_array` | 316.1 | 19.0 | 371.6 | 103.9 |
| `join_select` | 265.0 | 510.3 | 520.0 | 93.2 |
| `join_where_count` | 56.8 | 16.4 | 158.9 | 78.9 |
| `last_match` | 0.1 | 44.9 | 2.1 | 9.2 |
| `long_count_aggregate` | 41.1 | 7.7 | 80.6 | 6.3 |
| `max_aggregate` | 47.1 | 6.9 | 90.8 | 9.9 |
| `min_aggregate` | 49.8 | 6.0 | 93.2 | 9.9 |
| `order_by_multi_key` | 541.1 | 692.2 | 1185.1 | 419.9 |
| `order_distinct_take` | 241.4 | 34.5 | 153.4 | 25.0 |
| `order_reverse_normalized` | 64.6 | 26.1 | 96.6 | 21.6 |
| `order_take_desc` | 64.5 | 24.7 | 102.5 | 21.3 |
| `reverse_distinct_by` | 478.1 | 257.9 | 909.1 | 40.1 |
| `reverse_take` | 0.1 | 132.8 | 2.1 | 0.0 |
| `reverse_take_select` | 0.1 | 127.3 | 2.0 | 0.0 |
| `select_count` | 0.2 | 4.3 | 51.4 | 0.0 |
| `select_where` | 338.1 | 578.4 | 750.1 | 17.6 |
| `select_where_count` | 51.9 | 8.0 | 90.8 | 7.3 |
| `select_where_order_take` | 57.6 | 27.2 | 117.1 | 16.9 |
| `select_where_sum` | 63.3 | 9.3 | 136.9 | 11.6 |
| `single_match` | 0.1 | 6.8 | 2.0 | 4.7 |
| `skip_take` | 0.8 | 19.0 | 3.5 | 0.2 |
| `skip_while_match` | 7.2 | 10.6 | 53.8 | 7.2 |
| `sort_first` | 67.2 | 19.0 | 104.6 | 13.5 |
| `sort_take` | 66.8 | 21.7 | 91.2 | 21.3 |
| `sort_take_select` | 66.3 | 19.9 | 214.9 | 21.4 |
| `sum_aggregate` | 42.4 | 5.2 | 156.5 | 2.2 |
| `sum_where` | 45.0 | 8.5 | 130.0 | 6.2 |
| `take_count` | 6.3 | 27.9 | 15.5 | 0.4 |
| `take_count_filtered` | 1.8 | 7.4 | 4.1 | 0.2 |
| `take_sum_aggregate` | 1.4 | 14.5 | 4.0 | 0.1 |
| `take_where_count` | 1.4 | 15.0 | 3.5 | 0.1 |
| `take_while_match` | 17.1 | 13.5 | 53.5 | 3.0 |
| `to_array_filter` | 110.9 | 78.9 | 242.6 | 18.8 |
| `where_join_count` | 57.1 | 13.5 | 134.7 | 35.3 |
| `zip_count_pred` | 61.7 | 9.3 | 104.7 | 28.4 |
| `zip_dot_product` | 77.2 | 8.9 | 108.1 | 21.7 |
| `zip_dot_product_3arg` | 77.9 | 9.1 | 96.4 | 21.0 |

## JIT

| Benchmark | SQLite | DuckDB | PostgreSQL | Array |
|---|---:|---:|---:|---:|
| `aggregate_match` | 48.2 | 7.2 | 171.0 | 0.4 |
| `all_match` | 36.9 | 4.5 | 80.3 | 0.5 |
| `any_match` | 0.1 | 7.3 | 1.8 | 0.0 |
| `average_aggregate` | 41.8 | 5.6 | 162.8 | 0.8 |
| `bare_order_where` | 281.4 | 360.3 | 803.0 | 59.1 |
| `chained_where` | 48.2 | 9.3 | 98.6 | 0.7 |
| `contains_match` | 0.0 | 5.8 | 1.8 | 0.2 |
| `count_aggregate` | 40.9 | 7.1 | 95.6 | 0.4 |
| `cross_join` | 9874.4 | 7837.9 | 41264.4 | 1278.4 |
| `distinct_by_count` | 64.5 | 29.7 | 148.5 | 1.4 |
| `distinct_by_order_take` | 401.4 | 132.8 | 726.7 | 2.6 |
| `distinct_by_order_to_array` | 417.3 | 132.3 | 865.0 | 2.4 |
| `distinct_count` | 65.7 | 33.7 | 177.9 | 1.4 |
| `distinct_count_pred` | 406.0 | 70.8 | 904.5 | 1.4 |
| `distinct_take` | 0.1 | 32.0 | 148.2 | 0.0 |
| `element_at_match` | 0.1 | 17.3 | 1.8 | 0.0 |
| `first_match` | 0.1 | 7.1 | 1.8 | 0.0 |
| `first_or_default_match` | 0.1 | 6.8 | 1.7 | 0.0 |
| `groupby_average` | 291.1 | 11.2 | 219.7 | 1.9 |
| `groupby_count` | 258.3 | 9.2 | 169.7 | 1.6 |
| `groupby_first` | 425.5 | 126.3 | 824.3 | 1.9 |
| `groupby_having_count` | 248.4 | 11.4 | 191.0 | 1.9 |
| `groupby_having_hidden_sum` | 297.4 | 13.9 | 260.0 | 2.1 |
| `groupby_having_post_where` | 298.4 | 15.6 | 239.0 | 2.1 |
| `groupby_max` | 299.8 | 12.8 | 165.4 | 1.9 |
| `groupby_min` | 304.2 | 11.1 | 175.4 | 2.5 |
| `groupby_multi_reducer` | 321.1 | 12.8 | 290.0 | 2.3 |
| `groupby_select_order` | 287.2 | 13.3 | 217.8 | 1.8 |
| `groupby_select_sum` | 300.1 | 39.9 | 252.4 | 4.1 |
| `groupby_sum` | 297.6 | 12.9 | 274.1 | 1.9 |
| `groupby_where_count` | 128.5 | 11.6 | 164.3 | 1.4 |
| `groupby_where_sum` | 146.6 | 12.0 | 197.9 | 1.5 |
| `join_count` | 57.7 | 11.5 | 220.7 | 7.2 |
| `join_groupby_count` | 270.7 | 20.2 | 301.3 | 13.8 |
| `join_groupby_to_array` | 314.1 | 22.2 | 402.1 | 13.5 |
| `join_select` | 153.6 | 308.5 | 450.6 | 23.9 |
| `join_where_count` | 56.1 | 17.3 | 181.4 | 12.2 |
| `last_match` | 0.1 | 42.1 | 1.8 | 0.7 |
| `long_count_aggregate` | 40.6 | 6.7 | 90.5 | 0.4 |
| `max_aggregate` | 47.3 | 5.8 | 91.3 | 0.4 |
| `min_aggregate` | 48.7 | 6.1 | 90.6 | 0.4 |
| `order_by_multi_key` | 358.2 | 305.4 | 964.2 | 115.5 |
| `order_distinct_take` | 232.1 | 33.2 | 154.8 | 1.8 |
| `order_reverse_normalized` | 64.6 | 23.8 | 109.5 | 0.9 |
| `order_take_desc` | 65.5 | 21.3 | 105.1 | 0.9 |
| `reverse_distinct_by` | 485.4 | 265.3 | 843.7 | 2.3 |
| `reverse_take` | 0.1 | 134.7 | 1.8 | 0.0 |
| `reverse_take_select` | 0.1 | 129.2 | 1.8 | 0.0 |
| `select_count` | 0.2 | 3.8 | 46.7 | 0.0 |
| `select_where` | 157.5 | 220.5 | 518.8 | 5.2 |
| `select_where_count` | 51.2 | 7.9 | 96.0 | 0.4 |
| `select_where_order_take` | 57.2 | 27.8 | 112.1 | 1.0 |
| `select_where_sum` | 62.9 | 9.8 | 120.6 | 0.4 |
| `single_match` | 0.0 | 6.0 | 1.7 | 0.4 |
| `skip_take` | 0.4 | 15.5 | 2.5 | 0.0 |
| `skip_while_match` | 6.5 | 10.5 | 48.1 | 0.8 |
| `sort_first` | 63.3 | 17.8 | 93.4 | 0.4 |
| `sort_take` | 64.8 | 25.8 | 96.3 | 0.9 |
| `sort_take_select` | 65.2 | 23.2 | 237.9 | 1.1 |
| `sum_aggregate` | 42.3 | 5.2 | 144.8 | 0.4 |
| `sum_where` | 44.2 | 8.4 | 106.7 | 0.4 |
| `take_count` | 2.6 | 21.6 | 10.2 | 0.1 |
| `take_count_filtered` | 1.7 | 7.9 | 4.5 | 0.0 |
| `take_sum_aggregate` | 1.4 | 20.1 | 3.7 | 0.0 |
| `take_where_count` | 1.4 | 16.2 | 3.2 | 0.0 |
| `take_while_match` | 17.3 | 14.7 | 50.4 | 0.2 |
| `to_array_filter` | 86.9 | 29.0 | 183.3 | 3.3 |
| `where_join_count` | 79.5 | 18.8 | 163.3 | 3.6 |
| `zip_count_pred` | 86.3 | 11.3 | 105.1 | 0.1 |
| `zip_dot_product` | 78.1 | 9.2 | 114.0 | 0.1 |
| `zip_dot_product_3arg` | 76.1 | 8.0 | 105.2 | 0.1 |
<!-- BENCH:TABLES END -->

## How to re-run

From this folder, with the packages installed (see [README.md](README.md)):

```bash
cd examples/benchmarks/sql
# INTERP sweep
for f in sqlite duckdb postgres array; do
    daslang -project_root . ../../../dastest/dastest.das -- --bench --bench-format json \
        --test $f.das >> _bench_interp.json 2>&1
done
# JIT sweep (-jit before dastest.das)
for f in sqlite duckdb postgres array; do
    daslang -jit -project_root . ../../../dastest/dastest.das -- --bench --bench-format json \
        --test $f.das >> _bench_jit.json 2>&1
done
# regenerate the tables below
daslang _update_results.das -- --results results.md \
    --interp _bench_interp.json --jit _bench_jit.json --date YYYY-MM-DD \
    --site-json ../../../site/files/performance_engines.json --machine "<capture box>"
```

`--site-json` also regenerates the record behind daslang.io/performance.html's engine board
from the same sweep, so the page and these tables can never disagree; `--machine` names the
capture box on the page. Skip both only for a local scratch run you are not committing.
(`-load_module <dasDuckDB checkout> -load_module <dasPostgreSQL checkout>` works in place of
the `-project_root .` install when iterating against local provider checkouts.)
