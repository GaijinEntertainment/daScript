options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: [sql_view] + _create_view — a view reads like a table through the
// same select_from / _sql surface; captured locals inline via to_sql_literal.

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfOrders2")]
struct ConfOrder2 {
    @sql_primary_key Id : int
    Amount : int
    Status : string
}

[sql_view(name = "ConfBigOrders")]
struct ConfBigOrder {
    Id : int
    Amount : int
    Status : string
}

[test(description = "create_view makes a view that reads back typed rows like a table", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_view(t : T?) {
    t |> run("_create_view + typed reads through the view") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfOrder2>)
            db |> insert([
                ConfOrder2(Id = 0, Amount = 50, Status = "pending"),
                ConfOrder2(Id = 0, Amount = 250, Status = "shipped"),
                ConfOrder2(Id = 0, Amount = 800, Status = "shipped")])
            let floor = 100
            db |> _create_view(type<ConfBigOrder>,
                db |> select_from(type<ConfOrder2>) |> _where(_.Amount >= floor))
            let all_big <- db |> select_from(type<ConfBigOrder>)
            t |> equal(length(all_big), 2)
            let shipped_big <- _sql(db |> select_from(type<ConfBigOrder>)
                |> _where(_.Status == "shipped") |> _order_by(_.Amount))
            t |> equal(length(shipped_big), 2)
            t |> equal(shipped_big[0].Amount, 250)
            // Captured `floor` was inlined at creation time — new rows re-evaluate the view body.
            db |> insert(ConfOrder2(Id = 0, Amount = 120, Status = "pending"))
            t |> equal(_sql(db |> select_from(type<ConfBigOrder>) |> count()), 3)
            db |> drop_view_if_exists(type<ConfBigOrder>)
        }
    }
}
