options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: [sql_fts5] + text_match — CAPABILITY-GATED (caps.fts5).
// A provider without full-text search drops this FILE from its copy of the suite
// (the [sql_fts5] annotation itself would not compile) — see README.md.

require dastest/testing_boost public

require _conformance_provider

[sql_fts5(name = "conf_docs_idx")]
struct ConfDoc {
    Body : string
    @sql_fts_rank Rank : float
}

[test(description = "text_match finds tokenized content in an fts5 table and ranks the hits", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_fts5(t : T?) {
    t |> run("text_match finds tokenized content, ranked") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfDoc>)
            db |> insert(ConfDoc(Body = "the quick brown fox jumps over the lazy dog"))
            db |> insert(ConfDoc(Body = "a slow red fox sleeps"))
            db |> insert(ConfDoc(Body = "nothing to see here"))
            let foxes <- _sql(db |> select_from(type<ConfDoc>)
                |> _where(_.Body |> text_match("fox"))
                |> _order_by(_.Rank))
            t |> equal(length(foxes), 2)
            let quick <- _sql(db |> select_from(type<ConfDoc>)
                |> _where(_.Body |> text_match("quick fox")))
            t |> equal(length(quick), 1)
        }
    }
}
