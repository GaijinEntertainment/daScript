options gen2
options indenting = 4
options _dasllama_internal = true

module _rf_helper_module shared public

// Fixture: a helper another module's kernel hands a buffer to - the classifier reads the parameter's direction here.

def rf_mod_store(var dst : array<float>; i : uint; v : float) {
    dst[i] = v
}
