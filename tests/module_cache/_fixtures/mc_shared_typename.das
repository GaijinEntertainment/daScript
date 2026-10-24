options gen2

require mc_shared_typename_macro

[export]
def main() {
    print("SHARED_TYPENAME={shared_typename()}\n")
    print("SHARED_TYPENAME_DLL={das_is_dll_build()}\n")
}
