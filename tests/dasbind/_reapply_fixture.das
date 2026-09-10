options gen2
options no_aot

require dasbind

[extern(cdecl, name="dasbind_probe_stack14", windows_library="dasbind_probe", linux_library="dasbind_probe.so", macos_library="dasbind_probe.dylib")]
def probe_stack14(a0 : int8; a1 : int16; a2 : int; a3 : int64; a4 : float; a5 : double; a6 : int;
                  a7 : int8; a8 : int8; a9 : int16; a10 : int; a11 : int64; a12 : bool; a13 : int) : int64 { return 0l; }

[export]
def main {
    probe_stack14(int8(1), int16(2), 3, 4l, 0.5, 0.25lf, 6, int8(7), int8(8), int16(9), 10, 11l, true, 13)
}
