options gen2
module _mint_macro shared

require daslib/ast_boost

class OneCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr => <- new ExprConstInt(at = expr.at, value = 1)
}

class MintedFunctionAnnotation : AstFunctionAnnotation {}

class MintedBlockAnnotation : AstBlockAnnotation {}

class MintedStructureAnnotation : AstStructureAnnotation {}

class MintedEnumerationAnnotation : AstEnumerationAnnotation {}

[function_macro(name = "mints")]
class MintsAnnotation : AstFunctionAnnotation {
    //! registers a call macro and one annotation of each kind named after the annotated function, from the annotation's
    //! apply, and fails the compile unless each annotation is then found in the compiling module
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        add_new_call_macro("{func.name}_one", new OneCallMacro())
        add_new_function_annotation("{func.name}_function", new MintedFunctionAnnotation())
        add_new_contract_annotation("{func.name}_contract", new MintedFunctionAnnotation())
        add_new_block_annotation("{func.name}_block", new MintedBlockAnnotation())
        add_new_structure_annotation("{func.name}_structure", new MintedStructureAnnotation())
        add_new_enumeration_annotation("{func.name}_enumeration", new MintedEnumerationAnnotation())
        for (kind in ["function", "contract", "block", "structure", "enumeration"]) {
            if (module_find_annotation(compiling_module(), "{func.name}_{kind}") == null) {
                errors := "{func.name}_{kind} is not registered into the compiling module"
                return false
            }
        }
        return true
    }
}
