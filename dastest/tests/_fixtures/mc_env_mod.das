options gen2
options indenting = 4

module mc_env_mod shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/fio

[call_macro(name = "cache_env")]
class CacheEnvMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       expr : ExprCallMacro?) : ExpressionPtr {
        let value = get_env_variable("DAS_CACHE_KEY_PROBE")
        return <- qmacro($v(value))
    }
}
