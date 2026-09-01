options gen2
options indenting = 4

module mc_dep_mod shared private

require daslib/ast
require daslib/ast_boost
require daslib/fio

//! the module-cache dependency gate's fixture macro - it pins MC_DEP_FILE exactly the way a
//! tune-sidecar read does (llvm_tune's read_manifest calls add_module_cache_dependency)
[function_macro(name = "dep_probe")]
class private DepProbe : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        add_module_cache_dependency(get_env_variable("MC_DEP_FILE"))
        return true
    }
}
