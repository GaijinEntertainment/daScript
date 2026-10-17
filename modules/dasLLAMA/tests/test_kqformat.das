options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

// Pins the K-quant format taxonomy (dasllama_kqformat): enum int values, the stride truth
// against LITERAL byte counts (not read back through the table), the predicates, and the
// one enum<->kernel-id bridge — so the two-id-space footgun ("6 means Q6_K in the kernel-id
// space and KqFmt value 6 in the enum") can never silently return. The radix guard (every id
// under the Vulkan decode block's layer-key radix) skips in a build without the vulkan module.

require dastest/testing_boost public
require dasllama/dasllama_kqformat
require dasllama/dasllama_common   // kq_fmt_row_ok - the dense-row demotion
require dasllama/dasllama_blocks   // kq_bytes_per_weight - the split estimate's per-format price
require dasllama/dasllama_gguf   // ggml_type_bytes - the disk-byte oracle of the descriptor rows; gguf_need_tensor's refusal
require dasllama/dasllama_load   // conv_row_name - the load report row a converted plane reads under
require strings
require dasllama/dasllama_gemm_schema   // the int-id delegation under test
require ?vulkan dasllama/dasllama_vulkan_common   // DAT_KEY_FMTS, the decode block's layer-key radix
require daslib/enum_trait   // the radix guard walks the whole KqFmt enum

[test]
def test_kqfmt_under_dat_key_radix(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> run("every KqFmt id sits under the decode block's layer-key radix") @(t : T?) {
            for (f in type<KqFmt>) {   //! a radix at or below a member's id hands one layer another layer's mirror and sets
                t |> success(int64(int(f)) < DAT_KEY_FMTS, "KqFmt.{f} ({int(f)}) is under DAT_KEY_FMTS ({DAT_KEY_FMTS})")
            }
        }
    } else {
        t |> skip("no vulkan module")
    }
}

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
        t |> equal(int(KqFmt.mx4), 15)
        t |> equal(int(KqFmt.iq4nl32), 16)
        t |> equal(int(KqFmt.iq3s4), 17)
        t |> equal(int(KqFmt.q4032), 18)
        t |> equal(int(KqFmt.q5032), 19)
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
        t |> success(!kq_sb(KqFmt.mx4), "mx4 rides per-32 planes")
        t |> success(kq_block32(KqFmt.mx4) && kq_block32(int(KqFmt.mx4)), "mx4 is a per-32 format on both predicates")
        t |> equal(kq_qsb(KqFmt.mx4), 16l)   // 16 nibble bytes a block
        t |> equal(kq_ssb(KqFmt.mx4), 1l)    // one E8M0 byte a block
        t |> equal(kq_elems(KqFmt.mx4), 32l)
        t |> success(!kq_sb(KqFmt.iq4nl32) && kq_block32(KqFmt.iq4nl32) && kq_block32(int(KqFmt.iq4nl32)), "iq4nl32 is a per-32 format on both predicates")
        t |> equal(kq_qsb(KqFmt.iq4nl32), 16l)   // per 32-BLOCK: the 16 nibble bytes
        t |> equal(kq_ssb(KqFmt.iq4nl32), 2l)    // per 32-BLOCK: f16 d
        t |> equal(kq_elems(KqFmt.iq4nl32), 32l)
        t |> equal(kq_ggml_type(KqFmt.iq4nl32), GGML_TYPE_IQ4_NL)   // iq4nl's disk blocks, q51's plane form
        t |> success(!kq_sb(KqFmt.q4032) && kq_block32(KqFmt.q4032) && kq_block32(int(KqFmt.q4032)), "q4032 is a per-32 format on both predicates")
        t |> equal(kq_qsb(KqFmt.q4032), 16l)   // per 32-BLOCK: the 16 nibble bytes
        t |> equal(kq_ssb(KqFmt.q4032), 2l)    // per 32-BLOCK: f16 d
        t |> equal(kq_elems(KqFmt.q4032), 32l)
        t |> equal(kq_ggml_type(KqFmt.q4032), GGML_TYPE_Q4_0)
        t |> equal(kq_desc(KqFmt.q4032).schema_id, -1)          // no kq kernel takes it by id
        t |> success(!kq_sb(KqFmt.q5032) && kq_block32(KqFmt.q5032) && kq_block32(int(KqFmt.q5032)), "q5032 is a per-32 format on both predicates")
        t |> equal(kq_qsb(KqFmt.q5032), 20l)   // per 32-BLOCK: the 16 nibble bytes and the qh word
        t |> equal(kq_ssb(KqFmt.q5032), 2l)    // per 32-BLOCK: f16 d
        t |> equal(kq_elems(KqFmt.q5032), 32l)
        t |> equal(kq_ggml_type(KqFmt.q5032), GGML_TYPE_Q5_0)
        t |> equal(kq_desc(KqFmt.q5032).schema_id, -1)          // no kq kernel takes it by id
        for (q in range(16)) {
            t |> equal(int(Q40_LUT[q]), q - 8, "the linear codebook's level {q}")
        }
        t |> success(kq_sb(KqFmt.iq3s4) && kq_sb(int(KqFmt.iq3s4)), "iq3s4 is a superblock format on both predicates")
        t |> equal(kq_qsb(KqFmt.iq3s4), IQ4XS_QSB)   // the k4 nibble tiling: 128 codebook-index bytes
        t |> equal(kq_ssb(KqFmt.iq3s4), IQ3S_SSB)    // iq3s's own scale row: f16 d + 8 x (1 + 2s)
        t |> equal(kq_elems(KqFmt.iq3s4), 256l)
        t |> equal(kq_ggml_type(KqFmt.iq3s4), GGML_TYPE_IQ3_S)   // IQ3_S disk blocks, the iq4xs plane shape
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
    t |> run("the IQ4_NL codebook is upstream's kvalues_iq4nl") @(t : T?) {
        t |> equal(int(IQ4NL_LUT[0]), -127)
        t |> equal(int(IQ4NL_LUT[7]), -10)
        t |> equal(int(IQ4NL_LUT[8]), 1)
        t |> equal(int(IQ4NL_LUT[15]), 113)
        for (i in range(15)) {
            t |> success(int(IQ4NL_LUT[i]) < int(IQ4NL_LUT[i + 1]), "the codebook is monotonic")
        }
    }
    t |> run("the iq3s4 codebook is the signed odd magnitudes of the IQ3_S grid") @(t : T?) {
        t |> equal(int(IQ3S4_LUT[0]), -15)
        t |> equal(int(IQ3S4_LUT[7]), -1)
        t |> equal(int(IQ3S4_LUT[8]), 1)
        t |> equal(int(IQ3S4_LUT[15]), 15)
        for (i in range(16)) {
            t |> equal(iq3s4_index(int(IQ3S4_LUT[i])), i, "the index inverts the codebook")
        }
        for (i in range(15)) {
            t |> equal(int(IQ3S4_LUT[i + 1]) - int(IQ3S4_LUT[i]), 2, "the codebook steps by 2")
        }
    }
}

[test]
def test_kqfmt_grid(t : T?) {
    t |> run("the IQ3_S grid is upstream's iq3s_grid, and the worker-safe local agrees") @(t : T?) {
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
        t |> equal(kq_schema_id(KqFmt.iq3s4), 35)
    }
    t |> run("gemm_schema's int-id forms delegate to the same truth") @(t : T?) {
        for (f in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40, KqFmt.iq4xs, KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs, KqFmt.iq3s4]) {
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
        t |> equal(kq_elems(KqFmt.iq3s4), 256l)
        t |> equal(kq_elems(KqFmt.q8), 32l)
        t |> equal(kq_elems(KqFmt.q51), 32l)
    }
    t |> run("every descriptor row's disk bytes are the ggml block bytes of its type, and its ids round-trip") @(t : T?) {
        for (f in type<KqFmt>) {
            let d = kq_desc(f)
            t |> equal(d.fmt, f, "row {f} names itself")
            t |> equal(d.disk_bytes, ggml_type_bytes(d.ggml_type, d.elems), "{f}: disk bytes per stride unit")
            t |> equal(d.elems, kq_sb(f) ? 256l : 32l, "{f}: stride unit")
            if (kq_sb(f)) {
                var back = KqFmt.q8
                t |> success(kq_fmt_of_id(d.schema_id, back) && back == f, "{f}: the schema id names the row back")
                t |> success(d.qsb > 0l && d.ssb > 0l, "{f}: a lattice format has a plane pair")
            } else {
                t |> equal(d.schema_id, -1, "{f}: no kernel id off the lattice")
            }
        }
        var none = KqFmt.q8
        t |> success(!kq_fmt_of_id(99, none), "an id no format carries")
        none = KqFmt.k4
        t |> success(!kq_fmt_of_id(-1, none), "the off-lattice rows' -1 names no format")
        t |> equal(none, KqFmt.k4, "a refused id leaves the out slot alone")
        t |> success(!kq_sb(-1) && !kq_sb(KQ_FMT_COUNT) && !kq_sb(99), "an int off the enum is not a lattice format")
        var tripped = false
        try {
            let f = kq_fmt_of_id(99, "test")
            t |> success(false, "unreachable: {f}")
        } recover {
            tripped = true
        }
        t |> success(tripped, "the panicking overload refuses an id no row carries")
    }
    t |> run("kq_bytes_per_weight is the stride table's arithmetic on every superblock format") @(t : T?) {
        for (f in type<KqFmt>) {
            if (kq_sb(f)) {
                let want = float(kq_qsb(f) + kq_ssb(f)) / float(kq_elems(f))
                t |> equal(kq_bytes_per_weight(f), want, "bytes per weight of {f}")
            }
        }
        t |> equal(kq_bytes_per_weight(KqFmt.q8), 34.0 / 32.0, "q8: 32 quants + an f16 scale per block")
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
        t |> equal(kq_stream_code(KqFmt.mx4), 39)
        t |> equal(kq_stream_code(KqFmt.iq4nl32), 46)
        t |> equal(kq_stream_code(KqFmt.iq3s4), 35)
        t |> equal(kq_stream_code(KqFmt.q4032), 47)
        t |> equal(kq_stream_code(KqFmt.q5032), 48)
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

[test]
def test_b32_plane_words(t : T?) {
    t |> run("a per-32 block's quant words and scale bytes are its disk bytes") @(t : T?) {
        t |> equal(int64(KQ_Q51_QWORDS * 4u + KQ_Q51_SBYTES), kq_desc(KqFmt.q51).disk_bytes, "q51: five quant words (the nibbles, the qh word) and the d | m halves")
        t |> equal(int64(KQ_B32_QWORDS * 4u + KQ_IQ4NL32_SBYTES), kq_desc(KqFmt.iq4nl32).disk_bytes, "iq4nl32: four nibble words and the d half")
        t |> equal(int64(KQ_B32_QWORDS * 4u + KQ_MX4_SBYTES), kq_desc(KqFmt.mx4).disk_bytes, "mx4: four nibble words and the E8M0 byte")
        t |> equal(int64(KQ_B32_QWORDS * 4u + KQ_Q4032_SBYTES), kq_desc(KqFmt.q4032).disk_bytes, "q4032: four nibble words and the d half")
        t |> equal(int64(KQ_Q5032_QWORDS * 4u + KQ_Q5032_SBYTES), kq_desc(KqFmt.q5032).disk_bytes, "q5032: five quant words (the nibbles, the qh word) and the d half")
        t |> equal(B32_NIB_BYTES, 16l, "the 16 nibble bytes of a per-32 block")
        t |> equal(B5_QH_BYTES, 4l, "the qh word after them")
        t |> equal(B32_NIB_BYTES + B5_QH_BYTES, kq_qsb(KqFmt.q51), "q51's quant block")
        t |> equal(B32_NIB_BYTES + B5_QH_BYTES, kq_qsb(KqFmt.q5032), "q5032's quant block")
        for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
            t |> success(kq_qsb(f) <= int64(B32_MAX_QB) && kq_ssb(f) <= int64(B32_MAX_SB),
                "{f}: one block fits the one-block scratch")
        }
    }
}

[test]
def test_kq_fmt_row_demotion(t : T?) {
    t |> run("the 32-block disk formats take their per-32 planes on rows the superblock walkers cannot tile") <| @(t : T?) {
        // 704 = 22 blocks of 32, not a 256-multiple: q40 and iq4nl to their per-32 planes, k5 to q8
        t |> equal(kq_fmt_row_ok(KqFmt.q40, 704l), KqFmt.q4032)
        t |> equal(kq_fmt_row_ok(KqFmt.q40, 650l), KqFmt.q8)              // off the 32 lattice too: the q8 rail
        t |> equal(kq_fmt_row_ok(KqFmt.iq4nl, 704l), KqFmt.iq4nl32)
        t |> equal(kq_fmt_row_ok(KqFmt.k5, 704l), KqFmt.q8)
        t |> equal(kq_fmt_row_ok(KqFmt.iq4nl, 768l), KqFmt.iq4nl)
        t |> equal(kq_fmt_row_ok(KqFmt.q40, 768l), KqFmt.q40)
        // the two other gemma widths off the superblock lattice: 1152 (gemma-3-1b dim), 2112 (gemma-4 26B dense down)
        for (w in [1152l, 2112l]) {
            t |> equal(kq_fmt_row_ok(KqFmt.q40, w), KqFmt.q4032, "q40 at {w}")
            t |> equal(kq_fmt_row_ok(KqFmt.iq4nl, w), KqFmt.iq4nl32, "iq4nl at {w}")
            t |> equal(kq_fmt_row_ok(KqFmt.k5, w), KqFmt.q8, "k5 at {w}")
            t |> equal(kq_fmt_row_ok(KqFmt.q51, w), KqFmt.q51, "q51 at {w}")
            t |> equal(kq_fmt_row_ok(KqFmt.q5032, w), KqFmt.q5032, "q5032 at {w}")
        }
        // the 256-superblock disk formats never see a non-multiple row (the file cannot hold one)
        t |> equal(kq_fmt_row_ok(KqFmt.iq2xxs, 768l), KqFmt.iq2xxs)
    }
    t |> run("an IQ4_NL or Q4_0 plane off the superblock lattice takes the per-32 planes, dense, expert or table") <| @(t : T?) {
        t |> equal(kq_fmt_expert_ok(KqFmt.iq4nl, 640l), KqFmt.iq4nl32)   // Qwen3.8-Flash-Next's down stacks: 20 blocks of 32
        t |> equal(kq_fmt_expert_ok(KqFmt.iq4nl, 768l), KqFmt.iq4nl)     // a superblock multiple keeps the lattice form
        t |> equal(kq_fmt_expert_ok(KqFmt.iq4nl, 650l), KqFmt.q8)        // off the 32 lattice too: the q8 rail
        t |> equal(kq_fmt_expert_ok(KqFmt.q40, 640l), KqFmt.q4032)       // a Q4_0 stack off the lattice: the per-32 Q4_0 planes
        t |> equal(kq_fmt_expert_ok(KqFmt.q40, 1408l), KqFmt.q4032)      // Qwen1.5-MoE's expert downs: 44 blocks of 32
        t |> equal(kq_fmt_row_ok(KqFmt.iq4nl, 896l), KqFmt.iq4nl32)      // Qwen2.5-0.5B's dense rows and table: 28 blocks of 32
        t |> equal(kq_fmt_row_ok(KqFmt.q40, 896l), KqFmt.q4032)
        t |> equal(kq_fmt_row_ok(KqFmt.q51, 2048l), KqFmt.q51)           // a q51 plane - dense or table - serves off its per-32 planes
        t |> equal(kq_fmt_row_ok(KqFmt.q51, 650l), KqFmt.q8)
    }
    t |> run("a Q5_0 plane keeps its q5032 tag on any 32-multiple row, on or off the superblock lattice") <| @(t : T?) {
        t |> equal(kq_fmt_row_ok(KqFmt.q5032, 896l), KqFmt.q5032)        // Qwen2.5-0.5B's dense rows and table
        t |> equal(kq_fmt_row_ok(KqFmt.q5032, 4864l), KqFmt.q5032)       // its FFN down: 19 superblocks, no k5 re-encode
        t |> equal(kq_fmt_expert_ok(KqFmt.q5032, 1408l), KqFmt.q5032)    // Qwen1.5-MoE's expert downs
        t |> equal(kq_fmt_row_ok(KqFmt.q5032, 650l), KqFmt.q8)           // off the 32 lattice: the q8 rail
    }
}

def private stamp_family(_f : KqTag_k4) : string => "k4"
def private stamp_family(_f : KqTag_q51) : string => "q51"
def private stamp_family(_f : auto(TT)) : string => "other:{typeinfo typename(type<TT>)}"

def private stamped_family(fmt : KqFmt) : string {
    var r = ""
    kq_fmt_stamp(fmt) <| $(F) {
        r = stamp_family(F)
    }
    return r
}

[test]
def test_kq_fmt_stamp(t : T?) {
    t |> run("kq_fmt_stamp binds every member's tag, so a per-format overload family resolves at compile time") <| @(t : T?) {
        t |> equal("k4", stamped_family(KqFmt.k4))
        t |> equal("q51", stamped_family(KqFmt.q51))
        for (f in type<KqFmt>) {
            if (f != KqFmt.k4 && f != KqFmt.q51) {
                t |> equal("other:dasllama_kqformat::KqTag_{f} const", stamped_family(f), "{f}")
            }
        }
    }
}

[test]
def test_gguf_need_tensor_refuses(t : T?) {
    t |> run("gguf_need_tensor: a tensor the file lacks panics naming the family and the tensor") @(t : T?) {
        let m : GGUFMeta
        let why = panic_text() ${
            gguf_need_tensor(m, "absent.weight", "probe")
        }
        t |> success(find(why, "probe") >= 0 && find(why, "absent.weight") >= 0, "the refusal names both: {why}")
    }
}

[test]
def test_gguf_decline_unread_types(t : T?) {
    t |> run("a tensor of a type the reader has no arm for declines the load, naming the tensor, the type and the served type to requantize to") @(t : T?) {
        for (gt, name, target in [GGML_TYPE_Q4_1, GGML_TYPE_IQ1_S, GGML_TYPE_IQ1_M, GGML_TYPE_TQ1_0, GGML_TYPE_TQ2_0],
                ["Q4_1", "IQ1_S", "IQ1_M", "TQ1_0", "TQ2_0"], ["Q5_1", "IQ2_XXS", "IQ2_XXS", "Q4_0", "Q4_0"]) {
            var m <- GGUFMeta(tensors <- [GGUFTensorInfo(name = "blk.0.ffn_up.weight", ggml_type = GGML_TYPE_Q8_0, n_elem = 256l),
                GGUFTensorInfo(name = "blk.0.ffn_down.weight", ggml_type = gt, n_elem = 256l)])
            let why = panic_text() ${
                gguf_decline_unread_types(m)
            }
            t |> success(find(why, "blk.0.ffn_down.weight") >= 0 && find(why, "{name} on disk") >= 0 && find(why, "requantize it to {target}") >= 0,
                "{name}: the decline names the tensor, the type and the served type {target}: {why}")
            delete m
        }
        let q8_1 = 9   //! ggml's Q8_1: the reader carries no arm for it, the nearest-type table no row
        var unrowed <- GGUFMeta(tensors <- [GGUFTensorInfo(name = "blk.0.ffn_gate.weight", ggml_type = q8_1, n_elem = 256l)])
        let why = panic_text() ${
            gguf_decline_unread_types(unrowed)
        }
        t |> success(find(why, "blk.0.ffn_gate.weight") >= 0 && find(why, "type 9 on disk") >= 0 && find(why, "requantize it to a served type (Q8_0)") >= 0,
            "a type with no nearest-type row names the tensor, its type id and the Q8_0 fallback: {why}")
        delete unrowed
        var served <- GGUFMeta(tensors <- [for (gt in [GGML_TYPE_F32, GGML_TYPE_BF16, GGML_TYPE_Q8_0, GGML_TYPE_Q4_K, GGML_TYPE_IQ2_XXS, GGML_TYPE_MXFP4]);
            GGUFTensorInfo(name = "t{gt}", ggml_type = gt, n_elem = 256l)])
        let none = panic_text() ${
            gguf_decline_unread_types(served)
        }
        t |> equal(none, "", "a file of served types loads on")
        delete served
    }
}

//! the Metal per-32 GEMV family's block constants (`MetalBlk32GemvT`'s `BE`, `QW`, `SH`: the template's defaults and the q5_1
//! stamps' overrides, spelled as literals in the kernel class) against the descriptor rows
[test]
def test_metal_b32_block_constants(t : T?) {
    t |> run("BE, QW and SH are the per-32 rows' block width, quant words and scale halves") @(t : T?) {
        t |> equal(kq_elems(KqFmt.q51), 32l, "BE: 32 weights a block")
        t |> equal(kq_elems(KqFmt.iq4nl32), 32l, "BE on iq4nl32")
        t |> equal(kq_qsb(KqFmt.iq4nl32), 4l * 4l, "QW = 4: iq4nl32's 16 nibble bytes")
        t |> equal(kq_qsb(KqFmt.q51), 5l * 4l, "QW = 5 on q51: the nibble words and the qh word")
        t |> equal(kq_ssb(KqFmt.iq4nl32), 1l * 2l, "SH = 1: iq4nl32's f16 d")
        t |> equal(kq_ssb(KqFmt.q51), 2l * 2l, "SH = 2 on q51: f16 d then f16 m")
        t |> equal(kq_qsb(KqFmt.q4032), 4l * 4l, "QW = 4 on q4032: iq4nl32's 16 nibble bytes")
        t |> equal(kq_ssb(KqFmt.q4032), 1l * 2l, "SH = 1 on q4032: its f16 d")
        t |> equal(kq_qsb(KqFmt.q5032), 5l * 4l, "B32W = 5 (MetalKqTileDecodeT's q5032 stamps) and QW = 5 (MetalEmbedB32Q5032, MetalMoeMulMmQ51TensorT): KQ_Q5032_QWORDS words a block")
        t |> equal(kq_ssb(KqFmt.q5032), 1l * 2l, "SH = 1 on q5032: its f16 d")
        t |> equal(kq_qsb(KqFmt.q51), int64(5u * 4u),
            "B32W = 5 on the q51 tile stamps, QW = 5 on MetalEmbedB32Q51")
        t |> equal(kq_ssb(KqFmt.q51), int64(2u * 2u),
            "B32S = 2 on the q51 tile stamps, SH = 2 on MetalEmbedB32Q51")
        t |> equal(kq_ssb(KqFmt.iq4nl32), int64(1u * 2u), "B32S = 1 and SH = 1: one d")
        t |> equal(kq_elems(KqFmt.q51), int64(32u), "B32E = 32 on MetalKqMulMmB32TensorT: a per-32 block's elements")
    }
}

//! the Metal K-quant token-table gathers' superblock constants
//! (`MetalEmbedKqT`'s `SB_UINTS`, `MetalEmbedK45T`'s `QH0`) against
//! the descriptor rows
[test]
def test_metal_kq_table_constants(t : T?) {
    t |> run("SB_UINTS and QH0 are a superblock's quant words and k5's qh strip start") @(t : T?) {
        t |> equal(kq_qsb(KqFmt.k4), int64(32u * 4u), "SB_UINTS = 32: k4's 128 nibble bytes")
        t |> equal(kq_qsb(KqFmt.k5), int64(40u * 4u), "SB_UINTS = 40 on k5: nibbles, then qh")
        t |> equal(kq_qsb(KqFmt.k4) / 4l, int64(32u), "QH0 = 32: k5's qh past k4's nibble words")
        for (f in [KqFmt.iq4xs, KqFmt.iq4nl, KqFmt.q40, KqFmt.iq3s4]) {
            t |> equal(kq_qsb(f), int64(32u * 4u), "SB_UINTS = 32 on the codebook tables: {f}")
        }
    }
}

//! a load report row names the plane's site beside its disk type and the form it serves in: a kq plane by its `KqFmt` member
[test]
def test_load_report_row_names_site(t : T?) {
    t |> run("the GGUF role reads off a tensor name, and the site words off the role") @(t : T?) {
        t |> equal(gguf_tensor_role("blk.12.attn_q.weight"), "attn_q")
        t |> equal(gguf_tensor_role("token_embd.weight"), "token_embd")
        t |> equal(gguf_tensor_role("blk.3.nextn.eh_proj.weight"), "nextn.eh_proj")
        t |> equal(kq_site_words("ffn_down_exps"), "expert stack down")
        t |> equal(kq_site_words("per_layer_token_embd"), "PLE table")
        t |> equal(kq_site_words("nextn.eh_proj"), "nextn.eh_proj", "a role with no words is named as itself")
    }
    t |> run("a row names the site, the disk type and the served form") @(t : T?) {
        t |> equal(conv_row_name("blk.3.attn_q.weight", GGML_TYPE_Q5_K, "kq transcode", ConvServed.kq, KqFmt.k5),
            "attention q: Q5_K -> k5 plane", "a kq plane by its KqFmt member")
        t |> equal(conv_row_name("token_embd.weight", GGML_TYPE_BF16, "q8 REQUANT (dequant+quant)", ConvServed.q8, KqFmt.q8),
            "token table: BF16 -> q8 REQUANT (dequant+quant)", "a converted copy by the conversion that made it")
        t |> equal(conv_row_name("blk.0.ffn_gate_exps.weight", GGML_TYPE_Q4_K, "kq transcode", ConvServed.kq, KqFmt.k4),
            "expert stack gate: Q4_K -> k4 plane")
    }
}

let private STAMPED_SKIP = "q8/b k4 !b32"
let private STAMPED_TAILS = "a b"

//! `kq_tile_stamped` answers the pairs a `kq_tile_stamp` over the same words arms: a pair word drops one tail of a member, a member
//! word every tail, `!b32` every member but the per-32 ones; the same words without `!b32` keep a superblock member (the control)
[test]
def test_kq_tile_stamped(t : T?) {
    let q51 = int(KqFmt.q51)
    let q5032 = int(KqFmt.q5032)
    let q8 = int(KqFmt.q8)
    let k4 = int(KqFmt.k4)
    let k6 = int(KqFmt.k6)
    let a = 0
    let b = 1
    t |> success(kq_tile_stamped(q51, STAMPED_SKIP, a, STAMPED_TAILS), "a per-32 member keeps its first tail")
    t |> success(kq_tile_stamped(q5032, STAMPED_SKIP, b, STAMPED_TAILS), "... and its second")
    t |> success(!kq_tile_stamped(k6, STAMPED_SKIP, a, STAMPED_TAILS), "`!b32` drops a superblock member")
    t |> success(!kq_tile_stamped(k4, STAMPED_SKIP, b, STAMPED_TAILS), "a member word drops every tail")
    t |> success(!kq_tile_stamped(q8, STAMPED_SKIP, b, STAMPED_TAILS), "a pair word drops its tail")
    t |> success(kq_tile_stamped(k6, "q8/b k4", a, STAMPED_TAILS), "without `!b32` the superblock member keeps its tails (the control)")
    t |> success(kq_tile_stamped(q8, "q8/b k4", a, STAMPED_TAILS), "a pair word keeps the member's other tail")
    t |> success(!kq_tile_stamped(k4, "q8/b k4", a, STAMPED_TAILS), "... and the member word still drops it")
}
