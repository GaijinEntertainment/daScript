options gen2
options indenting = 4

module llvm_cpu_class shared public

require llvm/daslib/llvm_env
require daslib/match
require daslib/strings_boost
require daslib/bitfield_trait
require daslib/bitfield_boost
require strings

//! The CPU classes a shipped defaults profile is keyed by, with the target each class compiles
//! for. A class is a feature SET every box of the class carries - the same set the minting box
//! stamped into the profile's `features` fingerprint - never a box model. `DAS_JIT_BASELINE`
//! names one, and then the target machine, the emitter's tier gates, `requires=` eligibility and
//! the profile ladder all read the class's set instead of this box's cpuid.

//! One bit per CPU feature a class, a `requires=` expression or cpuid names, in fingerprint
//! order; `cpu_features_text` spells a set the LLVM way (`sse4_2` is "sse4.2", `amx_tile` "amx-tile").
bitfield public CpuFeatures {
    avx
    avx2
    f16c
    fma
    sse4_2
    avxvnni
    avxvnniint8
    avx512f
    avx512bw
    avx512vl
    avx512vnni
    avx512vbmi
    avx512bf16
    amx_tile
    amx_int8
    amx_bf16
    amx_fp16
    neon
    dotprod
    i8mm
    bf16
    fullfp16
    sve
    lse
    simd128
}

//! The CPU classes; `cpu_class_name` spells one the way profile files, flags and logs do. An
//! arch's `*_generic` class is the arch itself with no feature beyond it - a build that knows
//! nothing about its target's CPU - and no profile or ladder names it; none is an arch with no classes.
enum public CpuClass {
    none
    x86_generic
    x86_base
    x86_avx
    x86_avx2
    x86_vnni256
    x86_vnni512
    x86_amx
    arm_generic
    arm_neon
    arm_i8mm
    arm_sve
    wasm_generic
    wasm_simd128
}

//! `x86_avx2` is "x86-avx2" - the profile key `<dir>/<name>.tune-defaults.json`; "" for none.
def public cpu_class_name(c : CpuClass) : string => c == CpuClass.none ? "" : replace("{c}", "_", "-")

//! The class `name` spells, or none for "", a generic class and a name no class carries.
def public cpu_class_of(name : string) : CpuClass {
    for (c in JIT_CPU_CLASSES) {
        return c.klass if (!cpu_class_generic(c.klass) && cpu_class_name(c.klass) == name)
    }
    return CpuClass.none
}

//! The arch a class belongs to, as the arch's generic class; none for none.
def public cpu_class_arch(c : CpuClass) : CpuClass {
    return match (c) {
        CpuClass.x86_generic | CpuClass.x86_base | CpuClass.x86_avx | CpuClass.x86_avx2 | CpuClass.x86_vnni256 | CpuClass.x86_vnni512 | CpuClass.x86_amx => CpuClass.x86_generic
        CpuClass.arm_generic | CpuClass.arm_neon | CpuClass.arm_i8mm | CpuClass.arm_sve => CpuClass.arm_generic
        CpuClass.wasm_generic | CpuClass.wasm_simd128 => CpuClass.wasm_generic
        CpuClass.none => CpuClass.none
    }
}

//! True for an arch's generic class and none: a class that names no feature.
def public cpu_class_generic(c : CpuClass) : bool => cpu_class_arch(c) == c

//! Every feature bit `c`'s arch carries; none for none.
def public cpu_arch_features(c : CpuClass) : CpuFeatures {
    return match (cpu_class_arch(c)) {
        CpuClass.x86_generic => (CpuFeatures.avx | CpuFeatures.avx2 | CpuFeatures.f16c | CpuFeatures.fma | CpuFeatures.sse4_2 | CpuFeatures.avxvnni | CpuFeatures.avxvnniint8
            | CpuFeatures.avx512f | CpuFeatures.avx512bw | CpuFeatures.avx512vl | CpuFeatures.avx512vnni | CpuFeatures.avx512vbmi | CpuFeatures.avx512bf16
            | CpuFeatures.amx_tile | CpuFeatures.amx_int8 | CpuFeatures.amx_bf16 | CpuFeatures.amx_fp16)
        CpuClass.arm_generic => CpuFeatures.neon | CpuFeatures.dotprod | CpuFeatures.i8mm | CpuFeatures.bf16 | CpuFeatures.fullfp16 | CpuFeatures.sve | CpuFeatures.lse
        CpuClass.wasm_generic => CpuFeatures.simd128
        _ => CpuFeatures(0u)
    }
}

//! The arch `name` spells - a target triple or `get_architecture_name()` - as its generic class.
def public cpu_arch_of(name : string) : CpuClass {
    if (starts_with(name, "x86_64") || starts_with(name, "amd64")) return CpuClass.x86_generic
    if (starts_with(name, "aarch64") || starts_with(name, "arm64")) return CpuClass.arm_generic
    if (starts_with(name, "wasm")) return CpuClass.wasm_generic
    return CpuClass.none
}

struct public JitCpuClass {
    klass : CpuClass
    cpu : string              // the LLVM cpu the class's target machine schedules for
    features : CpuFeatures    // the features every box of the class has
}

let public JIT_CPU_CLASSES = [
    // the cpu is the arch's bare baseline on every row: a level like x86-64-v3 implies features
    // the set does not name (bmi2, lzcnt, movbe; v4 adds avx512cd/dq), which the backend would emit
    // and cpuid is never asked for - the set alone is what the box is tested for and what LLVM enables
    JitCpuClass(klass = CpuClass.x86_generic, cpu = "", features = CpuFeatures(0u)),
    JitCpuClass(klass = CpuClass.x86_base, cpu = "x86-64", features = CpuFeatures.sse4_2),
    JitCpuClass(klass = CpuClass.x86_avx, cpu = "x86-64", features = CpuFeatures.avx | CpuFeatures.f16c | CpuFeatures.sse4_2),
    JitCpuClass(klass = CpuClass.x86_avx2, cpu = "x86-64", features = CpuFeatures.avx2 | CpuFeatures.f16c | CpuFeatures.fma | CpuFeatures.sse4_2),
    JitCpuClass(klass = CpuClass.x86_vnni256, cpu = "x86-64", features = CpuFeatures.avx2 | CpuFeatures.f16c | CpuFeatures.fma | CpuFeatures.sse4_2 | CpuFeatures.avxvnni),
    JitCpuClass(klass = CpuClass.x86_vnni512, cpu = "x86-64",
        features = CpuFeatures.avx2 | CpuFeatures.f16c | CpuFeatures.fma | CpuFeatures.sse4_2 | CpuFeatures.avx512f | CpuFeatures.avx512bw | CpuFeatures.avx512vl | CpuFeatures.avx512vnni | CpuFeatures.avx512vbmi),
    JitCpuClass(klass = CpuClass.x86_amx, cpu = "x86-64",
        features = (CpuFeatures.avx2 | CpuFeatures.f16c | CpuFeatures.fma | CpuFeatures.sse4_2 | CpuFeatures.avxvnni | CpuFeatures.avx512f | CpuFeatures.avx512bw | CpuFeatures.avx512vl | CpuFeatures.avx512vnni | CpuFeatures.avx512vbmi
            | CpuFeatures.avx512bf16 | CpuFeatures.amx_tile | CpuFeatures.amx_int8 | CpuFeatures.amx_bf16)),
    JitCpuClass(klass = CpuClass.arm_generic, cpu = "", features = CpuFeatures(0u)),
    JitCpuClass(klass = CpuClass.arm_neon, cpu = "generic", features = CpuFeatures.neon | CpuFeatures.dotprod | CpuFeatures.fullfp16 | CpuFeatures.lse),
    JitCpuClass(klass = CpuClass.arm_i8mm, cpu = "generic",
        features = CpuFeatures.neon | CpuFeatures.dotprod | CpuFeatures.fullfp16 | CpuFeatures.lse | CpuFeatures.i8mm | CpuFeatures.bf16),
    JitCpuClass(klass = CpuClass.arm_sve, cpu = "generic",
        features = CpuFeatures.neon | CpuFeatures.dotprod | CpuFeatures.fullfp16 | CpuFeatures.lse | CpuFeatures.i8mm | CpuFeatures.bf16 | CpuFeatures.sve),
    JitCpuClass(klass = CpuClass.wasm_generic, cpu = "", features = CpuFeatures(0u)),
    JitCpuClass(klass = CpuClass.wasm_simd128, cpu = "generic", features = CpuFeatures.simd128)
]

//! The adoption ladder of `c`'s arch, most capable class first; empty for an arch with no classes.
def public jit_cpu_class_ladder(c : CpuClass) : array<CpuClass> {
    return <- match (cpu_class_arch(c)) {
        CpuClass.x86_generic => [CpuClass.x86_amx, CpuClass.x86_vnni512, CpuClass.x86_vnni256, CpuClass.x86_avx2, CpuClass.x86_avx, CpuClass.x86_base]
        CpuClass.arm_generic => [CpuClass.arm_sve, CpuClass.arm_i8mm, CpuClass.arm_neon]
        CpuClass.wasm_generic => [CpuClass.wasm_simd128]
        _ => array<CpuClass>()
    }
}

//! The class's row; none's row carries no features.
def public jit_cpu_class_row(klass : CpuClass) : JitCpuClass {
    for (c in JIT_CPU_CLASSES) {
        return c if (c.klass == klass)
    }
    return JitCpuClass()
}

def public jit_cpu_class_names() : string {
    var names <- [for (c in JIT_CPU_CLASSES); cpu_class_name(c.klass); where !cpu_class_generic(c.klass)]
    let joined = join(names, ", ")
    delete names
    return joined
}

def private feature_spelling(bit_name : string) : string =>
    bit_name == "sse4_2" ? "sse4.2" : replace(bit_name, "_", "-")

//! The set the LLVM names in `text` spell, split on `;` and `,`; a name no bit carries is dropped.
def public cpu_features_of(text : string) : CpuFeatures {
    var set : CpuFeatures
    let parts <- split(replace(text, ",", ";"), ";")
    for (name, bit in each_bit_name(type<CpuFeatures>), count()) {
        set[bit] = parts |> has_value(feature_spelling(name))
    }
    return set
}

//! The set's LLVM names joined by `sep`, each after `prefix`, in bit order.
def public cpu_features_text(set : CpuFeatures; sep : string; prefix : string = "") : string {
    return build_string() $(var writer) {
        var first = true
        for (name, bit in each_bit_name(type<CpuFeatures>), count()) {
            continue if (!set[bit])
            writer |> write(first ? "" : sep)
            writer |> write(prefix)
            writer |> write(feature_spelling(name))
            first = false
        }
    }
}

//! The running CPU's set, read off `cpu_supports`.
def public cpu_features_native() : CpuFeatures {
    var set : CpuFeatures
    for (name, bit in each_bit_name(type<CpuFeatures>), count()) {
        set[bit] = cpu_supports(feature_spelling(name))
    }
    return set
}

//! True when `set` passes a `requires=` expression: a `,`-separated AND of `|`-separated LLVM
//! names, `!` ahead of a name holding where the feature is absent. An empty expression passes;
//! a name no bit carries is absent from every set.
def public cpu_features_pass(set : CpuFeatures; requires : string) : bool {
    for (conj in split(requires, ",")) {
        continue if (empty(conj))
        var anyOk = false
        for (alt in split(conj, "|")) {
            let absent = starts_with(alt, "!")
            let name = absent ? slice(alt, 1) : alt
            let bit = cpu_features_of(name)
            let has = uint(bit) != 0u && (set & bit) == bit
            anyOk ||= !empty(name) && has != absent
        }
        return false if (!anyOk)
    }
    return true
}

//! The x64 instruction tiers the emitter and the kernel grids select on.
bitfield public X64Tiers {
    vnni256
    avx512bw
    avx512vnni
    avx512vbmi
    vnniint8
    amx
    amx_bf16
}

//! The tiers a feature set may emit.
[arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#x64-tier-gates")]
def public x64_tiers(f : CpuFeatures) : X64Tiers {
    var t : X64Tiers
    t.vnni256 = f.avx2 && (f.avxvnni || (f.avx512vnni && f.avx512vl))
    t.avx512bw = f.avx2 && f.avx512f && f.avx512bw
    t.avx512vnni = t.avx512bw && f.avx512vnni
    t.avx512vbmi = t.avx512bw && f.avx512vbmi
    t.vnniint8 = f.avx2 && f.avxvnniint8
    t.amx = f.amx_tile && f.amx_int8
    t.amx_bf16 = t.avx512bw && f.avx512bf16 && f.amx_tile && f.amx_bf16
    return t
}

//! What a target may emit for: the named class's set, else this box's cpuid where the box is the
//! target and nothing where it is not, with `forced` added either way - all of it cut to the bits
//! the arch carries (none's arch is this box's).
def public target_features(klass : CpuClass; on_box : bool; forced : CpuFeatures) : CpuFeatures {
    let known = cpu_class_generic(klass) ? (on_box ? cpu_features_native() : CpuFeatures(0u)) : jit_cpu_class_row(klass).features
    let arch = klass == CpuClass.none ? cpu_arch_of(get_architecture_name()) : klass
    return (known | forced) & cpu_arch_features(arch)
}

//! True when one row of a `[tune]` family runs on `set`: `rows` holds each row's `requires=`,
//! "" for a row with none.
def public any_row_runs(rows : array<string>; set : CpuFeatures) : bool {
    for (row in rows) {
        return true if (cpu_features_pass(set, row))
    }
    return false
}

//! The baseline class this process builds for - `DAS_JIT_BASELINE` - or none when the build
//! targets the running box.
def public jit_baseline_class() : CpuClass => cpu_class_of(strip(g_env_jit.jit_baseline))

//! The class a cross compile builds for: a wasm64 triple is wasm-simd128, another triple the
//! baseline's class; none when the build is not a cross one.
[arch(at = "../ARCHITECTURE_CPU_CLASSES.md#cross-class")]
def public jit_cross_class() : CpuClass {
    let triple = get_target_triple()
    return CpuClass.none if (empty(triple))
    return starts_with(triple, "wasm64") ? CpuClass.wasm_simd128 : jit_baseline_class()
}

//! The class this compile builds for; none when it builds for the box
def public jit_target_class() : CpuClass {
    let baseline = jit_baseline_class()
    return baseline == CpuClass.none ? jit_cross_class() : baseline
}

//! "" when the baseline is unset or names a class of `arch` (an arch's generic class); else the refusal text.
[arch(at = "../ARCHITECTURE_CPU_CLASSES.md#cpu-classes")]
def public jit_baseline_error(arch : CpuClass) : string {
    let baseline = strip(g_env_jit.jit_baseline)
    if (empty(baseline)) {
        return ""
    }
    let row = jit_cpu_class_row(cpu_class_of(baseline))
    if (row.klass == CpuClass.none) {
        return "DAS_JIT_BASELINE={baseline} names no CPU class - the classes are {jit_cpu_class_names()}"
    }
    if (cpu_class_arch(row.klass) != arch) {
        return "DAS_JIT_BASELINE={baseline} is a {cpu_class_name(cpu_class_arch(row.klass))} class, but the build targets {cpu_class_name(arch)} - a baseline never crosses architectures (use --jit-target for that)"
    }
    return ""
}
