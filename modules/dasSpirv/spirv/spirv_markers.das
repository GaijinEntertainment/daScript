options gen2
options indenting = 4

//! The emitter-free marker annotations a shader body carries in
//! SPIR-V's spelling: a kernel module that stamps no shader of its
//! own - a shared template file, or another emitter's home reading
//! the same vocabulary - requires this and the builtin surface, never
//! the SPIR-V emitter. Each marker is read by name at emission; the
//! body stays CPU-callable, so reference math can call the exact code
//! the GPU runs.
module spirv_markers shared public

require spirv_builtins public
require daslib/ast
require daslib/ast_boost

//! [spirv_decode] - a cm2 DECODE function
//! (SPV_NV_cooperative_matrix2): referenced via @@name from
//! coopmatLoadTensorDecode and emitted as a real function with the
//! rigid (PhysicalStorageBuffer block pointer, uint[2], uint[2])
//! signature; the signature is validated at emission
//! (register_decode_func)
[function_macro(name="spirv_decode")]
class SpirvDecodeAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        func.moreFlags.neverInline = true
        return true
    }
}

//! [spirv_combine] - a cm2 REDUCE combine function: referenced via
//! @@name from coopmatReduce and emitted as a function of the
//! by-value (T, T) -> T shape (T = the tile component type); the
//! signature is validated at emission (register_callback_func)
[function_macro(name="spirv_combine")]
class SpirvCombineAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        func.moreFlags.neverInline = true
        return true
    }
}

//! [spirv_inline] - a shader helper the emitter inlines at every call
//! site instead of emitting a function; a marker the emitter reads,
//! the CPU body untouched
[function_macro(name="spirv_inline")]
class SpirvInlineAnnotation : AstFunctionAnnotation {}

//! [spirv_per_element] - a cm2 per-element function for
//! coopmatPerElement: by-value (row : uint, col : uint, elem : T,
//! extras...) -> T; the extras mirror the call site's trailing
//! arguments (a coopmat extra arrives as its per-element component);
//! the signature is validated at emission (register_callback_func)
[function_macro(name="spirv_per_element")]
class SpirvPerElementAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        func.moreFlags.neverInline = true
        return true
    }
}
