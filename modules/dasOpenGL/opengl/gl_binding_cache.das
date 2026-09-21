options gen2
options persistent_heap
module gl_binding_cache shared public
require opengl/opengl public
require daslib/rtti

//! Opt-in exclusive ownership of program/sampler/raster/VAO state. End or invalidate
//! before foreign GL code. Uniform locations and values are refreshed every scope,
//! so program relinking/reuse between scopes cannot leave stale bindings.
struct GlUniformBinding { epoch : uint64; location : int = -1; bytes : array<uint8> }
struct GlProgramBindings { uniforms : table<string; GlUniformBinding> }
struct GlUniformDecision { location : int = -1; upload : bool }
var private binding_programs : table<uint; GlProgramBindings>
var private binding_textures : table<uint64; uint>
var gl_binding_filter_active = false
var private binding_epoch = 0ul
var private binding_program, binding_vao, binding_ebo : uint
var private binding_program_known, binding_vao_known, binding_ebo_known : bool
var private binding_active_texture = -1
var private binding_cull, binding_blend : int = -1
var private binding_blend_src, binding_blend_dst : uint
var private binding_blend_func_known = false
var private binding_attributes : float4[8]
var private binding_attribute_known = 0u

def gl_binding_invalidate(){
    //! Mark cached state unknown without changing the actual GL state.

    binding_epoch++; clear(binding_textures); binding_program_known = false; binding_vao_known = false; binding_ebo_known = false
    binding_active_texture = -1; binding_cull = -1; binding_blend = -1; binding_blend_func_known = false
    binding_attribute_known = 0u
}
def gl_binding_begin(){
    //! Start a non-nested ownership interval. Use gl_binding_scope for nesting.
    verify(!gl_binding_filter_active); gl_binding_invalidate(); gl_binding_filter_active = true}
def gl_binding_end(){
    //! Release ownership and restore the conventional unbound program state.
    gl_binding_filter_active = false; glUseProgram(0u); gl_binding_invalidate()}
def gl_binding_scope(enabled : bool; body : block<() : void>){
    //! Run a batch without changing the behavior of callers outside an opted-in scope.

    let own = enabled && !gl_binding_filter_active
    if (own){gl_binding_begin()}
    try {invoke(body)} recover {if (own){gl_binding_end()}; panic(this_context().last_exception)}
    if (own){gl_binding_end()}
}
def gl_binding_forget_program(program : uint){
    //! Drop retained CPU snapshots when a program is retired. No GL calls are made.
    if (key_exists(binding_programs, program)){delete binding_programs[program]; erase(binding_programs, program)}}
def gl_binding_clear(){
    //! Clear retained CPU snapshots after a context is retired.
    delete binding_programs; delete binding_textures; gl_binding_filter_active = false; gl_binding_invalidate()}
[arch(at="../ARCHITECTURE.md#binding-cache")]
def gl_binding_value_changed(var entry : GlUniformBinding; epoch : uint64; data : void?; size : int) : bool {
    let changed = entry.epoch != epoch || length(entry.bytes) != size || (size > 0 && unsafe(memcmp(unsafe(addr(entry.bytes[0])), data, size)) != 0)
    if (changed){resize(entry.bytes, size); if (size > 0){unsafe{memcpy(addr(entry.bytes[0]), data, size)}}; entry.epoch = epoch}
    return changed
}
def gl_binding_uniform(program : uint; name : string; data : void?; size : int) : GlUniformDecision {
    //! Resolve one loose uniform and report its location and upload decision.

    var program_state & = unsafe(binding_programs[program]); var entry & = program_state.uniforms[name]
    if (entry.epoch != binding_epoch){entry.location = glGetUniformLocation(program, name)}
    if (entry.location < 0){entry.epoch = binding_epoch; return GlUniformDecision()}
    let changed = gl_binding_value_changed(entry, binding_epoch, data, size)
    return GlUniformDecision(location = entry.location, upload = changed)
}
def gl_use_program_filtered(program : uint){if (!gl_binding_filter_active || !binding_program_known || binding_program != program){glUseProgram(program); binding_program = program; binding_program_known = true}}
def gl_release_program(){
    //! Preserve the bound program only while the caller owns a batch.
    if (!gl_binding_filter_active){glUseProgram(0u)}}
[arch(at="../ARCHITECTURE.md#binding-cache")]
def gl_bind_texture_filtered(unit : int; target, texture : uint){
    if (!gl_binding_filter_active){glActiveTexture(GL_TEXTURE0 + uint(unit)); glBindTexture(target, texture); return }
    let key = (uint64(target) << 32ul) | uint64(unit)
    if (key_exists(binding_textures, key) && binding_textures[key] == texture){return }
    if (binding_active_texture != unit){glActiveTexture(GL_TEXTURE0 + uint(unit)); binding_active_texture = unit}
    glBindTexture(target, texture); binding_textures[key] = texture
}
[arch(at="../ARCHITECTURE.md#binding-cache")]
def gl_material_state_filtered(double_sided, blend : bool){
    if (!gl_binding_filter_active || binding_cull != (double_sided ? 0 : 1)){
        if (double_sided){glDisable(GL_CULL_FACE)}else{glEnable(GL_CULL_FACE); glCullFace(GL_BACK)}
        binding_cull = (double_sided ? 0 : 1)
    }
    if (!gl_binding_filter_active || binding_blend != (blend ? 1 : 0)){if (blend){glEnable(GL_BLEND)}else{glDisable(GL_BLEND)}; binding_blend = (blend ? 1 : 0)}
    if (blend && (!gl_binding_filter_active || !binding_blend_func_known || binding_blend_src != GL_SRC_ALPHA || binding_blend_dst != GL_ONE_MINUS_SRC_ALPHA)){
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); binding_blend_src = GL_SRC_ALPHA; binding_blend_dst = GL_ONE_MINUS_SRC_ALPHA; binding_blend_func_known = true
    }
}
[arch(at="../ARCHITECTURE.md#binding-cache")]
def gl_bind_vertex_array_filtered(vao : uint){if (!gl_binding_filter_active || !binding_vao_known || binding_vao != vao){glBindVertexArray(vao); binding_vao = vao; binding_vao_known = true; binding_ebo_known = false}}
def gl_bind_element_buffer_filtered(ebo : uint){if (!gl_binding_filter_active || !binding_ebo_known || binding_ebo != ebo){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); binding_ebo = ebo; binding_ebo_known = true}}
[arch(at="../ARCHITECTURE.md#binding-cache")]
def gl_vertex_constant_filtered(index : uint; value : float4){
    if (!gl_binding_filter_active || index >= 8u || (binding_attribute_known & (1u << index)) == 0u || binding_attributes[index] != value){
        glVertexAttrib4f(index, value.x, value.y, value.z, value.w)
        if (index < 8u){binding_attributes[index] = value; binding_attribute_known |= 1u << index}
    }
}

def gl_binding_vertex_arrays_drawn(enabled_mask : uint){
    //! Invalidate generic values after array draws because enabled arrays may leave them unspecified.
    binding_attribute_known &= ~enabled_mask}
