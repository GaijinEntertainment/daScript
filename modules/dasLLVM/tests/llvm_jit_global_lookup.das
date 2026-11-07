options gen2
require dastest/testing_boost public
require _spawn_common
require daslib/strings_boost
require strings
require math
require _exe_test_common

// A das global reaches JIT code through a jit_get_global_mnh lookup at function entry. The emitter
// resolves each global ONCE per function: every access then shares one base pointer, so LLVM sees
// xs[j] and xs[j + 1] as adjacent (vectorizable, alias-analyzable) and the runtime pays one hash
// lookup per global per call instead of one per access.

def private count_of(s : string; needle : string) : int {
    var n = 0
    var at = find(s, needle, 0)
    while (at >= 0) {
        n++
        at = find(s, needle, at + length(needle))
    }
    return n
}

//! the `callee` lookups of a dumped function outside its entry
//! block and its loop preheaders
def private lookups_inside_loop(fn_ir : string; callee = "jit_get_global_mnh") : int {
    var n = 0
    let blocks <- split(fn_ir, "\n\n")
    for (blk, ix in blocks, count()) {
        let block_label = slice(blk, 0, max(0, find(blk, ":")))
        if (ix > 0 && find(block_label, ".preheader") < 0) {
            n += count_of(blk, "@{callee}(")
        }
    }
    return n
}

[test]
def test_global_lookup_once_per_function(t : T?) {
    with_child_log_info() {
        t |> run("four accesses to two globals emit one lookup per global") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "FOUR_SUMS ") >= 0, "the fixture printed its sum: {tail(out)}")
            let ir = function_ir(out, "`four_sums")
            t |> success(!empty(ir), "the dump carries four_sums: {tail(out)}")
            let lookups = count_of(ir, "@jit_get_global_mnh(")
            t |> equal(2, lookups, "one jit_get_global_mnh per global (xs, ys) in four_sums, not one per access:\n{ir}")
            //! the lazy half: a global written on one branch only is never looked up ahead of the branch - the
            //! entry block carries no constant-hash lookup (LLVM may fold both branches into ONE lookup whose
            //! hash is a select, which still probes once per call and never for the untaken global)
            let bir = function_ir(out, "`branchy Cb")
            t |> success(!empty(bir), "the dump carries branchy: {tail(out)}")
            let total = count_of(bir, "@jit_get_global_mnh(")
            t |> success(total >= 1 && total <= 2, "at most one lookup per branch global (ga, gb) in branchy, got {total}:\n{bir}")
            let entry = entry_block_ir(bir)
            let hoisted = count_of(entry, "@jit_get_global_mnh(") - count_of(entry, "@jit_get_global_mnh(i64 %")
            t |> equal(0, hoisted, "no constant-hash lookup in branchy's entry block - an untaken branch pays nothing:\n{bir}")
        }
        t |> run("a lookup under a branch in a loop leaves the loop") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "SUM_SCALED ") >= 0,
                "the fixture printed its scaled sum: {tail(out)}")
            let ir = function_ir(out, "`sum_scaled Ci")
            t |> success(!empty(ir), "the dump carries sum_scaled: {tail(out)}")
            t |> equal(2, count_of(ir, "@jit_get_global_mnh("),
                "one lookup per global (lut, scale) in sum_scaled:\n{ir}")
            t |> equal(0, lookups_inside_loop(ir),
                "no lookup is left inside the loop or the branch:\n{ir}")
        }
        t |> run("a shared-global lookup under a branch in a loop leaves the loop") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_root.das", [])
            t |> success(find(out, "SUM_SHARED_SCALED ") >= 0,
                "the fixture printed its shared scaled sum: {tail(out)}")
            let ir = function_ir(out, "`sum_shared_scaled Ci")
            t |> success(!empty(ir), "the dump carries sum_shared_scaled: {tail(out)}")
            t |> equal(1, count_of(ir, "call ptr @jit_get_shared_mnh("),
                "one lookup of shared_scale in sum_shared_scaled:\n{ir}")
            t |> equal(0, lookups_inside_loop(ir, "jit_get_shared_mnh"),
                "no shared lookup is left inside the loop or the branch:\n{ir}")
        }
        t |> run("a solid-context global is one base load per function and no lookup") @(t : T?) {
            let out = jit_dump(t, "modules/dasLLVM/tests/_global_lookup_solid_root.das", [])
            t |> success(find(out, "FOUR_SUMS ") >= 0, "the solid fixture printed its sum: {tail(out)}")
            let ir = function_ir(out, "`four_sums")
            t |> success(!empty(ir), "the dump carries four_sums: {tail(out)}")
            t |> equal(0, count_of(ir, "@jit_get_global_mnh("), "a solid-context global never goes through the hash lookup:\n{ir}")
            t |> equal(1, count_of(ir, "load ptr, ptr"), "one load of the context's globals base serves every access of both globals:\n{ir}")
        }
    }
}
