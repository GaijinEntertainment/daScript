options gen2

require dastest/testing_boost
require daslib/module_path
require daslib/strings_boost
require daslib/fio
require strings

// Obj cache on the split substrate: per-partition .o files keyed by a chained module-prefix
// hash, so invalidation is POSITIONAL - a changed das-module re-emits itself and everything
// after it in program module order; the unchanged prefix links its cached objects. The
// fixture program (main -> mod_a -> mod_b) is COPIED to a unique temp dir so rounds can edit
// module sources, and the child runs with CWD = that dir, so .jitted_scripts lands inside it
// and the whole tree deletes at the end. NOTE: the round markers hardcode the /3 partition
// denominator - adding a require to any fixture re-baselines every round.

// re-derive a fixture from its pristine source, optionally with one edit applied
def private copy_fixture(src_dir, dst_dir, fname : string; from_s : string = ""; to_s : string = "") : bool {
    var content = ""
    fopen("{src_dir}/{fname}", "rb") $(f) {
        if (f != null) {
            content = fread(f)
        }
    }
    if (empty(content)) {
        return false
    }
    if (!empty(from_s)) {
        content = content |> replace(from_s, to_s)
    }
    var wrote = false
    fopen("{dst_dir}/{fname}", "wb") $(f) {
        if (f != null) {
            f |> fwrite(content)
            wrote = true
        }
    }
    return wrote
}

// the DLL name is content-addressed (unknown up front) - walk the child's cache tree for it
def private delete_dlls_under(root : string) : int {
    var deleted = 0
    dir(root) $(sub) {
        if (sub == "." || sub == "..") return
        dir("{root}/{sub}") $(fname) {
            if (fname |> ends_with(".dll")) {
                var err : string
                if (remove("{root}/{sub}/{fname}", err)) {
                    deleted++
                }
            }
        }
    }
    return deleted
}

def private run_child(cmd : string; markers : array<string>) : tuple<rc : int; saw : array<bool>> {
    var saw : array<bool>
    saw |> resize(length(markers))
    var rc : int
    unsafe {
        rc = popen_timeout(cmd, 300.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = fgets(f)
                for (m, i in markers, count()) {
                    saw[i] ||= find(ln, m) >= 0
                }
            }
        }
    }
    return <- (rc = rc, saw <- saw)
}

def private expect_round(t : T?; round, cmd, cached_marker, result_marker : string) {
    let r <- run_child(cmd, [cached_marker, result_marker])
    t |> equal(r.rc, 0, "{round}: child rc")
    t |> success(r.saw[0], "{round}: {cached_marker}")
    t |> success(r.saw[1], "{round}: {result_marker}")
}

[test]
def test_jit_obj_cache(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        // the cache rides the LLVM JIT DLL path; the child needs -jit too
        t |> skip("needs -jit and a DLL daslang build")
        return
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    // absolutize: the child cd's away, and argv[0] is shell-dependent (cmd.exe/sh keep it relative)
    let bin = get_full_file_name(args[0])
    let fixtures = get_this_module_dir()
    // unique per run: round 1 is guaranteed cold and parallel workers can't collide
    var terr : string
    let tmp = create_temp_directory("das_objcache", terr)
    if (empty(tmp)) {
        t |> success(false, "create_temp_directory: {terr}")
        return
    }
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_client.das"), "stage client fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das"), "stage mod_a fixture")
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_b.das"), "stage mod_b fixture")
    // child CWD = tmp so .jitted_scripts lands inside it; windows cd needs /d for cross-drive
    let cd_prefix = get_platform_name() == "windows" ? "cd /d \"{tmp |> replace("/", "\\")}\" && " : "cd \"{tmp}\" && "
    let cmd = "{cd_prefix}\"{bin}\" -jit \"{tmp}/_llvm_split_modules_client.das\" -- --jit-split-modules=4"
    // round 1: cold - every partition emits
    expect_round(t, "round 1 cold", cmd, "obj cache - 0/3 partitions cached", "SPLIT_RESULT 340 19 36 33 -1")
    // round 2: DLL deleted, objects kept - the all-hit edge, a link-only build
    let dlls = delete_dlls_under("{tmp}/.jitted_scripts")
    t |> success(dlls > 0, "round 2 precondition: found a DLL to delete")
    expect_round(t, "round 2 all-hit", cmd, "obj cache - 3/3 partitions cached", "SPLIT_RESULT 340 19 36 33 -1")
    // round 3: edit the LAST module (main) - only it re-emits
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_client.das", "var acc = 0", "var acc = 1000"),
        "round 3 edit client")
    expect_round(t, "round 3 tail edit", cmd, "obj cache - 2/3 partitions cached", "SPLIT_RESULT 1340 19 36 33 -1")
    // round 4: edit the MIDDLE module (mod_a) - mod_b keeps its .o, mod_a AND the unchanged
    // main both re-emit: everything from the changed module on (positional invalidation)
    t |> success(copy_fixture(fixtures, tmp, "_llvm_split_modules_mod_a.das",
        "=> helper_b(x) + slow_b(x)", "=> helper_b(x) + slow_b(x) + 1"), "round 4 edit mod_a")
    expect_round(t, "round 4 middle edit", cmd, "obj cache - 1/3 partitions cached", "SPLIT_RESULT 1350 19 36 33 -1")
    var err : string
    rmdir_rec(tmp, err)
}
