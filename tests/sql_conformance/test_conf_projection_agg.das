options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: projections (single column / named tuple / computed scalar) and
// aggregates (count / sum / average / min / max), grouped and ungrouped (+ _having).

require dastest/testing_boost public

require _conformance_provider

require daslib/strings_boost

[sql_table(name = "ConfSales")]
struct ConfSale {
    @sql_primary_key Id : int
    Region : string
    Amount : int
}

def private seed(db) {
    db |> create_table(type<ConfSale>)
    db |> insert([
        ConfSale(Id = 0, Region = "east", Amount = 100),
        ConfSale(Id = 0, Region = "east", Amount = 300),
        ConfSale(Id = 0, Region = "west", Amount = 200),
        ConfSale(Id = 0, Region = "west", Amount = 50),
        ConfSale(Id = 0, Region = "north", Amount = 999)])
}

[test(description = "single-column, named-tuple and computed projections return the contract values, with select-list binds ordered first")]
def test_conf_projections(t : T?) {
    t |> run("single-column projection") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let amounts <- _sql(db |> select_from(type<ConfSale>)
                |> _where(_.Region == "east") |> _order_by(_.Amount) |> _select(_.Amount))
            t |> equal(length(amounts), 2)
            t |> equal(amounts[0], 100)
        }
    }
    t |> run("named-tuple projection") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let pairs <- _sql(db |> select_from(type<ConfSale>)
                |> _where(_.Region == "west") |> _order_by(_.Amount)
                |> _select((Where = _.Region, HowMuch = _.Amount)))
            t |> equal(length(pairs), 2)
            t |> equal(pairs[0].Where, "west")
            t |> equal(pairs[0].HowMuch, 50)
        }
    }
    t |> run("computed-scalar projection") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let doubled <- _sql(db |> select_from(type<ConfSale>)
                |> _where(_.Region == "north") |> _select(_.Amount + _.Amount))
            t |> equal(doubled[0], 1998)
        }
    }
    t |> run("literal bind in computed projection + _where (SELECT-list binds order first)") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            // `SELECT ("Amount") * (?) ... WHERE "Region" = ?` — the projection's literal bind
            // must enumerate BEFORE the WHERE bind (SELECT list parses first).
            let doubled <- _sql(db |> select_from(type<ConfSale>)
                |> _where(_.Region == "north") |> _select(_.Amount * 2))
            t |> equal(length(doubled), 1)
            t |> equal(doubled[0], 1998)
        }
    }
}

[test(description = "scalar aggregates and the one-scan aggregate return the contract values, including defaults for an empty source and the same shape over arrays")]
def test_conf_aggregates(t : T?) {
    t |> run("count / sum / min / max / average over the table") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            t |> equal(_sql(db |> select_from(type<ConfSale>) |> count()), 5)
            t |> equal(_sql(db |> select_from(type<ConfSale>) |> _select(_.Amount) |> sum()), 1649)
            t |> equal(_sql(db |> select_from(type<ConfSale>) |> _select(_.Amount) |> min()), 50)
            t |> equal(_sql(db |> select_from(type<ConfSale>) |> _select(_.Amount) |> max()), 999)
            let avg = _sql(db |> select_from(type<ConfSale>)
                |> _where(_.Region == "east") |> _select(_.Amount) |> average())
            t |> numericEqual(avg, 200.0lf)
        }
    }
    t |> run("_aggregate returns several global aggregates from one scan") @(tt : T?) {
        with_conf_db() $(db) {
            seed(db)
            let stats = _sql(
                db |> select_from(type<ConfSale>)
                   |> _where(_.Region != "north")
                   |> _aggregate($(_rows) => (
                        N = _rows |> count,
                        LongN = _rows |> long_count,
                        Total = _rows |> _select(_.Amount) |> sum,
                        MinAmount = _rows |> _select(_.Amount) |> min,
                        MaxAmount = _rows |> _select(_.Amount) |> max,
                        Average = _rows |> _select(_.Amount) |> average)))
            tt |> equal(stats.N, 4)
            tt |> equal(stats.LongN, 4l)
            tt |> equal(stats.Total, 650)
            tt |> equal(stats.MinAmount, 50)
            tt |> equal(stats.MaxAmount, 300)
            tt |> numericEqual(stats.Average, 162.5lf)
            let sql = _sql_text(
                db |> select_from(type<ConfSale>)
                   |> _aggregate($(_rows) => (
                        N = _rows |> count,
                        Total = _rows |> _select(_.Amount) |> sum)))
            tt |> success(sql |> contains("COUNT(*) AS \"N\""))
            tt |> success(sql |> contains("SUM(\"Amount\") AS \"Total\""))
        }
    }
    t |> run("_aggregate returns type defaults for an empty source") @(tt : T?) {
        with_conf_db() $(db) {
            seed(db)
            let stats = _sql(
                db |> select_from(type<ConfSale>)
                   |> _where(_.Region == "missing")
                   |> _aggregate($(_rows) => (
                        N = _rows |> count,
                        Total = _rows |> _select(_.Amount) |> sum,
                        MinAmount = _rows |> _select(_.Amount) |> min,
                        MaxAmount = _rows |> _select(_.Amount) |> max,
                        Average = _rows |> _select(_.Amount) |> average)))
            tt |> equal(stats.N, 0)
            tt |> equal(stats.Total, 0)
            tt |> equal(stats.MinAmount, 0)
            tt |> equal(stats.MaxAmount, 0)
            tt |> numericEqual(stats.Average, 0.0lf)
        }
    }
    t |> run("_aggregate keeps the same shape for in-memory arrays") @(tt : T?) {
        let rows = [
            ConfSale(Id = 1, Region = "east", Amount = 100),
            ConfSale(Id = 2, Region = "east", Amount = 300),
            ConfSale(Id = 3, Region = "west", Amount = 50)]
        let stats = rows |> _aggregate($(_rows) => (
            N = _rows |> count,
            LongN = _rows |> long_count,
            Total = _rows |> _select(_.Amount) |> sum,
            MinAmount = _rows |> _select(_.Amount) |> min,
            MaxAmount = _rows |> _select(_.Amount) |> max))
        tt |> equal(stats.N, 3)
        tt |> equal(stats.LongN, 3l)
        tt |> equal(stats.Total, 450)
        tt |> equal(stats.MinAmount, 50)
        tt |> equal(stats.MaxAmount, 300)
    }
}

[test(description = "group_by with aggregates and having return one filtered row per group")]
def test_conf_group_by(t : T?) {
    t |> run("_group_by + aggregate _select") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let per_region <- _sql(db |> select_from(type<ConfSale>)
                |> _group_by(_.Region)
                |> _order_by(_._0)
                |> _select((Region = _._0, Total = _._1 |> _select($(s : ConfSale) => s.Amount) |> sum())))
            t |> equal(length(per_region), 3)
            t |> equal(per_region[0].Region, "east")
            t |> equal(per_region[0].Total, 400)
        }
    }
    t |> run("_having filters groups post-aggregation") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let big <- _sql(db |> select_from(type<ConfSale>)
                |> _group_by(_.Region)
                |> _having(_._1 |> _select($(s : ConfSale) => s.Amount) |> sum() > 300)
                |> _select((Region = _._0, Total = _._1 |> _select($(s : ConfSale) => s.Amount) |> sum())))
            t |> equal(length(big), 2)
        }
    }
}
