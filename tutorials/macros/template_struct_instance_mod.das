options gen2

module template_struct_instance_mod shared public

require daslib/typemacro_boost public

// The shared body. `class template` parses like a class but is never compiled
// on its own — only stamped copies of it are. `KT` is not declared anywhere:
// an unresolved name in type position is a type parameter, and every instance
// must bind it with a `typedef`. `@template_constant` marks a field as a
// compile-time parameter: instances pick its value with `override`, and the
// field itself is erased from the stamped class.
[ |> template_struct_instance]
class template public TopKeeperT {
    best : KT
    n : int = 0
    @template_constant KEEP_LATEST : bool = false

    def feed(v : KT) {
        if (n == 0) {
            best = v
        } else {
            static_if (KEEP_LATEST) {
                best = v
            } else {
                if (v > best) {
                    best = v
                }
            }
        }
        n++
    }
    def report : string {
        return "kept {best} out of {n}"
    }
}
