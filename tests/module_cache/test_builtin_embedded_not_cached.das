options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common

//! the cache is binary, so the record name is searched as bytes; fread's string stops at its first NUL
def private bytes_have(data : array<uint8> const#; needle : string) : bool {
    var found = false
    peek_data(needle) $(nd) {
        let n = length(nd)
        for (i in range(length(data) - n + 1)) {
            var same = true
            for (j in range(n)) {
                if (data[i + j] != nd[j]) {
                    same = false
                    break
                }
            }
            if (same) {
                found = true
                break
            }
        }
    }
    return found
}

def private cache_names(path : string; needle : string) : bool {
    var found = false
    fopen(path, "rb") $(f) {
        return if (f == null)
        fmap(f) $(data) {
            found = bytes_have(data, needle)
        }
    }
    return found
}

//! a builtin module's embedded source compiles with the stream hidden: the cache carries no
//! record of it, so the module's cumulative hash is the same on a cold run and a warm one and a
//! program requiring the module serves clean (src/ast/ARCHITECTURE.md#module-cache-read)
[test]
def test_builtin_embedded_not_cached(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_embedded", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let drv = path_join(tmp, "mc_embedded_drv.das")
    fwrite(drv, fread("{root}/tests/module_cache/_fixtures/mc_embedded_drv.das"))
    let cache = path_join(tmp, "mc.bin")
    let cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{root}\" -module-cache \"{cache}\" \"{drv}\""
    var cold : string
    let coldRc = run_child(cmd, cold)
    var coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    coldOk = coldOk && report_child(t, "cold", coldRc, cold, "EMBEDDED_DRV=")
    if (cache_names(cache, "unit_test.das")) {
        t |> failure("the cache carries a record of the embedded unit_test.das")
        coldOk = false
    }
    t |> success(coldOk, "cold run wrote the cache without the embedded source")
    var warm : string
    let warmRc = run_child(cmd, warm)
    var warmOk = report_child(t, "warm", warmRc, warm, "deser: clean")
    warmOk = warmOk && report_child(t, "warm", warmRc, warm, "EMBEDDED_DRV=")
    if (find(warm, "cumulative hash") >= 0) {
        t |> failure("the builtin module's cumulative hash differs between the cold run and the warm one")
        warmOk = false
    }
    t |> success(warmOk, "warm run serves every record, the builtin module's hash unchanged")
    var err : string
    rmdir_rec(tmp, err)
}
