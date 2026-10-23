options gen2
options indenting = 4

// Measures how much of a Vulkan kernel module the CUDA emitter
// already covers: every `[spirv_kernel]` method of each MODULE file
// given goes through generate_cuda unchanged, and each emitted text
// through NVRTC when the box has a driver and NVRTC. The report
// buckets the kernels - emitted and compiled, emitted but rejected by
// NVRTC, refused by the emitter (by the refusal's first message,
// identifiers stripped) - and lists the kernels under every refusal.
// --out writes each emitted text as
// `<module>.<class>.<method>.cu` for reading.
//
//   daslang utils/internal/cuda-survey/main.das -- [--out <dir>]
//     [--emit-only] <file.das>...

require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require daslib/clargs
require daslib/cmp
require cuda/cuda_emit
require cuda/das_cuda_boost

[CommandLineArgs]
struct SurveyArgs {
    @clarg_doc = "Directory the emitted .cu texts are written to (none when empty)"
    out : string

    @clarg_doc = "The emitter pass alone: skip compiling the emitted texts through NVRTC"
    emit_only : bool

    @clarg_positional
    @clarg_doc = "The .das module files whose [spirv_kernel] methods to survey"
    files : array<string>
}

struct KernelRow {
    kname : string
    text : string
    entry : string
    refusal : string     //! the emitter's first message, "" when it emitted
    nvrtc_log : string   //! NVRTC's log when it rejected the text, "" otherwise
}

//! the message with every backtick-quoted span and every number
//! removed, so one refusal kind buckets
def refusal_kind(msg : string) : string {
    var out : array<uint8>
    var in_tick = false
    peek_data(msg) $(d) {
        for (c in d) {
            let ci = int(c)
            if (ci == '`') {
                in_tick = !in_tick
                continue
            }
            continue if (in_tick || is_number(ci))
            out |> push(c)
        }
    }
    return string(out)
}

def survey_file(file : string; var rows : array<KernelRow>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.aot_module = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    to_log(LOG_ERROR, "{file}: compile failed\n{issues}\n")
                    return
                }
                program_for_each_module(program) $(var mod : Module?) {
                    for_each_function(mod, "") $(var fn : Function?) {
                        let ann = find_annotation(fn, "spirv_kernel")
                        return if (ann == null || fn.classParent == null)
                        var err : array<string>
                        var census : table<string>
                        var smem = 0ul
                        let cfg = CudaEmitConfig(kernel_name = "",
                            block_size = uint3(
                                uint(ann.arguments |> find_arg("local_size_x") ?as tInt ?? 1),
                                uint(ann.arguments |> find_arg("local_size_y") ?as tInt ?? 1),
                                uint(ann.arguments |> find_arg("local_size_z") ?as tInt ?? 1)))
                        let text = generate_cuda(fn, err, cfg, census, smem)
                        rows |> push(KernelRow(kname = "{mod.name}.{fn.classParent.name}.{fn.name}",
                            text = text, entry = cuda_kernel_entry_name(fn, cfg),
                            refusal = empty(err) ? "" : err[0]))
                    }
                }
            }
        }
    }
}

//! true when the texts went through NVRTC; false (and a logged
//! reason) when the box cannot
def nvrtc_pass(var rows : array<KernelRow>) : bool {
    if (!cuda_available() || !cuda_nvrtc_available()) {
        to_log(LOG_WARNING,
            "cuda-survey: no CUDA driver or NVRTC on this box; the NVRTC pass is skipped\n")
        return false
    }
    var ran = false
    with_cuda_context() $(cc : CudaContext?) {
        if (cc == null) {
            to_log(LOG_WARNING, "cuda-survey: no CUDA device; the NVRTC pass is skipped\n")
            return
        }
        ran = true
        let opts <- cuda_compile_options(cc)
        for (r in rows) {
            continue if (!empty(r.refusal))
            var log : string
            var inscope image <- cuda_compile(r.text, r.entry, opts, CudaImage.cubin, log)
            if (empty(image)) {
                r.nvrtc_log = empty(log) ? "(no log)" : log
            }
        }
    }
    return ran
}

def report(rows : array<KernelRow>; nvrtc_ran : bool) {
    var emitted = 0
    var compiled = 0
    var rejected = 0
    var kinds : table<string; array<string>>
    for (r in rows) {
        if (!empty(r.refusal)) {
            kinds[refusal_kind(r.refusal)] |> push("{r.kname}: {r.refusal}")
            continue
        }
        emitted++
        if (empty(r.nvrtc_log)) {
            compiled++
        } else {
            rejected++
        }
    }
    let total = length(rows)
    let nvrtc = (nvrtc_ran ? "{compiled} compiled through NVRTC, {rejected} rejected"
        : "NVRTC pass not run")
    to_log(LOG_WARNING, "cuda-survey: {total} [spirv_kernel] methods - {emitted} emitted " +
        "({nvrtc}), {total - emitted} refused by the emitter\n")
    var sorted <- [for (k, v in keys(kinds), values(kinds)); (count = length(v), kind = k)]
    sort(sorted) <| cmp_by(-_.count)
    for (s in sorted) {
        to_log(LOG_WARNING, "  {s.count} x {s.kind}\n")
        for (kname in kinds[s.kind]) {
            to_log(LOG_INFO, "      {kname}\n")
        }
    }
    for (r in rows) {
        continue if (empty(r.nvrtc_log))
        to_log(LOG_WARNING, "  NVRTC rejects {r.kname}:\n{r.nvrtc_log}\n")
    }
}

def dump_texts(rows : array<KernelRow>; out_dir : string) {
    if (!stat(out_dir).is_dir) {
        mkdir_rec(out_dir)
    }
    for (r in rows) {
        continue if (!empty(r.refusal))
        fwrite(path_join(out_dir, "{r.kname}.cu"), r.text)
    }
}

[export]
def main : int {
    var args : SurveyArgs
    let rc = parse_args_with_help(args, "cuda-survey")
    return rc if (rc >= 0)
    if (empty(args.files)) {
        to_log(LOG_ERROR, "cuda-survey: no files given\n")
        return 2
    }
    var rows : array<KernelRow>
    for (file in args.files) {
        survey_file(file, rows)
    }
    let nvrtc_ran = !args.emit_only && nvrtc_pass(rows)
    if (!empty(args.out)) {
        dump_texts(rows, args.out)
    }
    report(rows, nvrtc_ran)
    return empty(rows) ? 1 : 0
}
