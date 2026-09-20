options gen2
options _dasllama_internal = true

//! two token sequences the same, in order - the compare every token-exact cell reads
def same_ids(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}
