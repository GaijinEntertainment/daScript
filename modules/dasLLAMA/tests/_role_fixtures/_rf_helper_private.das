options gen2
options indenting = 4
options _dasllama_internal = true

module _rf_helper_private shared public

// Fixture: a PRIVATE function that shares the module helper's name and takes the buffer read-only - no kernel
// can call it, so it must not vote on the direction.

def private rf_mod_store(src : array<float>; i : uint; v : float) : float => src[i] + v

def rf_priv_peek(src : array<float>) : float => rf_mod_store(src, 0u, 0.)
