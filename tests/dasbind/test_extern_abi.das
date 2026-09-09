options gen2
options no_aot
options rtti = true
options strict_smart_pointers = false

require dastest/testing_boost public
require dasbind
require daslib/ast
require daslib/rtti

//! every probe sums its arguments under distinct multipliers, so one argument landing in the
//! wrong register or stack slot changes the total; the library is tests/dasbind/probe

//! declares this file's probe_stack14 again, so compiling it re-applies the extern in the process
let REAPPLY_FIXTURE = "{get_das_root()}/tests/dasbind/_reapply_fixture.das"

[extern(cdecl, name="dasbind_probe_dbl_at45", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_dbl_at45(a0, a1, a2, a3 : int64; a4, a5 : double) : double { return 0.0lf; }

[extern(cdecl, name="dasbind_probe_dbl_at6", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_dbl_at6(a0, a1, a2, a3, a4, a5 : int64; a6 : double; a7 : int64) : double { return 0.0lf; }

[extern(cdecl, name="dasbind_probe_dbl_at67", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_dbl_at67(a0, a1, a2, a3, a4, a5 : int64; a6, a7 : double) : double { return 0.0lf; }

[extern(cdecl, name="dasbind_probe_regs7", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_regs7(a0 : int8; a1 : int16; a2 : int; a3 : int64; a4 : float; a5 : double; a6 : int) : int64 { return 0l; }

[extern(cdecl, name="dasbind_probe_stack14", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_stack14(a0 : int8; a1 : int16; a2 : int; a3 : int64; a4 : float; a5 : double; a6 : int;
                  a7 : int8; a8 : int8; a9 : int16; a10 : int; a11 : int64; a12 : bool; a13 : int) : int64 { return 0l; }

[extern(cdecl, name="dasbind_probe_stack_narrow", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_stack_narrow(a0, a1, a2, a3, a4, a5, a6, a7 : float; a8, a9, a10, a11, a12, a13, a14, a15 : int64;
                       a16 : int8; a17 : int16; a18 : float; a19 : int) : float { return 0.0; }

[extern(cdecl, name="dasbind_probe_ref_short", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_ref_short(r0 : float const&; a1 : int64; r2 : int const&) : int64 { return 0l; }

[extern(cdecl, name="dasbind_probe_ref_args", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_ref_args(r0 : float const&; a1, a2, a3, a4, a5, a6, a7 : int64; r8 : int const&) : int64 { return 0l; }

[extern(cdecl, name="dasbind_probe_ref_result", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_ref_result(a0, a1, a2, a3, a4, a5, a6 : int64; src : float const&) : float const& { return src; }

[extern(cdecl, name="dasbind_probe_stack_dbl_result", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_stack_dbl_result(a0 : double; a1, a2, a3, a4, a5, a6, a7, a8 : int64; a9 : int) : double { return 0.0lf; }

[test]
def test_extern_argument_passing(t : T?) {
    t |> run("doubles in positions 4 and 5 reach the callee's float registers") @(t : T?) {
        t |> equal(probe_dbl_at45(1l, 2l, 3l, 4l, 1.5lf, 2.5lf), 4331.5lf)
    }
    t |> run("a double in position 6, followed by an integer so no register residue can stand in") @(t : T?) {
        t |> equal(probe_dbl_at6(1l, 2l, 3l, 4l, 5l, 6l, 1.5lf, 7l), 7654324.0lf)
    }
    t |> run("doubles in positions 6 and 7") @(t : T?) {
        t |> equal(probe_dbl_at67(1l, 2l, 3l, 4l, 5l, 6l, 1.5lf, 2.5lf), 654331.5lf)
    }
    t |> run("seven mixed-width arguments, all in registers") @(t : T?) {
        t |> equal(probe_regs7(int8(1), int16(2), 3, 4l, 0.5, 0.25lf, 6), 4367l)
    }
    t |> run("fourteen mixed-width arguments, the tail on the stack") @(t : T?) {
        t |> equal(probe_stack14(int8(1), int16(2), 3, 4l, 0.5, 0.25lf, 6, int8(7), int8(8), int16(9), 10, 11l, true, 13), 5576l)
    }
    t |> run("eight floats and eight integers fill the registers; int8, int16, float and int land on the stack; float result") @(t : T?) {
        t |> equal(probe_stack_narrow(0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8l, 9l, 10l, 11l, 12l, 13l, 14l, 15l, int8(16), int16(17), 18.5, 19), 2687.5)
    }
    t |> run("a double result from a call with a stack argument") @(t : T?) {
        t |> equal(probe_stack_dbl_result(0.5lf, 1l, 2l, 3l, 4l, 5l, 6l, 7l, 8l, 9), 54.5lf)
    }
    t |> run("a float reference is a pointer, not a float, in the wrapper table's mask") @(t : T?) {
        let fv = 1.5
        let iv = 7
        t |> equal(probe_ref_short(fv, 4l, iv), 50l)
    }
    t |> run("a float reference rides an integer register and an int reference an 8-byte stack slot") @(t : T?) {
        let fv = 1.5
        let iv = 7
        t |> equal(probe_ref_args(fv, 1l, 2l, 3l, 4l, 5l, 6l, 7l, iv), 269l)
    }
    t |> run("a reference result comes back in the integer register, whatever it points to") @(t : T?) {
        let fv = 1.5
        t |> equal(probe_ref_result(1l, 2l, 3l, 4l, 5l, 6l, 7l, fv), 1.5)
    }
    t |> run("the call survives the extern being applied again by another program in the process") @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                compile_file(REAPPLY_FIXTURE, access, unsafe(addr(mg)), cop) $(ok, program, issues) {
                    t |> success(ok, "the fixture compiles: {issues}")
                }
            }
        }
        t |> equal(probe_stack14(int8(1), int16(2), 3, 4l, 0.5, 0.25lf, 6, int8(7), int8(8), int16(9), 10, 11l, true, 13), 5576l)
    }
}
