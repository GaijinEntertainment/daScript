options gen2
options indenting = 4

require mc_apply_lib

//! `probe_kind(1)` is the call macro's site: 42 when the macro registered, 1 when it did not
[export]
def main() {
    print("PROBED={probe_kind(1)}\n")
}
