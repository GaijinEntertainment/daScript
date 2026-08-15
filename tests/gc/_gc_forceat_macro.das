options gen2
options indenting = 4

module _gc_forceat_macro shared private

require daslib/ast_boost
require daslib/templates_boost
require _stackwalk_common

// generates a function the way daslib code shipping qmacro bodies does: built from a
// template in THIS file, then force_at-stamped to the annotated struct's location -
// the pattern whose output locals must still gate live in the GC walk

[structure_macro(name = "gen_forced_walker")]
class GenForcedWalker : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        // runtime-built string: a const-foldable single-use let gets legally forwarded
        // past the park by the optimizer, and then there is nothing to walk
        var fn = qmacro_function("forced_walker") $(var w : CollectingWalker?) : int {
            let forced_precious = "FORCED_{length(w.frames)}"
            run_walk(w)
            return length(forced_precious)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        compiling_module() |> add_function(fn)
        return true
    }
}
