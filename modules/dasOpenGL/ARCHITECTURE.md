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
alpha blend function. It leaves all other GL capabilities to the caller.

The element-buffer binding is VAO state. Switching VAOs invalidates only cached
element-buffer knowledge.

Generic vertex attribute values are context state rather than VAO state.
