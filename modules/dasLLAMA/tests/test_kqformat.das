options gen2
options _comment_hygiene = true

// Pins the K-quant format taxonomy (dasllama_kqformat): enum int values, the stride truth
// against LITERAL byte counts (not read back through the table), the predicates, and the
// one enum<->kernel-id bridge — so the two-id-space footgun ("6 means Q6_K in one space and
// q8n in the other") can never silently return.

require dastest/testing_boost public
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // the int-id delegation under test

[test]
def test_kqfmt_enum_values(t : T?) {
    t |> run("int(KqFmt.*) values are pinned — device tags and stream codes key off them") @(t : T?) {
        t |> equal(int(KqFmt.q8), 0)
        t |> equal(int(KqFmt.k4), 1)
        t |> equal(int(KqFmt.k5), 2)
        t |> equal(int(KqFmt.k6), 3)
        t |> equal(int(KqFmt.q40), 4)
        t |> equal(int(KqFmt.q51), 5)
        t |> equal(int(KqFmt.q8n), 6)
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
        t |> success(!kq_sb(KqFmt.q8n), "q8n is a device tag")
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
    }
}

[test]
def test_kqfmt_id_bridge(t : T?) {
    t |> run("kq_schema_id is the one enum->kernel-id bridge (4/5/6/40)") @(t : T?) {
        t |> equal(kq_schema_id(KqFmt.k4), 4)
        t |> equal(kq_schema_id(KqFmt.k5), 5)
        t |> equal(kq_schema_id(KqFmt.k6), 6)
        t |> equal(kq_schema_id(KqFmt.q40), 40)
    }
    t |> run("gemm_schema's int-id forms delegate to the same truth") @(t : T?) {
        for (f in [KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40]) {
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
        t |> equal(kq_elems(KqFmt.q8), 32l)
        t |> equal(kq_elems(KqFmt.q51), 32l)
        t |> equal(kq_elems(KqFmt.q8n), 32l)
    }
    t |> run("stream/repack region codes are pinned") @(t : T?) {
        t |> equal(kq_stream_code(KqFmt.q8), 0)
        t |> equal(kq_stream_code(KqFmt.q51), 2)
        t |> equal(kq_stream_code(KqFmt.k4), 4)
        t |> equal(kq_stream_code(KqFmt.k5), 5)
        t |> equal(kq_stream_code(KqFmt.k6), 6)
        t |> equal(kq_stream_code(KqFmt.q40), 40)
    }
}
