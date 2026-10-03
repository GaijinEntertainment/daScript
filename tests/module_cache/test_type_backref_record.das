options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require _mc_common

//! the per-module compile-time line a served main module prints: `cache read took <t>,  (<file>)` -
//! the main module's name is empty, so the line ends in the empty name and the file
def served_main(out, file : string) : bool {
    for (line in split(out, "\n")) {
        if (line |> starts_with("cache read took") && line |> ends_with(",  ({file})")) return true
    }
    return false
}

//! an AOT-consuming host runs the AOT'd program, not the script; spawned as a child it prints no
//! compile-time log, so the cell skips there
def aot_host_skips(t : T?) : bool {
    let hostArgs <- get_command_line_arguments()
    if (find_index(hostArgs, "--use-aot") < 0) return false
    t |> skip("an AOT-consuming host runs no child script")
    return true
}

//! one host process compiles two scripts that require `json_boost`, a `shared` module. On the warm
//! run the first script's compile holds it, so the second script's stream reads its record into a
//! throwaway copy; a main-module type equal to one first written in that record must not reach the
//! copy through the stream's type table, or the second compile reads a freed struct.
//! The children run inside the temp directory, so the default cache they write is under it.
[test]
def test_type_backref_stays_in_module_record(t : T?) {
    if (aot_host_skips(t)) return
    var terr : string
    let tmp = create_temp_directory("das_mc_backref", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let fixtures = "{root}/tests/module_cache/_fixtures"
    let second = "{fixtures}/mc_backref_second.das"
    let enter = get_platform_name() == "windows" ? "cd /d {tmp} &&" : "cd {tmp} &&"
    let cmd = "{enter} {get_full_file_name(das_exe())} -dasroot {root} -log-compile-time {fixtures}/mc_backref_first.das {second}"
    var cold : string
    let coldOk = run_child_reported(t, "cold", cmd, cold, "MC_BACKREF_SECOND ( 1)")
    t |> success(coldOk, "the cold run compiles and runs both scripts")
    var warm : string
    var warmOk = run_child_reported(t, "warm", cmd, warm, "MC_BACKREF_SECOND ( 1)")
    warmOk = warmOk && find(warm, "MC_BACKREF_FIRST") >= 0 && served_main(warm, second)
    t |> success(warmOk, "the warm run serves the second script beside the held json_boost and runs it:\n{warm}")
    var err : string
    rmdir_rec(tmp, err)
}
