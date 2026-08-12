options gen2
module _llvm_cache_common

// Shared harness for the jit cache round-trip tests (llvm_obj_cache, llvm_ast_roundtrip):
// fixture staging into a per-run temp dir, DLL eviction, and a child runner that greps
// stdout for marker lines.

require dastest/testing_boost
require daslib/strings_boost
require daslib/fio
require strings

// re-derive a fixture from its pristine source, optionally with one edit applied
def public copy_fixture(src_dir, dst_dir, fname : string; from_s : string = ""; to_s : string = "") : bool {
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
def public delete_dlls_under(root : string) : int {
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

// run cmd, report exit code plus which marker substrings appeared on stdout
def public run_child(cmd : string; markers : array<string>) : tuple<rc : int; saw : array<bool>> {
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

// one child run: rc must be 0 and every marker must appear on stdout
def public expect_round(t : T?; round, cmd : string; markers : array<string>) {
    let r <- run_child(cmd, markers)
    t |> equal(r.rc, 0, "{round}: child rc")
    for (m, ok in markers, r.saw) {
        t |> success(ok, "{round}: {m}")
    }
}
