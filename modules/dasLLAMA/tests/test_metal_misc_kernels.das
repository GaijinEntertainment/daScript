options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + builder dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_whisper   // nolint:STYLE030,LINT019 — wdec_kscale, read in the Apple half alone
require dasllama/dasllama_math
require dasllama/dasllama_tower     // layernorm + the shipped gelu flavors + the gemma4v pieces — the tower oracles
require dasllama/dasllama_rope      // build_rope_tabs_rows — the rope2d gate's tables
require dasllama/dasllama_qwen3v   // q3v_reorder_src - the stem assemble gate's merge walk
require _kernel_oracles   // pos_add_2d_ref
require _sel_oracle   // select_rows_oracle, sel_words: the selection landing's CPU reading, the Vulkan cell's own
require dasllama/dasllama_gpu_tier   // the SEL_* landing layout the selection gate reads its planes by
require ?das_metal dasllama/dasllama_metal_kernels  // the decode elementwise/misc kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require dasllama/dasllama_common   // bump_weights_epoch: the region and ring-slack drops; the layout shell's Model
require ?das_metal dasllama/dasllama_kqformat   // kq_fmt_of_id - the token-table gather gates name their format
require ?das_metal dasllama/dasllama_convert    // dequant_b32_plane_block - the per-32 gather gate's rows
require ?das_metal dasllama/dasllama_metal_prefill  // metal_moe_kq_twin_pick + its knee knob - the K-quant MoE twins' pick cell
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell
require _attn_oracle
require _compares   // Bar: the cells' bars
require ?das_metal _mtl_toy               // the [metal_dispatch] multi-kernel fixture
require ?das_metal dasllama/dasllama_metal_lens   // the requires= tripwire observer: captured, set, restored
require daslib/f16_cvt              // f32_to_f16 — packing the synthetic scale planes
require math
require strings
require daslib/strings_boost
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 - the row stamps' activation codes, inside the device arms

// The decode-side elementwise/misc corpus vs direct CPU oracles on synthetic data: copy_row,
// argmax (the CPU sampler's exact lowest-index tie-break, exercised with deliberate exact
// ties), the q8 and every K-quant format's token-table gathers (pow2 scales -> bit-exact), the fused add+rms family (simd_sum
// reassociates -> the shared 1e-4 bar), per-head QK-norm in both its in-place and V-fusion
// binding shapes, geglu, and the logit suppress/softcap pins. Outputs sit in sentinel-padded
// buffers compared WHOLE, so a write outside the target span is a red. Without das_metal in the
// build, or with no Metal device, every check skips.

let private EPS = 1.0e-5
let private PAD_SENTINEL = -1000.0   //! the fill of an output's pad slots: the compare holds it unmoved
let private BAR_BITS = Bar(exact = true, poison_scaled = true)   //! bit-exact; the poisoned last element moves 0.5 of its magnitude floored at 1, so no magnitude absorbs it
let private BAR_TIGHT = Bar(rel = 1e-4, poison_scaled = true)   //! the shared 1e-4 bar: simd_sum reassociation and the device transcendentals

def private copy_row_gate(t : T?; var dev, queue; n : int) {
    let tag = "copy_row n={n}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_copy_row)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope src : array<float>
        src |> resize(n)
        for (i in range(n)) {
            src[i] = 0.13 * float(i % 51) - 3.1
        }
        var inscope want : array<float>
        want |> resize(n + 16)
        for (i in range(n + 16)) {
            want[i] = i < n ? src[i] : PAD_SENTINEL   // the guard must leave the pad untouched
        }
        let bs = mc |> in_(src)
        let bd = mc |> out_words(n + 16, PAD_SENTINEL)
        let bn = mc |> in_plain([uint(n)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bs], 0ul, mc.buf[bd], 0ul, mc.buf[bn], int64(n))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bd], want, BAR_BITS, FLAT_SHAPE)
    }
}

def private argmax_gate(t : T?; var dev, queue; n, hi1, hi2 : int) {
    let tag = "argmax n={n} ties at {hi1},{hi2}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_argmax)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope lg : array<float>
        lg |> resize(n)
        for (i in range(n)) {
            lg[i] = 0.07 * float(i % 89) - 5.0
        }
        lg[hi1] = 9.5   // exact tie — the LOWER index must win (the CPU sampler's rule)
        lg[hi2] = 9.5
        let blg = mc |> in_(lg)
        let btok = mc |> out_words(1, 0xFFFFFFFFu)
        let bn = mc |> in_plain([uint(n)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[blg], mc.buf[btok], mc.buf[bn])
            }
        }
        return if (!ran)
        check_buf_exact(t, tag, mc.buf[btok], [uint(min(hi1, hi2))])
    }
}

//! the selection kernel over `rows` rows of `y` under `words`: every row's landing unpacked as the host reads it
def private cls_select_run(t : T?; var dev, queue; y : array<float>; words : array<uint>; vocab, rows : int; tag : string) : array<SelRow> {
    var out : array<SelRow>
    let cap = int(SEL_CAP)
    let hdrw = int(SEL_HDR)
    let recw = int(SEL_RECENT)
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_cls_select)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let py = mc |> in_(y)
        let ps = mc |> in_plain(words)
        let ph = mc |> out_words(hdrw * rows, 0xFFFFFFFFu)
        let pi = mc |> out_words(cap * rows, 0xFFFFFFFFu)
        let pv = mc |> out_(cap * rows)
        let pr = mc |> out_(recw * rows)
        var ka = GkClsSelArgs(vocab = uint(vocab))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[py], mc.buf[ps], mc.buf[ph], mc.buf[pi], mc.buf[pv], mc.buf[pr], ka, int64(rows))
            }
        }
        return if (!ran)
        var inscope hdr <- mc |> plane_as(ph, type<uint>, hdrw * rows)
        var inscope ids <- mc |> plane_as(pi, type<uint>, cap * rows)
        var inscope vals <- mc |> plane_as(pv, type<float>, cap * rows)
        var inscope rec <- mc |> plane_as(pr, type<float>, recw * rows)
        out <- sel_unpack( // nolint:PERF030 - fresh; cell runs once
            hdr, ids, vals, rec, words, int64(rows))
    }
    return <- out
}

//! the selection gate: the shared six-row fixture at `vocab` (`sel_fixture`) through the Metal stamp against the CPU oracle, every
//! landing compared by the shared gate (`sel_gate_compare`, the Vulkan cell's reading)
def private cls_select_gate(t : T?; var dev, queue; vocab : int) {
    let tag = "cls_select vocab={vocab}"
    var inscope f <- sel_fixture(int64(vocab))
    var inscope got <- cls_select_run(t, dev, queue, f.y, f.words, vocab, int(f.rows), tag)
    return if (long_length(got) != f.rows)
    var inscope want <- select_rows_oracle(f.y, f.temps, f.recent, int64(vocab), f.rows, SEL_K, SEL_CAP)
    sel_gate_compare(t, tag, got, want, int64(vocab))
}

def private argmax_rows_gate(t : T?; var dev, queue; nrows, n : int) {
    let tag = "argmax_rows nrows={nrows} n={n}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_argmax_rows)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope lg : array<float>
        lg |> resize(nrows * n)
        var inscope want : array<uint>
        want |> resize(nrows)
        for (r in range(nrows)) {
            for (i in range(n)) {
                lg[r * n + i] = 0.07 * float((i + 3 * r) % 89) - 5.0
            }
            let hi = (r * 37 + 11) % n
            lg[r * n + hi] = 9.5
            if (r % 2 == 1) {   // an exact tie at a HIGHER index - the lower one keeps the row (the CPU sampler's rule)
                lg[r * n + min(hi + 5, n - 1)] = 9.5
            }
            want[r] = uint(hi)
        }
        let blg = mc |> in_(lg)
        let btok = mc |> out_words(nrows, 0xFFFFFFFFu)
        let bn = mc |> in_plain([uint(n)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[blg], mc.buf[btok], mc.buf[bn], int64(nrows))
            }
        }
        return if (!ran)
        check_buf_exact(t, tag, mc.buf[btok], want)
    }
}

def private embed_q8_gate(t : T?; var dev, queue; dim, vocab, tok : int) {
    let tag = "embed_q8 dim={dim} vocab={vocab} tok={tok}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_embed_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let escale = 2.0
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(vocab, dim / 32, blob, wdeq, 256)
        let toks = fixed_array(tok, vocab - 1)   //! two rows: the grid's row axis, each row its own token
        var inscope want : array<float>
        want |> resize(2 * dim)
        for (r in range(2)) {
            for (i in range(dim)) {
                want[r * dim + i] = wdeq[toks[r] * dim + i] * escale
            }
        }
        let bblob = mc |> in_plain(blob)
        let btok = mc |> in_plain([uint(toks[0]), uint(toks[1])])
        let bxo = mc |> out_words(2 * dim, PAD_SENTINEL)
        let bdim = mc |> in_plain([uint(dim)])
        let besc = mc |> in_plain([escale])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // one blob, its byte and half-scale views
                invoke(st.enc, enc, mc.buf[bblob], 0ul, mc.buf[bblob], mc.buf[btok], 0ul, mc.buf[bxo], 0ul, mc.buf[bdim], mc.buf[besc], int64(dim), 2l)
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxo], want, BAR_BITS, FLAT_SHAPE)
    }
}

//! the greedy chain's fp32-table gather - the row gather at one row under the embed scale: the winner's row off a table
//! bound past a lead of foreign floats, times the scale, the adds off
def private embed_f32_gate(t : T?; var dev, queue; dim, vocab, tok, lead : int) {
    let tag = "embed_f32 dim={dim} vocab={vocab} tok={tok} lead={lead}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_row_gather)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let escale = 0.5
        var inscope rows <- [for (i in range(lead + vocab * dim)); i < lead ? 977.0 : float((i * 7 + 3) % 31 - 15) * 0.125]
        var inscope want <- [for (i in range(dim)); rows[lead + tok * dim + i] * escale]
        let btab = mc |> in_plain(rows)
        let btok = mc |> in_plain([uint(tok)])
        let bxo = mc |> out_words(dim, PAD_SENTINEL)
        let bnone = mc |> in_plain([1.0e9])   // the add planes, bound and never read
        var ka = GkRowGatherArgs(c = uint(dim), rows = 1u, aoff = uint(lead), boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = escale)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[btab], mc.buf[btok], mc.buf[bnone], mc.buf[bnone], mc.buf[bxo], ka, int64(dim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxo], want, BAR_BITS, FLAT_SHAPE)
    }
}

//! the token-table gather of one K-quant format through the engine's dispatcher: three rows - a middle token, token 0 and the
//! last - off the format's metal-form planes, the token list read past a lead id and the rows written past a lead row, each row
//! against the engine's own plane-row dequant under the embed scale
def private embed_kq_gate(t : T?; var dev, queue; st : auto(ST); fmt, dim, vocab : int) {
    let f = kq_fmt_of_id(fmt, "embed_kq_gate")
    let tag = "embed_kq {f} dim={dim} vocab={vocab}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nb = dim / 256
        let nsb = vocab * nb
        let escale = 2.0
        var inscope kq, ks : array<uint8>
        kq_fill_planes(fmt, nsb, kq, ks)
        let toks = fixed_array(vocab / 2, 0, vocab - 1)
        var inscope want : array<float>
        want |> resize(4 * dim)
        for (i in range(dim)) {
            want[i] = PAD_SENTINEL
        }
        var inscope wrow : array<float>
        wrow |> resize(dim)
        for (r in range(3)) {
            kq_row_ref(fmt, toks[r] * nb, nb, nsb, kq, ks, wrow)
            for (i in range(dim)) {
                want[(r + 1) * dim + i] = wrow[i] * escale
            }
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let btok = mc |> in_plain([0x7FFFFFFFu, uint(toks[0]), uint(toks[1]), uint(toks[2])])
        let bxo = mc |> out_words(4 * dim, PAD_SENTINEL)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                enc_embed_kq_rows(enc, f, mc.buf[bkq], 0ul, mc.buf[bks], 0ul, kq_split_doff(fmt, nsb), mc.buf[btok], 4ul, mc.buf[bxo],
                    uint64(dim * 4), int64(dim), 3l, escale)
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxo], want, BAR_BITS, FLAT_SHAPE)
    }
}

//! the per-32 token-table gather (`MetalEmbedB32T`) of one format: three rows - a middle token, token 0 and the last - off a
//! plane pair past `lead` blocks of other rows (an untied table sits past the classifier's), bound where `enc_embed_table` binds it
//! (`b32_table_bind`: on the 16-byte grid below, the blocks past it in `b0`), the token list read past a lead id and the rows written past a lead
//! row, each row bit for bit the engine's own block dequant under the embed scale
def private embed_b32_gate(t : T?; var dev, queue; f : KqFmt; dim, vocab, lead : int; want_b0 : int = 0) {
    let tag = "embed_b32 {f} dim={dim} vocab={vocab} lead={lead}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sq = stamp_of(@@enc_q51_embed_b32)
        let si = stamp_of(@@enc_iq4nl32_embed_b32)
        let s4 = stamp_of(@@enc_q4032_embed_b32)
        let s5 = stamp_of(@@enc_q5032_embed_b32)
        var pso = f == KqFmt.q51 ? mc |> keep_stamp(sq, err) : (f == KqFmt.iq4nl32 ? mc |> keep_stamp(si, err)
            : (f == KqFmt.q5032 ? mc |> keep_stamp(s5, err) : mc |> keep_stamp(s4, err)))
        return if (!pso_ok(t, tag, pso, err))
        let nb = dim / int(KQ_B32_ELEMS)
        let nblk = lead + vocab * nb
        let qb = int(kq_qsb(f))
        let sb = int(kq_ssb(f))
        var inscope kq, ks : array<uint8>
        kq |> resize(nblk * qb)
        ks |> resize(nblk * sb)
        for (i in range(nblk * qb)) {
            kq[i] = uint8((i * 37 + 11) & 255)
        }
        for (b in range(nblk)) {
            let d = f32_to_f16(0.0625 * float(1 + (b * 7) % 13))
            ks[b * sb] = uint8(d & 255u)
            ks[b * sb + 1] = uint8(d >> 8u)
            if (sb == 4) {
                let m = f32_to_f16(-0.5 * float(1 + (b * 5) % 7))
                ks[b * sb + 2] = uint8(m & 255u)
                ks[b * sb + 3] = uint8(m >> 8u)
            }
        }
        let escale = 2.0
        let toks = fixed_array(vocab / 2, 0, vocab - 1)
        var inscope want : array<float>
        want |> resize(4 * dim)
        for (i in range(dim)) {
            want[i] = PAD_SENTINEL
        }
        for (r in range(3)) {
            for (b in range(nb)) {
                dequant_b32_plane_block(f, kq, ks, int64(lead + toks[r] * nb + b), want, int64((r + 1) * dim + b * int(KQ_B32_ELEMS)))
            }
        }
        for (i in range(dim, 4 * dim)) {
            want[i] *= escale
        }
        let bks = mc |> in_plain(ks)
        let bkq = mc |> in_plain(kq)
        let btok = mc |> in_plain([0x7FFFFFFFu, uint(toks[0]), uint(toks[1]), uint(toks[2])])
        let bxo = mc |> out_words(4 * dim, PAD_SENTINEL)
        let bind = b32_table_bind(int64(lead) * KQ_B32_ELEMS, f)   //! the engine's own bind (`enc_embed_table`'s)
        t |> equal(int(bind.b0), want_b0, "{tag}: the table sits {want_b0} blocks past its bind")
        var ka = EmbedB32Args(dim = uint(dim), escale = escale, b0 = uint(bind.b0))
        let soff = uint64(bind.off / KQ_B32_ELEMS * int64(sb))
        let qoff = uint64(bind.off / KQ_B32_ELEMS * int64(qb))
        t |> success(soff % 16ul == 0ul && qoff % 16ul == 0ul, "{tag}: the binds sit on the 16-byte grid (scale {soff}, quant {qoff})")
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (f == KqFmt.q51) {
                stamp_run(sq, pso) {
                    invoke(sq.enc, enc, mc.buf[bks], soff, mc.buf[bkq], qoff, mc.buf[btok], 4ul, mc.buf[bxo], uint64(dim * 4), ka, int64(dim / 16), 3l)
                }
            } elif (f == KqFmt.iq4nl32) {
                stamp_run(si, pso) {
                    invoke(si.enc, enc, mc.buf[bks], soff, mc.buf[bkq], qoff, mc.buf[btok], 4ul, mc.buf[bxo], uint64(dim * 4), ka, int64(dim / 16), 3l)
                }
            } elif (f == KqFmt.q5032) {
                stamp_run(s5, pso) {
                    invoke(s5.enc, enc, mc.buf[bks], soff, mc.buf[bkq], qoff, mc.buf[btok], 4ul, mc.buf[bxo], uint64(dim * 4), ka, int64(dim / 16), 3l)
                }
            } else {
                stamp_run(s4, pso) {
                    invoke(s4.enc, enc, mc.buf[bks], soff, mc.buf[bkq], qoff, mc.buf[btok], 4ul, mc.buf[bxo], uint64(dim * 4), ka, int64(dim / 16), 3l)
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxo], want, BAR_BITS, FLAT_SHAPE)
    }
}

//! the residual step's Metal stamps (`GkAddRmsRows`): which builder a cell dispatches
enum private ArStamp {
    plain        //! the add + norm
    plain_hx     //! ... with the f16 twin beside
    pre          //! the sandwich seam: the partner under its post norm, the add, the norm
    pre_hx       //! ... with the f16 twin beside
    pre_add      //! the sandwich seam with no norm stored
}

//! the residual step against the CPU's sequential form, under the 1e-4 bar (simd_sum reassociation): x = (x + a') . ascale,
//! a' = rmsnorm(a) . wpa on the sandwich stamps, y = rmsnorm(x) . w; the sandwich stamps' rows told apart from the plain form's
def private add_rms_gate(t : T?; var dev, queue; stamp : ArStamp; single : bool; dim, nrows : int; ascale : float; window : bool = false;
                         wide : bool = false) {
    let tag = "add_rms {stamp}{single ? " 1-row grid" : ""}{window ? " window" : ""}{wide ? " 1024-lane" : ""} dim={dim} rows={nrows} ascale={ascale}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let s1 = stamp_of(@@enc_addrms_c)
        let sb = stamp_of(@@enc_add_rms_b_c)
        let sh = stamp_of(@@pf_enc_add_rms_bhx_c)
        let p1 = stamp_of(@@enc_preaddrms_c)
        let pb = stamp_of(@@enc_preaddrms_b_c)
        let ph = stamp_of(@@enc_preaddrms_bhx_c)
        let a1 = stamp_of(@@enc_preadd_c)
        let ab = stamp_of(@@enc_preadd_b_c)
        let wb = stamp_of(@@pf_enc_add_rms_b_c)
        let wh = stamp_of(@@pf_enc_add_rms_bhx_c)
        let wp = stamp_of(@@pf_enc_preaddrms_b_c)
        let wph = stamp_of(@@pf_enc_preaddrms_bhx_c)
        let wa = stamp_of(@@pf_enc_preadd_b_c)
        let lb = stamp_of(@@enc_add_rms_bw_c)
        let lh = stamp_of(@@enc_add_rms_bhxw_c)
        let lp = stamp_of(@@enc_preaddrms_bw_c)
        let lph = stamp_of(@@enc_preaddrms_bhxw_c)
        let la = stamp_of(@@enc_preadd_bw_c)
        var pso = (stamp == ArStamp.plain ? mc |> keep_stamp(s1, err) : (stamp == ArStamp.plain_hx ? mc |> keep_stamp(sh, err)
            : (stamp == ArStamp.pre ? mc |> keep_stamp(p1, err) : (stamp == ArStamp.pre_hx ? mc |> keep_stamp(ph, err) : mc |> keep_stamp(a1, err)))))
        return if (!pso_ok(t, tag, pso, err))
        let pre = stamp != ArStamp.plain && stamp != ArStamp.plain_hx
        let total = nrows * dim
        var inscope xv, av, w : array<float>
        xv |> resize(total)
        av |> resize(total)
        w |> resize(2 * dim)   //! [post norm | norm], `seam_norms`' layout on a sandwich stamp; a plain stamp binds the norm row alone
        for (i in range(total)) {
            xv[i] = 0.11 * float(i % 43) - 2.3
            av[i] = 0.07 * float(i % 31) - 1.1
        }
        for (i in range(2 * dim)) {
            w[i] = i < dim ? 0.4 + 0.05 * float(i % 11) : 0.5 + 0.04 * float(i % 13)
        }
        var inscope want_x, want_y, plain_x, an : array<float>
        want_x |> resize(total)
        want_y |> resize(total)
        plain_x |> resize(total)
        an |> resize(total)
        unsafe {
            for (r in range(nrows)) {
                rmsnorm(addr(an[r * dim]), addr<float const?>(av[r * dim]), addr<float const?>(w[0]), int64(dim), EPS)
            }
        }
        for (i in range(total)) {
            want_x[i] = (xv[i] + (pre ? an[i] : av[i])) * ascale
            plain_x[i] = (xv[i] + av[i]) * ascale
        }
        unsafe {
            for (r in range(nrows)) {
                rmsnorm(addr(want_y[r * dim]), addr<float const?>(want_x[r * dim]), addr<float const?>(w[dim]), int64(dim), EPS)
            }
        }
        var ka = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = EPS, ascale = ascale)
        var inscope wnorm <- [for (i in range(dim)); w[dim + i]]
        let bx = mc |> io_(xv)
        let ba = mc |> in_(av)
        let bw = pre ? mc |> in_(w) : mc |> in_(wnorm)
        let by = mc |> out_words(total, PAD_SENTINEL)
        let bh = mc |> out_words(total, float16(PAD_SENTINEL))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (x, a, w1, y, yh) = (mc.buf[bx], mc.buf[ba], mc.buf[bw], mc.buf[by], mc.buf[bh])
            if (window) {   //! the window's builders: the same kernels at 256 lanes a row
                if (stamp == ArStamp.plain) {
                    stamp_run(wb, pso) {
                        invoke(wb.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.plain_hx) {
                    stamp_run(wh, pso) {
                        invoke(wh.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.pre) {
                    stamp_run(wp, pso) {
                        invoke(wp.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.pre_hx) {
                    stamp_run(wph, pso) {
                        invoke(wph.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                    }
                } else {
                    stamp_run(wa, pso) {
                        invoke(wa.enc, enc, x, a, w1, ka, int64(nrows))
                    }
                }
            } elif (wide) {   //! the decode rows' builders past the slab: the same kernels at 1024 lanes a row
                if (stamp == ArStamp.plain) {
                    stamp_run(lb, pso) {
                        invoke(lb.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.plain_hx) {
                    stamp_run(lh, pso) {
                        invoke(lh.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.pre) {
                    stamp_run(lp, pso) {
                        invoke(lp.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                } elif (stamp == ArStamp.pre_hx) {
                    stamp_run(lph, pso) {
                        invoke(lph.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                    }
                } else {
                    stamp_run(la, pso) {
                        invoke(la.enc, enc, x, a, w1, ka, int64(nrows))
                    }
                }
            } elif (stamp == ArStamp.plain) {
                if (single) {   //! the single-row builder's one threadgroup
                    stamp_run(s1, pso) {
                        invoke(s1.enc, enc, x, a, w1, y, ka)
                    }
                } else {
                    stamp_run(sb, pso) {
                        invoke(sb.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                }
            } elif (stamp == ArStamp.plain_hx) {
                stamp_run(sh, pso) {
                    invoke(sh.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                }
            } elif (stamp == ArStamp.pre) {
                if (single) {
                    stamp_run(p1, pso) {
                        invoke(p1.enc, enc, x, a, w1, y, ka)
                    }
                } else {
                    stamp_run(pb, pso) {
                        invoke(pb.enc, enc, x, a, w1, y, ka, int64(nrows))
                    }
                }
            } elif (stamp == ArStamp.pre_hx) {
                stamp_run(ph, pso) {
                    invoke(ph.enc, enc, x, a, w1, y, yh, ka, int64(nrows))
                }
            } elif (single) {
                stamp_run(a1, pso) {
                    invoke(a1.enc, enc, x, a, w1, ka)
                }
            } else {
                stamp_run(ab, pso) {
                    invoke(ab.enc, enc, x, a, w1, ka, int64(nrows))
                }
            }
        }
        return if (!ran)
        check_buf(t, "{tag} x", mc.buf[bx], want_x, BAR_TIGHT, Shape(row_width = dim))
        t |> success(moved_off(want_x, xv, 0, BAR_TIGHT) && moved_off(want_x, xv, total - 1, BAR_TIGHT),
            "{tag} x in place: the first row's first column and the last row's last column moved off x")
        if (stamp == ArStamp.pre_add) {
            var inscope untouched <- [for (_i in range(total)); PAD_SENTINEL]
            check_buf(t, "{tag} y untouched", mc.buf[by], untouched, BAR_EXACT, Shape(row_width = dim))
        } else {
            check_buf(t, "{tag} y", mc.buf[by], want_y, BAR_TIGHT, Shape(row_width = dim))
        }
        if (stamp == ArStamp.plain_hx || stamp == ArStamp.pre_hx) {
            check_hx_narrow(t, "{tag} y twin", mc.buf[bh], mc.buf[by], total, total)
        }
        if (pre) {
            var inscope got <- buf_download(mc.buf[bx], total, type<float>)
            t |> success(check_count(got, plain_x, BAR_TIGHT) > 0, "{tag}: the rows read off the plain step's (the partner is normed)")
        }
    }
}

let private QKN_V_NONE = 0
let private QKN_V_FK = 1     //! the k groups also write V: the raw k row under its rms (a layer with no v plane)
let private QKN_V_NORM = 2   //! nk more groups norm the V rows in place, weightless (gemma-4's v-norm)

// per-head QK-norm: vfusion=false is the in-place decode shape (soffe == koffe), vfusion=true
// the no-wv form (reads the pre-norm K slice, writes the v segment); `vmode` the V arms of the in-place shape
def private qk_norm_gate(t : T?; var dev, queue; hs, nq, nk, nrows : int; vfusion : bool; vmode : int = QKN_V_NONE) {
    let tag = "qk_norm hs={hs} nq={nq} nk={nk} rows={nrows} vfusion={vfusion} vmode={vmode}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_qk_norm)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let qd = nq * hs
        let kvd = nk * hs
        let rstride = qd + 2 * kvd
        let soffe = qd                          // K region read base
        let koffe = vfusion ? qd + kvd : qd     // write base: v segment under fusion, in-place otherwise
        var inscope xv, w : array<float>
        xv |> resize(nrows * rstride)
        w |> resize(2 * hs)
        for (i in range(nrows * rstride)) {
            xv[i] = 0.11 * float(i % 47) - 2.5
        }
        for (i in range(2 * hs)) {
            w[i] = 0.5 + 0.04 * float(i % 19)
        }
        var inscope want := xv
        var inscope ones <- [for (_i in range(hs)); 1.0]
        let vbase = qd + kvd
        for (r in range(nrows)) {
            for (tg2 in range(nq + nk)) {
                let isK = tg2 >= nq
                let h = isK ? tg2 - nq : tg2
                let rbase = r * rstride + (isK ? soffe : 0) + h * hs
                let wbase = r * rstride + (isK ? koffe : 0) + h * hs
                unsafe {
                    rmsnorm(addr(want[wbase]), addr<float const?>(xv[rbase]), addr<float const?>(w[isK ? hs : 0]), int64(hs), EPS)
                }
            }
            for (h in range(nk)) {
                let vrow = r * rstride + vbase + h * hs
                if (vmode == QKN_V_FK) {
                    unsafe {
                        rmsnorm(addr(want[vrow]), addr<float const?>(xv[r * rstride + soffe + h * hs]), addr<float const?>(ones[0]), int64(hs), EPS)
                    }
                } elif (vmode == QKN_V_NORM) {
                    unsafe {
                        rmsnorm(addr(want[vrow]), addr<float const?>(xv[vrow]), addr<float const?>(ones[0]), int64(hs), EPS)
                    }
                }
            }
        }
        let bx = mc |> io_(xv)
        let bw = mc |> in_(w)
        var ka = qk_norm_args(uint(nq), uint(nk), uint(hs), uint(rstride), uint(soffe), uint(koffe), uint(hs), EPS)
        ka.vfk = vmode == QKN_V_FK ? 1u : 0u
        ka.vnorm = vmode == QKN_V_NORM ? 1u : 0u
        ka.vbase = uint(vbase)
        let ngroups = nq + nk + (vmode == QKN_V_NORM ? nk : 0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[bx], mc.buf[bw], ka, int64(ngroups), int64(nrows))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_TIGHT, Shape(row_width = rstride, head_width = hs))
    }
}

//! one in-place two-input activation stamp `st` (a `stamp_of` of its g, u, total builder) against `oracle`, which rewrites `want` - a copy of g - from g and u
def private ew2_act_gate(t : T?; var dev, queue; name : string; st; total : int;
                         oracle : block<(var want : array<float>; g, u : array<float>) : void>) {
    let tag = "{name} total={total}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope g, u : array<float>
        g |> resize(total)
        u |> resize(total)
        for (i in range(total)) {
            g[i] = 0.21 * float(i % 43) - 4.3
            u[i] = 0.17 * float(i % 23) - 1.9
        }
        var inscope want := g
        invoke(oracle, want, g, u)
        let bg = mc |> io_(g)
        let bu = mc |> in_(u)
        let btot = mc |> in_plain([uint(total)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], 0ul, mc.buf[bu], 0ul, mc.buf[btot], int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bg], want, BAR_TIGHT, FLAT_SHAPE)
    }
}

//! The CPU's f16-table GEGLU-quick on the shared stamp with the gemma4v bounds fused: at open bounds the kernel's binary16 round
//! trips land on the table values (BAR_TIGHT: the exp identity's rounding parts from the table's tanh by under a tenth of an f16 ulp);
//! at tight bounds the output moves (the control).
def private geglu_gate(t : T?; var dev, queue; total, ff : int) {
    let tag = "geglu_quick total={total}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_geglu_quick)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope g <- [for (i in range(total)); 0.21 * float(i % 43) - 4.3]
        var inscope u <- [for (i in range(total)); 0.17 * float(i % 23) - 1.9]
        var inscope want := g
        geglu_quick_lut_rows(want, g, u, int64(ff), int64(total / ff))
        var inscope tight : array<float>
        tight |> resize(total)
        for (i in range(total)) {
            tight[i] = clamp(gelu_quick_lut_f32(clamp(g[i], -1.0, 1.5)) * clamp(u[i], -0.5, 0.75), -0.3, 0.4)
        }
        let bg = mc |> io_(g)
        let bu = mc |> in_(u)
        var ka = GkGegluArgs(nelem = uint(total), glo = -1e30, ghi = 1e30, ulo = -1e30, uhi = 1e30, dlo = -1e30, dhi = 1e30)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], mc.buf[bu], ka, int64((total + 3) / 4))
            }
        }
        return if (!ran)
        t |> success(mismatch_exact(want, g) > 0, "{tag}: the reference moves off the input")
        check_buf(t, tag, mc.buf[bg], want, BAR_TIGHT, FLAT_SHAPE)
        mc |> plane_upload(bg, g)
        var kt = GkGegluArgs(nelem = uint(total), glo = -1.0, ghi = 1.5, ulo = -0.5, uhi = 0.75, dlo = -0.3, dhi = 0.4)
        let ran2 = mc |> cell_record(t, "{tag} bounds") $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], mc.buf[bu], kt, int64((total + 3) / 4))
            }
        }
        return if (!ran2)
        check_buf(t, "{tag} bounds", mc.buf[bg], tight, BAR_TIGHT, FLAT_SHAPE)
        var apart = 0
        for (a, b in tight, want) {
            apart += abs(a - b) > 1e-4 ? 1 : 0
        }
        t |> success(apart > total / 4, "{tag}: the bounds move the rows ({apart} of {total}) - the control")
    }
}

// ===== the gemma4v tower primitives =====

// clamp: dst = clamp(src, lo, hi) with the bounds read off a clamp record in a blob-shaped buffer
// (`side` picks the input or output pair); exact vs the CPU clamp, the pad sentinel untouched.
let private CLAMP_IN = 0    // the record's input pair (imin, imax)
let private CLAMP_OUT = 1   // its output pair (omin, omax)

def private clamp_gate(t : T?; var dev, queue; total : int; side : int; inplace : bool) {
    let tag = "clamp total={total} {side == CLAMP_IN ? "in" : "out"} inplace={inplace}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_clamp_rows)
        let sc = stamp_of(@@enc_clamp_copy)
        var pso = inplace ? (mc |> keep_stamp(st, err)) : (mc |> keep_stamp(sc, err))
        return if (!pso_ok(t, tag, pso, err))
        var inscope src : array<float>
        src |> resize(total)
        for (i in range(total)) {
            src[i] = 0.37 * float(i % 97) - 17.0   // -17 .. 18.5
        }
        // a record like the tower's: imin imax omin omax active, sitting past some junk floats
        var inscope rec <- [0.0, 0.0, 0.0, -6.0, 5.0, -2.5, 9.25, 1.0]
        let lo = side == CLAMP_IN ? rec[3] : rec[5]
        let hi = side == CLAMP_IN ? rec[4] : rec[6]
        var inscope want : array<float>
        want |> resize(total + 16)
        for (i in range(total + 16)) {
            want[i] = i < total ? clamp(src[i], lo, hi) : PAD_SENTINEL
        }
        if (inplace) {
            want |> resize(total)
        }
        let bs = mc |> io_(src)
        let bd = inplace ? bs : (mc |> out_words(total + 16, PAD_SENTINEL))
        var ka = GkClampArgs(nelem = uint(total), lo = lo, hi = hi, scale = 1.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (inplace) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bs], ka, int64((total + 3) / 4))
                }
            } else {
                stamp_run(sc, pso) {
                    invoke(sc.enc, enc, mc.buf[bs], mc.buf[bd], ka, int64((total + 3) / 4))
                }
            }
        }
        return if (!ran)
        t |> success(mismatch_exact(subarray(want, range(total)), src) > 0, "{tag}: the reference moves off the input")
        check_buf(t, tag, mc.buf[bd], want, BAR_BITS, FLAT_SHAPE)
    }
}

//! The head restride on the shared template's two Metal stamps: the pad writes the tile panel [npos x heads x hsp] off the compact
//! rows with zero lanes past hs and the output past the panel keeps its sentinel; the unpad reads the compact rows back off a
//! padded panel. Both bit for bit against the host walk.
def private head_restride_gate(t : T?; var dev, queue; npos, n_head, hs, hsp : int) {
    let tag = "head_restride npos={npos} heads={n_head} {hs}->{hsp}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sp = stamp_of(@@enc_head_pad)
        let su = stamp_of(@@enc_head_unpad)
        var pso_p = mc |> keep_stamp(sp, err)
        return if (!pso_ok(t, "{tag} pad", pso_p, err))
        var pso_u = mc |> keep_stamp(su, err)
        return if (!pso_ok(t, "{tag} unpad", pso_u, err))
        let d = n_head * hs
        let total_p = npos * n_head * hsp
        var inscope src : array<float>
        src |> resize(npos * d)
        for (i in range(length(src))) {
            src[i] = 0.31 * float(i % 89) - 13.0
        }
        var inscope want_p : array<float>
        want_p |> resize(total_p + 16)
        for (i in range(total_p + 16)) {
            if (i >= total_p) {
                want_p[i] = PAD_SENTINEL   // the pad sentinel
            } else {
                let lane = i % hsp
                let hp = i / hsp
                want_p[i] = lane < hs ? src[hp * hs + lane] : 0.0
            }
        }
        var ka = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos), nheads = uint(n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u, boff = 0u)
        let bs = mc |> in_(src)
        let bp = mc |> out_words(total_p + 16, PAD_SENTINEL)
        let bc = mc |> out_words(npos * d, PAD_SENTINEL)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(sp, pso_p) {
                invoke(sp.enc, enc, mc.buf[bs], mc.buf[bp], ka, int64(total_p))
            }
            metal_memory_barrier(enc)
            stamp_run(su, pso_u) {
                invoke(su.enc, enc, mc.buf[bp], mc.buf[bc], ka, int64(total_p))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} pad", mc.buf[bp], want_p, BAR_BITS, Shape(row_width = hsp))
        check_buf(t, "{tag} unpad", mc.buf[bc], src, BAR_BITS, Shape(row_width = hs))
    }
}

// two-axis NEOX rope over token-major rows vs the CPU rope_neox_2d_rows on the same tables (the
// kernel reads the four tables packed per row as [cx|sx|cy|sy]); approx bar (fma ordering).
def private rope2d_gate(t : T?; var dev, queue; npos, n_head, hs : int) {
    let d = n_head * hs
    let pairs = hs / 4   // rotated pairs per axis, one packed table row
    let tag = "rope2d npos={npos} heads={n_head} hs={hs}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rope2d)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope pos_x, pos_y : array<int64>
        pos_x |> resize(npos)
        pos_y |> resize(npos)
        for (i in range(npos)) {
            pos_x[i] = int64(i % 7)
            pos_y[i] = int64(i / 7)
        }
        var inscope cx, sx, cy, sy, no_ff : array<float>
        build_rope_tabs_rows(cx, sx, 100.0, 1.0, 1.0, no_ff, false, pos_x, int64(npos), int64(hs / 2))
        build_rope_tabs_rows(cy, sy, 100.0, 1.0, 1.0, no_ff, false, pos_y, int64(npos), int64(hs / 2))
        var inscope x : array<float>
        x |> resize(npos * d)
        for (i in range(npos * d)) {
            x[i] = 0.13 * float(i % 29) - 1.7
        }
        var inscope want := x
        rope_neox_2d_rows(want, int64(npos), int64(n_head), int64(hs), cx, sx, cy, sy)
        var inscope tabs : array<float>
        tabs |> resize(npos * 4 * pairs)
        for (r in range(npos)) {
            for (j in range(pairs)) {
                tabs[r * 4 * pairs + j] = cx[r * pairs + j]
                tabs[r * 4 * pairs + pairs + j] = sx[r * pairs + j]
                tabs[r * 4 * pairs + 2 * pairs + j] = cy[r * pairs + j]
                tabs[r * 4 * pairs + 3 * pairs + j] = sy[r * pairs + j]
            }
        }
        let total = npos * n_head * pairs
        let bx = mc |> io_(x)
        let btabs = mc |> in_(tabs)
        var ka = GkRope2dArgs(nheads = uint(n_head), hs = uint(hs), rows = uint(npos))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[btabs], ka, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_TIGHT, Shape(row_width = d, head_width = hs))
    }
}

// LayerNorm w+b vs the shipped CPU layernorm (dasllama_audio) — the tower/embedder norm.
// The bias fixture keeps |b| >= 0.1 so a bias-blind kernel cannot pass.
def private layernorm_gate(t : T?; var dev, queue; dim, nrows : int; eps : float; inplace : bool) {
    let tag = "layernorm dim={dim} nrows={nrows} eps={eps} inplace={inplace}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_ln)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope x, w, b : array<float>
        x |> resize(nrows * dim)
        w |> resize(dim)
        b |> resize(dim)
        for (i in range(nrows * dim)) {
            x[i] = 0.23 * float(i % 31) - 3.1
        }
        if (nrows > 0 && dim > 0) {
            for (i in range(dim)) {   // one constant row exercises the zero-variance eps path
                x[(nrows - 1) * dim + i] = 1.75
            }
        }
        for (i in range(dim)) {
            w[i] = 0.5 + 0.04 * float(i % 13)
            b[i] = -0.7 + 0.05 * float(i % 17)
        }
        var inscope want : array<float>
        want |> resize(nrows * dim)
        unsafe {
            for (r in range(nrows)) {
                layernorm(addr(want[r * dim]), addr < float const? >(x[r * dim]),
                    addr < float const? >(w[0]), addr < float const? >(b[0]), int64(dim), eps)
            }
        }
        var inscope wn <- clone_to_move(w)
        wn |> push_from(b)
        let bx = mc |> io_(x)
        let bwn = mc |> in_(wn)
        let by = inplace ? bx : (mc |> out_(nrows * dim))
        var ka = GkPostAddLnArgs(d = uint(dim), rows = uint(nrows), woff = 0u, boff = 0u, eps = eps, ln_on = 1u, lnoff = 0u, lnboff = uint(dim), ascale = 0.0, xboff = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[by], mc.buf[bwn], ka, int64(nrows))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_TIGHT, Shape(row_width = dim))
    }
}

// The two FORM-MATCHED tower GELUs vs their shipped CPU flavors on the same fixture. The LUT
// flavor runs the CPU's own clamps + f16 round-trips in kernel (bar = one f16 ulp at the range
// edge, tie flips only); the erf flavor's A&S polynomial sits within ~1e-5 of the CPU's
// double-precision erf. Saturation/NaN probes ride the tail slots of every arm.
def private gelu_flavor_gate(t : T?; var dev, queue; total : int; lut : bool) {
    let tag = "gelu_{lut ? "lut" : "erf"} total={total}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_act_rows)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope g : array<float>
        g |> resize(total)
        for (i in range(total)) {
            g[i] = 0.21 * float(i % 43) - 4.3
        }
        if (total >= 10) {   // the saturation + zero probes ride the tail slots; +-70000 reach the LUT form's +-10 clamps, where the
            g[total - 8] = 70000.0      // unclamped tanh form overflows to inf and NaN
            g[total - 9] = -70000.0
            g[total - 1] = 50.0
            g[total - 2] = -50.0
            g[total - 3] = 100.0
            g[total - 4] = -100.0
            g[total - 5] = 0.0
            g[total - 6] = 1e-30
            g[total - 7] = -1e-30
        }
        var inscope want := g
        if (lut) {
            gelu_tanh_lut(want, int64(total))
        } else {
            gelu_erf(want, int64(total))
        }
        let bg = mc |> io_(g)
        var ka = GkBiasActArgs(d = 0u, nelem = uint(total), boff = 0u, act = lut ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], ka, int64((total + 3) / 4))
            }
        }
        return if (!ran || total < 8)
        // LUT: one f16 ulp at |g|~10 (RNE tie flips between the tanh and exp-identity evals);
        // erf: the CPU's f32 NR erfc fit against the shader's evaluation of the same fit - a few ulp apart
        check_buf(t, "{tag}: form-matched inside the flavor bar", mc.buf[bg], want, Bar(abs_floor = lut ? 1e-2 : 1e-5), FLAT_SHAPE)
        var inscope back <- mc |> plane_run(bg)
        t |> success(back[total - 1] == 50.0 && back[total - 3] == 100.0, "{tag}: positive saturation exact")
        t |> success(back[total - 2] == 0.0 && back[total - 4] == 0.0, "{tag}: negative saturation exact")
        t |> success(nan_count(back) == 0, "{tag}: no NaN anywhere")
    }
}

// The form-delta cell: the analytic tanh-GELU vs the two shipped CPU flavors — the measured
// per-element distances whose 32-layer coherent amplification (~4e-2 rel_l2 on qwen2audio)
// is WHY the GPU ships form-matched flavors instead of one substituted form.
def private gelu_form_delta(t : T?) {
    let n = 65536
    var xs : array<float>
    xs |> resize(n)
    for (i in range(n)) {
        xs[i] = -10.0 + 20.0 * float(i) / 65535.0
    }
    var an := xs
    var lut := xs
    var erfv := xs
    gelu(an, int64(n))             // the analytic tanh form (the decode GELU)
    gelu_tanh_lut(lut, int64(n))   // the SHIPPED whisper flavor (f16-LUT round trip)
    gelu_erf(erfv, int64(n))       // the SHIPPED tower flavor (exact erf)
    var max_lut = 0.0
    var max_erf = 0.0
    for (i in range(n)) {
        max_lut = max(max_lut, nan_wide_diff(an[i], lut[i]))
        max_erf = max(max_erf, nan_wide_diff(an[i], erfv[i]))
    }
    to_log(LOG_INFO, "gelu_form_delta over [-10,10]: |analytic-lut| max {max_lut}, |analytic-erf| max {max_erf}\n")
    // measured 3.90625e-3 = the f16 HALF-ULP at the range edge (|x|~10, spacing 2^-7): the LUT
    // quantizes its INPUT, so the delta is absolute half-ulp * gelu' ~ 1, not relative
    t |> success(max_lut <= 4.5e-3, "gelu analytic-vs-LUT substitution bounded (measured {max_lut})")
    t |> success(max_erf <= 1e-3, "gelu analytic-vs-erf substitution bounded (measured {max_erf})")
    delete xs
    delete an
    delete lut
    delete erfv
}

//! The HX twin's compare: `bh` holds the RNE narrow of each value the same thread stored to `bf`, the sentinel past `live`.
//! `weighted` off: a ones vector against the weightless CPU norm (gemma4uv's idiom); `hx`: MetalRmsNormHX, its f16 twin the RNE narrow of the f32 store.
enum RmsStamp {
    rows   //! MetalRmsNorm, the rows form: one 256-lane threadgroup a row
    hx     //! MetalRmsNormHX, its f16 twin the RNE narrow of the f32 store
    tw     //! the tower seam's RMS arm (`MetalTwRms`, add off), out of place and in place
}

def private rms_gate(t : T?; var dev, queue; dim, nrows : int; eps : float; weighted : bool; stamp : RmsStamp) {
    let hx = stamp == RmsStamp.hx
    let tw = stamp == RmsStamp.tw
    let tag = "rms{hx ? "_hx" : (tw ? "_tw" : "")}{weighted ? "" : "_weightless"} dim={dim} nrows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sh = stamp_of(@@pf_enc_rms_hx)
        let sp = stamp_of(@@pf_enc_rms)
        let st = stamp_of(@@enc_tw_rms)
        var pso = hx ? (mc |> keep_stamp(sh, err)) : (tw ? (mc |> keep_stamp(st, err)) : (mc |> keep_stamp(sp, err)))
        return if (!pso_ok(t, tag, pso, err))
        var inscope x, w : array<float>
        x |> resize(nrows * dim)
        w |> resize(dim)
        for (i in range(nrows * dim)) {
            x[i] = 0.19 * float(i % 37) - 2.9
        }
        for (i in range(dim)) {
            w[i] = weighted ? 0.5 + 0.01 * float(i % 7) : 1.0
        }
        var inscope want := x
        if (weighted) {
            unsafe {
                for (r in range(nrows)) {
                    rmsnorm(addr(want[r * dim]), addr<float const?>(x[r * dim]), addr<float const?>(w[0]), int64(dim), eps)
                }
            }
        } else {
            rms_rows_weightless(want, int64(dim), int64(nrows), eps)
        }
        let bx = mc |> in_(x)
        let bw = mc |> in_(w)
        let by = mc |> out_(nrows * dim)
        let byh = mc |> out_words(nrows * dim, float16(PAD_SENTINEL))
        let bdim = mc |> in_plain([uint(dim)])
        let beps = mc |> in_plain([eps])
        var ka = gk_rms_args(uint(dim), uint(nrows), eps, 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (px, pw, py, pd, pe) = (mc.buf[bx], mc.buf[bw], mc.buf[by], mc.buf[bdim], mc.buf[beps])
            if (hx) {
                stamp_run(sh, pso) {
                    invoke(sh.enc, enc, px, 0ul, pw, py, pd, pe, mc.buf[byh], int64(nrows))
                }
            } elif (tw) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, px, py, pw, ka, int64(nrows))
                }
            } else {
                stamp_run(sp, pso) {
                    invoke(sp.enc, enc, px, 0ul, pw, py, pd, pe, int64(nrows))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, tw ? lead_poisoned(BAR_APPROX, want) : BAR_TIGHT, Shape(row_width = dim))
        if (hx) {
            check_hx_narrow(t, "{tag} hx", mc.buf[byh], mc.buf[by], nrows * dim, nrows * dim)
        }
        if (tw) {
            let bxi = mc |> io_(x)
            let ran_in = mc |> cell_record(t, "{tag} in place") $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bxi], mc.buf[bxi], mc.buf[bw], ka, int64(nrows))
                }
            }
            return if (!ran_in)
            check_buf(t, "{tag} in place", mc.buf[bxi], want, BAR_APPROX, Shape(row_width = dim))
            t |> success(moved_off(want, x, 0, BAR_APPROX), "{tag} in place: the rows moved off x")
        }
    }
}

//! HX: g the base combine, gh its RNE narrow; TOTB: a launch over the padded bound writes nothing past totb[0] * row_elems; unbounded is HX at rows == bound_rows.
def private ew2_hx_gate(t : T?; var dev, queue; geglu, hx, bounded : bool; rows, row_elems, bound_rows : int) {
    let tag = "{geglu ? "geglu" : "swiglu"}{hx ? "_hx" : ""}{bounded ? "_b" : ""} rows={rows} elems={row_elems} bound={bound_rows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let shx = geglu ? stamp_of(@@enc_geglu_hx) : stamp_of(@@enc_swiglu_hx)   //! the unbounded arms are the HX twins
        let shb = geglu ? stamp_of(@@enc_geglu_hxb) : stamp_of(@@enc_swiglu_hxb)
        let sb = geglu ? stamp_of(@@enc_geglu_b) : stamp_of(@@enc_swiglu_b)
        var pso = !bounded ? (mc |> keep_stamp(shx, err)) : (hx ? (mc |> keep_stamp(shb, err)) : (mc |> keep_stamp(sb, err)))
        return if (!pso_ok(t, tag, pso, err))
        let total = bound_rows * row_elems
        let live = rows * row_elems
        var inscope sg, su : array<float>
        sg |> resize(total)
        su |> resize(total)
        for (k in range(total)) {
            sg[k] = 0.21 * float(k % 43) - 4.3
            su[k] = 0.31 * float(k % 17) - 2.5
        }
        var inscope want := sg
        if (geglu) {
            geglu(want, su, int64(live))   // the tanh form; the GPU's exp identity agrees inside the approx bar
        } else {
            swiglu(want, su, int64(live))
        }
        let bsg = mc |> io_(sg)
        let bsu = mc |> in_(su)
        let btot = mc |> in_plain([uint(total)])
        let bgh = mc |> out_words(total, float16(PAD_SENTINEL))
        // the row count sits at the bind offset, past a poison word - the basep[ne] shape
        let btb = mc |> in_plain([0xFFFFFFFFu, uint(rows)])
        let btm = mc |> in_plain([uint(row_elems)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (g, u, tot, gh, tb, tm) = (mc.buf[bsg], mc.buf[bsu], mc.buf[btot], mc.buf[bgh], mc.buf[btb], mc.buf[btm])
            if (!bounded) {
                stamp_run(shx, pso) {
                    invoke(shx.enc, enc, g, 0ul, u, 0ul, tot, gh, int64(total))
                }
            } elif (hx) {   // the bound row count sits one word into its plane
                stamp_run(shb, pso) {
                    invoke(shb.enc, enc, g, 0ul, u, 0ul, tot, gh, tb, 4ul, tm, int64(total))
                }
            } else {
                stamp_run(sb, pso) {
                    invoke(sb.enc, enc, g, 0ul, u, 0ul, tot, tb, 4ul, tm, int64(total))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bsg], want, lead_poisoned(BAR_TIGHT, want), Shape(row_width = row_elems))
        if (hx) {
            check_hx_narrow(t, "{tag} hx", mc.buf[bgh], mc.buf[bsg], total, live)
        }
    }
}

// The gemma4uv 2-axis position add — bit-EXACT vs the CPU's (row + ex) + ey order; the
// split-exponent fixture makes any reassociation visible, the non-square grid any axis swap.
def private pos_add_gate(t : T?; var dev, queue; npos, dim, ncols, pos_entries : int) {
    let tag = "posadd2d npos={npos} ncols={ncols}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_posadd2d)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = npos * dim
        var inscope x, tbl : array<float>
        x |> resize(total)
        tbl |> resize(2 * pos_entries * dim)
        for (i in range(total)) {
            x[i] = 1.0e3 + 0.37 * float(i % 29)          // large exponent
        }
        for (i in range(pos_entries * dim)) {
            tbl[i] = 1.0e-3 * float(i % 41 + 1)          // x-table rows: tiny exponent
            tbl[pos_entries * dim + i] = 9.0e2 + 0.21 * float(i % 19)   // y-table rows: large again
        }
        var inscope want <- pos_add_2d_ref(x, tbl, npos, dim, ncols, pos_entries)
        let bx = mc |> io_(x)
        let btbl = mc |> in_(tbl)
        var pa = GkPosAdd2DArgs(dim = uint(dim), ncols = uint(ncols), pos_entries = uint(pos_entries), total = uint(total), tboff = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[btbl], pa, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_BITS, Shape(row_width = dim))
    }
}

//! the patch im2col stamp bit-exact vs the CPU `im2col_rgb_patches`: a non-square canvas, a padded stride over a sentinel, the pixel map, a poisoned control
def private patch_im2col_gate(t : T?; var dev, queue; w, h, p, pad : int; scale, shift : float) {
    let tag = "patch_im2col {w}x{h} p={p} pad={pad} scale={scale}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_tw_patch_im2col)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let n_cols = w / p
        let npos = n_cols * (h / p)
        let row_dim = 3 * p * p + pad
        let total = (npos + 1) * row_dim   //! one pad row past the patches
        var inscope planes <- [for (i in range(w * h * 3)); float((i * 37) % 251 - 125) * 0.007]
        var inscope mapped <- [for (v in planes); v * scale + shift]
        var inscope want : array<float>
        let got_rows = im2col_rgb_patches(mapped, int64(w), int64(h), int64(p), want, int64(row_dim))
        t |> equal(got_rows, int64(npos), "{tag}: the CPU im2col's row count")
        want |> resize(total)
        let bsrc = mc |> in_(planes)
        let bdst = mc |> out_words(total, -7.0)
        var pa = GkPatchIm2colArgs(w = uint(w), p = uint(p), row_dim = uint(row_dim), npos = uint(npos), total = uint(total), scale = scale, shift = shift)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bdst], pa, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bdst], want, lead_poisoned(BAR_BITS, want), Shape(row_width = row_dim))
    }
}

//! the qwen3v stem assemble stamp bit-exact vs the CPU (crow + bp) + prow order on an even pw x ph grid, the merge walk told apart from the natural order
def private q3v_stem_gate(t : T?; var dev, queue; pw, ph, dim : int) {
    let tag = "q3v_stem pw={pw} ph={ph} dim={dim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_q3v_stem)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let npos = pw * ph
        let total = npos * dim
        let boff = 5
        let poff = 3
        var inscope xn <- [for (i in range(total)); 1.0e3 + 0.37 * float(i % 29)]
        var inscope b <- [for (i in range(boff + dim)); 1.0e-3 * float(i % 41 + 1)]
        var inscope pos <- [for (i in range(poff + total)); 9.0e2 + 0.21 * float(i % 19)]
        var inscope want : array<float>
        want |> resize(total)
        var moved = 0
        for (r in range(npos)) {
            let src = int(q3v_reorder_src(int64(r), int64(pw)))
            moved += src != r ? 1 : 0
            for (i in range(dim)) {
                want[r * dim + i] = (xn[src * dim + i] + b[boff + i]) + pos[poff + src * dim + i]
            }
        }
        t |> success(moved > npos / 2, "{tag}: the merge walk moves most rows off the natural order ({moved} of {npos})")
        let bxn = mc |> in_(xn)
        let bb = mc |> in_(b)
        let bpos = mc |> in_(pos)
        let bx = mc |> out_(total)
        var pa = GkStemAsmArgs(dim = uint(dim), pw = uint(pw), total = uint(total), boff = uint(boff), poff = uint(poff))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bxn], mc.buf[bb], mc.buf[bpos], mc.buf[bx], pa, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_BITS, Shape(row_width = dim))
    }
}

//! the whisper decoder's K/V stores on the shared stamps, bit-exact vs the CPU walk: the keys (KX), the values and append (VX), the f16 plane
def private wdec_kx_gate(t : T?; var dev, queue; ta, dim, hh : int; off_layer : bool) {
    let tag = "wdec_kv kx ta={ta} dim={dim} hh={hh} off={off_layer}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_wdec_kv)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hs = dim / hh
        let total = ta * dim
        let kscale = wdec_kscale(int64(hs))
        var inscope src <- [for (i in range(total)); 0.25 * float(i % 97) - 3.0]
        let sentinel = -777.0
        var inscope want <- [for (_i in range(2 * total)); sentinel]
        let base = off_layer ? total : 0
        for (h in range(hh)) {
            for (j in range(hs)) {
                for (tt in range(ta)) {
                    want[base + (h * hs + j) * ta + tt] = src[tt * dim + h * hs + j] * kscale
                }
            }
        }
        let bsrc = mc |> in_(src)
        let bkx = mc |> out_words(2 * total, sentinel)
        let bw = mc |> in_plain([0.0])
        var ka = GkWdecKvArgs(rows = uint(ta), sstride = uint(dim), soff = 0u, nheads = uint(hh), tstride = uint(ta), p0 = 0u,
            dst_off = uint(base), boff = 0u, bias_on = 0u, mode = WDEC_KV_KX, hs = uint(hs), kscale = wdec_kscale(int64(hs)))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bkx], mc.buf[bw], ka, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bkx], want, BAR_BITS, Shape(row_width = ta))
    }
}

//! the values (VX) and the f16 plane (`half`): n rows at p0 into a wider tstride slab, the bias folded, `soff` picking a column
//! block out of a fused row
def private wdec_vx_gate(t : T?; var dev, queue; ta, dim, hh : int; t0, tstride : int; hasb : bool; soff_cols : int = 0; src_stride : int = 0; half : bool = false) {
    let tag = "wdec_kv {half ? "f16" : "vx"} ta={ta} dim={dim} hh={hh} t0={t0} hasb={hasb} soff={soff_cols}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_wdec_kv)
        let sh = stamp_of(@@enc_wdec_kv_h)
        var pso = half ? (mc |> keep_stamp(sh, err)) : (mc |> keep_stamp(st, err))
        return if (!pso_ok(t, tag, pso, err))
        let hs = dim / hh
        let sstride = src_stride > 0 ? src_stride : dim
        let total = ta * dim
        let sentinel = -555.0
        var inscope src <- [for (i in range(ta * sstride)); 0.5 * float(i % 89) - 11.0]
        var inscope bias <- [for (i in range(dim)); 0.125 * float(i % 13) - 0.5]
        let slab = hh * tstride * hs
        var inscope want <- [for (_i in range(slab)); sentinel]
        for (h in range(hh)) {
            for (tt in range(ta)) {
                for (j in range(hs)) {
                    let v = src[tt * sstride + soff_cols + h * hs + j] + (hasb ? bias[h * hs + j] : 0.0)
                    want[h * tstride * hs + (t0 + tt) * hs + j] = half ? float(float16(v)) : v
                }
            }
        }
        let bsrc = mc |> in_(src)
        let bvx = half ? (mc |> out_words(slab, float16(sentinel))) : (mc |> out_words(slab, sentinel))
        let bb = mc |> in_(bias)
        var ka = GkWdecKvArgs(rows = uint(ta), sstride = uint(sstride), soff = uint(soff_cols), nheads = uint(hh), tstride = uint(tstride), p0 = uint(t0),
            dst_off = 0u, boff = 0u, bias_on = hasb ? 1u : 0u, mode = half ? WDEC_KV_F16 : WDEC_KV_VX, hs = uint(hs), kscale = wdec_kscale(int64(hs)))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (half) {
                stamp_run(sh, pso) {
                    invoke(sh.enc, enc, mc.buf[bsrc], mc.buf[bvx], mc.buf[bb], ka, int64(total))
                }
            } else {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bvx], mc.buf[bb], ka, int64(total))
                }
            }
        }
        return if (!ran)
        if (half) {
            check_buf_h(t, tag, mc.buf[bvx], want, BAR_BITS, Shape(row_width = hs))
        } else {
            check_buf(t, tag, mc.buf[bvx], want, BAR_BITS, Shape(row_width = hs))
        }
    }
}

// The chunked twin pair vs the SAME oracle: Part writes per-chunk (m, s, y) partials, Comb
// merges by log-sum-exp — the pair must land on the single-pass answer (1e-4, exp reassoc).
// Negative control recorded 2026-08-16: an off-by-one oracle len redded all three cells.
def private wdec_attn_pc_gate(t : T?; var dev, queue; n, dim, hh, tstride, len0 : int; causal : bool) {
    let tag = "wdec_attn_pc n={n} dim={dim} hh={hh} len0={len0} causal={causal}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sp = stamp_of(@@enc_wdec_attn_part)
        let sc = stamp_of(@@enc_wdec_attn_comb)
        var pp = mc |> keep_stamp(sp, err)
        return if (!pso_ok(t, "{tag} part", pp, err))
        var pc = mc |> keep_stamp(sc, err)
        return if (!pso_ok(t, "{tag} comb", pc, err))
        let hs = dim / hh
        let scale = 1.0 / sqrt(float(hs))
        let maxlen = causal ? len0 + n - 1 : len0
        let nch = (maxlen + 255) / 256
        var inscope q, qb : array<float>
        var inscope kvh, vvh : array<uint16>
        var inscope kv, vv : array<float>
        q |> resize(n * dim)
        qb |> resize(dim)
        kvh |> resize(hh * tstride * hs)
        vvh |> resize(hh * tstride * hs)
        kv |> resize(hh * tstride * hs)
        vv |> resize(hh * tstride * hs)
        for (i in range(n * dim)) {
            q[i] = 0.11 * float(i % 37) - 1.7
        }
        for (i in range(dim)) {
            qb[i] = 0.07 * float(i % 11) - 0.3
        }
        for (i in range(hh * tstride * hs)) {
            kvh[i] = uint16(f32_to_f16(0.09 * float(i % 41) - 1.9))
            vvh[i] = uint16(f32_to_f16(0.13 * float(i % 29) - 1.1))
            kv[i] = f16_to_f32(uint(kvh[i]))
            vv[i] = f16_to_f32(uint(vvh[i]))
        }
        // the oracle over [hh][tstride][hs] planes (head h at h * tstride * hs, key stride hs), the q bias folded into the query rows as the kernel folds it
        var inscope qq : array<float>
        qq |> resize(n * dim)
        for (i in range(n * dim)) {
            qq[i] = q[i] + qb[i % dim]
        }
        var inscope want : array<float>
        want |> resize(n * dim)
        for (h in range(hh)) {
            for (r in range(n)) {
                attn_row_oracle(qq, r * dim + h * hs, kv, h * tstride * hs, vv, h * tstride * hs, hs, 0, hs, 1,
                    causal ? len0 + r : len0, scale, want, r * dim + h * hs)
            }
        }
        let bq = mc |> in_(q)
        let bk = mc |> in_plain(kvh)
        let bv = mc |> in_plain(vvh)
        let bqb = mc |> in_(qb)
        let bpms = mc |> out_words(nch * hh * n * 2, -888.0)
        let bpy = mc |> out_words(nch * hh * n * hs, -888.0)
        let by = mc |> out_words(n * dim, -444.0)
        var kp_args = GkWdecAttnArgs(dim = uint(dim), tstride = uint(tstride), len0 = uint(len0), causal = causal ? 1u : 0u, nheads = uint(hh),
            nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u, hs = uint(hs), scale = scale)
        var kc_args = GkWdecCombArgs(dim = uint(dim), nchunks = uint(nch), nheads = uint(hh), nrows = uint(n), hs = uint(hs))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (pms, py) = (mc.buf[bpms], mc.buf[bpy])
            stamp_run(sp, pp) {
                invoke(sp.enc, enc, mc.buf[bq], mc.buf[bk], mc.buf[bv], pms, py, mc.buf[bqb], kp_args, int64(nch * hh * n))
            }
            metal_memory_barrier(enc)
            stamp_run(sc, pc) {
                invoke(sc.enc, enc, pms, py, mc.buf[by], kc_args, int64(hh * n))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_TIGHT, Shape(row_width = dim, head_width = hs))
    }
}

//! In place on x: x += y + b[i % dim] (y at slot 1, the rest one up), or x = gelu_lut(x + b[i % dim]) against the CPU LUT form over a fixture spanning its clamp.
def private bias_rows_gate(t : T?; var dev, queue; nrows, dim : int; gelu : bool) {
    let tag = "bias_{gelu ? "gelu_lut" : "add_res"} nrows={nrows} dim={dim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sg = stamp_of(@@enc_bias_act)
        let sr = stamp_of(@@enc_bias_add_res)
        var pso = gelu ? (mc |> keep_stamp(sg, err)) : (mc |> keep_stamp(sr, err))
        return if (!pso_ok(t, tag, pso, err))
        let total = nrows * dim
        var inscope x, y, b : array<float>
        x |> resize(total)
        y |> resize(total)
        b |> resize(dim)
        for (i in range(total)) {
            x[i] = gelu ? 0.02 * float(i % 512) - 5.0 : 0.3 * float(i % 23) - 2.0
            y[i] = 0.17 * float(i % 31) - 1.0
        }
        for (i in range(dim)) {
            b[i] = gelu ? 0.03 * float(i % 17) - 0.25 : 0.05 * float(i % 7) - 0.1
        }
        var inscope want : array<float>
        want |> resize(total)
        for (i in range(total)) {
            want[i] = gelu ? x[i] + b[i % dim] : x[i] + (y[i] + b[i % dim])
        }
        if (gelu) {
            gelu_tanh_lut(want, int64(total))   // the CPU LUT form — the form-matched oracle
        }
        let bx = mc |> io_(x)
        let by = mc |> in_(y)
        let bb = mc |> in_(b)
        let bdim = mc |> in_plain([uint(dim)])
        let btot = mc |> in_plain([uint(total)])
        var ka = GkBiasActArgs(d = uint(dim), nelem = uint(total), boff = 0u, act = BIAS_ACT_GELU_TANH)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (gelu) {
                stamp_run(sg, pso) {
                    invoke(sg.enc, enc, mc.buf[bx], mc.buf[bb], ka, int64((total + 3) / 4))
                }
            } else {
                stamp_run(sr, pso) {
                    invoke(sr.enc, enc, mc.buf[bx], mc.buf[by], mc.buf[bb], 0ul, mc.buf[bdim], mc.buf[btot], int64(total))
                }
            }
        }
        return if (!ran)
        t |> success(mismatch_exact(want, x) > 0, "{tag}: the reference moves off the input")
        check_buf(t, tag, mc.buf[bx], want, BAR_TIGHT, Shape(row_width = dim))
    }
}

//! The conv-frontend im2col against the CPU walk - a bit-exact scatter with the "same"-pad zero edges, both source
//! layouts (chmajor mel, row-major y1) and both strides: k=3 on the shared stamp, the layout by push,
//! the panel padded to the GEMM's 32 lattice with zeros; a poisoned source element must red the panel.
def private im2col3_gate(t : T?; var dev, queue; rows, nch, srclen, stride : int; chmajor : bool) {
    let tag = "im2col3 rows={rows} nch={nch} stride={stride} chmajor={chmajor}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_im2col)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let kdim = (3 * nch + 31) / 32 * 32
        let mp = (rows + 31) / 32 * 32
        let total = mp * kdim
        var inscope src : array<float>
        src |> resize(srclen * nch)
        for (i in range(srclen * nch)) {
            src[i] = 0.21 * float(i % 53) - 4.0
        }
        var inscope want : array<float>
        want |> resize(total)
        for (i in range(rows)) {
            for (dk in range(3)) {
                let sp = i * stride + dk - 1
                for (c in range(nch)) {
                    want[i * kdim + dk * nch + c] = (sp >= 0 && sp < srclen
                        ? (chmajor ? src[c * srclen + sp] : src[sp * nch + c])
                        : 0.0)
                }
            }
        }
        var ka = GkIm2col1dArgs(cin = uint(nch), tin = uint(srclen), tout = uint(rows), k = 3u, stride = uint(stride), pad_l = 1u, dilation = 1u, transposed = 0u,
            kdim = uint(kdim), xoff = 0u, coloff = 0u, row_major = chmajor ? 0u : 1u, r0 = 0u, mp = uint(mp))
        let bsrc = mc |> in_(src)
        let bcol = mc |> out_words(total, -333.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bcol], ka, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bcol], want, BAR_BITS, Shape(row_width = kdim))
        src[0] += 2.0   // row 0's centre tap reads it, on both layouts
        mc |> plane_upload(bsrc, src)
        mc |> plane_refill(bcol)
        let ran2 = mc |> cell_record(t, "{tag} poison") $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bcol], ka, int64(total))
            }
        }
        if (ran2) {
            var inscope poisoned_panel <- mc |> plane_run(bcol)
            t |> success(mismatch_exact(poisoned_panel, want) > 0, "{tag}: a poisoned source element reds the panel")
        }
    }
}

// The 2D conv im2col (k=3 s=2 p=1) vs the CPU conv2d_s2 walk — bit-exact scatter with the
// zero pad edges, the K pad (kd > cin*9) and the strided source (sstride > cin, the padded
// GEMM rows). Negative control recorded 2026-08-16: dropping the -1 pad shift redded every cell.
def private im2col2d_gate(t : T?; var dev, queue; iw, ih, cin, kd, sstride : int) {
    let tag = "im2col2d iw={iw} ih={ih} cin={cin} kd={kd} ss={sstride}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_im2col2d)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let ow = (iw - 1) / 2 + 1
        let oh = (ih - 1) / 2 + 1
        let rows = oh * ow
        let total = rows * kd
        var inscope src : array<float>
        src |> resize(ih * iw * sstride)
        for (i in range(ih * iw * sstride)) {
            src[i] = 0.31 * float(i % 47) - 6.0
        }
        var inscope want : array<float>
        want |> resize(total)
        for (oy in range(oh)) {
            for (ox in range(ow)) {
                let r = oy * ow + ox
                for (k in range(kd)) {
                    var v = 0.0
                    if (k < cin * 9) {
                        let ic = k / 9
                        let kh = (k % 9) / 3
                        let kw = k % 3
                        let iy = oy * 2 + kh - 1
                        let ix = ox * 2 + kw - 1
                        if (iy >= 0 && iy < ih && ix >= 0 && ix < iw) {
                            v = src[(iy * iw + ix) * sstride + ic]
                        }
                    }
                    want[r * kd + k] = v
                }
            }
        }
        let bsrc = mc |> in_(src)
        let bcol = mc |> out_words(total, -222.0)
        var ka = GkIm2col2dArgs(iw = uint(iw), ih = uint(ih), cin = uint(cin), sstride = uint(sstride), kdp = uint(kd), rows = uint(rows), soff = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bsrc], mc.buf[bcol], ka, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bcol], want, BAR_BITS, Shape(row_width = kd))
    }
}

//! the classifier epilogue (`MetalClsEpilogue`) over `rows` logits rows `vocab` wide in one dispatch: the final softcap at `cap`
//! (0: none) against `cap * tanh(x / cap)`, then the `nids` suppressed ids (0: none) pinned to -1e30 on every row
def private cls_epi_gate(t : T?; var dev, queue; vocab, rows, nids : int; cap : float) {
    let tag = "cls_epi vocab={vocab} rows={rows} nids={nids} cap={cap}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_cls_epi)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = vocab * rows
        var inscope xv <- [for (i in range(total)); 0.31 * float(i % 61) - 9.2]   //! spans well past +-cap so the saturation arms run
        var inscope ids <- [for (i in range(max(nids, 1))); uint((i * 37 + 5) % vocab)]
        var inscope want <- [for (v in xv); cap > 0.0 ? cap * tanh(v / cap) : v]
        for (r in range(rows)) {
            for (i in range(nids)) {
                want[r * vocab + int(ids[i])] = -1.0e30
            }
        }
        let bx = mc |> io_(xv)
        let bids = mc |> in_plain(ids)
        var ka = GkClsEpiArgs(vocab = uint(vocab), cap = cap, nsupp = uint(nids), rows = uint(rows))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[bids], ka, int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_TIGHT, Shape(row_width = vocab))
    }
}

// ===== the hyper-connection mixer + the n-gram side input (qwen4exp) =====

//! the hyper-connection cells' bar at `rel`: its control moves element `at` by 0.5 - past the bar at these magnitudes
def private hc_bar(bar : Bar; at : int) : Bar {
    var b = bar
    b.poison = 0.5
    b.poison_scaled = false
    b.poison_at = at
    return b
}

// the grouped norm over nrows wide rows: stream c of every row normalizes over dim and scales by gamma row c
def private hc_norm_gate(t : T?; var dev, queue; dim, hc, nrows : int) {
    let tag = "hc_norm dim={dim} hc={hc} nrows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_hc_norm)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        let rows = nrows * hc
        var inscope x, w, want : array<float>
        x |> resize(rows * dim)
        w |> resize(hcd)
        want |> resize(rows * dim)
        for (i in range(rows * dim)) {
            x[i] = 0.23 * float(i % 41) - 3.1 + 0.7 * float(i / dim)
        }
        for (i in range(hcd)) {
            w[i] = 0.4 + 0.02 * float(i % 11) + 0.1 * float(i / dim)
        }
        unsafe {
            for (r in range(rows)) {
                rmsnorm(addr(want[r * dim]), addr<float const?>(x[r * dim]), addr<float const?>(w[(r % hc) * dim]), int64(dim), EPS)
            }
        }
        let bx = mc |> in_(x)
        let bw = mc |> in_(w)
        let by = mc |> out_words(rows * dim, PAD_SENTINEL)
        var ka = GkHcNormArgs(dim = uint(dim), hc = uint(hc), woff = 0u, row0 = 0u, eps = EPS)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[by], mc.buf[bw], ka, int64(rows))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, hc_bar(BAR_TIGHT, (rows - 1) * dim + 3), Shape(row_width = hcd, head_width = dim))
    }
}

// the wide residual rows from the embed rows: every stream of row p reads row p
def private hc_init_gate(t : T?; var dev, queue; dim, hc, nrows : int) {
    let tag = "hc_init dim={dim} hc={hc} nrows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_hc_init)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        var inscope x, want : array<float>
        x |> resize(nrows * dim)
        want |> resize(nrows * hcd)
        for (i in range(nrows * dim)) {
            x[i] = 0.13 * float(i % 59) - 3.7
        }
        for (p in range(nrows)) {
            for (c in range(hc)) {
                for (i in range(dim)) {
                    want[(p * hc + c) * dim + i] = x[p * dim + i]
                }
            }
        }
        let bx = mc |> in_(x)
        let bres = mc |> out_words(nrows * hcd, PAD_SENTINEL)
        var ka = GkHcInitArgs(dim = uint(dim), hc = uint(hc), rows = uint(nrows))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[bres], ka, int64(nrows * hcd))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bres], want, hc_bar(BAR_EXACT, nrows * hcd - 1), Shape(row_width = hcd, head_width = dim))
    }
}

// the mixer's gate input in place: lo = silu(lo / hc)
def private hc_lo_gate(t : T?; var dev, queue; lr, hc : int) {
    let tag = "hc_lo lr={lr} hc={hc}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_hc_lo)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope x, want : array<float>
        x |> resize(lr)
        want |> resize(lr)
        for (i in range(lr)) {
            x[i] = 0.37 * float(i % 29) - 5.0
            want[i] = float(silu_d(double(x[i]) / double(hc)))
        }
        x[0] = 2.0   //! the in-place witness: silu(2 / hc) is not 2
        want[0] = float(silu_d(2.0lf / double(hc)))
        let bx = mc |> io_(x)
        let btot = mc |> in_plain([uint(lr)])
        let bhc = mc |> in_plain([uint(hc)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], 0ul, mc.buf[btot], mc.buf[bhc], int64(lr))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, hc_bar(BAR_TIGHT, 1), FLAT_SHAPE)
        let g0 = (mc |> plane_run(bx))[0]
        t |> success(abs(g0 - 2.0) > 1.0, "{tag}: the row moved off its input at [0] ({g0})")
    }
}

// the mixer's collapse over nrows wide rows: y[p][i] = the stream mean of xn[p][c][i] * σ(gate[p][c][i])
def private hc_mix_gate(t : T?; var dev, queue; dim, hc, nrows : int) {
    let tag = "hc_mix dim={dim} hc={hc} nrows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_hc_mix)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        var inscope xn, gt, want : array<float>
        xn |> resize(nrows * hcd)
        gt |> resize(nrows * hcd)
        want |> resize(nrows * dim)
        for (i in range(nrows * hcd)) {
            xn[i] = 0.11 * float(i % 43) - 2.2
            gt[i] = 0.29 * float(i % 31) - 4.3
        }
        for (p in range(nrows)) {
            for (i in range(dim)) {
                var acc = 0.0lf
                for (c in range(hc)) {
                    acc += double(xn[(p * hc + c) * dim + i]) * sigmoid_d(double(gt[(p * hc + c) * dim + i]))
                }
                want[p * dim + i] = float(acc / double(hc))
            }
        }
        let bxn = mc |> in_(xn)
        let bgt = mc |> in_(gt)
        let by = mc |> out_words(nrows * dim, PAD_SENTINEL)
        var ka = GkHcMixArgs(dim = uint(dim), hc = uint(hc), rows = uint(nrows))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bxn], mc.buf[bgt], mc.buf[by], ka, int64(nrows * dim))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, hc_bar(BAR_TIGHT, nrows * dim - 1), Shape(row_width = dim))
    }
}

// the scatter in place over nrows rows: res[p][c] += o[p] * 2 σ(inj[p][c] / hc); row 0's stream 1 carries a zero logit (the plain add)
def private hc_combine_gate(t : T?; var dev, queue; dim, hc, nrows : int) {
    let tag = "hc_combine dim={dim} hc={hc} nrows={nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_hc_combine)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        var inscope res, o, inj, want : array<float>
        res |> resize(nrows * hcd)
        o |> resize(nrows * dim)
        inj |> resize(nrows * hc)
        want |> resize(nrows * hcd)
        for (i in range(nrows * hcd)) {
            res[i] = 0.17 * float(i % 37) - 3.0
        }
        for (i in range(nrows * dim)) {
            o[i] = 0.5 + 0.13 * float(i % 23)   //! positive: every stream moves off its input
        }
        for (j in range(nrows * hc)) {
            inj[j] = j == 1 ? 0.0 : 1.7 * float(j % hc) - 2.6 + 0.3 * float(j / hc)
        }
        for (p in range(nrows)) {
            for (c in range(hc)) {
                let wgt = 2.0lf * sigmoid_d(double(inj[p * hc + c]) / double(hc))
                for (i in range(dim)) {
                    let k = (p * hc + c) * dim + i
                    want[k] = float(double(res[k]) + double(o[p * dim + i]) * wgt)
                }
            }
        }
        let bres = mc |> io_(res)
        let bo = mc |> in_(o)
        let binj = mc |> in_(inj)
        var ka = GkHcCombArgs(dim = uint(dim), hc = uint(hc), rows = uint(nrows))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bres], mc.buf[bo], mc.buf[binj], ka, int64(nrows * hcd))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bres], want, hc_bar(BAR_TIGHT, nrows * hcd - 1), Shape(row_width = hcd, head_width = dim))
        let g1 = (mc |> plane_run(bres))[dim]
        t |> success(abs(g1 - res[dim]) > 0.4, "{tag}: stream 1 moved off its input at [0] ({g1} from {res[dim]})")
    }
}

// one (row, stream) a threadgroup: the normed key . normed query gate (signed sqrt, σ) scales the value
// row into the stream's gated row, and the grouped-normed gated row lands as the conv input - the
// decode form (direct = 0) at ring slot (pos0 + p) % rows, the window form (direct = 1) at panel row p -
// the destination bound at a row offset; row 0's stream 0 dot is positive, stream 1's negative,
// stream 2's key is zero (the sign-free arm, g = 1/2)
def private ple_gate_gate(t : T?; var dev, queue; dim, hc, rows, pos0, roff_rows, nrows : int; direct : bool) {
    let tag = "ple_gate dim={dim} hc={hc} pos0={pos0} roff_rows={roff_rows} nrows={nrows} direct={direct}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_ple_gate)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        let dst_rows = direct ? nrows : rows   //! the panel holds the window's rows, the ring `rows` slots
        var inscope res, key, val, wk, wq, wc : array<float>
        res |> resize(nrows * hcd)
        key |> resize(nrows * hcd)
        val |> resize(nrows * dim)
        wk |> resize(hcd)
        wq |> resize(hcd)
        wc |> resize(hcd)
        for (i in range(nrows * hcd)) {
            res[i] = 0.21 * float(i % 47) - 4.0
        }
        for (i in range(hcd)) {
            wk[i] = 0.7 + 0.01 * float(i % 13)
            wq[i] = 0.9 + 0.02 * float(i % 7)
            wc[i] = 0.5 + 0.03 * float(i % 5)
        }
        for (p in range(nrows)) {
            for (c in range(hc)) {
                for (i in range(dim)) {
                    let k = (p * hc + c) * dim + i
                    let r = res[k]
                    key[k] = c == 0 ? r + 0.3 : (c == 1 ? -r : (c == 2 ? 0.0 : 0.05 * float((i + p) % 9) - 0.2))
                }
            }
        }
        for (i in range(nrows * dim)) {
            val[i] = 0.31 * float(i % 19) - 2.7
        }
        // the oracle: ple_apply_row's gate half in plain code, a row at a time
        var inscope gwant, kn, qn : array<float>
        gwant |> resize(nrows * hcd)
        kn |> resize(nrows * hcd)
        qn |> resize(nrows * hcd)
        var inscope dst_want : array<float>
        dst_want |> resize((roff_rows + dst_rows) * hcd)
        for (i in range((roff_rows + dst_rows) * hcd)) {
            dst_want[i] = PAD_SENTINEL
        }
        var gates : float[8]
        unsafe {
            for (p in range(nrows)) {
                let slot = direct ? p : (pos0 + p) % rows
                for (c in range(hc)) {
                    let b = (p * hc + c) * dim
                    rmsnorm(addr(kn[b]), addr<float const?>(key[b]), addr<float const?>(wk[c * dim]), int64(dim), EPS)
                    rmsnorm(addr(qn[b]), addr<float const?>(res[b]), addr<float const?>(wq[c * dim]), int64(dim), EPS)
                    var s = 0.0lf
                    for (i in range(dim)) {
                        s += double(kn[b + i]) * double(qn[b + i])
                    }
                    s /= sqrt(double(dim))
                    let mag = sqrt(max(abs(s), 1.0e-6lf))
                    let sg = s > 0.0lf ? mag : (s < 0.0lf ? -mag : 0.0lf)
                    let g = sigmoid_d(sg)
                    if (p == 0) {
                        gates[c] = float(g)
                    }
                    for (i in range(dim)) {
                        gwant[b + i] = float(double(val[p * dim + i]) * g)
                    }
                    rmsnorm(addr(dst_want[(roff_rows + slot) * hcd + c * dim]), addr<float const?>(gwant[b]), addr<float const?>(wc[c * dim]), int64(dim), EPS)
                }
            }
        }
        t |> success(gates[0] > 0.5 && gates[1] < 0.5 && abs(gates[2] - 0.5) < 1.0e-6, "{tag}: the fixture reaches all three sign arms ({gates[0]}, {gates[1]}, {gates[2]})")
        var inscope wn : array<float>   //! the norm rows as the drivers bind them: key, query, conv input
        wn |> push_from(wk)
        wn |> push_from(wq)
        wn |> push_from(wc)
        let bres = mc |> in_(res)
        let bkey = mc |> in_(key)
        let bval = mc |> in_(val)
        let bwn = mc |> in_(wn)
        let bg = mc |> out_words(nrows * hcd, PAD_SENTINEL)
        let bdst = mc |> out_words((roff_rows + dst_rows) * hcd, PAD_SENTINEL)
        var ka = GkPleGateArgs(dim = uint(dim), hc = uint(hc), nrows = uint(rows), pos0 = uint(pos0), eps = EPS, inv_sq = 1.0 / sqrt(float(dim)),
            direct = direct ? 1u : 0u, coff = uint(roff_rows * hcd))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bkey], mc.buf[bres], mc.buf[bval], mc.buf[bg], mc.buf[bdst], mc.buf[bwn], ka, int64(nrows * hc))
            }
        }
        return if (!ran)
        let slot0 = direct ? 0 : pos0 % rows
        let rows_shape = Shape(row_width = hcd, head_width = dim)
        check_buf(t, tag, mc.buf[bg], gwant, hc_bar(BAR_TIGHT, 2 * dim + 1), rows_shape)
        check_buf(t, "{tag} dst", mc.buf[bdst], dst_want, hc_bar(BAR_TIGHT, (roff_rows + slot0) * hcd + 5), rows_shape)   //! the whole destination: the written rows, every other row at the sentinel
    }
}

// the dilated causal conv over the conv-input rows + the residual add in place over nrows rows:
// res += gated + silu(conv). The ring holds the positions before the window at slot p % rows (the
// newest wins), zero where nothing wrote, so a tap before the sequence reads zero; the window form
// (direct = 1) reads positions pos0.. from the panel, the decode form (direct = 0, nrows 1) reads every
// tap from the ring - pos0 picks the live-tap regime (one, some, all; wrapped past rows)
def private ple_conv_gate(t : T?; var dev, queue; dim, hc, kern, dil, rows, pos0, nrows : int; direct : bool) {
    let tag = "ple_conv dim={dim} hc={hc} kern={kern} dil={dil} rows={rows} pos0={pos0} nrows={nrows} direct={direct}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_ple_conv)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hcd = hc * dim
        let roff_rows = 2
        let coff_rows = 1
        var inscope res, gated, w, ring, panel : array<float>
        res |> resize(nrows * hcd)
        gated |> resize(nrows * hcd)
        w |> resize(hcd * kern)
        ring |> resize((roff_rows + rows) * hcd)
        panel |> resize((coff_rows + nrows) * hcd)
        for (i in range(nrows * hcd)) {
            res[i] = 0.19 * float(i % 53) - 5.0
            gated[i] = 0.7 + 0.07 * float(i % 17)   //! positive: the add moves every element off its input
        }
        for (i in range(hcd * kern)) {
            w[i] = 0.23 * float(i % 29) - 3.2
        }
        for (i in range((roff_rows + rows) * hcd)) {
            ring[i] = i < roff_rows * hcd ? PAD_SENTINEL : 0.0   //! rows under the offset carry the sentinel, the ring itself is zero-filled
        }
        for (i in range((coff_rows + nrows) * hcd)) {
            panel[i] = i < coff_rows * hcd ? PAD_SENTINEL : 0.0
        }
        // the rows the ring holds: every position before the window (direct), or every position through the row (decode)
        let ring_end = direct ? pos0 : pos0 + nrows
        for (p in range(ring_end)) {   // the driver's order: every position lands at its slot, the newest wins
            let rb = (roff_rows + p % rows) * hcd
            for (i in range(hcd)) {
                ring[rb + i] = 0.11 * float((i + 7 * p) % 31) - 1.6
            }
        }
        if (direct) {
            for (p in range(nrows)) {
                for (i in range(hcd)) {
                    panel[(coff_rows + p) * hcd + i] = 0.11 * float((i + 7 * (pos0 + p)) % 31) - 1.6
                }
            }
        }
        var inscope want : array<float>
        want |> resize(nrows * hcd)
        for (p in range(nrows)) {
            let pos = pos0 + p
            for (chn in range(hcd)) {
                var acc = 0.0lf
                for (k in range(kern)) {
                    let src = pos - (kern - 1 - k) * dil
                    if (src < 0) {
                        continue
                    }
                    let v = (direct && src >= pos0) ? panel[(coff_rows + src - pos0) * hcd + chn] : ring[(roff_rows + src % rows) * hcd + chn]
                    acc += double(w[chn * kern + k]) * double(v)
                }
                want[p * hcd + chn] = float(double(res[p * hcd + chn]) + double(gated[p * hcd + chn]) + silu_d(acc))
            }
        }
        t |> equal(rows, (kern - 1) * dil + 1, "{tag}: the ring's rows are the taps' reach")
        var inscope wn : array<float>   //! the taps past the three norm rows, as the drivers bind them
        wn |> resize(3 * hcd)
        wn |> push_from(w)
        let bres = mc |> io_(res)
        let bg = mc |> in_(gated)
        let bring = mc |> in_(ring)
        let bpanel = mc |> in_(panel)
        let bwn = mc |> in_(wn)
        var ka = GkPleConvArgs(dim = uint(dim), hc = uint(hc), kern = uint(kern), dil = uint(dil), rows = uint(nrows), pos0 = uint(pos0),
            direct = direct ? 1u : 0u, coff = uint(coff_rows * hcd), ringoff = uint(roff_rows * hcd))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bres], mc.buf[bg], mc.buf[bring], mc.buf[bwn], mc.buf[bpanel], ka, int64(nrows * hcd))
            }
        }
        return if (!ran)
        let rows_shape = Shape(row_width = hcd, head_width = dim)
        check_buf(t, tag, mc.buf[bres], want, hc_bar(BAR_TIGHT, nrows * hcd / 2), rows_shape)
        let g0 = (mc |> plane_run(bres))[0]
        t |> success(abs(g0 - res[0]) > 0.5, "{tag}: the row moved off its input at [0] ({g0} from {res[0]})")
        check_buf(t, "{tag} ring", mc.buf[bring], ring, BAR_BITS, rows_shape)   //! the conv reads the ring and the panel and writes neither
        check_buf(t, "{tag} panel", mc.buf[bpanel], panel, BAR_BITS, rows_shape)
    }
}

// The [metal_dispatch] multi-kernel mechanism, end to end: MtlToy's two kernel= instances
// dispatch through their GENERATED builders (kn_ rail, not hand binds) chained in one encoder
// — scale (ys = xs*gain) then shift (ys += gain). The per-kernel STAGING divergence (shift
// stages no xs) is proven at compile time — ast_dump of the builders + the lens cover rule;
// the hazard rail is un-armed in suites, so this gate witnesses binds/kargs/grid, not staging.
var private g_toy_trips : array<string>   // what the requires= tripwire reported (the hook target)

def private record_toy_trip(msg : string) {
    g_toy_trips |> push(msg)
}

def private mtl_toy_lens_gate(t : T?; var dev, queue; n : int) {
    let tag = "mtl_toy multi-kernel n={n}"
    var err : string
    g_mtl_toy_scale_pso = pso_stamp(dev, mtl_toy_scale_msl, err)
    t |> success(g_mtl_toy_scale_pso != null, "{tag}: scale pipeline: {err}")
    g_mtl_toy_shift_pso = pso_stamp(dev, mtl_toy_shift_msl, err)
    t |> success(g_mtl_toy_shift_pso != null, "{tag}: shift pipeline: {err}")
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        // the builders dispatch on the fixture's global PSO slots: the cell releases them and nulls the slots after
        mc |> keep_pso(g_mtl_toy_scale_pso)
        mc |> keep_pso(g_mtl_toy_shift_pso)
        return if (g_mtl_toy_scale_pso == null || g_mtl_toy_shift_pso == null)
        let gain = 0.5
        var inscope xv : array<float>
        xv |> resize(n)
        for (i in range(n)) {
            xv[i] = 0.11 * float(i % 73) - 4.2
        }
        var inscope want : array<float>
        want |> resize(n + 16)
        for (i in range(n + 16)) {
            want[i] = i < n ? xv[i] * gain + gain : PAD_SENTINEL   // the ka.n guard must leave the pad untouched
        }
        let bxs = mc |> in_(xv)
        let bys = mc |> out_words(n + 16, PAD_SENTINEL)
        var ka = MtToyArgs(n = uint(n), gain = gain)
        let saved_hook = metal_requires_hook()
        set_metal_requires_hook(@@record_toy_trip)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_mtl_toy_scale(enc, mc.buf[bxs], mc.buf[bys], ka, int64(n))
            enc_mtl_toy_shift(enc, mc.buf[bxs], mc.buf[bys], ka, int64(n))
        }
        if (ran) {
            check_buf(t, tag, mc.buf[bys], want, BAR_BITS, FLAT_SHAPE)
        }
        // the requires= tripwire: n % 8 (params) on scale, ka.n % 8 (kargs) on shift - an aligned n
        // passes silently, n + 4 trips both with the builder's name and the broken contract
        t |> equal(length(g_toy_trips), 0, "{tag}: aligned dispatches raise no requires trip")
        var kbad = MtToyArgs(n = uint(n + 4), gain = gain)
        let ran_bad = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
            enc_mtl_toy_scale(enc, mc.buf[bxs], mc.buf[bys], kbad, int64(n + 4))
            enc_mtl_toy_shift(enc, mc.buf[bxs], mc.buf[bys], kbad, int64(n + 4))
        }
        t |> success(ran_bad, "{tag}: misaligned encode still runs under the hook: {err}")
        t |> equal(length(g_toy_trips), 2, "{tag}: both misaligned dispatches trip requires=")
        if (length(g_toy_trips) == 2) {
            t |> equal(g_toy_trips[0], "enc_mtl_toy_scale: n % 8 != 0")
            t |> equal(g_toy_trips[1], "enc_mtl_toy_shift: ka.n % 8 != 0")
        }
        restore_metal_requires_hook(saved_hook)
        delete g_toy_trips
    }
    g_mtl_toy_scale_pso = null
    g_mtl_toy_shift_pso = null
}

// The alignments the ringed attention, router, PLE projection and per-32 gather builders declare (`requires =`): an aligned value
// passes every builder silently, a misaligned one trips each builder whose contract it breaks, by the builder's name and the
// broken item. The builders record into a graph capture the cell discards, so no misaligned dispatch reaches the device.
var private g_align_trips : array<string>

def private record_align_trip(msg : string) {
    g_align_trips |> push(msg)
}

def private want_trips(var want : array<string>; names : array<string>; item : string) {
    want |> reserve(length(want) + length(names))
    for (n in names) {
        want |> push("{n}: {item} != 0")
    }
}

def private dispatch_align_gate(t : T?; var dev, queue) {
    let ring64 <- [ "enc_attnp16_c", "enc_attnp32_c", "enc_attnp_q8_c", "enc_attnp_tq4_c", "enc_attnpb16_c", "enc_attnpb32_c",
                    "enc_attnpb_q8_c", "enc_attnpb_tq4_c", "enc_qk_dev_c", "enc_qk_dev128_c" ]
    let ring32 <- [ "enc_attnd16_c", "enc_attnd32_c", "enc_attndw16_c", "enc_attndw32_c", "enc_attndx16_c", "enc_attndx32_c",
                    "enc_attnd_q8_c", "enc_attnd_tq4_c", "enc_attndw_q8_c", "enc_attndw_tq4_c", "enc_attndx_q8_c", "enc_attndx_tq4_c" ]
    let routers <- [ "enc_moe_router", "enc_moe_router_b" ]
    let projs <- [ "enc_ple_proj_bf16", "enc_ple_proj_bf16_one" ]
    let gathers <- [ "enc_q51_embed_b32", "enc_iq4nl32_embed_b32", "enc_q4032_embed_b32", "enc_q5032_embed_b32" ]
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        let b = mc.buf[mc |> out_words(64, 0.0)]
        let saved_hook = metal_requires_hook()
        set_metal_requires_hook(@@record_align_trip)
        var inscope want : array<string>
        gr_arm(true)
        for (ring in fixed_array(0u, 128u, 96u, 48u)) {
            var pk = SqAttnPartArgs(ring = ring)
            var pb = SqAttnPartBArgs(ring = ring)
            var dk = SqAttnDArgs(ring = ring)
            var qk = AttnArgs(ring = ring)
            enc_attnp16_c(null, b, 0ul, b, 0ul, b, b, pk, 1l, 32l)
            enc_attnp32_c(null, b, 0ul, b, 0ul, b, b, pk, 1l, 32l)
            enc_attnp_q8_c(null, b, 0ul, b, b, 0ul, b, b, b, pk, 1l, 32l)
            enc_attnp_tq4_c(null, b, 0ul, b, b, 0ul, b, b, b, pk, 1l, 32l)
            enc_attnpb16_c(null, b, b, b, b, b, 0ul, pb, 1l, 32l)
            enc_attnpb32_c(null, b, b, b, b, b, 0ul, pb, 1l, 32l)
            enc_attnpb_q8_c(null, b, b, b, b, b, b, b, 0ul, pb, 1l, 32l)
            enc_attnpb_tq4_c(null, b, b, b, b, b, b, b, 0ul, pb, 1l, 32l)
            enc_qk_dev_c(null, b, 0ul, b, 0ul, b, qk, 32l, 64l, 1l)
            enc_qk_dev128_c(null, b, 0ul, b, 0ul, b, qk, 128l, 64l, 1l)
            enc_attnd16_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attnd32_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndw16_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndw32_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndx16_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndx32_c(null, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attnd_q8_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attnd_tq4_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndw_q8_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndw_tq4_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndx_q8_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            enc_attndx_tq4_c(null, b, b, b, b, b, b, b, 0ul, dk, 1l, 4l, 1l)
            if (ring % 64u != 0u) {
                want |> want_trips(ring64, "ka.ring % 64")
            }
            if (ring % 32u != 0u) {
                want |> want_trips(ring32, "ka.ring % 32")
            }
        }
        //! (dim, woff, xs): the aligned control, an even dim and woff off the float4 lattice, an odd pair, an xs off the lattice
        for (s in fixed_array((8l, 4u, 0u), (6l, 2u, 0u), (5l, 1u, 0u), (8l, 0u, 6u))) {
            var pa = moe_router_args(4l, s._0, 1l, int64(s._2))
            pa.woff = s._1
            enc_moe_router(null, b, b, b, b, pa, 4l, 1l)
            enc_moe_router_b(null, b, b, b, b, pa, 4l, 1l)
            enc_ple_proj_bf16(null, b, b, b, pa, 4l, 1l)
            enc_ple_proj_bf16_one(null, b, b, b, pa, 4l, 1l)
            for (n in routers) {
                want |> want_trips(s._0 % 4l != 0l ? [n] : array<string>(), "pa.dim % 4")
                want |> want_trips(s._1 % 4u != 0u ? [n] : array<string>(), "pa.woff % 4")
                want |> want_trips(s._2 % 4u != 0u ? [n] : array<string>(), "pa.xs % 4")
            }
            for (n in projs) {
                want |> want_trips(s._0 % 2l != 0l ? [n] : array<string>(), "pa.dim % 2")
                want |> want_trips(s._1 % 2u != 0u ? [n] : array<string>(), "pa.woff % 2")
            }
        }
        //! the per-32 gathers' row width: the aligned control, then one off the 32-block (the body counts a row's blocks as dim / 32)
        for (dim in fixed_array(64u, 48u)) {
            var ga = EmbedB32Args(dim = dim, escale = 1.0)
            enc_q51_embed_b32(null, b, 0ul, b, 0ul, b, 0ul, b, 0ul, ga, 4l, 1l)
            enc_iq4nl32_embed_b32(null, b, 0ul, b, 0ul, b, 0ul, b, 0ul, ga, 4l, 1l)
            enc_q4032_embed_b32(null, b, 0ul, b, 0ul, b, 0ul, b, 0ul, ga, 4l, 1l)
            enc_q5032_embed_b32(null, b, 0ul, b, 0ul, b, 0ul, b, 0ul, ga, 4l, 1l)
            if (dim % 32u != 0u) {
                want |> want_trips(gathers, "ka.dim % 32")
            }
        }
        gr_arm(false)
        restore_metal_requires_hook(saved_hook)
        t |> equal(length(want), 50, "dispatch_align: the misaligned rings, router shapes and gather widths owe fifty trips")
        t |> equal(join(g_align_trips, "\n"), join(want, "\n"), "dispatch_align: each builder trips on exactly the items its value breaks")
        delete g_align_trips
    }
}

[test]
def test_metal_misc_kernels(t : T?) {
    t |> run("decode elementwise/misc kernels == direct CPU oracles") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                copy_row_gate(t, dev, queue, 1000)
                argmax_gate(t, dev, queue, 5000, 4321, 1234)    // exact tie — lower index wins
                argmax_gate(t, dev, queue, 700, 0, 699)         // winner at index 0
                argmax_rows_gate(t, dev, queue, 5, 5000)        // the same-slab verify's per-row accept walk
                argmax_rows_gate(t, dev, queue, 9, 700)         // MTP_MAX_ROWS rows, n below one threadgroup
                cls_select_gate(t, dev, queue, 262144)
                cls_select_gate(t, dev, queue, 1000)
                cls_select_gate(t, dev, queue, 248320)   //! the served vocab: a tail stride under a whole subgroup's worth
                embed_q8_gate(t, dev, queue, 512, 3, 2)
                for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032]) {
                    embed_b32_gate(t, dev, queue, f, 96, 5, 0)
                    embed_b32_gate(t, dev, queue, f, 2048, 7, 7 * 64)   //! past a whole other table's blocks: an untied table's offset
                    embed_b32_gate(t, dev, queue, f, 96, 5, 3, 3)       //! a table off a superblock: three blocks past the bind
                    //! ... and a row of nine blocks past a bind every four blocks (q51's 20- and 4-byte blocks) or eight (the 2-byte scales)
                    embed_b32_gate(t, dev, queue, f, 288, 4, 7 * 64 + 13, f == KqFmt.q51 ? 1 : 5)
                }
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_k4), 4, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_k5), 5, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_k6), 6, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq4xs), 44, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq3s4), 35, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq4nl), 45, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_q40), 40, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_nv4), 49, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_k3), 3, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_k2), 2, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq3s), 33, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq3xxs), 34, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq2s), 23, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq2xs), 24, 512, 5)
                embed_kq_gate(t, dev, queue, stamp_of(@@enc_embed_kq_iq2xxs), 25, 768, 5)
                embed_f32_gate(t, dev, queue, 96, 5, 3, 0)
                embed_f32_gate(t, dev, queue, 512, 3, 2, 64)
                for (st in fixed_array(ArStamp.plain, ArStamp.pre, ArStamp.pre_add)) {
                    add_rms_gate(t, dev, queue, st, true, 1500, 1, 1.0)
                    add_rms_gate(t, dev, queue, st, true, 1024, 1, 0.5)    //! the gemma4 layer_out_scale slot
                    add_rms_gate(t, dev, queue, st, false, 1500, 3, 0.75)
                    add_rms_gate(t, dev, queue, st, false, 4096, 2, 1.0)    //! the slab's whole width
                    add_rms_gate(t, dev, queue, st, true, 5376, 1, 0.75)    //! past the slab: the gemma-4 31B's row
                    add_rms_gate(t, dev, queue, st, false, 5376, 3, 0.75)
                    add_rms_gate(t, dev, queue, st, false, 8192, 2, 1.0)    //! twice the slab
                }
                for (st in fixed_array(ArStamp.plain_hx, ArStamp.pre_hx)) {
                    add_rms_gate(t, dev, queue, st, false, 1500, 3, 0.75)
                    add_rms_gate(t, dev, queue, st, false, 5376, 3, 0.75)
                }
                for (st in fixed_array(ArStamp.plain, ArStamp.plain_hx, ArStamp.pre, ArStamp.pre_hx, ArStamp.pre_add)) {
                    add_rms_gate(t, dev, queue, st, false, 2560, 5, 0.75, true)
                    add_rms_gate(t, dev, queue, st, false, 4096, 2, 1.0, true)
                    add_rms_gate(t, dev, queue, st, false, 5376, 5, 0.75, true)
                    add_rms_gate(t, dev, queue, st, false, 5376, 1, 0.75, false, true)
                    add_rms_gate(t, dev, queue, st, false, 5376, 4, 0.75, false, true)
                    add_rms_gate(t, dev, queue, st, false, 8192, 2, 1.0, false, true)
                }
                qk_norm_gate(t, dev, queue, 64, 4, 2, 2, false)     // in-place decode shape
                qk_norm_gate(t, dev, queue, 64, 4, 2, 2, true)      // the no-wv V-fusion shape
                qk_norm_gate(t, dev, queue, 128, 2, 1, 1, false)
                qk_norm_gate(t, dev, queue, 512, 2, 1, 2, false)     // past the 256 lanes: a second element a lane (gemma-4's global heads)
                qk_norm_gate(t, dev, queue, 96, 3, 1, 2, true)       // a head off the lane lattice
                qk_norm_gate(t, dev, queue, 256, 4, 2, 2, false, QKN_V_NORM)   // gemma-4's v-norm in the same dispatch (the block-codec decode)
                qk_norm_gate(t, dev, queue, 512, 2, 1, 2, false, QKN_V_FK)     // ... and its no-wv global layers: V off the pre-norm K
                // the tanh-form CPU geglu; the GPU's exp identity agrees inside the approx bar
                ew2_act_gate(t, dev, queue, "geglu", stamp_of(@@enc_geglu), 800) $(var want : array<float>; g, u : array<float>) {
                    geglu(want, u, int64(length(g)))
                }

                // ===== tower primitives =====
                layernorm_gate(t, dev, queue, 384, 4, 1.0e-5, false)    // whisper-tiny d; strided tail
                layernorm_gate(t, dev, queue, 896, 2, 1.0e-5, false)    // qwen3a d
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-5, false)   // whisper-large d
                layernorm_gate(t, dev, queue, 3840, 3, 1.0e-5, false)   // gemma4uv n_embd
                layernorm_gate(t, dev, queue, 6912, 3, 1.0e-5, false)   // gemma4uv patch_dim - widest row
                layernorm_gate(t, dev, queue, 1280, 1, 1.0e-5, false)   // single-row grid
                layernorm_gate(t, dev, queue, 100, 2, 1.0e-5, false)    // smaller than a simdgroup stride
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-3, false)   // non-default eps: the uniform is live
                layernorm_gate(t, dev, queue, 1280, 2, 1.0e-5, true)    // IN PLACE - the gemma4uv production shape
                gelu_flavor_gate(t, dev, queue, 800, true)     // %64 != 0 tail guard
                gelu_flavor_gate(t, dev, queue, 1000, true)
                gelu_flavor_gate(t, dev, queue, 5120, true)    // whisper-large n_ff
                gelu_flavor_gate(t, dev, queue, 800, false)
                gelu_flavor_gate(t, dev, queue, 5120, false)
                gelu_form_delta(t)
                rms_gate(t, dev, queue, 3840, 3, 1.0e-6, false, RmsStamp.rows)
                rms_gate(t, dev, queue, 2048, 3, 1.0e-6, true, RmsStamp.hx)
                rms_gate(t, dev, queue, 300, 5, 1.0e-6, true, RmsStamp.tw)   //! the row past the 256-thread stride
                rms_gate(t, dev, queue, 70, 3, 1.0e-6, true, RmsStamp.tw)
                rms_gate(t, dev, queue, 512, 2, 1.0e-6, true, RmsStamp.tw)   //! two whole trips of the stride, no tail
                ew2_hx_gate(t, dev, queue, false, true, false, 3, 2048, 3)
                ew2_hx_gate(t, dev, queue, true, true, false, 3, 2048, 3)
                // the bound-total stamps: 40 live rows of 64 inside a 96-row launch, all four combine/twin arms
                ew2_hx_gate(t, dev, queue, false, true, true, 40, 64, 96)
                ew2_hx_gate(t, dev, queue, false, false, true, 40, 64, 96)
                ew2_hx_gate(t, dev, queue, true, true, true, 40, 64, 96)
                ew2_hx_gate(t, dev, queue, true, false, true, 40, 64, 96)
                pos_add_gate(t, dev, queue, 98, 64, 14, 1120)   // 672x336 grid: NON-square, axis order visible
                pos_add_gate(t, dev, queue, 49, 64, 7, 1120)
                q3v_stem_gate(t, dev, queue, 6, 4, 70)   //! a non-square grid at a width off the 64-thread group
                patch_im2col_gate(t, dev, queue, 24, 16, 4, 0, 1.0, 0.0)
                patch_im2col_gate(t, dev, queue, 28, 14, 7, 13, 2.0, -1.0)   //! the padded stride, gemma4v's pixel map
                q3v_stem_gate(t, dev, queue, 8, 8, 64)    // square control
                clamp_gate(t, dev, queue, 1000, CLAMP_IN, false)    // separate dst, %64 tail
                clamp_gate(t, dev, queue, 1000, CLAMP_OUT, true)    // in place
                // the CPU's f16-table GEGLU-quick: the kernel's binary16 round trips land on its table values, so the approx bar (one f16 ulp of form slack)
                geglu_gate(t, dev, queue, 3072 * 3, 3072)   // the E2B ffn width
                geglu_gate(t, dev, queue, 100, 100)         // %64 tail
                rope2d_gate(t, dev, queue, 36, 12, 64)         // the E2B 96x96 grid shape (6x6 patches)
                rope2d_gate(t, dev, queue, 5, 2, 8)            // tiny: one head pair, q = 2
                head_restride_gate(t, dev, queue, 7, 16, 72, 128)   // the gemma3v pad and its compact back
                head_restride_gate(t, dev, queue, 3, 2, 5, 5)       // identity at an odd width, %64 tail
                wdec_kx_gate(t, dev, queue, 96, 128, 2, false)   // whisper hs=64; exact total%64
                wdec_kx_gate(t, dev, queue, 12, 16, 2, true)     // ceil grid guard + the layer-offset slot
                wdec_vx_gate(t, dev, queue, 96, 128, 2, 0, 96, false)     // the cross_kv shape (t0=0, tstride=ta)
                wdec_vx_gate(t, dev, queue, 12, 16, 2, 0, 12, true)       // ceil grid + the folded bias
                wdec_vx_gate(t, dev, queue, 3, 16, 2, 5, 20, true)        // the KV APPEND shape: t0>0 into a wider slab
                wdec_vx_gate(t, dev, queue, 1, 16, 2, 0, 20, false, 16, 48)   // the fused-QKV column pick (soff, 3d row)
                wdec_vx_gate(t, dev, queue, 3, 16, 2, 5, 20, true, 0, 0, true)   // the append into the resident f16 plane
                wdec_vx_gate(t, dev, queue, 3, 16, 2, 5, 20, false, 0, 0, true)  // ... with no bias row: the K store's form
                wdec_attn_pc_gate(t, dev, queue, 1, 128, 2, 600, 600, false)   // 3 chunks, full last chunk boundary
                wdec_attn_pc_gate(t, dev, queue, 3, 128, 2, 300, 259, true)    // causal rows crossing a chunk edge
                wdec_attn_pc_gate(t, dev, queue, 1, 128, 2, 256, 256, false)   // exactly one chunk
                bias_rows_gate(t, dev, queue, 3, 128, false)
                bias_rows_gate(t, dev, queue, 2, 640, true)
                im2col3_gate(t, dev, queue, 20, 16, 20, 1, true)    // conv1 shape: chmajor, s1, same len
                im2col3_gate(t, dev, queue, 10, 32, 20, 2, false)   // conv2 shape: row-major, s2, halved rows
                im2col3_gate(t, dev, queue, 7, 16, 13, 2, true)     // odd tail + the right pad edge
                im2col2d_gate(t, dev, queue, 10, 12, 1, 32, 1)      // conv1 shape: cin 1, K padded 9->32
                im2col2d_gate(t, dev, queue, 10, 8, 4, 36, 8)       // dense K, strided source (padded rows)
                im2col2d_gate(t, dev, queue, 5, 6, 4, 36, 4)        // odd grid, tight source
                cls_epi_gate(t, dev, queue, 900, 3, 7, 30.0)   //! a row off the threadgroup grid, three rows in the one dispatch
                cls_epi_gate(t, dev, queue, 2000, 1, 37, 0.0)  //! the pins alone
                cls_epi_gate(t, dev, queue, 900, 2, 0, 30.0)   //! the cap alone
                // ===== the hyper-connection mixer + the n-gram side input (qwen4exp) =====
                hc_norm_gate(t, dev, queue, 2560, 4, 1)      // the served shape, the single row
                hc_norm_gate(t, dev, queue, 100, 3, 5)       // under one threadgroup stride, five rows
                hc_lo_gate(t, dev, queue, 320, 4)
                hc_lo_gate(t, dev, queue, 100, 4)            // %64 tail
                hc_init_gate(t, dev, queue, 2560, 4, 1)
                hc_init_gate(t, dev, queue, 100, 3, 5)       // %64 tail, five rows
                hc_mix_gate(t, dev, queue, 2560, 4, 1)
                hc_mix_gate(t, dev, queue, 100, 3, 5)
                hc_combine_gate(t, dev, queue, 2560, 4, 1)
                hc_combine_gate(t, dev, queue, 100, 3, 5)
                ple_gate_gate(t, dev, queue, 2560, 4, 10, 17, 3, 1, false)   // the decode form: the ring bound three rows in, slot 7
                ple_gate_gate(t, dev, queue, 100, 4, 10, 0, 0, 1, false)     // slot 0 at offset 0, under a stride
                ple_gate_gate(t, dev, queue, 100, 4, 10, 30, 2, 6, true)     // the window form: six rows into the panel
                ple_conv_gate(t, dev, queue, 256, 4, 4, 3, 10, 0, 1, false)     // one live tap
                ple_conv_gate(t, dev, queue, 256, 4, 4, 3, 10, 5, 1, false)     // two live taps
                ple_conv_gate(t, dev, queue, 256, 4, 4, 3, 10, 25, 1, false)    // every tap live, the ring wrapped
                ple_conv_gate(t, dev, queue, 100, 3, 4, 3, 10, 12, 1, false)    // %64 tail, the first wrap
                ple_conv_gate(t, dev, queue, 100, 3, 4, 3, 10, 0, 14, true)     // the window form from position 0: taps before the sequence and inside the panel
                ple_conv_gate(t, dev, queue, 100, 3, 4, 3, 10, 23, 6, true)     // a later window: taps in the ring (wrapped) and in the panel
                mtl_toy_lens_gate(t, dev, queue, 1000)      // n%64 != 0 — the tail workgroup's guard
                dispatch_align_gate(t, dev, queue)
            }
        } else {
            t |> skip("das_metal is not built on this platform; misc kernel checks skipped")
        }
    }
}

// writes `src` into a per-tag temp directory and compiles it with this binary - the shared half
// of every lens-refusal fixture, whose claim is the child's exit code plus its diagnostic text
def private spawn_lens_compile(tag, src : string) : tuple<rc : int; out : string> =>
    spawn_compile_only(src, "lens_{tag}", 120.0)

//! the require preamble every lens fixture opens with
def private lens_fixture_head : string {
    return join([
        "options gen2",
        "options _dasllama_internal = true",
        "",
        "require dasllama/dasllama_lint public",
        "require das_metal",
        "require metal/msl_shader",
        "require dasllama/dasllama_metal_common",
        "require dasllama/dasllama_metal_lens",
        ""], "\n")
}

//! one [metal_kernel] method of the fixture class - `kann` is the annotation line, so a named
//! kernel and a derived one share this body
def private wg_kernel_method(kann, mname : string) : string {
    return join([kann,
        "    def {mname} \{",
        "        let i = gl_GlobalInvocationID.x",
        "        acc[gl_LocalInvocationID.x] = 1.0",
        "        ys[i] = acc[gl_LocalInvocationID.x]",
        "    \}"], "\n")
}

//! spawns a compile of a one-class [metal_dispatch] program carrying @workgroup state; `extra`
//! is spliced verbatim into the dispatch attribute list (the requires= grammar gate rides it).
//! A non-empty `stamp` replaces the hand-written attribute with that `stamp =` spec, so the
//! class's threadgroup memory can only come from the lens's own derivation - which the program's
//! main then names. `kname` gives the kernel the emitter's `name =` form, which moves every msl
//! global of that kernel onto the given stem; `twins` gives the class a second kernel method of
//! the stamp family's prefix.
def private spawn_wg_fixture(with_tgmem : bool; tag : string; extra : string = ""; stamp : string = "";
                             kname : string = ""; twins : bool = false; params : string = "n : int64") : tuple<rc : int; out : string> {
    let tgarg = (with_tgmem ? "tgmem = \"wg_fixture_msl_tgmem\", " : "") + extra
    let attr = (empty(stamp)
        ? "[metal_dispatch(name = \"enc_wg_fixture\", pso = \"g_wg_fixture_pso\", {tgarg}tg = 64, grid = \"n/64\", params = \"{params}\")]"
        : "[metal_dispatch(stamp = \"{stamp}\", pso = \"g_wg_fixture_pso\")]")
    let kernname = !empty(kname) ? kname : (empty(stamp) ? "wg_fixture_msl" : "")
    let kann = empty(kernname) ? "    [metal_kernel]" : "    [metal_kernel(name = \"{kernname}\")]"
    let kmethods = (twins
        ? wg_kernel_method(kann, "metal_moe_mulmm_one") + "\n" + wg_kernel_method(kann, "metal_moe_mulmm_two")
        : wg_kernel_method(kann, "kmain"))
    let derived = empty(kernname) ? "WgFixture_kmain_msl" : kernname
    let probe = (empty(stamp) || twins) ? "" : "    assert({derived}_tgmem > 0ul)"
    let src = join([
        lens_fixture_head(),
        attr,
        "class WgFixture \{",
        "    @ssbo @binding = 0 @role = \"write\" ys : array<float>",
        "    @workgroup acc : float[64]",
        "",
        kmethods,
        "\}",
        "",
        "var g_wg_fixture_pso : MetalComputePipeline?",
        "",
        "[export]",
        "def main() \{",
        probe,
        "    print(\"ok\\n\")",
        "\}"], "\n")
    return spawn_lens_compile(tag, src)
}

//! spawns a compile of a one-function program whose main carries `body` - the lens's host-side
//! call macros are refused at a call site, where no class annotation can reach them
def private spawn_lens_call(tag, body : string) : tuple<rc : int; out : string> {
    let src = join([
        lens_fixture_head(),
        "var g_wgf_slot : MetalBuffer?",
        "let g_wgf_source = \"msl source text\"",
        "",
        "[export]",
        "def main() \{",
        body,
        "\}"], "\n")
    return spawn_lens_compile(tag, src)
}

//! device-free: a driver's forward answer through `metal_served` - only a served one opens the GPU-served spin window
[test]
def test_metal_served(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let was = get_dispatch_gpu_served()
        set_dispatch_gpu_served(false)
        t |> success(!metal_served(false), "a declined forward passes through as declined")
        t |> success(!get_dispatch_gpu_served(), "and leaves the pool on the CPU window")
        t |> success(metal_served(true), "a served forward passes through as served")
        t |> success(get_dispatch_gpu_served(), "and opens the GPU-served window")
        t |> success(!metal_served(false), "a later decline still passes through")
        t |> success(get_dispatch_gpu_served(), "and the window stays until the next weights load")
        set_dispatch_gpu_served(was)
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers are not compiled")
    }
}

//! the concatenated weight regions key on every piece: two concats over one first piece and two different second pieces each
//! hold their own bytes, and a repeat of either is its cached buffer
[test]
def test_metal_region_cat_keys(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_metal_decode_gate(t) $(var _queue) {
            var inscope a <- [for (i in range(64)); float(i)]
            var inscope b <- [for (i in range(64)); 100.0 + float(i)]
            var inscope c <- [for (i in range(64)); 200.0 + float(i)]
            let rb = 256ul
            let regions0 = weight_regions_cached()
            unsafe {
                let ab = upload_region_cat(addr<void?>(a[0]), rb, addr<void?>(b[0]), rb)
                let ac = upload_region_cat(addr<void?>(a[0]), rb, addr<void?>(c[0]), rb)
                var inscope got_ab <- buf_download(ab, 128, type<float>)
                var inscope got_ac <- buf_download(ac, 128, type<float>)
                t |> equal(got_ab[64], 100.0, "the first concat's second piece is b's")
                t |> equal(got_ac[64], 200.0, "the second concat's second piece is c's, not the first concat's")
                t |> equal(got_ac[63], 63.0, "both concats lead with a")
                t |> success(upload_region_cat(addr<void?>(a[0]), rb, addr<void?>(b[0]), rb) == ab, "a repeat of the first concat is its cached buffer")
                t |> success(upload_region_cat(addr<void?>(a[0]), rb, addr<void?>(c[0]), rb) == ac, "... and of the second")
            }
            t |> equal(weight_regions_cached() - regions0, 2l, "two concats, two cached regions")
            bump_weights_epoch()
        }
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers are not compiled")
    }
}

//! a model swap returns the mirror rings' slack to its floor: the next model's rings are cut at its own windows, not the
//! outgoing model's widest, and the steps encoded over the old layout are stale
[test]
def test_metal_ring_slack_reload(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let was = g_mirror_ring_slack
        g_mirror_ring_slack = 4l * MIRROR_RING_SLACK_MIN
        let epoch0 = g_arena_epoch
        bump_weights_epoch()
        t |> equal(g_mirror_ring_slack, MIRROR_RING_SLACK_MIN, "the reload returns the slack to its floor")
        t |> success(g_arena_epoch > epoch0, "the reload's slack drop stales the steps encoded over the old layout")
        let epoch1 = g_arena_epoch
        bump_weights_epoch()
        t |> equal(g_mirror_ring_slack, MIRROR_RING_SLACK_MIN, "a reload at the floor leaves the slack there")
        t |> equal(g_arena_epoch, epoch1, "a reload at the floor moves no row, so no encoded step goes stale")
        g_mirror_ring_slack = was
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers are not compiled")
    }
}

//! a four-layer shell, every other layer sliding at `window`, one 64-wide K/V head a layer
def private layout_shell(window : int64) : Model {
    var t = Model()
    t.config.n_layers = 4l
    t.config.n_heads = 1l
    t.config.n_kv_heads = 1l
    t.config.head_size = 64l
    t.config.dim = 64l
    t.config.sliding_window = window
    t.config.swa_pattern = 2l
    t.kv_row_prefix <- [0l, 64l, 128l, 192l]
    return <- t
}

//! device-free: the mirror's layout key names its layout - one field moved, the key moves
[test]
def test_metal_mirror_layout_key(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let slack0 = g_mirror_ring_slack
        var a <- layout_shell(512l)
        var b <- layout_shell(512l)
        with_gpu_kv_ring(1) {
            let ka = mirror_layout_key_of(a)
            t |> equal(mirror_layout_key_of(b), ka, "two shells of one layout share a key (the control)")
            b.config.sliding_window = 256l
            t |> success(mirror_layout_key_of(b) != ka, "another sliding window is another key")
            b.config.sliding_window = 512l
            b.kv_row_prefix[3] = 256l
            t |> success(mirror_layout_key_of(b) != ka, "another row prefix is another key")
            b.kv_row_prefix[3] = 192l
            b.kv_row_prefix |> push(256l)
            t |> success(mirror_layout_key_of(b) != ka, "another layer count is another key")
            set_gpu_kv_ring(0)
            t |> success(mirror_layout_key_of(a) != ka, "the ring pin off is another key")
            set_gpu_kv_ring(1)
            g_mirror_ring_slack = 2l * MIRROR_RING_SLACK_MIN
            t |> success(mirror_layout_key_of(a) != ka, "another slack is another key")
            g_mirror_ring_slack = slack0
            t |> equal(mirror_layout_key_of(a), ka, "the slack put back is the first key again")
        }
        delete a
        delete b
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers are not compiled")
    }
}

//! device-free: the K-quant MoE twins' pick - a crowned family takes the twin at every shape, an uncrowned
//! box at or under the knee's mean rows an expert (the adaptive op's region), never past it
[test]
def test_metal_moe_kq_twin_pick(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let was = set_metal_moe_kq_twin_avg(16l)
        t |> success(metal_moe_kq_twin_pick(true, 1024l, 8l, 256l), "a crowned family takes the twin at 32 rows an expert")
        t |> success(metal_moe_kq_twin_pick(false, 375l, 8l, 256l), "uncrowned, the house window's 11.7 rows an expert take the twin")
        t |> success(metal_moe_kq_twin_pick(false, 512l, 8l, 256l), "uncrowned, 16 rows an expert is the knee's last twin shape")
        t |> success(!metal_moe_kq_twin_pick(false, 544l, 8l, 256l), "uncrowned, 17 rows an expert take the base form")
        t |> equal(16l, set_metal_moe_kq_twin_avg(0l), "the knob answers the value it replaced")
        t |> success(!metal_moe_kq_twin_pick(false, 2l, 8l, 256l), "at a knee of 0 an uncrowned box never takes the twin")
        t |> success(metal_moe_kq_twin_pick(true, 2l, 8l, 256l), "and a crowned family still does")
        set_metal_moe_kq_twin_avg(was)
    } else {
        t |> skip("das_metal is not built on this platform; the Metal drivers are not compiled")
    }
}

[test]
def test_lens_tgmem_gate(t : T?) {
    t |> run("[metal_dispatch] on @workgroup state refuses to compile without tgmem=") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let bad = spawn_wg_fixture(false, "bad")
            t |> success(bad.rc != 0, "the tgmem-less fixture must not compile")
            t |> success(find(bad.out, "no tgmem=") >= 0, "the refusal names the gate: {bad.out}")
            let good = spawn_wg_fixture(true, "ok")
            t |> equal(0, good.rc, "the tgmem= twin compiles: {good.out}")
            // the stamped arm: no tgmem= to give, so the builder can only bind the name the lens
            // derives - main names that same global, and an unresolved one fails the compile
            let stamped = spawn_wg_fixture(false, "stamp", "", "kq_mm:wgfix:t")
            t |> equal(0, stamped.rc, "a stamped class derives its tgmem instead of refusing: {stamped.out}")
            t |> success(find(stamped.out, "no tgmem=") < 0, "the stamped class is not refused: {stamped.out}")
            // a named kernel moves every msl global off the <Class>_<method>_msl stem, so the
            // derivation must read the name= the emitter read; main names only wgf_named_tgmem,
            // and a builder binding the derived spelling cannot resolve it
            let named = spawn_wg_fixture(false, "stamp_named", "", "kq_mm:wgfix:t", "wgf_named")
            t |> equal(0, named.rc, "a stamped class with a named kernel binds <name>_tgmem: {named.out}")
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_lens_requires_gate(t : T?) {
    t |> run("[metal_dispatch] requires= takes only 'lhs % int' items") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let bad = spawn_wg_fixture(true, "req_bad", "requires = \"n % x\", ")
            t |> success(bad.rc != 0, "a non-integer modulus must not compile")
            t |> success(find(bad.out, "requires item") >= 0, "the refusal names the item grammar: {bad.out}")
            let bad2 = spawn_wg_fixture(true, "req_bad2", "requires = \"n\", ")
            t |> success(bad2.rc != 0, "an item without '%' must not compile")
            let good = spawn_wg_fixture(true, "req_ok", "requires = \"n % 64\", ")
            t |> equal(0, good.rc, "the well-formed twin compiles: {good.out}")
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_lens_params_gate(t : T?) {
    t |> run("[metal_dispatch] params= names only values a grid=, tg=, requires= or @span reads") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let unread = spawn_wg_fixture(true, "params_unread", "", "", "", false, "n : int64, zz : int64")
            t |> success(unread.rc != 0, "a param nothing reads must not compile")
            t |> success(find(unread.out, "is read by no grid=, tg=, requires= or @span") >= 0, "the refusal names the readers: {unread.out}")
            let req_only = spawn_wg_fixture(true, "params_req", "requires = \"zz % 64\", ", "", "", false, "n : int64, zz : int64")
            t |> equal(0, req_only.rc, "a param only a requires= item reads compiles: {req_only.out}")
            let substr = spawn_wg_fixture(true, "params_substr", "requires = \"rows % 64\", ", "", "", false, "n : int64, row : int64, rows : int64")
            t |> success(substr.rc != 0, "a param read only as a substring of another name is unread")
            t |> success(find(substr.out, "param 'row' is read by no") >= 0, "the refusal names the whole-identifier miss, not its longer neighbour: {substr.out}")
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_lens_stamp_gate(t : T?) {
    t |> run("[metal_dispatch] stamp= takes a three-token spec naming a served family and form") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let two = spawn_wg_fixture(false, "stamp_two", "", "kq_mm:wgfix")
            t |> success(two.rc != 0, "a two-token stamp must not compile")
            t |> success(find(two.out, "must be '<family>:<fmt>:<form>'") >= 0, "the refusal names the spec grammar: {two.out}")
            let fam = spawn_wg_fixture(false, "stamp_fam", "", "no_such_family:wgfix:t")
            t |> success(fam.rc != 0, "an unknown family must not compile")
            t |> success(find(fam.out, "names no family+form") >= 0, "the refusal names the served set: {fam.out}")
            let form = spawn_wg_fixture(false, "stamp_form", "", "kq_mm:wgfix:zz")
            t |> success(form.rc != 0, "an unserved form must not compile")
            t |> success(find(form.out, "names no family+form") >= 0, "the refusal names the served set: {form.out}")
            // the family matches its kernels by prefix, so a class carrying two of them leaves the
            // stamp with no one kernel to derive access and threadgroup memory from
            let twins = spawn_wg_fixture(false, "stamp_twins", "", "moe_mm_split:wgfix:t", "", true)
            t |> success(twins.rc != 0, "two kernel methods under one stamp must not compile")
            t |> success(find(twins.out, "one stamp dispatches one kernel") >= 0, "the refusal names the rule: {twins.out}")
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test]
def test_lens_call_macro_gates(t : T?) {
    t |> run("compile_stamp and release_handles refuse a shape they cannot spell") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let arity = spawn_lens_call("cs_arity", "    compile_stamp(g_wgf_source)")
            t |> success(arity.rc != 0, "compile_stamp at one argument must not compile")
            t |> success(find(arity.out, "compile_stamp expects") >= 0, "the refusal names the form: {arity.out}")
            let lit = spawn_lens_call("cs_lit", "    var ok = true\n    compile_stamp(\"not a global\", ok)")
            t |> success(lit.rc != 0, "compile_stamp on a non-variable must not compile")
            t |> success(find(lit.out, "compile_stamp expects") >= 0, "the refusal names the form: {lit.out}")
            let stem = spawn_lens_call("cs_stem", "    var ok = true\n    compile_stamp(g_wgf_source, ok)")
            t |> success(stem.rc != 0, "a source global off the _msl stem must not compile")
            t |> success(find(stem.out, "is not a kernel source global") >= 0, "the refusal names the stem rule: {stem.out}")
            let pair = spawn_lens_call("rp_arity", "    race_pso_pair_stamp(g_wgf_slot, \"fam\", g_wgf_source)")
            t |> success(pair.rc != 0, "race_pso_pair_stamp at three arguments must not compile")
            t |> success(find(pair.out, "race_pso_pair_stamp expects") >= 0, "the refusal names the form: {pair.out}")
            let pstem = spawn_lens_call("rp_stem", "    race_pso_pair_stamp(g_wgf_slot, \"fam\", g_wgf_source, g_wgf_source)")
            t |> success(pstem.rc != 0, "a race arm off the _msl stem must not compile")
            t |> success(find(pstem.out, "is not a kernel source global") >= 0, "the refusal names the stem rule: {pstem.out}")
            let rs = spawn_lens_call("rs_arity", "    race_stems(g_wgf_slot)")
            t |> success(rs.rc != 0, "race_stems with no arguments past the race must not compile")
            t |> success(find(rs.out, "race_stems expects") >= 0, "the refusal names the form: {rs.out}")
            let none = spawn_lens_call("rh_none", "    release_handles()")
            t |> success(none.rc != 0, "release_handles with no slots must not compile")
            t |> success(find(none.out, "expects one or more handle slots") >= 0, "the refusal names the rule: {none.out}")
            let slot = spawn_lens_call("rh_ok", "    release_handles(g_wgf_slot)")
            t |> equal(0, slot.rc, "one named slot compiles - the control that the fixture program itself is sound: {slot.out}")
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}

[test, arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def test_kq_rows_race_verdicts(t : T?) {
    t |> run("the decode race returns a small-batch verdict per K-quant format") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!metal_decode_init()) {
                t |> skip("no Metal device")
                return
            }
            let res <- metal_tensor_race_decode()
            var seen = 0
            var form = 0
            var half = 0
            for (r in res) {
                if (r.family == "kq_mvb2_k4_r2") {
                    form++
                } elif (r.family |> starts_with(KQ_HALF_CROWN_PREFIX)) {
                    half++
                } else {
                    continue if (!(r.family |> starts_with("kq_rows_")))
                    seen++
                }
                to_log(LOG_INFO, "race {r.family}: {r.winner} (base {r.base_ms:.3f} ms, twin {r.twin_ms:.3f} ms) {r.note}\n")
                t |> success(r.base_ms > 0.0lf && r.twin_ms > 0.0lf, "{r.family}: both arms timed (base {r.base_ms:.3f} ms, twin {r.twin_ms:.3f} ms) {r.note}")
                t |> success(r.winner == "tensor" || r.winner == "simdgroup", "{r.family}: a verdict, not a dispatch failure ({r.winner}: {r.note})")
                t |> success(!(r.note |> starts_with("twin OUTPUT MISMATCH")), "{r.family}: the twin's output agreed with the base arm's ({r.note})")
            }
            t |> equal(seen, length(KQ_ROWS_RACE_FORMATS), "every K-quant format with a small-batch twin raced")
            t |> equal(form, 1, "the k4 twin's two-row tile raced its two-column form")
            t |> equal(half, length(KQ_ROWS_RACE_FORMATS), "every K-quant format's half-x route raced the box's f32 rows route")
            //! every superblock format the Metal tier serves races its rows, so the table can drop no format; iq3s4's small-batch
            //! forms ship no half-x twin, so it alone has no row
            for (slot in range(KQ_FMT_COUNT)) {
                let kf = kq_fmt_of_slot(slot)
                continue if (kf == KqFmt.q8 || kf == KqFmt.iq3s4 || !kq_sb(kf) || !kq_fmt_gpu_supported(kf))
                var raced = false
                for (r in KQ_ROWS_RACE_FORMATS) {
                    raced = raced || r.fmt == kf
                }
                t |> success(raced, "{kf}: a served superblock format has its row in the race table")
            }
            // every rows verdict is written back before the half races, so each half race's base arm is the rows route
            // the box will run: the crown the process holds after the race is the verdict the race returned
            for (r in res) {
                continue if (!(r.family |> starts_with("kq_rows_")) || (r.family |> starts_with(KQ_HALF_CROWN_PREFIX)))
                for (f in KQ_ROWS_RACE_FORMATS) {
                    if (r.family == "kq_rows_{f.fmt}") {
                        t |> equal(r.winner == "tensor", metal_kq_rows_crowned(f.fmt), "{r.family}: the rows verdict the race returned is the one the process holds")
                    }
                }
            }
        } else {
            t |> skip("das_metal is not built on this platform")
        }
    }
}
