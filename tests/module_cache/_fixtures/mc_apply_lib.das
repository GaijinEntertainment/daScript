options gen2
options indenting = 4

module mc_apply_lib shared

require mc_apply_macro

//! the annotated def: compiling this module registers the `probe_kind` call macro into it
[mc_kind]
def probe_kind(x : int) : int {
    return x
}
