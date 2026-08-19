options gen2
options no_aot

// Fixtures for the canVisitPass driver contract. The pre-infer probes only RECORD what they
// saw (a macro_error raised mid-inference is wiped by the reporting re-leg's errors.clear());
// the lint macro judges once the fixture module is compiled.

module _pass_gate_helper shared private

require daslib/ast_boost

let private FIXTURE = "pass_gate_fixture"    // shared module: every probe filters on the module under test

var private g_open_max_index : int = -1     // highest pass the recording, always-yes probe was applied on
var private g_plain_fired : int = 0         // apply() count of the probe with no canVisitPass at all
var private g_zero_fired : int = 0          // gated probe apply() count
var private g_zero_off_pass : int = 0       // gated probe apply() with a non-zero (or unset) index

[pre_infer_macro]
class PassGateOpen : AstPassMacro {
    last : int = -1                          // the index the following apply() is judged against
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

// The pre-canVisitPass shape: no override at all, so the adapter's absent-method default applies.
[pre_infer_macro]
class PassGatePlain : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod.name == FIXTURE) {
            g_plain_fired++
        }
        return true
    }
}

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
                g_zero_off_pass++
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
            macro_error(prog, LineInfo(), "always-yes pre-infer macro never ran past pass 0 (max index {g_open_max_index})")
        }
        if (g_plain_fired < 2) {
            macro_error(prog, LineInfo(), "pre-infer macro without canVisitPass fired {g_plain_fired} time(s), expected every pass")
        }
        if (g_zero_fired == 0) {
            macro_error(prog, LineInfo(), "gated pre-infer macro never fired on pass 0")
        }
        if (g_zero_off_pass != 0) {
            macro_error(prog, LineInfo(), "gated pre-infer macro was applied {g_zero_off_pass} time(s) outside pass 0")
        }
        return true
    }
}
