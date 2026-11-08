options gen2
require dastest/testing_boost public

require daslib/cuckoo_hash_table public

[test(description = "the cuckoo hash function never returns 0, the empty-slot marker, for any of 65536 inputs")]
def test_cuckoo_hash_never_returns_the_empty_marker(t : T?) {
    var zeros = 0
    for (i in range(0, 65536)) {
        if (hash0(i) == 0ul) {
            zeros ++
        }
        if (hash_extra(i) == 0ul) {
            zeros ++
        }
    }
    t |> equal(zeros, 0, "0 marks an empty slot, so neither cuckoo hash may return it")
}

[test(description = "hash_extra rotates the hash by 32 bits instead of shifting, so a hash that fits in 32 bits cannot collapse to 0")]
def test_hash_extra_rotates_rather_than_shifts(t : T?) {
    for (i in range(0, 256)) {
        let h = hash(i)
        let rotated = (h >> 32ul) | (h << 32ul)
        t |> equal(hash_extra(i), rotated, "hash_extra keeps the low 32 bits, so a hash that fits in 32 cannot collapse to 0")
    }
}
