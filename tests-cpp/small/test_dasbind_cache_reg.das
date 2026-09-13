options gen2
options indenting = 4

module test_dasbind_cache_reg public

require dasbind public

[extern(cdecl, name="abs", windows_library="msvcrt", linux_library="libc.so.6", macos_library="libSystem.B.dylib")]
def c_abs(x : int) : int {
    return 0
}

//! the same native symbol under a second das signature: one bind name, two proxies
[extern(cdecl, name="abs", windows_library="msvcrt", linux_library="libc.so.6", macos_library="libSystem.B.dylib")]
def c_abs_bits(x : uint) : uint {
    return 0u
}
