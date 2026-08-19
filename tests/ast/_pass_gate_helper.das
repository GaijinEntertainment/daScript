options gen2
options no_aot

// Fixtures for the canVisitPass driver contract. The two pre-infer probes only RECORD what
// they saw (a macro_error raised mid-inference is wiped by the reporting re-leg's
// errors.clear()); the lint macro judges once the fixture module is compiled. Everything is
// keyed by module name so the probes stay silent for every other module in the process.

module _pass_gate_helper shared private

require daslib/ast_boost

let private FIXTURE = "pass_gate_fixture"

var private g_open_max_index : int = -1     // highest pass the ungated probe was applied on
var private g_zero_fired : int = 0          // gated probe apply() count
var private g_zero_bad : int = 0            // gated probe apply() with a non-zero (or unset) index

// No gate override beyond recording: must be applied on every pass.
[pre_infer_macro]
class PassGateOpen : AstPassMacro {
    last : int = -1
    def override canVisitPass(prog : ProgramPtr; mod : Module?; index : int) : bool {
        last = index
        return true
    }
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod.name == FIXTURE) {
            if (last > g_open_max_index) {
                g_open_max_index = last
            }
        }
        return true
    }
}

// Gate says pass 0 only.
[pre_infer_macro]
class PassGateZero : AstPassMacro {
    last : int = -1
    def override canVisitPass(prog : ProgramPtr; mod : Module?; index : int) : bool {
        last = index
        return index == 0
    }
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod.name == FIXTURE) {
            g_zero_fired++
            if (last != 0) {
                g_zero_bad++
            }
        }
        return true
    }
}

[lint_macro]
class PassGateVerdict : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod.name != FIXTURE) {
            return true
        }
        if (g_open_max_index < 1) {
            macro_error(prog, LineInfo(), "ungated pre-infer macro never ran past pass 0 (max index {g_open_max_index})")
        }
        if (g_zero_fired == 0) {
            macro_error(prog, LineInfo(), "gated pre-infer macro never fired on pass 0")
        }
        if (g_zero_bad != 0) {
            macro_error(prog, LineInfo(), "gated pre-infer macro was applied {g_zero_bad} time(s) outside pass 0")
        }
        return true
    }
}
