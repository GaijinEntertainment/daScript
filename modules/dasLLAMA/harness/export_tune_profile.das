options gen2

require llvm/daslib/llvm_tune
require daslib/clargs
require daslib/fio
require daslib/module_path

[CommandLineArgs]
struct ExportArgs {
    @clarg_doc = "Sidecar to export from (default: the DAS_TUNE_MANIFEST / app-beside resolution)"
    sidecar : string

    @clarg_doc = "Output directory (default: modules/dasLLAMA/performance/defaults beside this script)"
    @clarg_name = "out"
    out_dir : string

    @clarg_doc = "CPU class name to export as (default: the class the process is pinned to, else this box's)"
    @clarg_name = "class"
    klass : string
}

//! Export a minted sidecar's kernel winners as the shipped defaults profile for this box's
//! CPU class — run on a reference box after a full `--tune` mint, then check the file in.
//! `daslang harness/export_tune_profile.das -- --sidecar benchmarks/lcpp_bench.tune.json`
[export]
def main : int {
    var r <- parse_args(type<ExportArgs>)
    if (r |> is_err) {
        print("{r |> unwrap_err}\n")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (!empty(cfg.sidecar)) {
        set_tune_manifest_runtime_path(cfg.sidecar)
    }
    let klass = empty(cfg.klass) ? tune_mint_target().klass : cpu_class_of(cfg.klass)
    if (cpu_class_generic(klass)) {
        to_log(LOG_ERROR, "export_tune_profile: '{cfg.klass}' names no CPU class - the classes are {jit_cpu_class_names()}\n")
        return 1
    }
    let outDir = empty(cfg.out_dir) ? path_join(path_join(get_this_module_dir(), ".."), "performance/defaults") : cfg.out_dir
    mkdir(outDir)
    let outPath = path_join(outDir, "{cpu_class_name(klass)}.tune-defaults.json")
    return tune_profile_export(outPath, klass) ? 0 : 1
}
