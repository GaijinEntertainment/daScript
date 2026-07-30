The ID-stack helper — ``with_id(s) { ... }`` — for the few cases where the
indexed-widget form (``IDENT[i]`` / ``IDENT[key]``, API_REWORK §4.4) can't reach.
Pushes ``s`` onto both ImGui's internal ID stack and the boost container-path
stack; both pop on block return. Use it for composed sub-panels rendered by a
helper that doesn't know its slot, or for data-driven structure the call_macro
can't trace lexically.
