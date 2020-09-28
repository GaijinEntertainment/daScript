// options log_infer_passes = true
// options log_generics = true

require fio
require rtti
require ast
require daslib/ast_boost
require daslib/cpp_bind


[export]
def test
    let root = get_das_root() + "/src/builtin/"
    fopen(root + "ast_gen.inc","wb") <| $ ( cpp_file )
        log_cpp_class_adapter(cpp_file, "AstFunctionAnnotation_Adapter", typeinfo(ast_typedecl type<AstFunctionAnnotation>))
    return true

