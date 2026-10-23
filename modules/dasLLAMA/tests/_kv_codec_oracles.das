options gen2
options _dasllama_internal = true

// The K/V block codecs' CPU oracles and fixtures both homes' kernel cells read - the CPU cache's own codec (`kv_store_row` /
// `kv_load_row`, the tq4 batch rotations) over the cells' rows, the mirror bytes a store leaves, the staged rows with their
// edges. No device module: test_vulkan_kv_codec_kernels, _vk_codec_attn and test_cuda_kernels require it.

module _kv_codec_oracles shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_kernels_discrete   // nolint:STYLE030 — KV_TQ4_SIGNS, folded into SIGN_ROW
require _compares
require daslib/algorithm
require math

let MIRROR_FILL = 0xA5A5A5A5u
let MIRROR_FILL_BYTE = 0xA5u8
let ODD_LAYERBASE = 7     //! blocks before a layer's rows: odd, so its blocks start two bytes off a word
let SLACK_BLOCKS = 1
let HALF_MAX = uint16(0x7BFF)   //! 65504, the largest finite half: where an f16 store clamps
let HALF_NAN = uint16(0x7E00)
let HALF_NAN_PAIR = 0x7E007E00u
let PAST_HALF_RANGE = 4.0e7   //! times a fixture's values: a block scale past 65504, which the device clamps
let PAST_HALF_VALUES = 3.0e4  //! times a fixture's values: dequantized values to 1e5 under a block scale inside the half range
let SIGN_ROW = KV_TQ4_SIGNS   //! tq4's sign row: the driver's own
let SH_MIRROR_POS = 21   //! the shadow arms' mirror: this many positions from block ODD_LAYERBASE, the pass over [SH_POS0, SH_POS0 + SH_ROWS)
let SH_POS0 = 5
let SH_ROWS = 13

def block_bytes(kv : KVDtype) : int => int(kv_unit_bytes(kv))

//! the mirror plane the CPU codec leaves: `MIRROR_FILL`, then row `r` of `staged` (in the stored basis) at block `blocks[r]`
def codec_want(kv : KVDtype; staged : array<float>; blocks : array<int>; mir_bytes, kvd : int) : array<uint8> {
    var want : array<uint8>
    want |> resize(mir_bytes)
    want |> fill(MIRROR_FILL_BYTE)
    for (r in range(length(blocks))) {
        kv_store_row(unsafe(addr<uint8?>(want[blocks[r] * block_bytes(kv)])), kv, int64(kvd), 0l, 0l, int64(kvd), unsafe(addr<float const?>(staged[r * kvd])))
    }
    return <- want
}

//! Every block scale of `bytes` the CPU stored as an infinite half, set to the half the device clamps it to; the count set.
def clamp_inf_scales(var bytes : array<uint8>; kv : KVDtype) : int {
    var clamped = 0
    for (b in range(length(bytes) / block_bytes(kv))) {
        let at = b * block_bytes(kv)
        let hi = uint(bytes[at + 1])
        if ((hi & 0x7Fu) == 0x7Cu && bytes[at] == 0u8) {
            bytes[at] = 0xFFu8
            bytes[at + 1] = uint8((hi & 0x80u) | 0x7Bu)
            clamped++
        }
    }
    return clamped
}

let KS_KVD = 96
let KS_ROWS = 3
let KS_POS_PER_REGION = 12
let KS_ROW_BLOCKS = KS_KVD / 32
let KS_REGION_BLOCKS = KS_POS_PER_REGION * KS_ROW_BLOCKS

//! a mirror plane of `blocks` blocks past the odd layer base and a slack block, to a whole word
def mirror_bytes(kv : KVDtype; blocks : int) : int => ((ODD_LAYERBASE + blocks + SLACK_BLOCKS) * block_bytes(kv) + 3) / 4 * 4

//! each block's largest magnitude on another element; V's row-1 middle block zero; K's last and V's first row-2 block past half
def ks_staged() : tuple<k : array<float>; v : array<float>> {
    var k <- row_fill(KS_ROWS * KS_KVD, 37, 11, 0.071, 3.3)
    var v <- row_fill(KS_ROWS * KS_KVD, 29, 5, 0.043, 2.0, 89)
    for (i in range(32)) {
        v[KS_KVD + 32 + i] = 0.0
        k[2 * KS_KVD + 64 + i] *= PAST_HALF_RANGE
        v[2 * KS_KVD + i] *= PAST_HALF_RANGE
    }
    k[5] = -9.25   // a negative block maximum: the quant reads -127
    return <- (k = k, v = v)
}

//! tq4's sign row, as the CPU chain fills it
def tq4_sign_row() : array<float> {
    var signs : array<float>
    tq4_fill_signs(signs, SIGN_ROW)
    return <- signs
}

//! `n` rows of `stride` floats rotated head by head into tq4's basis, as the CPU chain rotates them
def tq4_rotated(rows_f : array<float>; n, stride, hs : int; signs : array<float>) : array<float> {
    var r := rows_f
    tq4_rotate_batch(r, int64(n), int64(stride), int64(hs), unsafe(addr<float const?>(signs[0])))
    return <- r
}

//! the CPU cache's bytes padded to a word past the last unit, and the rows the CPU dequant reads back: what the device attends over
def codec_mirror_of_stored(kv : KVDtype; stored : array<float>; n, kvd, base_units : int) : tuple<bytes : array<uint8>; deq : array<float>> {
    let unit_bytes = int(kv_unit_bytes(kv))
    let row_bytes = int(kv_row_bytes(kv, int64(kvd)))
    var bytes : array<uint8>
    bytes |> resize(((base_units * unit_bytes + n * row_bytes + 4 + 3) / 4) * 4)
    var deq : array<float>
    deq |> resize(n * kvd)
    var at = unsafe(addr<uint8?>(bytes[base_units * unit_bytes]))
    for (r in range(n)) {
        kv_store_row(at, kv, int64(kvd), int64(r), 0l, int64(kvd), unsafe(addr<float const?>(stored[r * kvd])))
        kv_load_row(at, kv, int64(kvd), int64(r), 0l, int64(kvd), unsafe(addr<float?>(deq[r * kvd])))
    }
    return <- (bytes = bytes, deq = deq)
}

//! the stamps and the CPU's `dot_q8q8kv` agree to this quantization of the query, not bit for bit to f32
def q8_query_roundtrip(q : array<float>) : array<float> {
    var out := q
    for (b in range(length(q) / 32)) {
        var amax = 0.0
        for (k in range(32)) {
            amax = max(amax, abs(q[b * 32 + k]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        for (k in range(32)) {
            out[b * 32 + k] = round(q[b * 32 + k] * id) * d
        }
    }
    return <- out
}
