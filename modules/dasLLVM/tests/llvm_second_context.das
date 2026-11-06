options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/ast_boost
require daslib/rtti
require daslib/debugger
require daslib/fio
require llvm/daslib/llvm_jit_cli
require llvm/daslib/llvm_jit_run

let private FIXTURE = "modules/dasLLVM/tests/_second_context_root.das"

typedef HolesTwice = tuple<compiled : bool; jit_ok : bool; first : int; second : int>

//! the fixture's hole counts from two contexts of one program: the
//! first runs the JIT, generating the DLL into `out_dir`, the second
//! simulates the program again, as dastest's `--together` does
def private holes_twice(out_dir : string) : HolesTwice {
    var res = (compiled = false, jit_ok = false, first = -1, second = -1)
    ast_gc_guard() {
        var inscope access <- make_file_access("")
        access |> add_extra_module("llvm_jit_finalizer",
            "{get_das_root()}/modules/dasLLVM/daslib/llvm_jit_finalizer.das")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.jit_enabled = true
                cop.version_2_syntax = true
                cop.aot_module = true
                cop.threadlock_context = true
                cop.jit_dll_mode = true
                cop.jit_output_path := "{out_dir}/second_context"
                let root = "{get_das_root()}/{FIXTURE}"
                compile_file(root, access, unsafe(addr(mg)), cop) $(ok, program, _errors) {
                    return if (!ok || program == null)
                    res.compiled = true
                    for (round in range(2)) {
                        simulate(program) $(sok, context, _serrors) {
                            return if (!sok)
                            if (round == 0) {
                                let jit = run_jit(get_ptr(program), get_ptr(context),
                                    JitCliOptions())
                                res.jit_ok = jit.ok
                            }
                            var holes = -1
                            unsafe {
                                invoke_in_context(context, "literal_holes", addr(holes))
                            }
                            if (round == 0) {
                                res.first = holes
                            } else {
                                res.second = holes
                            }
                        }
                    }
                }
            }
        }
    }
    return res
}

[test]
def test_second_context_literals(t : T?) {
    var terr : string
    let tmp = create_temp_directory("llvm_second_context", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let r = holes_twice(tmp)
    var err : string
    rmdir_rec(tmp, err)
    if (!r.compiled) {
        t |> failure("the fixture {FIXTURE} did not compile")
        return
    }
    t |> run("the first context runs the JIT and its init fills every nested literal") @(t : T?) {
        t |> success(r.jit_ok, "run_jit over the fixture")
        t |> equal(r.first, 0, "holes in the context that generated the code")
    }
    t |> run("a second context's init fills them the same: the AST kept its layout") @(t : T?) {
        t |> equal(r.second, 0, "holes in the context that installed the cached code")
    }
}
