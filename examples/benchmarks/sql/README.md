# Four-engine SQL benchmarks

The `benchmarks/sql` query families measured across four engines - in-tree
SQLite, [dasDuckDB](https://github.com/borisbat/dasDuckDB),
[dasPostgreSQL](https://github.com/borisbat/dasPostgreSQL), and a plain
`array<Car>` baseline - in both interpreted and JIT mode. Results:
[results.md](results.md).

Every family body lives once in `_sql_families.das` as a generic function over
the runner; the engine files (`sqlite.das` / `duckdb.das` / `postgres.das`) are
thin `[benchmark]` wrappers, and `array.das` runs the same chains as fused linq
folds with no database at all.

## Setup

The DuckDB and PostgreSQL providers are external packages, declared in this
folder's `.das_package`. Install them into `./modules` (one-time; needs git +
CMake + a C++ toolchain for the native halves):

```bash
cd examples/benchmarks/sql
daslang ../../../utils/daspkg/main.das -- install --root .
```

The PostgreSQL lane also needs a live server: point `DAS_PG_CONNINFO` at it
(default `host=localhost port=5499 user=postgres dbname=postgres`). **No server
is fine** - the lane self-skips and the results table shows ` - ` for its column.
Fixtures build in a throwaway schema and are dropped on exit.

## Run

From this folder, with `-project_root .` so the installed `./modules` are
scanned:

```bash
cd examples/benchmarks/sql
daslang -project_root . ../../../dastest/dastest.das -- --bench --test sqlite.das
daslang -jit -project_root . ../../../dastest/dastest.das -- --bench --test duckdb.das
```

Or everything at once, regenerating [results.md](results.md) - see its
"How to re-run" section.

`--bench-names <family>_<engine>` filters to a single benchmark; running a lane
file without `--bench` just compile-checks it.
