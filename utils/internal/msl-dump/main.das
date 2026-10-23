options gen2
options indenting = 4

// Dumps every `[metal_kernel]`'s emitted MSL text - the `*_msl`
// globals the annotation declares - from each MODULE file given (a
// shared module keeps its text globals; a program's main module
// folds them away), one `<module>.<global>.msl` per kernel under
// --out, so two dumps of the same files before and after an emitter
// change diff byte for byte (`git diff --no-index a b`). The
// dasLLAMA kernel modules require das_metal, so those dumps run on a
// Mac; tests/msl/_msl_common.das dumps anywhere.
//
//   daslang utils/internal/msl-dump/main.das -- --out <dir>
//     <file.das>...

require daslib/rtti
require daslib/ast
require daslib/debugger
require daslib/fio
require daslib/strings_boost
require daslib/clargs

[CommandLineArgs]
struct DumpArgs {
    @clarg_required
    @clarg_doc = "Directory the .msl files are written to"
    out : string

    @clarg_positional
    @clarg_doc = "The .das files whose [metal_kernel] text to dump"
    files : array<string>
}

def dump_file(file : string; out_dir : string) : int {
    var count = 0
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.aot_module = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    to_log(LOG_ERROR, "{file}: compile failed\n{issues}\n")
                    return
                }
                simulate(program) $(var sok : bool; var pctx : smart_ptr<Context>;
                                    var serrors : das_string) {
                    if (!sok) {
                        to_log(LOG_ERROR, "{file}: simulate failed\n{serrors}\n")
                        return
                    }
                    program_for_each_module(program) $(var mod : Module?) {
                        module_for_each_global(mod) $(vinfo : VarInfo) {
                            let name = "{vinfo.name}"
                            return if (!(name |> ends_with("_msl")))
                            let p = unsafe(get_context_global_variable(*get_ptr(pctx), name))
                            return if (p == null)
                            let text = *unsafe(reinterpret<string?>(p))
                            fwrite(path_join(out_dir, "{mod.name}.{name}.msl"), text)
                            count++
                        }
                    }
                }
            }
        }
    }
    return count
}

[export]
def main : int {
    var args : DumpArgs
    let rc = parse_args_with_help(args, "msl-dump")
    return rc if (rc >= 0)
    if (empty(args.files)) {
        to_log(LOG_ERROR, "msl-dump: no files given\n")
        return 2
    }
    if (!stat(args.out).is_dir) {
        mkdir_rec(args.out)
    }
    var total = 0
    for (file in args.files) {
        let n = dump_file(file, args.out)
        to_log(LOG_INFO, "{file}: {n} kernel(s)\n")
        total += n
    }
    to_log(LOG_WARNING, "msl-dump: {total} kernel(s) under {args.out}\n")
    return total > 0 ? 0 : 1
}
