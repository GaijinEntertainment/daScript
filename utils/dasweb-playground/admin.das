options gen2
options indenting = 4

require daslib/clargs
require samples_store
require build_queue

// Operator CLI (run on the box over ssh) — listing curation and build-queue
// nudges; store operations themselves live (and are tested) in samples_store
// and build_queue.
//
//   daslang admin.das -- --db /srv/dasweb-playground/samples.db --op list
//   daslang admin.das -- --db ... --op promote --hash <sha256> --title "..." --category examples
//   daslang admin.das -- --db ... --op demote --hash <sha256>
//   daslang admin.das -- --db ... --op enqueue --hash <sha256> [--mode page] [--force]
//   daslang admin.das -- --db ... --op build-status --hash <sha256>
//   daslang admin.das -- --db ... --op set-toolchain --toolchain <sha>

[CommandLineArgs]
struct AdminArgs {
    @clarg_doc = "SQLite database path"
    db : string = "/srv/dasweb-playground/samples.db"

    @clarg_doc = "list | promote | demote | enqueue | build-status | set-toolchain"
    op : string = "list"

    @clarg_doc = "Sample hash (promote/demote/enqueue/build-status)"
    hash : string = ""

    @clarg_doc = "Dropdown title (promote)"
    title : string = ""

    @clarg_doc = "Dropdown category (promote)"
    category : string = "examples"

    @clarg_doc = "Build delivery mode (enqueue): module | page"
    mode : string = "module"

    @clarg_doc = "Re-enqueue a failed build (enqueue)"
    force : bool = false

    @clarg_doc = "Toolchain id (set-toolchain); enqueue/build-status use the current one when empty"
    toolchain : string = ""

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool = false
}

[export]
def main : int { // nolint:STYLE037,STYLE038 — flat CLI dispatch, one arm per --op
    var r <- parse_args(type<AdminArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<AdminArgs>), "dasweb-playground admin")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<AdminArgs>), "dasweb-playground admin")
        return 0
    }
    var open_result <- try_open_sqlite(cfg.db)
    if (open_result |> is_err) {
        print("error: cannot open {cfg.db}: {open_result |> unwrap_err}\n")
        return 1
    }
    var inscope db <- open_result._value
    // sqlite3_open CREATES an empty file for a mistyped path, so "opened" does
    // not mean "has the schema" — and every store call below would panic in
    // prepare instead of reporting the path. Migrating first also makes the CLI
    // usable against a database that predates a newly shipped migration.
    db |> apply_recommended_pragmas()
    migrate_to_latest(db)
    if (cfg.op == "list") {
        let listed <- listed_samples(db)
        for (l in listed) {
            print("{l.hash}  [{l.category}] {l.title}\n")
        }
        print("{length(listed)} listed\n")
        return 0
    } elif (cfg.op == "promote") {
        if (empty(cfg.hash) || empty(cfg.title)) {
            print("error: promote needs --hash and --title\n")
            return 1
        }
        if (!promote_sample(db, cfg.hash, cfg.title, cfg.category)) {
            print("error: unknown hash {cfg.hash}\n")
            return 1
        }
        print("promoted {cfg.hash} as [{cfg.category}] {cfg.title}\n")
        return 0
    } elif (cfg.op == "demote") {
        if (empty(cfg.hash)) {
            print("error: demote needs --hash\n")
            return 1
        }
        if (!demote_sample(db, cfg.hash)) {
            print("error: unknown hash {cfg.hash}\n")
            return 1
        }
        print("demoted {cfg.hash} (permalink survives)\n")
        return 0
    } elif (cfg.op == "enqueue") {
        if (empty(cfg.hash)) {
            print("error: enqueue needs --hash\n")
            return 1
        }
        if (cfg.mode != BUILD_MODE_MODULE && cfg.mode != BUILD_MODE_PAGE) {
            print("error: --mode must be module or page\n")
            return 1
        }
        if (!sample_exists(db, cfg.hash)) {
            print("error: unknown hash {cfg.hash}\n")
            return 1
        }
        let toolchain = empty(cfg.toolchain) ? current_toolchain(db) : cfg.toolchain
        if (empty(toolchain)) {
            print("error: no toolchain announced yet (--toolchain overrides)\n")
            return 1
        }
        let er = enqueue_build(db, cfg.hash, toolchain, cfg.mode, current_unix_time(), force = cfg.force)
        print("{er.created ? "enqueued" : "already"} {cfg.hash} [{cfg.mode}] under {toolchain}: {er.state}\n")
        return 0
    } elif (cfg.op == "build-status") {
        if (empty(cfg.hash)) {
            print("error: build-status needs --hash\n")
            return 1
        }
        let toolchain = empty(cfg.toolchain) ? current_toolchain(db) : cfg.toolchain
        if (empty(toolchain)) {
            print("error: no toolchain announced yet (--toolchain overrides)\n")
            return 1
        }
        let st = build_status(db, cfg.hash, toolchain)
        if (!st.found) {
            print("no build job for {cfg.hash} under {toolchain}\n")
            return 1
        }
        print("{cfg.hash} under {toolchain}: {st.state} (mode {st.mode}, attempts {st.attempts})\n")
        if (st.state == BUILD_STATE_QUEUED) {
            print("queue position {st.position}\n")
        } elif (st.state == BUILD_STATE_FAILED) {
            print("{st.error}\n")
        } elif (st.state == BUILD_STATE_DONE) {
            print("artifact {st.artifact_path}: {st.artifact_files}\n")
        }
        return 0
    } elif (cfg.op == "set-toolchain") {
        if (empty(cfg.toolchain)) {
            print("error: set-toolchain needs --toolchain\n")
            return 1
        }
        set_current_toolchain(db, cfg.toolchain)
        print("current toolchain: {cfg.toolchain}\n")
        return 0
    }
    print("error: unknown --op '{cfg.op}' (list | promote | demote | enqueue | build-status | set-toolchain)\n")
    return 1
}

def private current_unix_time : int64 {
    return int64(get_clock())
}
