options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require daslib/fio
require math
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math
require dasllama/dasllama_kv_codec  // quantize_tq4kv_row/cvt_tq4kv_to_f32 — the codec under probe
require dasllama/dasllama_tokenizer
require llvm/daslib/f16_cvt
require dasllama/dasllama_env

// Is one channel order enough, or is it MoE-shaped? Run SmolLM2 over 5 text domains, learn a
// pair-granular hot/quiet partition per (layer, head, side) two ways — GLOBAL (pooled, normalized
// per domain) and ORACLE (that domain's own stats) — then score the sorted 8/4 mixed codec per
// domain under both. The gap is the multi-order headroom; hot-set churn says how many pairs
// actually move across the boundary.
// Usage: bin/daslang -jit modules/dasLLAMA/harness/tq4_order_probe.das

let ND = 5
let DOMAIN_NAMES = fixed_array("prose ", "code  ", "math  ", "german", "logs  ")

def domain_text(d : int) : string {
    if (d == 0) {
        return "The old lighthouse keeper climbed the spiral staircase for the last time, counting each of the one hundred and twelve steps as he had done every evening for forty years. Below him the harbor lights flickered on one by one, and the fishing boats came home through the narrow channel between the rocks. He remembered the winter of the great storm, when three ships had run aground in a single night and the whole town had formed a human chain through the freezing surf to pull the sailors ashore. His daughter wrote him letters from the city, asking when he would finally retire, but he always answered the same way: the light needs someone who knows the water. In the morning he packed his few belongings, left the key in the lock as instructed, and walked down the hill without looking back."
    }
    if (d == 1) {
        return "static int parse_header(const uint8_t *buf, size_t len, header_t *out) \{ if (len < HEADER_SIZE) return -EINVAL; out->magic = read_u32_le(buf); if (out->magic != MAGIC_V2) return -EBADMSG; out->version = buf[4]; out->flags = buf[5]; out->count = read_u16_le(buf + 6); out->crc = read_u32_le(buf + 8); uint32_t computed = crc32(buf + 12, len - 12); if (computed != out->crc) \{ log_warn(\"crc mismatch: %08x vs %08x\", computed, out->crc); return -EIO; \} for (int i = 0; i < out->count; ++i) \{ entry_t *e = &out->entries[i]; e->offset = read_u32_le(buf + 12 + i * 8); e->size = read_u32_le(buf + 16 + i * 8); if (e->offset + e->size > len) return -ERANGE; \} return 0; \}"
    }
    if (d == 2) {
        return "Let f(x) = 3x^2 - 7x + 2. The roots are x = (7 +/- sqrt(49 - 24)) / 6 = (7 +/- 5) / 6, so x = 2 or x = 1/3. The derivative f'(x) = 6x - 7 vanishes at x = 7/6, where f(7/6) = 3 * 49/36 - 49/6 + 2 = 49/12 - 98/12 + 24/12 = -25/12, the minimum. Integrating, the area between the roots is the integral from 1/3 to 2 of (3x^2 - 7x + 2) dx = [x^3 - 7x^2/2 + 2x] evaluated at the bounds, which gives 8 - 14 + 4 - (1/27 - 7/18 + 2/3) = -2 - 25/81... check: 1/27 = 6/162, 7/18 = 63/162, 2/3 = 108/162, so the bracket is 51/162 = 17/54, and the definite integral equals -125/54, negative as expected below the axis."
    }
    if (d == 3) {
        return "Der alte Uhrmacher oeffnete seine Werkstatt jeden Morgen um Punkt sieben Uhr, noch bevor die Strassenbahnen durch die enge Gasse fuhren. Auf seinem Tisch lagen winzige Zahnraeder, Federn und Schrauben in flachen Holzschachteln, jede mit einem handgeschriebenen Etikett versehen. Die Kunden brachten ihm Taschenuhren ihrer Grossvaeter, zerbrochene Wanduhren aus Kriegszeiten und manchmal auch moderne Armbanduhren, die kein anderer mehr reparieren wollte. Er arbeitete langsam und gruendlich, mit einer Lupe im Auge und unendlicher Geduld in den Haenden. Am Abend, wenn das Licht schraeg durch die staubigen Fenster fiel, zog er nacheinander alle Uhren auf und lauschte dem vielstimmigen Ticken, das die kleine Werkstatt wie ein Herzschlag erfuellte."
    }
    return "2026-07-12T08:14:03.221Z INFO  worker-3 request accepted id=7f3a92 route=/api/v2/ingest bytes=48211 2026-07-12T08:14:03.409Z DEBUG worker-3 cache miss key=usr:8812:profile ttl=300 2026-07-12T08:14:03.501Z INFO  worker-1 request accepted id=7f3a93 route=/api/v2/query bytes=1022 2026-07-12T08:14:03.688Z WARN  worker-2 slow query ms=1204 table=events shard=11 2026-07-12T08:14:04.012Z INFO  worker-3 response sent id=7f3a92 status=201 ms=791 2026-07-12T08:14:04.100Z ERROR worker-4 upstream timeout host=10.0.3.17 port=9200 retry=1 2026-07-12T08:14:04.315Z INFO  worker-1 response sent id=7f3a93 status=200 ms=814 2026-07-12T08:14:04.606Z DEBUG worker-2 gc pause ms=12 heap=412MB 2026-07-12T08:14:05.001Z INFO  worker-4 upstream recovered host=10.0.3.17 attempts=2"
}

// probe-local 8-bit symmetric amax block quant (f16-rounded scale), mixed-arm hot block
def quant_int8_block(var v : array<float>; i0, n : int64) {
    var amax = 0.0
    for (j in range64(i0, i0 + n)) {
        amax = max(amax, abs(v[j]))
    }
    let d = f16_to_f32(f32_to_f16(amax / 127.0))
    let id = d != 0.0 ? 1.0 / d : 0.0
    for (j in range64(i0, i0 + n)) {
        v[j] = float(clamp(int(floor(v[j] * id + 0.5)), -127, 127)) * d
    }
}

// mixed 8/4 rel-MSE of one (layer, head, side, session) row set under a given channel order
def score_mixed(src : float const?; kvd, hs, h, npast : int64; order : array<int>;
                var seg : array<float>; var dq : array<float>; var blk : array<uint8>) : tuple<double; double> {
    var e2 = 0.0lf
    var pw = 0.0lf
    unsafe {
        for (row in range64(npast)) {
            let off = row * kvd + h * hs
            for (i in range64(hs)) {
                seg[i] = src[off + int64(order[i])]
                pw += double(seg[i] * seg[i])
            }
            for (i in range64(hs)) {
                dq[i] = seg[i]
            }
            quant_int8_block(dq, 0l, 32l)
            if (hs > 32l) {
                quantize_tq4kv_row(addr(blk[0]), addr(seg[0]) + 32l, hs - 32l)
                cvt_tq4kv_to_f32(addr(dq[0]) + 32l, addr(blk[0]), hs - 32l)
            }
            for (i in range64(hs)) {
                let e = double(dq[i] - seg[i])
                e2 += e * e
            }
        }
    }
    return (e2 => pw)
}

def pair_order_from(scores : array<double>; np : int64; var order : array<int>) {
    let n = int(np)
    var porder <- [for (p in range(n)); p]
    porder |> sort() $(oa, ob) => scores[oa] > scores[ob]
    for (p in range(n)) {
        order[2 * p] = 2 * porder[p]
        order[2 * p + 1] = 2 * porder[p] + 1
    }
    delete porder
}

[export]
def main {   // nolint:STYLE037,STYLE038 — two stats passes over (layer, head, side, domain) sharing every accumulator
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "D:/Work/llama.cpp/models/" : env
    let path = path_join(mdir, "SmolLM2-135M-Instruct-Q8_0.gguf")
    if (!stat(path).is_valid) {
        panic("model not found: {path}")
    }
    var tr <- load_gguf(path, QuantMode.q8)
    let tok <- load_tokenizer_auto(path)
    tr.config.seq_len = min(tr.config.seq_len, 512l)
    let c & = unsafe(tr.config)
    let hs = layer_head_size(c, 0l)
    let np = hs / 2l           // rope pairs per head
    let nh = layer_n_kv_heads(c, 0l)
    let nl = c.n_layers

    var sessions : array<Session>
    var npasts : array<int64>
    sessions |> reserve(ND)
    npasts |> reserve(ND)
    for (d in range(ND)) {
        var sd <- make_run_state(c, KVDtype.f32, KVDtype.f32)
        var ids <- encode(tok, domain_text(d))
        with_job_que() {
            eval_(tr, sd, ids)
        }
        print("domain {DOMAIN_NAMES[d]}: {length(ids)} tokens\n")
        npasts |> push(sd.n_past)
        sessions |> emplace(sd)
        delete ids
    }

    // pass 1: pair-energy stats per (layer, head, side, domain)
    let NSLOT = nl * nh * 2l
    var pe : array<double>       // [slot * ND + d] * np + p
    var pt : array<double>       // per-slot per-domain totals
    pe |> resize(NSLOT * int64(ND) * np)
    pt |> resize(NSLOT * int64(ND))
    unsafe {
        for (d in range64(int64(ND))) {
            let s & = sessions[int(d)]
            let kb = addr<uint8 const?>(s.key_cache[0])
            let vb = addr<uint8 const?>(s.value_cache[0])
            for (l in range64(nl)) {
                if (layer_kv_source(c, l) != l) {
                    continue
                }
                let kvd = layer_kv_dim(c, l)
                let kp = reinterpret<float const?>(kb + s.kv_bprefix_k[l] * c.seq_len)
                let vp = reinterpret<float const?>(vb + s.kv_bprefix_v[l] * c.seq_len)
                for (h in range64(kvd / hs)) {
                    for (side in range64(2l)) {
                        let src = side == 0l ? kp : vp
                        let slot = (l * nh + h) * 2l + side
                        let base = (slot * int64(ND) + d) * np
                        for (row in range64(npasts[int(d)])) {
                            let off = row * kvd + h * hs
                            for (p in range64(np)) {
                                let a = src[off + 2l * p]
                                let b = src[off + 2l * p + 1l]
                                pe[base + p] += double(a * a + b * b)
                            }
                        }
                        var tot = 0.0lf
                        for (p in range64(np)) {
                            tot += pe[base + p]
                        }
                        pt[slot * int64(ND) + d] = tot
                    }
                }
            }
        }
    }

    // pass 2: per domain, per side: mixed 8/4 under GLOBAL (normalized-pooled) vs ORACLE order,
    // plus hot-set churn (of np/2 hot pairs)
    var gsc : array<double>
    var osc : array<double>
    var gorder : array<int>
    var oorder : array<int>
    var seg : array<float>
    var dq : array<float>
    var blk : array<uint8>
    gsc |> resize(np)
    osc |> resize(np)
    gorder |> resize(hs)
    oorder |> resize(hs)
    seg |> resize(hs)
    dq |> resize(hs)
    blk |> resize(hs / 32l * 18l)
    var ge2 : double[10]   // [d * 2 + side]
    var oe2 : double[10]
    var gpw : double[10]
    var churn : int[10]
    var nslots = 0
    unsafe {
        for (l in range64(nl)) {
            if (layer_kv_source(c, l) != l) {
                continue
            }
            let kvd = layer_kv_dim(c, l)
            for (h in range64(kvd / hs)) {
                for (side in range64(2l)) {
                    let slot = (l * nh + h) * 2l + side
                    // global order: pool per-domain normalized pair shares
                    for (p in range64(np)) {
                        gsc[p] = 0.0lf
                    }
                    for (d in range64(int64(ND))) {
                        let tot = pt[slot * int64(ND) + d]
                        if (tot > 0.0lf) {
                            let base = (slot * int64(ND) + d) * np
                            for (p in range64(np)) {
                                gsc[p] += pe[base + p] / tot
                            }
                        }
                    }
                    pair_order_from(gsc, np, gorder)
                    if (side == 0l) {
                        nslots ++
                    }
                    for (d in range64(int64(ND))) {
                        let s & = sessions[int(d)]
                        let cb = (side == 0l
                            ? addr<uint8 const?>(s.key_cache[0]) + s.kv_bprefix_k[l] * c.seq_len
                            : addr<uint8 const?>(s.value_cache[0]) + s.kv_bprefix_v[l] * c.seq_len)
                        let src = reinterpret<float const?>(cb)
                        let base = (slot * int64(ND) + d) * np
                        for (p in range64(np)) {
                            osc[p] = pe[base + p]
                        }
                        pair_order_from(osc, np, oorder)
                        // churn: oracle-hot pairs missing from global-hot (hot = top np/2 pairs)
                        for (j in range64(np / 2l)) {
                            let op = oorder[2l * j] / 2
                            var found = false
                            for (g in range64(np / 2l)) {
                                if (gorder[2l * g] / 2 == op) {
                                    found = true
                                    break
                                }
                            }
                            if (!found) {
                                churn[int(d) * 2 + int(side)] ++
                            }
                        }
                        let gr = score_mixed(src, kvd, hs, h, npasts[int(d)], gorder, seg, dq, blk)
                        let orr = score_mixed(src, kvd, hs, h, npasts[int(d)], oorder, seg, dq, blk)
                        ge2[int(d) * 2 + int(side)] += gr._0
                        oe2[int(d) * 2 + int(side)] += orr._0
                        gpw[int(d) * 2 + int(side)] += gr._1
                    }
                }
            }
        }
    }

    let slots_per_side = nslots   // (layer, head) slots contributing per domain per side
    print("\nglobal-vs-oracle, sorted 8/4 mixed (churn = avg oracle-hot pairs outside global-hot, of {np / 2l}):\n")
    for (d in range(ND)) {
        let kg = ge2[d * 2] / gpw[d * 2]
        let ko = oe2[d * 2] / gpw[d * 2]
        let vg = ge2[d * 2 + 1] / gpw[d * 2 + 1]
        let vo = oe2[d * 2 + 1] / gpw[d * 2 + 1]
        let ckf = float(churn[d * 2]) / float(slots_per_side)
        let cvf = float(churn[d * 2 + 1]) / float(slots_per_side)
        print("  {DOMAIN_NAMES[d]}  K: glob {kg}  oracle {ko}  gap {100.0lf * (kg / ko - 1.0lf)}%  churn {ckf}\n")
        print("          V: glob {vg}  oracle {vo}  gap {100.0lf * (vg / vo - 1.0lf)}%  churn {cvf}\n")
    }
    delete pe
    delete pt
    delete gsc
    delete osc
    delete gorder
    delete oorder
    delete seg
    delete dq
    delete blk
    delete npasts
    delete sessions
    delete tr
}
