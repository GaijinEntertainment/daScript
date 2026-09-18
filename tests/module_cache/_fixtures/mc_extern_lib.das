options gen2
options indenting = 4

module mc_extern_lib

require dasbind

//! a late binding resolves at the first call, so the library need not exist to compile this;
//! the binder still registers the call's proxy into dasbind while this module compiles
[extern(cdecl, late, name="mc_extern_nope", library="mc_extern_nope")]
def mc_extern_nope(x : int) : int {
    return 0
}

def mc_extern_probe() : int {
    return 7
}
