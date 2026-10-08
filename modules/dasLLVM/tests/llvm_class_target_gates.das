//! `with_class_target_gates` sets the tier gates to one CPU class's set for its block and hands every one of them back.
options gen2

require dastest/testing_boost public
require llvm/daslib/llvm_jit_common

struct Gates {
    avx2 : bool
    f16c : bool
    vnni256 : bool
    avx512bw : bool
    avx512vnni : bool
    avx512vbmi : bool
    vnniint8 : bool
    amx : bool
    amx_bf16 : bool
    dotprod : bool
    i8mm : bool
    fullfp16 : bool
}

def read_gates() : Gates {
    return Gates(avx2 = g_target_x64_avx2, f16c = g_target_x64_f16c, vnni256 = g_target_x64_vnni256,
        avx512bw = g_target_x64_avx512bw, avx512vnni = g_target_x64_avx512vnni,
        avx512vbmi = g_target_x64_avx512vbmi, vnniint8 = g_target_x64_vnniint8,
        amx = g_target_x64_amx, amx_bf16 = g_target_x64_amx_bf16, dotprod = g_target_arm64_dotprod,
        i8mm = g_target_arm64_i8mm, fullfp16 = g_target_arm64_fullfp16)
}

def write_gates(g : Gates) {
    g_target_x64_avx2 = g.avx2
    g_target_x64_f16c = g.f16c
    g_target_x64_vnni256 = g.vnni256
    g_target_x64_avx512bw = g.avx512bw
    g_target_x64_avx512vnni = g.avx512vnni
    g_target_x64_avx512vbmi = g.avx512vbmi
    g_target_x64_vnniint8 = g.vnniint8
    g_target_x64_amx = g.amx
    g_target_x64_amx_bf16 = g.amx_bf16
    g_target_arm64_dotprod = g.dotprod
    g_target_arm64_i8mm = g.i8mm
    g_target_arm64_fullfp16 = g.fullfp16
}

def count_set(g : Gates) : int {
    var n = 0
    for (v in [g.avx2, g.f16c, g.vnni256, g.avx512bw, g.avx512vnni, g.avx512vbmi, g.vnniint8,
            g.amx, g.amx_bf16, g.dotprod, g.i8mm, g.fullfp16]) {
        n += v ? 1 : 0
    }
    return n
}

[test]
def test_class_target_gates(t : T?) {
    t |> run("every gate set before the block reads set after it") @(t : T?) {
        let saved = read_gates()
        let is_x64 = g_target_is_x64
        let is_aarch64 = g_target_is_aarch64
        g_target_is_x64 = true
        g_target_is_aarch64 = false
        write_gates(Gates(avx2 = true, f16c = true, vnni256 = true, avx512bw = true, avx512vnni = true,
            avx512vbmi = true, vnniint8 = true, amx = true, amx_bf16 = true, dotprod = true,
            i8mm = true, fullfp16 = true))
        var inside : Gates
        with_class_target_gates("x86-avx2") $ {
            inside = read_gates()
        }
        let after = read_gates()
        g_target_is_x64 = is_x64
        g_target_is_aarch64 = is_aarch64
        write_gates(saved)
        t |> equal(count_set(after), 12, "all twelve gates read set after the block")
        t |> success(after.amx_bf16, "the amx-bf16 gate reads set after the block")
        t |> success(!inside.amx_bf16 && !inside.amx && !inside.avx512bw,
            "inside the block the x86-avx2 class has no amx and no avx512 gate")
        t |> success(inside.avx2, "inside the block the x86-avx2 class has its avx2 gate")
    }
}
