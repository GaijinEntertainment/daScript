options gen2
options no_aliasing = false

module _aliasing_generic_lax public

def swap_fields(var p : auto(TT)) {
    p = TT(a = p.b, b = p.a)
}
