options gen2
require cbind/cbind_boost
require daslib/fio
require daslib/strings_boost
require daslib/linq
require daslib/clargs

[CommandLineArgs]
struct Config {
    @clarg_short = "i"
    @clarg_doc = "Path to the Z3 C include dir (include/ of an unpacked Z3 release)"
    input : string

    @clarg_short = "o"
    @clarg_doc = "Output folder for generated bindings (default: <das_root>/modules/dasSMT/bindings)"
    output : string = "{get_das_root()}/modules/dasSMT/bindings"

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let private z3_c_headers <- array<string>(
    "z3.h",
    "z3_macros.h",
    "z3_api.h",
    "z3_ast_containers.h",
    "z3_algebraic.h",
    "z3_polynomial.h",
    "z3_rcf.h",
    "z3_fixedpoint.h",
    "z3_optimization.h",
    "z3_fpa.h",
    "z3_spacer.h"
)

def private in_z3_headers(f : string) : bool => z3_c_headers |> any() $(h) => f |> ends_with(h)

class Z3GenDinamic : DasGenBind {
    // dasbind's fastcall wrapper supports ~13 arguments; the base default of 16
    // would emit declarations that fail at bind time
    override MAX_ARGUMENTS = 14
    def Z3GenDinamic(pfn, pfp : string; args : array<string>) {
        super(pfn, pfp, args)
        extern_library = (alias = "z3", path = "libz3.dll")
    }
    def override skip_const(name : string) : bool {
        return name == "true" || name == "false"
    }
    def override skip_alias(var c : CXCursor) {
        let aliasf = clang_getCursorLocationFileName(c)
        return !in_z3_headers(aliasf)
    }
    def override skip_function(var c : CXCursor) : bool {
        let function_name = string(clang_getCursorSpelling(c))
        // user-propagator / on-clause registration is callback-driven and
        // unreachable from das; skip to keep the surface honest
        if (function_name |> starts_with("Z3_solver_propagate_") ||
                function_name == "Z3_solver_register_on_clause") return true
        let funf = clang_getCursorLocationFileName(c)
        if (!in_z3_headers(funf)) return true
        return skip_anyFunction(c, false)
    }
    def override skip_file(fname : string) : bool {
        // Z3's DEFINE_TYPE token-pastes `_ ## T` — the pasted `struct _Z3_*`
        // RecordDecls carry a null spelling file; skip_struct narrows what gets in
        if (empty(fname)) return false
        return !in_z3_headers(fname)
    }
    def override skip_struct(name : string) {
        return !(name |> starts_with("_Z3_"))
    }
}

[export]
def main {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "bind_z3")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.input)) {
        print_help(get_command_info(type<Config>), "bind_z3")
        return
    }
    let pfn = "cb_dasSMT.h"
    let pfp = "{get_das_root()}/modules/dasSMT/src/"
    let args <- array<string>(
        "-xc++-header",
        "-std=c++1z",
        "-I{get_full_file_name(pfp)}",
        "-I{cfg.input}"
    )
    let output_name = "{cfg.output}/z3_func.das"
    fopen(output_name, "wb") $(fdf) {
        fprint(fdf, "require dasbind public\n")
        fprint(fdf, "options gen2\n")
        fprint(fdf, "module z3_func shared\n")
        fprint(fdf, "require smt/bindings/z3_struct\n")
        fprint(fdf, "require smt/bindings/z3_enum\n")
        let output_const_name = "{cfg.output}/z3_const.das"
        fopen(output_const_name, "wb") $(dcf) {
            var glbind = new Z3GenDinamic(pfn, pfp, args)
            var scf = glbind.openFile("{cfg.output}/z3_struct.das")
            var ecf = glbind.openFile("{cfg.output}/z3_enum.das")
            fprint(scf, "options gen2\nmodule z3_struct shared\n")
            fprint(ecf, "options gen2\nmodule z3_enum shared\n")
            glbind->setFiles(dcf, fdf, scf, ecf)
            glbind->generate()
            unsafe {
                delete glbind
            }
        }
    }
}
