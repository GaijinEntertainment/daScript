options gen2
options indenting = 4

require daslib/clargs
require ladder_store
require daslib/fio

// Operator CLI (run on the box over ssh) — the verify/delete levers and manual
// official imports; store operations themselves live (and are tested) in
// ladder_store.
//
//   daslang admin.das -- --db /srv/dasllama-ladder/ladder.db --op verify-sidecar --sha <sha256>
//   daslang admin.das -- --db ... --op unverify-sidecar --sha <sha256>
//   daslang admin.das -- --db ... --op delete-sidecar --sha <sha256>
//   daslang admin.das -- --db ... --op verify-submission --id <id>
//   daslang admin.das -- --db ... --op unverify-submission --id <id>
//   daslang admin.das -- --db ... --op delete-submission --id <id>
//   daslang admin.das -- --db ... --op import-official --file records/m1.json

[CommandLineArgs]
struct AdminArgs {
    @clarg_doc = "SQLite database path"
    db : string = "/srv/dasllama-ladder/ladder.db"

    @clarg_doc = "verify-sidecar | unverify-sidecar | delete-sidecar | verify-submission | unverify-submission | delete-submission | import-official"
    op : string = ""

    @clarg_doc = "Sidecar content sha (the *-sidecar ops)"
    sha : string = ""

    @clarg_doc = "Submission id (the *-submission ops)"
    id : int = 0

    @clarg_doc = "Record store path (import-official)"
    file : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool = false
}

[export]
def main : int { // nolint:STYLE037 — flat CLI dispatch, one arm per --op
    var r <- parse_args(type<AdminArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<AdminArgs>), "dasllama-ladder admin")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.op)) {
        print_help(get_command_info(type<AdminArgs>), "dasllama-ladder admin")
        return cfg.help ? 0 : 1
    }
    var open_result <- try_open_sqlite(cfg.db)
    if (open_result |> is_err) {
        print("error: cannot open {cfg.db}: {open_result |> unwrap_err}\n")
        return 1
    }
    var inscope db <- open_result._value
    // sqlite3_open CREATES an empty file for a mistyped path, so migrate first:
    // every store call below would otherwise panic in prepare instead of
    // reporting the path, and the CLI stays usable across shipped migrations.
    db |> apply_recommended_pragmas()
    migrate_to_latest(db)
    if (cfg.op == "verify-sidecar" || cfg.op == "unverify-sidecar") {
        let ok = set_sidecar_verified(db, cfg.sha, cfg.op == "verify-sidecar")
        print(ok ? "done\n" : "unknown sidecar {cfg.sha}\n")
        return ok ? 0 : 1
    }
    if (cfg.op == "delete-sidecar") {
        let ok = delete_sidecar(db, cfg.sha)
        print(ok ? "deleted\n" : "unknown sidecar {cfg.sha}\n")
        return ok ? 0 : 1
    }
    if (cfg.op == "verify-submission" || cfg.op == "unverify-submission") {
        let n = set_submission_verified(db, cfg.id, cfg.op == "verify-submission")
        print("{n} run(s) updated\n")
        return n > 0 ? 0 : 1
    }
    if (cfg.op == "delete-submission") {
        let n = delete_submission(db, cfg.id)
        print("{n} row(s) removed\n")
        return n > 0 ? 0 : 1
    }
    if (cfg.op == "import-official") {
        let text = fread(cfg.file)
        if (empty(text)) {
            print("error: cannot read {cfg.file}\n")
            return 1
        }
        let res = import_official_store(db, text, int64(get_clock()))
        if (!res.ok) {
            print("error: {res.error}\n")
            return 1
        }
        print("{res.runs_added} run(s) imported, {res.runs_replaced} replaced\n")
        return 0
    }
    print("error: unknown op '{cfg.op}'\n")
    return 1
}
