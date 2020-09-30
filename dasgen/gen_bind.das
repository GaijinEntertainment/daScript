// options log_infer_passes = true
// options log_generics = true

require fio
require rtti
require ast
require debugapi
require daslib/ast_boost
require daslib/cpp_bind


[export]
def test
    let root = get_das_root() + "/src/builtin/"
    fopen(root + "ast_gen.inc","wb") <| $ ( cpp_file )
        log_cpp_class_adapter(cpp_file, "AstFunctionAnnotation_Adapter", typeinfo(ast_typedecl type<AstFunctionAnnotation>))
        log_cpp_class_adapter(cpp_file, "AstStructureAnnotation_Adapter", typeinfo(ast_typedecl type<AstStructureAnnotation>))
        log_cpp_class_adapter(cpp_file, "AstPassMacro_Adapter", typeinfo(ast_typedecl type<AstPassMacro>))
        log_cpp_class_adapter(cpp_file, "AstVarianMacro_Adapter", typeinfo(ast_typedecl type<AstVariantMacro>))
        log_cpp_class_adapter(cpp_file, "AstReaderMacro_Adapter", typeinfo(ast_typedecl type<AstReaderMacro>))
        log_cpp_class_adapter(cpp_file, "AstCallMacro_Adapter", typeinfo(ast_typedecl type<AstCallMacro>))
    fopen(root + "debugapi_gen.inc","wb") <| $ ( cpp_file )
        log_cpp_class_adapter(cpp_file, "DapiDataWalker_Adapter", typeinfo(ast_typedecl type<DapiDataWalker>))
        log_cpp_class_adapter(cpp_file, "DapiDebugAgent_Adapter", typeinfo(ast_typedecl type<DapiDebugAgent>))
        log_cpp_class_adapter(cpp_file, "DapiStackWalker_Adapter", typeinfo(ast_typedecl type<DapiStackWalker>))
    return true

