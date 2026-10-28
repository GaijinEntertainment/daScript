options gen2
options _dasllama_internal = true
options _cyclomatic_complexity = 0  // the pickers are format ladders, one arm a format

require dasllama/dasllama_kqformat
require dasllama/dasllama_metal_kernels   // the simdgroup base mul_mm builders
require dasllama/dasllama_metal_common   // the pipeline globals the base builders seat
require dasllama/dasllama_metal_prefill   // the dense mul_mm twins and dev-W dequant builders the pickers name
require _metal_kernel_common   // kernel_of: the kernel behind a [metal_dispatch] builder
require dastest/testing_boost   // T: the split cell's test context
require _compares   // Bar, NAN_SENTINEL: the split cell's bar and sentinel

// The dense Metal kq kernel pickers the kernel gates share: a format's mul_mm kernel by form, and its 64x64-chunk
// dev-W dequant pass - one home, so a new format or form lands once.
//! the 64x64-chunk dev-W dequant kernel of every split-scale and per-32 format (the K-quant deep class dequants per 32-block
//! through enc_dequant_k*h, another signature)
def kq_dq_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_dq_%")   //! generic, as the gates are: the Apple half alone instantiates it

//! the dense kq mul_mm tensor twin of a format by form: t (f32 X), th, th128, thdb and thdb128 (f16 X)
def kq_mm_form_kernel(f; form : string) {
    if (form == "t") return kq_mm_t_kernel(f)
    if (form == "th") return kq_mm_th_kernel(f)
    if (form == "th128") return kq_mm_th128_kernel(f)
    if (form == "thdb") return kq_mm_thdb_kernel(f)
    return kq_mm_thdb128_kernel(f)
}

//! the simdgroup base mul_mm stamp of a superblock format (the per-32 bases bind no strip view: another builder signature)
def kq_mm_base_stamp(f) => kq_form_ladder(f, "stamp_of", "enc_kq_mm_%_c", "!b32")

def private kq_mm_t_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_t_c")

def private kq_mm_th_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_th_c")

def private kq_mm_th128_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_th128_c")

def private kq_mm_thdb_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_thdb_c")

def private kq_mm_thdb128_kernel(f) => kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_thdb128_c")

//! the dense kq split-K stamp of a format: thsk (f16 X) or tsk (f32 X) - the builders carry the K split, another signature
def kq_mm_split_kernel(f; half : bool) => half ? kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_thsk_c") : kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_tsk_c")

//! the dense kq 16-row split-K stamp of a format: thsk16 (f16 X) or tsk16 (f32 X) - a window of 16 rows or fewer
def kq_mm_split16_kernel(f; half : bool) => half ? kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_thsk16_c") : kq_form_ladder(f, "kernel_of", "enc_kq_mm_%_tsk16_c")

//! the simdgroup base mul_mm split-K stamp of a superblock format (the per-32 base stamps bind no strip view: `kq_mm_b32_split_stamp`)
def kq_mm_base_split_stamp(f) => kq_form_ladder(f, "stamp_of", "enc_kq_mm_%_sk_c", "!b32")

//! the per-32 base mul_mm split-K stamp of a format
def kq_mm_b32_split_stamp(f) => kq_form_ladder(f, "stamp_of", "enc_kq_mm_%_sk_c", "b32")

//! the split-K reduce over `nsk` partial planes of `total` floats in `bpart` into `by`
def split_k_reduce_run(mc : MetalCell; enc : MetalComputeEncoder?; var psoR : MetalComputePipeline?; bpart, by, total, nsk : int) {
    let sr = stamp_of(@@enc_sk_reduce_c)
    var kaR = SkReduceArgs(total = uint(total / 4), ksplit = uint(nsk))
    stamp_run(sr, psoR) {
        invoke(sr.enc, enc, mc.buf[bpart], mc.buf[by], 0ul, kaR, int64(total / 4))
    }
}

//! One split-K parity cell's run (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`): the output and the partial planes under the
//! NaN sentinel, `stamp` writing into `bo` - the output at one way unless `planes` asks the planes there too, the planes past one -
//! the reduce summing them, the output against `want` at `bar` (over `env` where the bar is an envelope's)
def split_cell(t : T?; var mc : MetalCell; tag : string; m, kdim, ndim, nsk : int; planes : bool; want, env : array<float>; bar : Bar;
               stamp : block<(enc : MetalComputeEncoder?; bo, bk, bn : MetalBuffer?) : void>) {
    var err : string
    var psoR = mc |> keep_stamp(stamp_of(@@enc_sk_reduce_c), err)
    return if (!pso_ok(t, "{tag} reduce", psoR, err))
    let bpart = mc |> out_words(nsk * m * ndim, NAN_SENTINEL)
    let by = mc |> out_words(m * ndim, NAN_SENTINEL)
    let bk = mc |> in_plain([uint(kdim)])
    let bn = mc |> in_plain([uint(ndim)])
    let reduce = planes || nsk > 1
    let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
        invoke(stamp, enc, mc.buf[reduce ? bpart : by], mc.buf[bk], mc.buf[bn])
        if (reduce) {
            split_k_reduce_run(mc, enc, psoR, bpart, by, m * ndim, nsk)
        }
    }
    return if (!ran)
    if (empty(env)) {
        check_buf(t, tag, mc.buf[by], want, bar, Shape(row_width = ndim))
    } else {
        check_buf(t, tag, mc.buf[by], want, bar, Shape(row_width = ndim), env)
    }
}
