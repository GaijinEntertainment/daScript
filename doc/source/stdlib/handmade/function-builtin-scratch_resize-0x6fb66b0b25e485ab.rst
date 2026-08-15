resize through the eager path: reserves the exact final size first, so a grow frees the old buffer immediately even under very_safe_context; a shrink never reallocates. Requires unsafe.
