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
    out_dir : string

    @clarg_doc = "CPU class name to export as (default: this box's tune_cpu_class())"
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
    let klass = empty(cfg.klass) ? tune_cpu_class() : cfg.klass
    let outDir = empty(cfg.out_dir) ? path_join(path_join(get_this_module_dir(), ".."), "performance/defaults") : cfg.out_dir
    mkdir(outDir)
    let outPath = path_join(outDir, "{klass}.tune-defaults.json")
    return tune_profile_export(outPath, klass) ? 0 : 1
}
