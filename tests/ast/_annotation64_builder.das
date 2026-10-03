options gen2
module _annotation64_builder shared
options no_unused_function_arguments = false
require daslib/ast_boost

[structure_macro(name=wide_field_arguments)]
class WideFieldArguments : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        add_annotation_argument(st.fields[0].annotation, "signed", -9007199254740991l)
        add_annotation_argument(st.fields[0].annotation, "unsigned", 18446744073709551615ul)
        add_annotation_argument(st.fields[0].annotation, args[0])
        return true
    }
}
