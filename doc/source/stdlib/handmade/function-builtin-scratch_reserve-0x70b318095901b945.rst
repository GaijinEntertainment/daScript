reserve through the eager path: the old buffer is freed immediately even under very_safe_context, and the capacity is exact (reserve never rounds). Requires unsafe.
