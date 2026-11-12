options gen2
options no_aot // [extern] calls have no AOT form

module _extern_library_module public

require dasbind

let @extern_library probe_other = "dasbind_probe"

//! called from another module: lib= resolves here
[extern(cdecl, name="dasbind_probe_dbl_at45", lib=probe_other)]
def public probe_dbl_at45_other_module(a0, a1, a2, a3 : int64; a4, a5 : double) : double => 0.0lf
