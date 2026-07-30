Style stack primitives plus the ``with_style((key, val), …) { … }`` call_macro
for scoped style overrides. ``push_style_one`` is overloaded for color
(``ImGuiCol`` key with ``ImVec4`` or packed ``uint``) and var (``ImGuiStyleVar``
key with ``float`` or ``ImVec2``); ``pop_style_n(n)`` rolls back exactly ``n``
entries via a per-thread kind stack that batches into one ``PopStyleColor`` +
one ``PopStyleVar`` call. The ``with_style`` macro emits push/invoke/pop in
source order for arbitrary heterogeneous tuples — what daslang's uniform
varargs can't express.
