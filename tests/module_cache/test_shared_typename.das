options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common

//! A fresh child must read the folded typename from the previous compile.
[test]
def test_shared_typename_cache(t : T?) {
    let tmp = test_temp_dir()
    let root = get_das_root()
    let fixture = path_join(root,
        "tests/module_cache/_fixtures/mc_shared_typename.das")
    let cache = path_join(tmp, "shared_typename.bin")
    let win = get_platform_name() == "windows"
    let binary = win ? "daslang.exe" : "daslang"
    let exe = path_join(dir_name(get_full_file_name(das_exe())), binary)
    let enter = win ? "cd /d" : "cd"
    let cmd = ("{enter} \"{tmp}\" && \"{exe}\" -dasroot \"{root}\""
        + " -module-cache \"{cache}\" \"{fixture}\"")
    let marker = "SHARED_TYPENAME=tuple<a:float;b:float;c:float>\n"
    var cold : string
    let cold_ok = run_child_reported(t, "cold", cmd, cold, marker)
    t |> success(cold_ok && find(cold, "ser: wrote") >= 0,
        "cold run writes the correct folded typename:\n{cold}")
    var warm : string
    let warm_ok = run_child_reported(t, "warm", cmd, warm, marker)
    t |> success(warm_ok && find(warm, "deser: clean") >= 0,
        "warm run serves the correct folded typename:\n{warm}")
}
