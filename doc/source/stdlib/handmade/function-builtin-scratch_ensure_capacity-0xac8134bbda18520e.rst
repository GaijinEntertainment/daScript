ensure_capacity through the eager path: same doubling policy, but old buffers are freed immediately even under very_safe_context. Requires unsafe.
