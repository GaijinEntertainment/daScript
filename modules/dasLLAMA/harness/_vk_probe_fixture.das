options gen2
options indenting = 4

// The synthetic plane fixture the Vulkan probes share: a word hash for quant bytes and a packed
// f16-pair scale word, the same shapes the kernel suite's fixtures use.

module _vk_probe_fixture shared public

//! one hashed word of synthetic quant bytes for index i
def hash_word(i : uint) : uint {
    var x = i * 2654435761u
    x ^= x >> 16u
    x *= 2246822519u
    x ^= x >> 13u
    return x
}

//! two packed f16 scales per word, small positive values (the kernel-suite fixture shape)
def cool_f16_pair(i : uint) : uint {
    let h = 0x2c00u + (i % 64u)
    return h | (h << 16u)
}
