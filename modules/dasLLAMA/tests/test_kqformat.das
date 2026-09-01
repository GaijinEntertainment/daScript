options gen2
options _comment_hygiene = true
options _dasllama_internal = true

// Pins the K-quant format taxonomy (dasllama_kqformat): enum int values, the stride truth
// against LITERAL byte counts (not read back through the table), the predicates, and the
// one enum<->kernel-id bridge — so the two-id-space footgun ("6 means Q6_K in the kernel-id
// space and KqFmt value 6 in the enum") can never silently return.

require dastest/testing_boost public
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // the int-id delegation under test

[test]
def test_kqfmt_enum_values(t : T?) {
    t |> run("int(KqFmt.*) values are pinned - device tags and stream codes key off them") @(t : T?) {
        t |> equal(int(KqFmt.q8), 0)
        t |> equal(int(KqFmt.k4), 1)
        t |> equal(int(KqFmt.k5), 2)
        t |> equal(int(KqFmt.k6), 3)
        t |> equal(int(KqFmt.q40), 4)
        t |> equal(int(KqFmt.q51), 5)
        t |> equal(int(KqFmt.iq4xs), 6)
        t |> equal(int(KqFmt.k3), 7)
        t |> equal(int(KqFmt.iq3s), 8)
        t |> equal(int(KqFmt.iq3xxs), 9)
        t |> equal(int(KqFmt.iq4nl), 10)
        t |> equal(int(KqFmt.k2), 11)
        t |> equal(int(KqFmt.iq2s), 12)
        t |> equal(int(KqFmt.iq2xs), 13)
        t |> equal(int(KqFmt.iq2xxs), 14)
    }
}

[test]
def test_kqfmt_predicates(t : T?) {
    t |> run("kq_sb truth table: exactly the superblock lattice") @(t : T?) {
        t |> success(!kq_sb(KqFmt.q8), "q8 is not superblock")
        t |> success(kq_sb(KqFmt.k4), "k4 is superblock")
        t |> success(kq_sb(KqFmt.k5), "k5 is superblock")
        t |> success(kq_sb(KqFmt.k6), "k6 is superblock")
        t |> success(kq_sb(KqFmt.q40), "q40 is superblock")
        t |> success(!kq_sb(KqFmt.q51), "q51 rides per-32 planes")
        t |> success(kq_sb(KqFmt.iq4xs), "iq4xs is superblock")
        t |> success(kq_sb(int(KqFmt.iq4xs)), "the int-id predicate agrees for iq4xs")
        t |> success(kq_sb(KqFmt.k3), "k3 is superblock")
        t |> success(kq_sb(int(KqFmt.k3)), "the int-id predicate agrees for k3")
        t |> success(kq_sb(KqFmt.iq3s), "iq3s is superblock")
        t |> success(kq_sb(KqFmt.iq3xxs), "iq3xxs is superblock")
        t |> success(kq_sb(KqFmt.iq4nl), "iq4nl is superblock")
        t |> success(kq_sb(KqFmt.k2), "k2 is superblock")
        t |> success(kq_sb(int(KqFmt.iq3s)), "the int-id predicate agrees for iq3s")
        t |> success(kq_sb(int(KqFmt.iq3xxs)), "the int-id predicate agrees for iq3xxs")
        t |> success(kq_sb(int(KqFmt.iq4nl)), "the int-id predicate agrees for iq4nl")
        t |> success(kq_sb(int(KqFmt.k2)), "the int-id predicate agrees for k2")
        t |> success(kq_sb(KqFmt.iq2s), "iq2s is superblock")
        t |> success(kq_sb(int(KqFmt.iq2s)), "the int-id predicate agrees for iq2s")
        t |> success(kq_sb(KqFmt.iq2xs), "iq2xs is superblock")
        t |> success(kq_sb(int(KqFmt.iq2xs)), "the int-id predicate agrees for iq2xs")
        t |> success(kq_sb(KqFmt.iq2xxs), "iq2xxs is superblock")
        t |> success(kq_sb(int(KqFmt.iq2xxs)), "the int-id predicate agrees for iq2xxs")
    }
}

[test]
def test_kqfmt_strides(t : T?) {
    t |> run("typed strides match the literal byte layouts") @(t : T?) {
        t |> equal(kq_qsb(KqFmt.k4), 128l)    // 128 nibble bytes
        t |> equal(kq_ssb(KqFmt.k4), 20l)     // 16B disk block + 4B pad
        t |> equal(kq_qsb(KqFmt.k5), 160l)    // 128 nibbles + 32 qh
        t |> equal(kq_ssb(KqFmt.k5), 20l)
        t |> equal(kq_qsb(KqFmt.k6), 192l)    // 128 ql + 64 qh
        t |> equal(kq_ssb(KqFmt.k6), 18l)     // 16 sub-scales + f16 d
        t |> equal(kq_qsb(KqFmt.q40), 128l)   // k4 tiling
        t |> equal(kq_ssb(KqFmt.q40), 16l)    // 8 x f16 d
        t |> equal(kq_qsb(KqFmt.q51), 20l)    // per 32-BLOCK: 16 nibbles + 4 qh
        t |> equal(kq_ssb(KqFmt.q51), 4l)     // per 32-BLOCK: f16 d + f16 m
        t |> equal(kq_qsb(KqFmt.iq4xs), 128l) // the q40 tiling (disk k/k+16 pairing)
        t |> equal(kq_ssb(KqFmt.iq4xs), 20l)  // f16 d + 8 int8 sub-scales in the k4 row shape
        t |> equal(kq_qsb(KqFmt.k3), 96l)     // 64 qs + 32 hmask, verbatim
        t |> equal(kq_ssb(KqFmt.k3), 18l)     // the k6 row: 16 int8 sub-scales + f16 d
        t |> equal(kq_qsb(KqFmt.iq3s), 104l)  // [64 qs][8 qh][32 signs], verbatim
        t |> equal(kq_qsb(KqFmt.iq3xxs), 96l)  // [64 qs][32 aux], verbatim
        t |> equal(kq_qsb(KqFmt.iq4nl), 128l)  // q40's nibble plane, verbatim
        t |> equal(kq_ssb(KqFmt.iq3s), 20l)   // the k4 row: f16 d + 8 int8 (1 + 2s)
        t |> equal(kq_ssb(KqFmt.iq3xxs), 20l) // the same row: f16 d halved + 8 x (2ls + 1)
        t |> equal(kq_ssb(KqFmt.iq4nl), 16l)  // q40's row: 8 x f16 d
        t |> equal(kq_qsb(KqFmt.k2), 64l)     // 64 qs bytes, verbatim
        t |> equal(kq_ssb(KqFmt.k2), 20l)     // [f16 d][f16 dmin][16 sc/min bytes]
        t |> equal(kq_qsb(KqFmt.iq2s), 72l)   // [32 idx][32 signs][8 qh], verbatim
        t |> equal(kq_ssb(KqFmt.iq2s), 20l)   // f16 d/8 + 16 x (1 + 2s) strips
        t |> equal(kq_qsb(KqFmt.iq2xs), 64l)  // 32 u16 [9-bit grid idx][7-bit ksigns idx] words, verbatim
        t |> equal(kq_ssb(KqFmt.iq2xs), 20l)  // the iq2s row exactly - f16 d/8 + 16 x (1 + 2s)
        t |> equal(kq_qsb(KqFmt.iq2xxs), 64l)  // 8 blocks x [4 grid bytes][aux32 signs + ls], verbatim
        t |> equal(kq_ssb(KqFmt.iq2xxs), 20l)  // the iq2s row exactly - f16 d/8 + 16 x (1 + 2s)
    }
    t |> run("named constants agree with the literals") @(t : T?) {
        t |> equal(K4_QSB, 128l)
        t |> equal(K4_SSB, 20l)
        t |> equal(K5_QSB, 160l)
        t |> equal(K5_SSB, 20l)
        t |> equal(K6_QSB, 192l)
        t |> equal(K6_SSB, 18l)
        t |> equal(Q40_QSB, 128l)
        t |> equal(Q40_SSB, 16l)
        t |> equal(Q51_QB, 20l)
        t |> equal(Q51_SB, 4l)
        t |> equal(IQ4XS_QSB, 128l)
        t |> equal(IQ4XS_SSB, 20l)
        t |> equal(K3_QSB, 96l)
        t |> equal(K3_SSB, 18l)
        t |> equal(K2_QSB, 64l)
        t |> equal(K2_SSB, 20l)
        t |> equal(IQ2S_QSB, 72l)
        t |> equal(IQ2S_SSB, 20l)
        t |> equal(IQ2XS_QSB, 64l)
        t |> equal(IQ2XS_SSB, 20l)
        t |> equal(IQ2XXS_QSB, 64l)
        t |> equal(IQ2XXS_SSB, 20l)
        t |> equal(IQ3S_QSB, 104l)
        t |> equal(IQ3S_SSB, 20l)
    }
    t |> run("the IQ4_NL codebook is ggml's kvalues_iq4nl") @(t : T?) {
        t |> equal(int(IQ4NL_LUT[0]), -127)
        t |> equal(int(IQ4NL_LUT[7]), -10)
        t |> equal(int(IQ4NL_LUT[8]), 1)
        t |> equal(int(IQ4NL_LUT[15]), 113)
        for (i in range(15)) {
            t |> success(int(IQ4NL_LUT[i]) < int(IQ4NL_LUT[i + 1]), "the codebook is monotonic")
        }
    }
}

[test]
def test_kqfmt_grid(t : T?) {
    t |> run("the IQ3_S grid is ggml's iq3s_grid, and the worker-safe local agrees") @(t : T?) {
        t |> equal(IQ3S_GRID[0], 0x01010101u)
        t |> equal(IQ3S_GRID[511], 0x0f0f0101u)
        let lut = iq3s_grid()
        t |> equal(IQ3S_GRID[256], lut[256])
        t |> equal(IQ3S_GRID[303], lut[303])
    }
}

[test]
def test_kqfmt_id_bridge(t : T?) {
    t |> run("kq_schema_id is the one enum->kernel-id bridge (3/4/5/6/40/44/33)") @(t : T?) {
        t |> equal(kq_schema_id(KqFmt.k4), 4)
        t |> equal(kq_schema_id(KqFmt.k5), 5)
        t |> equal(kq_schema_id(KqFmt.k6), 6)
        t |> equal(kq_schema_id(KqFmt.q40), 40)
        t |> equal(kq_schema_id(KqFmt.iq4xs), 44)
        t |> equal(kq_schema_id(KqFmt.k3), 3)
        t |> equal(kq_schema_id(KqFmt.iq3s), 33)
        t |> equal(kq_schema_id(KqFmt.iq3xxs), 34)
        t |> equal(kq_schema_id(KqFmt.iq4nl), 45)
        t |> equal(kq_schema_id(KqFmt.k2), 2)
        t |> equal(kq_schema_id(KqFmt.iq2s), 23)
        t |> equal(kq_schema_id(KqFmt.iq2xs), 24)
        t |> equal(kq_schema_id(KqFmt.iq2xxs), 25)
    }
    t |> run("gemm_schema's int-id forms delegate to the same truth") @(t : T?) {
        for (f in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40, KqFmt.iq4xs, KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs]) {
            t |> equal(kq_qsb(kq_schema_id(f)), kq_qsb(f))
            t |> equal(kq_ssb(kq_schema_id(f)), kq_ssb(f))
        }
    }
}

[test]
def test_kqfmt_elems_and_stream(t : T?) {
    t |> run("elems per stride unit: 256 for the lattice, 32 elsewhere") @(t : T?) {
        t |> equal(kq_elems(KqFmt.k4), 256l)
        t |> equal(kq_elems(KqFmt.k5), 256l)
        t |> equal(kq_elems(KqFmt.k6), 256l)
        t |> equal(kq_elems(KqFmt.q40), 256l)
        t |> equal(kq_elems(KqFmt.iq4xs), 256l)
        t |> equal(kq_elems(KqFmt.k3), 256l)
        t |> equal(kq_elems(KqFmt.iq3s), 256l)
        t |> equal(kq_elems(KqFmt.iq3xxs), 256l)
        t |> equal(kq_elems(KqFmt.iq4nl), 256l)
        t |> equal(kq_elems(KqFmt.k2), 256l)
        t |> equal(kq_elems(KqFmt.iq2s), 256l)
        t |> equal(kq_elems(KqFmt.iq2xs), 256l)
        t |> equal(kq_elems(KqFmt.iq2xxs), 256l)
        t |> equal(kq_elems(KqFmt.q8), 32l)
        t |> equal(kq_elems(KqFmt.q51), 32l)
    }
    t |> run("stream/repack region codes are pinned") @(t : T?) {
        t |> equal(kq_stream_code(KqFmt.q8), 0)
        t |> equal(kq_stream_code(KqFmt.q51), 2)
        t |> equal(kq_stream_code(KqFmt.k4), 4)
        t |> equal(kq_stream_code(KqFmt.k5), 5)
        t |> equal(kq_stream_code(KqFmt.k6), 6)
        t |> equal(kq_stream_code(KqFmt.q40), 40)
        t |> equal(kq_stream_code(KqFmt.iq4xs), 44)
        t |> equal(kq_stream_code(KqFmt.k3), 3)
        t |> equal(kq_stream_code(KqFmt.iq3s), 33)
        t |> equal(kq_stream_code(KqFmt.iq3xxs), 34)
        t |> equal(kq_stream_code(KqFmt.iq4nl), 45)
        t |> equal(kq_stream_code(KqFmt.k2), 20)   // kernel id 2 = q51's stream tag
        t |> equal(kq_stream_code(KqFmt.iq2s), 23)
        t |> equal(kq_stream_code(KqFmt.iq2xs), 24)
        t |> equal(kq_stream_code(KqFmt.iq2xxs), 25)
    }
}

[test]
def test_iq3xxs_tables(t : T?) {
    t |> run("the halved IQ3_XXS grid, ksigns and the f16 halver hold their contracts") @(t : T?) {
        let lut = iq3xxs_grid()
        for (i in range(256)) {
            t |> equal(IQ3XXS_GRID[i], lut[i])
            for (b in range(4)) {
                let v = int((lut[i] >> uint(8 * b)) & 0xFFu)
                // the disk levels 4..62 halved: even 2..30 plus 31 (the 62 top level)
                t |> success(v == 31 || (v >= 2 && v <= 30 && v % 2 == 0), "halved grid byte in range")
            }
        }
        let ks = ksigns_iq2xs()
        for (i in range(128)) {
            t |> equal(int(KSIGNS_IQ2XS[i]), int(ks[i]))
            var parity = 0
            for (b in range(7)) {
                parity ^= (i >> b) & 1
            }
            t |> equal(int(ks[i]), i | (parity << 7))   // bit 7 = even parity of the 7-bit index
        }
        // exact halving: a normal f16 halves by exponent decrement (0x3400 = 0.25 -> 0x3000 =
        // 0.125), the sign rides along, and the exp==1 edge shifts into the subnormal space
        t |> equal(f16_half_bits(0x3400u), 0x3000u)
        t |> equal(f16_half_bits(0xB400u), 0xB000u)
        t |> equal(f16_half_bits(0x0400u), 0x0200u)
    }
}
