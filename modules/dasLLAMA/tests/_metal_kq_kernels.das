options gen2
options _dasllama_internal = true
options _cyclomatic_complexity = 0  // the pickers are format ladders, one arm a format

require dasllama/dasllama_kqformat
require dasllama/dasllama_metal_kernels   // the simdgroup base mul_mm builders
require dasllama/dasllama_metal_common   // the pipeline globals the base builders seat
require dasllama/dasllama_metal_prefill   // the dense mul_mm twins and dev-W dequant builders the pickers name
require _metal_kernel_common   // kernel_of: the kernel behind a [metal_dispatch] builder

// The dense Metal kq kernel pickers the kernel gates share: a format's mul_mm kernel by form, and its 64x64-chunk
// dev-W dequant pass - one home, so a new format or form lands once.
//! the 64x64-chunk dev-W dequant kernel of every split-scale and per-32 format (the K-quant deep class dequants per 32-block
//! through enc_dequant_k*h, another signature)
def kq_dq_kernel(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_dq_iq4xs)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_dq_iq3s4)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_dq_iq4nl)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_dq_q40)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_dq_nv4)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_dq_k3)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_dq_k2)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_dq_iq3s)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_dq_iq3xxs)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_dq_iq2s)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_dq_iq2xs)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_dq_iq2xxs)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_dq_q51)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_dq_iq4nl32)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_dq_q4032)
    if (f == KqFmt.q5032) return kernel_of(@@enc_kq_dq_q5032)
    panic("kq_dq_kernel: no dev-W dequant kernel for {f}")
    return kernel_of(@@enc_kq_dq_iq4xs)
}

//! the dense kq mul_mm tensor twin of a format by form: t (f32 X), th, th128, thdb and thdb128 (f16 X)
def kq_mm_form_kernel(f; form : string) {
    if (form == "t") return kq_mm_t_kernel(f)
    if (form == "th") return kq_mm_th_kernel(f)
    if (form == "th128") return kq_mm_th128_kernel(f)
    if (form == "thdb") return kq_mm_thdb_kernel(f)
    return kq_mm_thdb128_kernel(f)
}

//! the simdgroup base mul_mm stamp of a superblock format (the per-32 bases bind no strip view: another builder signature)
def kq_mm_base_stamp(f) {
    if (f == KqFmt.k4) return stamp_of(@@enc_kq_mm_k4_c)
    if (f == KqFmt.k5) return stamp_of(@@enc_kq_mm_k5_c)
    if (f == KqFmt.k6) return stamp_of(@@enc_kq_mm_k6_c)
    if (f == KqFmt.iq4xs) return stamp_of(@@enc_kq_mm_iq4xs_c)
    if (f == KqFmt.iq3s4) return stamp_of(@@enc_kq_mm_iq3s4_c)
    if (f == KqFmt.iq4nl) return stamp_of(@@enc_kq_mm_iq4nl_c)
    if (f == KqFmt.q40) return stamp_of(@@enc_kq_mm_q40_c)
    if (f == KqFmt.nv4) return stamp_of(@@enc_kq_mm_nv4_c)
    if (f == KqFmt.k3) return stamp_of(@@enc_kq_mm_k3_c)
    if (f == KqFmt.k2) return stamp_of(@@enc_kq_mm_k2_c)
    if (f == KqFmt.iq3s) return stamp_of(@@enc_kq_mm_iq3s_c)
    if (f == KqFmt.iq3xxs) return stamp_of(@@enc_kq_mm_iq3xxs_c)
    if (f == KqFmt.iq2s) return stamp_of(@@enc_kq_mm_iq2s_c)
    if (f == KqFmt.iq2xs) return stamp_of(@@enc_kq_mm_iq2xs_c)
    return stamp_of(@@enc_kq_mm_iq2xxs_c)
}

def private kq_mm_t_kernel(f) {
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_t_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_t_c)
    if (f == KqFmt.k6) return kernel_of(@@enc_kq_mm_k6_t_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_mm_iq4xs_t_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_mm_iq3s4_t_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_mm_iq4nl_t_c)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_mm_q40_t_c)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_mm_nv4_t_c)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_mm_k3_t_c)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_mm_k2_t_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_t_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_t_c)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_mm_iq2s_t_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_mm_iq2xs_t_c)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_mm_iq2xxs_t_c)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_mm_q51_t_c)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_mm_iq4nl32_t_c)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_mm_q4032_t_c)
    return kernel_of(@@enc_kq_mm_q5032_t_c)
}

def private kq_mm_th_kernel(f) {
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_th_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_th_c)
    if (f == KqFmt.k6) return kernel_of(@@enc_kq_mm_k6_th_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_mm_iq4xs_th_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_mm_iq3s4_th_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_mm_iq4nl_th_c)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_mm_q40_th_c)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_mm_nv4_th_c)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_mm_k3_th_c)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_mm_k2_th_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_th_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_th_c)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_mm_iq2s_th_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_mm_iq2xs_th_c)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_mm_iq2xxs_th_c)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_mm_q51_th_c)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_mm_iq4nl32_th_c)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_mm_q4032_th_c)
    return kernel_of(@@enc_kq_mm_q5032_th_c)
}

def private kq_mm_th128_kernel(f) {
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_th128_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_th128_c)
    if (f == KqFmt.k6) return kernel_of(@@enc_kq_mm_k6_th128_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_mm_iq4xs_th128_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_mm_iq3s4_th128_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_mm_iq4nl_th128_c)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_mm_q40_th128_c)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_mm_nv4_th128_c)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_mm_k3_th128_c)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_mm_k2_th128_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_th128_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_th128_c)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_mm_iq2s_th128_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_mm_iq2xs_th128_c)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_mm_iq2xxs_th128_c)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_mm_q51_th128_c)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_mm_iq4nl32_th128_c)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_mm_q4032_th128_c)
    return kernel_of(@@enc_kq_mm_q5032_th128_c)
}

def private kq_mm_thdb_kernel(f) {
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_thdb_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_thdb_c)
    if (f == KqFmt.k6) return kernel_of(@@enc_kq_mm_k6_thdb_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_mm_iq4xs_thdb_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_mm_iq3s4_thdb_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_mm_iq4nl_thdb_c)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_mm_q40_thdb_c)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_mm_nv4_thdb_c)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_mm_k3_thdb_c)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_mm_k2_thdb_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_thdb_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_thdb_c)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_mm_iq2s_thdb_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_mm_iq2xs_thdb_c)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_mm_iq2xxs_thdb_c)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_mm_q51_thdb_c)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_mm_iq4nl32_thdb_c)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_mm_q4032_thdb_c)
    return kernel_of(@@enc_kq_mm_q5032_thdb_c)
}

def private kq_mm_thdb128_kernel(f) {
    if (f == KqFmt.k4) return kernel_of(@@enc_kq_mm_k4_thdb128_c)
    if (f == KqFmt.k5) return kernel_of(@@enc_kq_mm_k5_thdb128_c)
    if (f == KqFmt.k6) return kernel_of(@@enc_kq_mm_k6_thdb128_c)
    if (f == KqFmt.iq4xs) return kernel_of(@@enc_kq_mm_iq4xs_thdb128_c)
    if (f == KqFmt.iq3s4) return kernel_of(@@enc_kq_mm_iq3s4_thdb128_c)
    if (f == KqFmt.iq4nl) return kernel_of(@@enc_kq_mm_iq4nl_thdb128_c)
    if (f == KqFmt.q40) return kernel_of(@@enc_kq_mm_q40_thdb128_c)
    if (f == KqFmt.nv4) return kernel_of(@@enc_kq_mm_nv4_thdb128_c)
    if (f == KqFmt.k3) return kernel_of(@@enc_kq_mm_k3_thdb128_c)
    if (f == KqFmt.k2) return kernel_of(@@enc_kq_mm_k2_thdb128_c)
    if (f == KqFmt.iq3s) return kernel_of(@@enc_kq_mm_iq3s_thdb128_c)
    if (f == KqFmt.iq3xxs) return kernel_of(@@enc_kq_mm_iq3xxs_thdb128_c)
    if (f == KqFmt.iq2s) return kernel_of(@@enc_kq_mm_iq2s_thdb128_c)
    if (f == KqFmt.iq2xs) return kernel_of(@@enc_kq_mm_iq2xs_thdb128_c)
    if (f == KqFmt.iq2xxs) return kernel_of(@@enc_kq_mm_iq2xxs_thdb128_c)
    if (f == KqFmt.q51) return kernel_of(@@enc_kq_mm_q51_thdb128_c)
    if (f == KqFmt.iq4nl32) return kernel_of(@@enc_kq_mm_iq4nl32_thdb128_c)
    if (f == KqFmt.q4032) return kernel_of(@@enc_kq_mm_q4032_thdb128_c)
    return kernel_of(@@enc_kq_mm_q5032_thdb128_c)
}
