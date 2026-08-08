options gen2

module _template_struct_instance_mod shared public

require daslib/typemacro_boost public

[ |> template_struct_instance]
class template public XModKern {
    x : KT
    @template_constant BIAS : int = 0

    def value : int {
        return x + BIAS
    }
}
