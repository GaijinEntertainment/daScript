Internal dispatch used by new_thread_loop. Clones the capture into a detached
worker context and invokes its Boolean step until it returns false. When enabled,
collection runs between returned steps with the capture rooted. The final step
must release captured job-queue handles. Use new_thread_loop rather than calling
this type-erased native entry directly.
