options gen2

module _mc_global_offset_late shared public

require _mc_global_offset_solid

[_macro]
def late_macro_init() {
    g_a += g_c
}
