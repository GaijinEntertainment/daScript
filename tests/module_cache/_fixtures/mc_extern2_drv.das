options gen2
options indenting = 4

require mc_extern_dep
require dasbind

[extern(cdecl, late, name="mc_extern_nope2", library="mc_extern_nope")]
def mc_extern_nope2(x : int) : int {
    return 0
}

[export]
def main() {
    print("EXTERN2_DRV={mc_extern_dep_probe()}\n")
}
