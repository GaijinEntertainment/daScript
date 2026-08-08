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

[structure_macro(name = "tsi_test_bind_kt")]
class TsiTestBindKt : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var td = new TypeDecl(at = st.at, baseType = Type.tInt)
        st |> add_structure_alias("KT", td)
        return true
    }
}

[ |> template_struct_instance(late_bind = true)]
class template public LateKern {
    x : KT

    def take(v : KT) : KT {
        return v + x
    }
}
