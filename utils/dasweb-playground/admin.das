options gen2
options indenting = 4

require daslib/clargs
require samples_store

// Operator CLI (run on the box over ssh) — listing curation only; store
// operations themselves live (and are tested) in samples_store.
//
//   daslang admin.das -- --db /srv/dasweb-playground/samples.db --op list
//   daslang admin.das -- --db ... --op promote --hash <sha256> --title "..." --category examples
//   daslang admin.das -- --db ... --op demote --hash <sha256>

[CommandLineArgs]
struct AdminArgs {
    @clarg_doc = "SQLite database path"
    db : string = "/srv/dasweb-playground/samples.db"

    @clarg_doc = "list | promote | demote"
    op : string = "list"

    @clarg_doc = "Sample hash (promote/demote)"
    hash : string = ""

    @clarg_doc = "Dropdown title (promote)"
    title : string = ""

    @clarg_doc = "Dropdown category (promote)"
    category : string = "examples"

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool = false
}

[export]
def main : int {
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
    }
    print("error: unknown --op '{cfg.op}' (list | promote | demote)\n")
    return 1
}
