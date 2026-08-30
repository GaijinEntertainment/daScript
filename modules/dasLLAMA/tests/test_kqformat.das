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
        t |> success(kq_sb(int(KqFmt.iq3s)), "the int-id predicate agrees for iq3s")
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
        t |> equal(kq_ssb(KqFmt.iq3s), 20l)   // the k4 row: f16 d + 8 int8 (1 + 2s)
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
    }
    t |> run("gemm_schema's int-id forms delegate to the same truth") @(t : T?) {
        for (f in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40, KqFmt.iq4xs, KqFmt.k3, KqFmt.iq3s]) {
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
    }
}
