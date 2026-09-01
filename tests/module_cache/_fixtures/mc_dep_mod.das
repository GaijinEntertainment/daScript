options gen2
options indenting = 4

module mc_dep_mod shared private

require daslib/ast
require daslib/ast_boost
require daslib/fio

// The module-cache dependency gate's fixture macro: [dep_probe] registers the file named by
// MC_DEP_FILE as a compile-time input of the compiling module, exactly the way a tune-sidecar
// read does (add_module_cache_dependency in llvm_tune's read_manifest).
[function_macro(name = "dep_probe")]
class private DepProbe : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        add_module_cache_dependency(get_env_variable("MC_DEP_FILE"))
        return true
    }
}
