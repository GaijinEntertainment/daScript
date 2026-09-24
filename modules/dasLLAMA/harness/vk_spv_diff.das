options gen2
options stack = 524288

// The SPIR-V dump differ: two DASLLAMA_VK_SPV_DUMP directories in, the stamps whose emitted words
// moved, appeared or vanished out - the identity gate a kernel fold ships with. Exit 0 when every
// stamp both dumps hold is byte-identical, neither side has a stamp the other lacks, and both sides
// hold at least one stamp (an empty dump directory proves nothing).
//
//   daslang modules/dasLLAMA/harness/vk_spv_diff.das -- <base_dir> <fresh_dir> [--verbose]

require daslib/clargs
require daslib/fio
require daslib/linq   // sequence_equal
require strings

[CommandLineArgs]
struct Args {
    @clarg_positional
    @clarg_doc = "The pre-fold dump directory"
    base : string

    @clarg_positional
    @clarg_doc = "The dump directory to compare against it"
    fresh : string

    @clarg_doc = "Also list the stamps that did not move"
    verbose : bool = false
}

//! the stamps of a dump directory by their file stem, each its emitted words as bytes
def read_dump(dirp : string) : table<string; array<uint8>> {
    var out : table<string; array<uint8>>
    dir(dirp) $(name) {
        return if (!ends_with(name, ".spv"))
        fopen(path_join(dirp, name), "rb") $(f) {
            return if (f == null)
            fmap(f) $(data) {
                out[stem(name)] := data
            }
        }
    }
    return <- out
}

//! two dumps' stamps sorted by name into the four bins: byte-identical on both sides, moved, only in `fresh`, only in `base`
struct DumpDiff {
    same : array<string>
    moved : array<string>
    appeared : array<string>
    vanished : array<string>
}

def sorted_keys(t : table<string; array<uint8>>) : array<string> {
    var ks <- [for (k in keys(t)); k]
    ks |> sort()
    return <- ks
}

def diff_dumps(var base, fresh : table<string; array<uint8>>) : DumpDiff {
    var d : DumpDiff
    for (k in sorted_keys(base)) {
        if (!key_exists(fresh, k)) {
            d.vanished |> push(k)
        } elif (!sequence_equal(base[k], fresh[k])) {
            d.moved |> push(k)
        } else {
            d.same |> push(k)
        }
    }
    for (k in sorted_keys(fresh)) {
        d.appeared |> push(k) if (!key_exists(base, k))
    }
    return <- d
}

//! the gate's verdict over a diff: identical means every stamp both sides hold matches and neither side holds a stamp alone,
//! and a side with no stamps at all fails it (nothing was compared)
def dumps_identical(d : DumpDiff; base_count, fresh_count : int) : bool {
    return base_count > 0 && fresh_count > 0 && empty(d.moved) && empty(d.appeared) && empty(d.vanished)
}

[export]
def main : int {
    var cfg = Args()
    let rc = parse_args_with_help(cfg, "vk_spv_diff")
    return rc if (rc >= 0)
    var base <- read_dump(cfg.base)
    var fresh <- read_dump(cfg.fresh)
    var d <- diff_dumps(base, fresh)
    print("vk_spv_diff: {length(d.same)} identical, {length(d.moved)} moved, {length(d.appeared)} appeared, {length(d.vanished)} vanished\n")
    for (k in d.moved) {
        print("  moved     {k}  ({length(base[k])} -> {length(fresh[k])} bytes)\n")
    }
    for (k in d.appeared) {
        print("  appeared  {k}\n")
    }
    for (k in d.vanished) {
        print("  vanished  {k}\n")
    }
    if (cfg.verbose) {
        for (k in d.same) {
            print("  same      {k}\n")
        }
    }
    if (empty(base) || empty(fresh)) {
        print("vk_spv_diff: {empty(base) ? cfg.base : cfg.fresh} holds no .spv stamp - nothing was compared\n")
    }
    return dumps_identical(d, length(base), length(fresh)) ? 0 : 1
}
