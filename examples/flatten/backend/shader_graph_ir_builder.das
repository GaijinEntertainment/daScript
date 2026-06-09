options gen2
options indenting = 4
module shader_graph_ir_builder shared public

//! Print-only stand-in for the native EdenSpark shader-graph IR builder.
//!
//! The real engine's `shader_graph_ir_builder` feeds nodes/pins/links into the
//! native shader-graph compiler (`sg_ir_*`). For the flatten example harness we
//! only need to SEE the emitted graph, so these implementations just print a
//! human-readable opcode dump as the backend walks the (post-flatten) shader.
//! Same call surface as the engine module, so `shader_dsl_boost` is unmodified.
//!
//! Everything prints at COMPILE time (the backend emits the graph from a macro
//! `finish` hook), so compiling a shader against this module dumps its opcodes.

require daslib/fio

// ShaderNodeKind { Input, Output, Op, Const, Var }
def private kind_name(k : int) : string {
    if (k == 0) { return "Input" }
    if (k == 1) { return "Output" }
    if (k == 2) { return "Op" }
    if (k == 3) { return "Const" }
    if (k == 4) { return "Var" }
    return "?"
}

// ShaderPinType / ShaderPropertyType share the leading float widths.
def private type_name(t : int) : string {
    if (t == 0) { return "float" }
    if (t == 1) { return "float2" }
    if (t == 2) { return "float3" }
    if (t == 3) { return "float4" }
    if (t == 4) { return "anyFloat/color" }
    if (t == 5) { return "texture2D" }
    return "?"
}

// In AST-dump mode (`run.sh --das`) the opcode graph is suppressed so the
// flattened daslang stands alone — the node/pin/link plumbing is noise when you
// just want to read flatten's output. The default `run.sh <shader>` view (and the
// regression / capability checks that parse `node ` lines) is unaffected.

def sg_ir_reset() {
    //! Begin a fresh shader graph.
    return if (sg_ir_want_ast_dump())
    print(";; ---- begin shader graph ----\n")
}

def sg_ir_set_contract(stage : string; contract : string) {
    //! Record the shader stage + I/O contract.
    return if (sg_ir_want_ast_dump())
    print(";; contract: stage={stage} type={contract}\n")
}

def sg_ir_add_property(name : string; ptype : int; defaultValue : float4; defaultTexture : string) {
    //! A material property (from a `var { ... }` block / shader argument).
    return if (sg_ir_want_ast_dump())
    if (empty(defaultTexture)) {
        print(";; property {name} : {type_name(ptype)} = {defaultValue}\n")
    } else {
        print(";; property {name} : {type_name(ptype)} = \"{defaultTexture}\"\n")
    }
}

def sg_ir_begin_node(id : int; typeId : string; kind : int; constValue : float4; propertyIndex : int) {
    //! Open a graph node. Its pins follow via sg_ir_add_*_pin.
    return if (sg_ir_want_ast_dump())
    var extra = ""
    if (kind == 3) {   // Const
        extra = "  const={constValue}"
    } elif (kind == 4) {   // Var
        extra = "  prop#{propertyIndex}"
    }
    print("node {id}  {typeId}  [{kind_name(kind)}]{extra}\n")
}

def sg_ir_add_input_pin(id : int; ptype : int) {
    return if (sg_ir_want_ast_dump())
    print("    in   pin {id} : {type_name(ptype)}\n")
}

def sg_ir_add_output_pin(id : int; ptype : int) {
    return if (sg_ir_want_ast_dump())
    print("    out  pin {id} : {type_name(ptype)}\n")
}

def sg_ir_add_link(fromPin : int; toPin : int) {
    return if (sg_ir_want_ast_dump())
    print("link  pin {fromPin} -> pin {toPin}\n")
}

def sg_ir_want_ast_dump() : bool {
    //! Opt-in flag for the post-flatten AST dump. The native engine sets this on
    //! the shader-debug disasm path; the harness wires it to `run.sh --das`
    //! (which exports FLATTEN_DUMP_DAS).
    return has_env_variable("FLATTEN_DUMP_DAS")
}

def sg_ir_set_ast_dump(astText : string) {
    //! Record the flattened daslang source the backend consumed (the branchless
    //! `?:` rewrite daslib/flatten produced). In `--das` mode this is the sole
    //! output — the opcode graph is suppressed.
    print(";; ---- flattened daslang ----\n{astText}\n")
}
