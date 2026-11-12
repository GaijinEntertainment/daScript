options gen2
options no_aot // a must-fail fixture: nothing to AOT

require dasbind

let not_tagged = "dasbind_probe"

let @extern_library not_literal = "dasbind_{"probe"}"

[extern(cdecl, name="dasbind_probe_dbl_at45", lib=no_such_alias)]
def probe_dbl_at45(a0, a1, a2, a3 : int64; a4, a5 : double) : double => 0.0lf

[extern(cdecl, name="dasbind_probe_dbl_at45", lib=not_tagged)]
def probe_dbl_at45_not_tagged(a0, a1, a2, a3 : int64; a4, a5 : double) : double => 0.0lf

[extern(cdecl, name="dasbind_probe_dbl_at45", lib=not_literal)]
def probe_dbl_at45_not_literal(a0, a1, a2, a3 : int64; a4, a5 : double) : double => 0.0lf
