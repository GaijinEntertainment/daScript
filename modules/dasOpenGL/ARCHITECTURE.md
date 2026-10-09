# OpenGL module architecture

## 1. Binding cache ownership {#binding-cache}

Generated shader binders route loose uniforms through `glUniformBound`. Outside an
owned interval, it resolves the location and uploads every call. Inside an owned
interval, it uses the binding cache.

Loose-uniform entries cache an epoch-scoped location and the exact payload bytes,
including fixed arrays. A changed epoch, size, or byte uploads again.

Texture binding cache keys combine the texture unit and target, so bindings on
different units or targets do not alias.

Material-state filtering owns culling enable and mode, blend enable, and the standard
straight-alpha blend function. RGB uses source alpha; destination alpha accumulates
coverage with source factor one, so opacity is not squared in RGBA targets.
It leaves all other GL capabilities to the caller.

The element-buffer binding is VAO state. Switching VAOs invalidates only cached
element-buffer knowledge.

Generic vertex attribute values are context state rather than VAO state.

## 2. Direct state access opt-in {#dsa}

`opengl_boost` drives GL through bind-then-edit calls by default. `try_use_dsa` switches the
helpers that create or fill objects - storage buffers, textures, std140 uniform buffers - to
the GL 4.5 named-object calls. It reads the context version first and answers false on a
context below 4.5, leaving the switch off; `use_dsa` is the same switch for a program that
requires DSA, and panics there instead, so the program fails at the switch, not at the first
missing entry point.

The `[vertex_buffer]` macro emits both `bind_vertex_buffer` overloads for every struct, so the
choice is per call rather than per switch: the pointer-offset form edits the bound VAO and
array buffer; the named form takes the VAO, the buffer and a binding slot, and attaches the
buffer to the slot with the struct's size as the stride.

A storage buffer created under DSA stays mapped, persistent and coherent, for its lifetime,
keyed by its GL name. `write_ssbo` and `read_ssbo` copy through that map when one exists and
map on demand otherwise, so buffers from either path mix in one program. A read through the
map waits on a fence first, so it returns what the GPU wrote, the same as the on-demand map
does. `delete_ssbo` unmaps and deletes.

The web build compiles `opengl_boost` against the GLES3 binding, which has no named-object
call, so every DSA branch sits under a `static_if` on the WebGL target - the platform or the
cross-compile target is `emscripten` - and compiles out there. On that target `try_use_dsa`
answers false without reading the context, and each entry point only DSA serves - the
`glTextureParameteri` and array `glNamedBufferData` / `glNamedBufferStorage` overloads, the
named `bind_vertex_buffer` - panics, so code shared with desktop still compiles.
