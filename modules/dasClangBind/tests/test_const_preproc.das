options gen2
require cbind/cbind_boost
require daslib/fio
require strings

// Regression test for cbind preprocessor-active constant generation.
//
// const_preproc_fixture.h defines CONST_PREPROC_K twice (16 under an undefined
// #ifdef, 0 under the active #else). The old raw-text regex scan picked the
// first textual match (16); collectActiveDefines walks libclang
// MacroDefinition cursors, which only exist for the preprocessor-active branch,
// so it must yield 0 with no trace of 16.
//
// Runs on the mingw CI worker where libclang/cbind is available (alongside the
// bind_clangbind self-binder). Returns non-zero on failure so CI can branch.
[export]
def main : int {
    let pfp = "{get_das_root()}/modules/dasClangBind/tests/"
    let pfn = "const_preproc_fixture.h"
    let fname = "{pfp}{pfn}"
    var cgb = new CppGenBind(pfn, pfp, DEFAULT_CLANG_ARGUMENTS)
    var rc = 1
    fopen(fname, "rb") $(f) {
        if (f == null) {
            to_log(LOG_ERROR, "FAIL: cannot open fixture {fname}\n")
        } else {
            let data = fread(f)
            let active = cgb->collectActiveDefines(fname, data)
            // the inactive-branch trap (16) must exist in the raw header text...
            let rawHasTrap = find(data, "16") >= 0
            // ...but be absent from the preprocessor-active reconstruction
            let kPresent = find(active, "CONST_PREPROC_K") >= 0
            let plainPresent = find(active, "CONST_PREPROC_PLAIN") >= 0
            let activeHasTrap = find(active, "16") >= 0
            if (rawHasTrap && kPresent && plainPresent && !activeHasTrap) {
                to_log(LOG_INFO, "PASS: cbind const-gen picks the active #else branch (CONST_PREPROC_K=0; inactive 16 stripped)\n")
                rc = 0
            } else {
                to_log(LOG_ERROR, "FAIL: rawHasTrap={rawHasTrap} kPresent={kPresent} plainPresent={plainPresent} activeHasTrap={activeHasTrap}. Active defines:\n{active}\n")
            }
        }
    }
    unsafe { delete cgb }
    return rc
}
