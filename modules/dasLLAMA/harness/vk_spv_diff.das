options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// The SPIR-V dump differ: two DASLLAMA_VK_SPV_DUMP directories in, the stamps whose emitted words
// moved, appeared or vanished out - the identity gate a kernel fold ships with. Exit 0 when every
// stamp both dumps hold is byte-identical and neither side has a stamp the other lacks.
//
//   daslang modules/dasLLAMA/harness/vk_spv_diff.das -- <base_dir> <fresh_dir> [--verbose]

require daslib/clargs
require daslib/fio
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

def sorted_keys(t : table<string; array<uint8>>) : array<string> {
    var ks <- [for (k in keys(t)); k]
    ks |> sort()
    return <- ks
}

def bytes_equal(a, b : array<uint8>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
}

[export]
def main : int {
    var cfg = Args()
    let rc = parse_args_with_help(cfg, "vk_spv_diff")
    return rc if (rc >= 0)
    var base <- read_dump(cfg.base)
    var fresh <- read_dump(cfg.fresh)
    var moved, appeared, vanished, same : array<string>
    for (k in sorted_keys(base)) {
        if (!key_exists(fresh, k)) {
            vanished |> push(k)
        } elif (!bytes_equal(base[k], fresh[k])) {
            moved |> push(k)
        } else {
            same |> push(k)
        }
    }
    for (k in sorted_keys(fresh)) {
        appeared |> push(k) if (!key_exists(base, k))
    }
    print("vk_spv_diff: {length(same)} identical, {length(moved)} moved, {length(appeared)} appeared, {length(vanished)} vanished\n")
    for (k in moved) {
        print("  moved     {k}  ({length(base[k])} -> {length(fresh[k])} bytes)\n")
    }
    for (k in appeared) {
        print("  appeared  {k}\n")
    }
    for (k in vanished) {
        print("  vanished  {k}\n")
    }
    if (cfg.verbose) {
        for (k in same) {
            print("  same      {k}\n")
        }
    }
    return (empty(moved) && empty(appeared) && empty(vanished)) ? 0 : 1
}
