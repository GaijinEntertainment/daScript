options gen2

require daslib/fio
require strings

//! Generates the per-type conformance harness under tests/type_lattice/.
//! Each lattice type gets one dastest file exercising: sizeof/alignof, zero-init,
//! default<>, ctor+equality, copy semantics, pass/return by value, v[i] read/write,
//! xyzw swizzles, struct field layout, array element, archive round-trip, print,
//! and (where the type has closed operators) arithmetic.
//! Run: daslang utils/dasgen/gen_type_conformance.das
//! The generated files are checked in; regenerate whenever the spec table changes.

struct TypeSpec {
    name : string           // das type name: "float2", "int8"
    family : string         // vector family prefix ("float") — names swizzle result types
    arity : int             // 1 for scalars
    kind : string           // ctor-argument literal spelling: "float" | "int" | "uint"
    ekind : string          // element literal spelling — differs for fp16 ("half": 1.h)
    has_arith : bool        // closed +,-,*,/ operators exist (div emitted for float kind only)
    has_bitops : bool       // the int-vector bit surface exists: << >> & | ^ + compound assigns
    size_bytes : int
    align_bytes : int
}

let LANE_LETTERS = fixed_array("x", "y", "z", "w")

def lit(kind : string; v : int) : string {
    if (kind == "float") {
        return "{v}."
    }
    if (kind == "half") {
        return "{v}h"
    }
    if (kind == "short") {
        return "int16({v})"
    }
    if (kind == "byte") {
        return "int8({v})"
    }
    if (kind == "ushort") {
        return "uint16({v})"
    }
    if (kind == "ubyte") {
        return "uint8({v})"
    }
    return kind == "uint" ? "{v}u" : "{v}"
}

// ctor expression with lane values produced by the block: lane(i) for i in 0..arity
def ctor_gen(spec : TypeSpec; lane : block<(i : int) : int>) : string {
    if (spec.arity == 1) {
        return "{spec.name}({lit(spec.kind, invoke(lane, 0))})"
    }
    return build_string() $(var writer) {
        writer |> write(spec.name)
        writer |> write("(")
        for (i in range(spec.arity)) {
            if (i != 0) {
                writer |> write(", ")
            }
            writer |> write(lit(spec.kind, invoke(lane, i)))
        }
        writer |> write(")")
    }
}

def ctor(spec : TypeSpec; base : int) : string {
    return ctor_gen(spec) $(i) => base + i
}

def wrap_size(spec : TypeSpec) : int {
    let a = spec.align_bytes
    let raw = a + spec.size_bytes    // pad byte at 0, v aligned up to a (a >= 1)
    return ((raw + a - 1) / a) * a
}

def emit_swizzle_section(var writer; spec : TypeSpec) {
    let n = spec.arity
    writer |> write("    t |> run(\"swizzle\") @(t : T?) \{\n")
    writer |> write("        var a = {ctor(spec, 1)}\n")
    writer |> write("        t |> equal(a.x, {lit(spec.ekind, 1)})\n")
    let last = LANE_LETTERS[n - 1]
    writer |> write("        t |> equal(a.{last}, {lit(spec.ekind, n)})\n")
    // full-reverse read (non-sequential mask → value swizzle)
    let rev = build_string() $(var rw) {
        for (i in range(n)) {
            rw |> write(LANE_LETTERS[n - 1 - i])
        }
    }
    writer |> write("        t |> success(a.{rev} == {ctor_gen(spec) $(i) => n - i}, \"reverse read\")\n")
    if (n > 2) {
        // prefix-pair read produces the 2-arity sibling
        var pair = spec
        pair.arity = 2
        pair.name = "{spec.family}2"
        writer |> write("        t |> success(a.xy == {ctor(pair, 1)}, \"pair read\")\n")
    }
    // single-lane write (ref path via field offset)
    writer |> write("        a.{last} = {lit(spec.ekind, 9)}\n")
    writer |> write("        t |> equal(a.{last}, {lit(spec.ekind, 9)})\n")
    // sequential multi-lane write (ref swizzle)
    var pw = spec
    pw.arity = 2
    pw.name = "{spec.family}2"
    writer |> write("        a.xy = {ctor(pw, 7)}\n")
    writer |> write("        t |> equal(a.x, {lit(spec.ekind, 7)})\n")
    writer |> write("        t |> equal(a.y, {lit(spec.ekind, 8)})\n")
    writer |> write("    \}\n")
}

let HEX_DIGITS = fixed_array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f")

def emit_s_swizzle_section(var writer; spec : TypeSpec) {
    let n = spec.arity
    writer |> write("    t |> run(\"s-namespace swizzle\") @(t : T?) \{\n")
    writer |> write("        var a = {ctor(spec, 1)}\n")
    let lastHex = HEX_DIGITS[n - 1]
    writer |> write("        t |> equal(a.s{lastHex}, {lit(spec.ekind, n)})\n")
    // reverse pair via .s digits
    var pair = spec
    pair.arity = 2
    pair.name = "{spec.family}2"
    writer |> write("        t |> success(a.s{lastHex}0 == {ctor_gen(pair) $(i) => i == 0 ? n : 1}, \"pair reorder\")\n")
    if (n >= 8) {
        var halfspec = spec
        halfspec.arity = n / 2
        halfspec.name = "{spec.family}{n / 2}"
        writer |> write("        t |> success(a.lo == {ctor(halfspec, 1)}, \".lo\")\n")
        writer |> write("        t |> success(a.hi == {ctor(halfspec, 1 + n / 2)}, \".hi\")\n")
    }
    writer |> write("        a.s0 = {lit(spec.ekind, 9)}\n")
    writer |> write("        t |> equal(a.s0, {lit(spec.ekind, 9)})\n")
    writer |> write("    \}\n")
}

def emit_arith_section(var writer; spec : TypeSpec) {
    // lane values stay small integers so every op below is EXACT in fp16 — the checks
    // pin semantics across interp/jit/aot without rounding sensitivity
    writer |> write("    t |> run(\"arithmetic\") @(t : T?) \{\n")
    writer |> write("        let a = {ctor(spec, 1)}\n")
    writer |> write("        let b = {ctor_gen(spec) $(i) => (1 + i) * 2}\n")
    writer |> write("        t |> success(a + b == {ctor_gen(spec) $(i) => (1 + i) * 3}, \"add\")\n")
    writer |> write("        t |> success(b - a == {ctor(spec, 1)}, \"sub\")\n")
    writer |> write("        t |> success(a * b == {ctor_gen(spec) $(i) => (1 + i) * (1 + i) * 2}, \"mul\")\n")
    if (spec.kind == "float") {
        writer |> write("        t |> success(b / a == {ctor_gen(spec) $(_i) => 2}, \"div\")\n")
    }
    writer |> write("        var c = a\n")
    writer |> write("        c += b\n")
    writer |> write("        t |> success(c == a + b, \"add assign\")\n")
    writer |> write("        c -= b\n")
    writer |> write("        t |> success(c == a, \"sub assign\")\n")
    if (spec.kind != "uint") {
        writer |> write("        t |> success(-a == {ctor_gen(spec) $(i) => -(1 + i)}, \"unary minus\")\n")
    }
    if (spec.arity > 1) {
        let s = lit(spec.ekind, 2)
        writer |> write("        t |> success(a * {s} == a + a, \"vec mul scalar\")\n")
        writer |> write("        t |> success({s} * a == a + a, \"scalar mul vec\")\n")
        if (spec.kind == "float") {
            writer |> write("        t |> success((a + a) / {s} == a, \"vec div scalar\")\n")
        }
        writer |> write("        c = a\n")
        writer |> write("        c *= {s}\n")
        writer |> write("        t |> success(c == a + a, \"mul assign scalar\")\n")
    }
    writer |> write("    \}\n")
}

// wrap a das-int lane value to `bits`-wide two's complement — the exact value a lane of that
// width holds after a wrapping op (shl, bitwise on negatives)
def wrap_lane(v : int; bits : int; signed_kind : bool) : int {
    if (bits >= 32) {
        return v
    }
    var w = v & ((1 << bits) - 1)
    if (signed_kind && w >= (1 << (bits - 1))) {
        w -= 1 << bits
    }
    return w
}

// the int-vector bit surface — >> (signed lanes arithmetic, unsigned logical), << (lanes wrap),
// & | ^, and the compound assigns; shift counts mask to the LANE width. Expected lanes are
// computed here in das int and wrapped to lane width.
def emit_bitops_section(var writer; spec : TypeSpec) {
    let bits = spec.size_bytes * 8 / spec.arity
    let signed_kind = spec.kind == "int"
    writer |> write("    t |> run(\"shift right\") @(t : T?) \{\n")
    if (signed_kind) {
        writer |> write("        let a = {ctor_gen(spec) $(i) => i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7}\n")
        writer |> write("        t |> success(a >> 2 == {ctor_gen(spec) $(i) => (i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7) >> 2}, \"arithmetic shift\")\n")
    } else {
        writer |> write("        let a = {ctor_gen(spec) $(i) => 200 + i * 3}\n")
        writer |> write("        t |> success(a >> 2 == {ctor_gen(spec) $(i) => (200 + i * 3) >> 2}, \"logical shift\")\n")
    }
    writer |> write("        t |> success(a >> {bits + 2} == a >> 2, \"count masks to lane width\")\n")
    writer |> write("        t |> success(a >> 0 == a, \"shift by zero\")\n")
    writer |> write("        var c = a\n")
    writer |> write("        c >>= 2\n")
    writer |> write("        t |> success(c == a >> 2, \"shift right assign\")\n")
    writer |> write("    \}\n")
    writer |> write("    t |> run(\"shift left\") @(t : T?) \{\n")
    if (signed_kind) {
        writer |> write("        let a = {ctor_gen(spec) $(i) => i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7}\n")
        writer |> write("        t |> success(a << 2 == {ctor_gen(spec) $(i) => wrap_lane((i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7) << 2, bits, true)}, \"lanes wrap\")\n")
    } else {
        writer |> write("        let a = {ctor_gen(spec) $(i) => 200 + i * 3}\n")
        writer |> write("        t |> success(a << 2 == {ctor_gen(spec) $(i) => wrap_lane((200 + i * 3) << 2, bits, false)}, \"lanes wrap\")\n")
    }
    writer |> write("        t |> success(a << {bits + 2} == a << 2, \"count masks to lane width\")\n")
    writer |> write("        var c = a\n")
    writer |> write("        c <<= 2\n")
    writer |> write("        t |> success(c == a << 2, \"shift left assign\")\n")
    writer |> write("    \}\n")
    writer |> write("    t |> run(\"bitwise\") @(t : T?) \{\n")
    if (signed_kind) {
        writer |> write("        let a = {ctor_gen(spec) $(i) => i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7}\n")
    } else {
        writer |> write("        let a = {ctor_gen(spec) $(i) => 200 + i * 3}\n")
    }
    writer |> write("        let b = {ctor_gen(spec) $(i) => 45 + i * 5}\n")
    let av <- [for (i in range(spec.arity)); spec.kind == "int" ? (i % 2 == 0 ? -(19 + i * 7) : 19 + i * 7) : 200 + i * 3]
    writer |> write("        t |> success((a & b) == {ctor_gen(spec) $(i) => wrap_lane(av[i] & (45 + i * 5), bits, signed_kind)}, \"and\")\n")
    writer |> write("        t |> success((a | b) == {ctor_gen(spec) $(i) => wrap_lane(av[i] | (45 + i * 5), bits, signed_kind)}, \"or\")\n")
    writer |> write("        t |> success((a ^ b) == {ctor_gen(spec) $(i) => wrap_lane(av[i] ^ (45 + i * 5), bits, signed_kind)}, \"xor\")\n")
    writer |> write("        var c = a\n")
    writer |> write("        c &= b\n")
    writer |> write("        t |> success(c == (a & b), \"and assign\")\n")
    writer |> write("        c = a\n")
    writer |> write("        c |= b\n")
    writer |> write("        t |> success(c == (a | b), \"or assign\")\n")
    writer |> write("        c = a\n")
    writer |> write("        c ^= b\n")
    writer |> write("        t |> success(c == (a ^ b), \"xor assign\")\n")
    writer |> write("    \}\n")
}

// lane-matched converts + saturating narrows — mirrors the das_sv_cvt/das_sv_cvt_sat
// registration matrix (module_builtin_vector_ctor.cpp)
def emit_convert_section(var writer; spec : TypeSpec) {
    let n = spec.arity
    assume fam = spec.family
    let isIntNarrow = (fam == "short" || fam == "byte" || fam == "ushort" || fam == "ubyte") && n <= 4
    let isByte8 = (fam == "byte" || fam == "ubyte") && n == 8
    let isShort8 = (fam == "short" || fam == "ushort") && n == 8
    let isHalfVec = fam == "half" && n <= 4
    if (!(isIntNarrow || isByte8 || isShort8 || isHalfVec || fam == "half" && n == 8 || spec.name == "float16")) {
        return
    }
    writer |> write("    t |> run(\"converts\") @(t : T?) \{\n")
    if (spec.name == "float16") {
        writer |> write("        let a = 1.5h\n")
        writer |> write("        t |> equal(float(a), 1.5)\n")
        writer |> write("        t |> success(float16(1.5) == a, \"narrow round trip\")\n")
    } elif (isHalfVec) {
        writer |> write("        let a = {ctor(spec, 1)}\n")
        var fspec = spec
        fspec.name = "float{n}"
        fspec.kind = "float"
        writer |> write("        let f = float{n}(a)\n")
        writer |> write("        t |> success(f == {ctor(fspec, 1)}, \"widen to float\")\n")
        writer |> write("        t |> success({spec.name}(f) == a, \"narrow round trip\")\n")
    } elif (fam == "half") {   // half8 <-> 2x float4 pack/unpack
        writer |> write("        let lo = float4(1., 2., 3., 4.)\n")
        writer |> write("        let hi = float4(5., 6., 7., 8.)\n")
        writer |> write("        let p = half8(lo, hi)\n")
        writer |> write("        t |> success(p == {ctor(spec, 1)}, \"pack\")\n")
        writer |> write("        t |> success(half8_lo(p) == lo, \"unpack lo\")\n")
        writer |> write("        t |> success(half8_hi(p) == hi, \"unpack hi\")\n")
    } else {
        let signed_ = fam == "short" || fam == "byte"
        var src = spec
        src.kind = signed_ ? "int" : "uint"
        var big = 100000
        if (isByte8) {
            src.name = signed_ ? "short8" : "ushort8"   // byte8/ubyte8 narrow from the 16-bit 8-lane form
            big = 30000
        } elif (isShort8) {
            src.name = signed_ ? "byte8" : "ubyte8"     // short8/ushort8 only WIDEN (from the 8-bit form)
        } else {
            src.name = (signed_ ? "int" : "uint") + "{n}"
        }
        if (isShort8) {
            writer |> write("        t |> success({spec.name}({ctor(src, 1)}) == {ctor(spec, 1)}, \"widen from 8-bit\")\n")
        } else {
            // widen round trip: lattice -> 32/16-bit -> back (values in range, C-truncation exact)
            writer |> write("        let a = {ctor(spec, 1)}\n")
            writer |> write("        let w = {src.name}(a)\n")
            writer |> write("        t |> success(w == {ctor(src, 1)}, \"widen\")\n")
            writer |> write("        t |> success({spec.name}(w) == a, \"narrow round trip\")\n")
            // saturating narrow: lane 0 clamps high, lane 1 clamps low (signed), tail lanes pass through
            var lim = 0
            if (fam == "short") {
                lim = 32767
            } elif (fam == "byte") {
                lim = 127
            } elif (fam == "ushort") {
                lim = 65535
            } else {
                lim = 255
            }
            let limv = lim
            let bigv = big
            if (signed_) {
                writer |> write("        let s = {spec.name}_sat({ctor_gen(src) $(i) => i == 0 ? bigv : (i == 1 ? -bigv : i)})\n")
                writer |> write("        t |> success(s == {ctor_gen(spec) $(i) => i == 0 ? limv : (i == 1 ? -limv - 1 : i)}, \"sat clamp\")\n")
            } else {
                writer |> write("        let s = {spec.name}_sat({ctor_gen(src) $(i) => i == 0 ? bigv : i})\n")
                writer |> write("        t |> success(s == {ctor_gen(spec) $(i) => i == 0 ? limv : i}, \"sat clamp\")\n")
            }
        }
    }
    writer |> write("    \}\n")
}

def emit_test_file(spec : TypeSpec) {  // nolint:STYLE038 — flat emitter, one write per generated line; the conditional sections are already helpers
    let t0 = ref_time_ticks()
    let path = get_das_root() + "/tests/type_lattice/test_{spec.name}.das"
    to_log(LOG_INFO, "[gen_type_conformance] START {path}\n")
    fopen(path, "wb") $(f) {
        let body = build_string() $(var writer) {
            writer |> write("// GENERATED by utils/dasgen/gen_type_conformance.das — DO NOT EDIT\n")
            writer |> write("// regen: daslang utils/dasgen/gen_type_conformance.das\n")
            writer |> write("options gen2\n")
            writer |> write("require dastest/testing_boost public\n")
            writer |> write("require daslib/archive\n")
            writer |> write("\n")
            writer |> write("struct Wrap_{spec.name} \{\n")
            writer |> write("    pad : uint8\n")
            writer |> write("    v : {spec.name}\n")
            writer |> write("\}\n")
            writer |> write("\n")
            writer |> write("def roundtrip_{spec.name}(v : {spec.name}) : {spec.name} => v\n")
            writer |> write("\n")
            writer |> write("[test]\n")
            writer |> write("def test_{spec.name}_conformance(t : T?) \{\n")
            writer |> write("    t |> run(\"sizeof and alignof\") @(t : T?) \{\n")
            writer |> write("        t |> equal(typeinfo sizeof(type<{spec.name}>), {spec.size_bytes})\n")
            writer |> write("        t |> equal(typeinfo alignof(type<{spec.name}>), {spec.align_bytes})\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"zero init and default\") @(t : T?) \{\n")
            writer |> write("        let z : {spec.name}\n")
            writer |> write("        t |> success(z == {ctor_gen(spec) $(_i) => 0}, \"zero init\")\n")
            writer |> write("        t |> success(default<{spec.name}> == {ctor_gen(spec) $(_i) => 0}, \"default<>\")\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"ctor and equality\") @(t : T?) \{\n")
            writer |> write("        let a = {ctor(spec, 1)}\n")
            writer |> write("        let b = {ctor(spec, 1)}\n")
            writer |> write("        let c = {ctor(spec, 2)}\n")
            writer |> write("        t |> success(a == b, \"a == b\")\n")
            writer |> write("        t |> success(a != c, \"a != c\")\n")
            if (spec.arity > 1) {
                writer |> write("        t |> success({spec.name}({lit(spec.kind, 3)}) == {ctor_gen(spec) $(_i) => 3}, \"splat ctor\")\n")
            }
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"copy is value copy\") @(t : T?) \{\n")
            writer |> write("        let a = {ctor(spec, 1)}\n")
            writer |> write("        var c = a\n")
            writer |> write("        c = {ctor(spec, 5)}\n")
            writer |> write("        t |> success(a == {ctor(spec, 1)}, \"source unchanged\")\n")
            writer |> write("        t |> success(c == {ctor(spec, 5)}, \"copy reassigned\")\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"pass and return by value\") @(t : T?) \{\n")
            writer |> write("        let a = {ctor(spec, 1)}\n")
            writer |> write("        t |> success(roundtrip_{spec.name}(a) == a, \"roundtrip\")\n")
            writer |> write("    \}\n")
            if (spec.arity > 1) {
                writer |> write("    t |> run(\"index read and write\") @(t : T?) \{\n")
                writer |> write("        var a = {ctor(spec, 1)}\n")
                for (i in range(spec.arity)) {
                    writer |> write("        t |> equal(a[{i}], {lit(spec.ekind, 1 + i)})\n")
                }
                writer |> write("        a[{spec.arity - 1}] = {lit(spec.ekind, 9)}\n")
                writer |> write("        t |> equal(a[{spec.arity - 1}], {lit(spec.ekind, 9)})\n")
                writer |> write("    \}\n")
                if (spec.arity <= 4) {
                    emit_swizzle_section(writer, spec)
                }
                emit_s_swizzle_section(writer, spec)
            }
            writer |> write("    t |> run(\"struct field and layout\") @(t : T?) \{\n")
            writer |> write("        var w = Wrap_{spec.name}(pad = uint8(1), v = {ctor(spec, 1)})\n")
            writer |> write("        t |> success(w.v == {ctor(spec, 1)}, \"field read\")\n")
            writer |> write("        w.v = {ctor(spec, 5)}\n")
            writer |> write("        t |> success(w.v == {ctor(spec, 5)}, \"field write\")\n")
            writer |> write("        t |> equal(typeinfo sizeof(type<Wrap_{spec.name}>), {wrap_size(spec)})\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"array element\") @(t : T?) \{\n")
            writer |> write("        var arr : array<{spec.name}>\n")
            writer |> write("        arr |> push({ctor(spec, 1)})\n")
            writer |> write("        arr |> push({ctor(spec, 5)})\n")
            writer |> write("        t |> equal(length(arr), 2)\n")
            writer |> write("        t |> success(arr[0] == {ctor(spec, 1)}, \"array read 0\")\n")
            writer |> write("        t |> success(arr[1] == {ctor(spec, 5)}, \"array read 1\")\n")
            writer |> write("        delete arr\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"archive round trip\") @(t : T?) \{\n")
            writer |> write("        var a = {ctor(spec, 1)}\n")
            writer |> write("        var data <- mem_archive_save(a)\n")
            writer |> write("        var back : {spec.name}\n")
            writer |> write("        data |> mem_archive_load(back)\n")
            writer |> write("        delete data\n")
            writer |> write("        t |> success(back == a, \"mem archive\")\n")
            writer |> write("    \}\n")
            writer |> write("    t |> run(\"print\") @(t : T?) \{\n")
            writer |> write("        let a = {ctor(spec, 1)}\n")
            writer |> write("        let s = \"\{a\}\"\n")
            writer |> write("        t |> success(!empty(s), \"interpolates\")\n")
            writer |> write("    \}\n")
            if (spec.has_arith) {
                emit_arith_section(writer, spec)
            }
            if (spec.has_bitops) {
                emit_bitops_section(writer, spec)
            }
            emit_convert_section(writer, spec)
            writer |> write("\}\n")
        }
        f |> fwrite(body)
    }
    to_log(LOG_INFO, "[gen_type_conformance] DONE {path} ({get_time_usec(t0)}us)\n")
}

[export]
def main {
    mkdir(get_das_root() + "/tests/type_lattice")
    var specs <- [
        // controls — existing types proving the harness itself
        TypeSpec(name = "float2", family = "float", arity = 2, kind = "float", ekind = "float", has_arith = true, size_bytes = 8, align_bytes = 4),
        TypeSpec(name = "float3", family = "float", arity = 3, kind = "float", ekind = "float", has_arith = true, size_bytes = 12, align_bytes = 4),
        TypeSpec(name = "float4", family = "float", arity = 4, kind = "float", ekind = "float", has_arith = true, size_bytes = 16, align_bytes = 4),
        TypeSpec(name = "int2", family = "int", arity = 2, kind = "int", ekind = "int", has_arith = true, has_bitops = true, size_bytes = 8, align_bytes = 4),
        TypeSpec(name = "uint4", family = "uint", arity = 4, kind = "uint", ekind = "uint", has_arith = true, has_bitops = true, size_bytes = 16, align_bytes = 4),
        TypeSpec(name = "int8", family = "int8", arity = 1, kind = "int", ekind = "int", has_arith = false, size_bytes = 1, align_bytes = 1),
        TypeSpec(name = "uint16", family = "uint16", arity = 1, kind = "uint", ekind = "uint", has_arith = false, size_bytes = 2, align_bytes = 2),
        // the 16/8-bit lattice — fp16 family has closed arithmetic; int families are storage
        TypeSpec(name = "float16", family = "float16", arity = 1, kind = "float", ekind = "half", has_arith = true, size_bytes = 2, align_bytes = 2),
        TypeSpec(name = "half2", family = "half", arity = 2, kind = "float", ekind = "half", has_arith = true, size_bytes = 4, align_bytes = 2),
        TypeSpec(name = "half3", family = "half", arity = 3, kind = "float", ekind = "half", has_arith = true, size_bytes = 6, align_bytes = 2),
        TypeSpec(name = "half4", family = "half", arity = 4, kind = "float", ekind = "half", has_arith = true, size_bytes = 8, align_bytes = 2),
        TypeSpec(name = "half8", family = "half", arity = 8, kind = "float", ekind = "half", has_arith = true, size_bytes = 16, align_bytes = 2),
        TypeSpec(name = "short2", family = "short", arity = 2, kind = "int", ekind = "short", has_arith = false, has_bitops = true, size_bytes = 4, align_bytes = 2),
        TypeSpec(name = "short3", family = "short", arity = 3, kind = "int", ekind = "short", has_arith = false, has_bitops = true, size_bytes = 6, align_bytes = 2),
        TypeSpec(name = "short4", family = "short", arity = 4, kind = "int", ekind = "short", has_arith = false, has_bitops = true, size_bytes = 8, align_bytes = 2),
        TypeSpec(name = "short8", family = "short", arity = 8, kind = "int", ekind = "short", has_arith = false, has_bitops = true, size_bytes = 16, align_bytes = 2),
        TypeSpec(name = "ushort2", family = "ushort", arity = 2, kind = "uint", ekind = "ushort", has_arith = false, has_bitops = true, size_bytes = 4, align_bytes = 2),
        TypeSpec(name = "ushort3", family = "ushort", arity = 3, kind = "uint", ekind = "ushort", has_arith = false, has_bitops = true, size_bytes = 6, align_bytes = 2),
        TypeSpec(name = "ushort4", family = "ushort", arity = 4, kind = "uint", ekind = "ushort", has_arith = false, has_bitops = true, size_bytes = 8, align_bytes = 2),
        TypeSpec(name = "ushort8", family = "ushort", arity = 8, kind = "uint", ekind = "ushort", has_arith = false, has_bitops = true, size_bytes = 16, align_bytes = 2),
        TypeSpec(name = "byte2", family = "byte", arity = 2, kind = "int", ekind = "byte", has_arith = false, has_bitops = true, size_bytes = 2, align_bytes = 1),
        TypeSpec(name = "byte3", family = "byte", arity = 3, kind = "int", ekind = "byte", has_arith = false, has_bitops = true, size_bytes = 3, align_bytes = 1),
        TypeSpec(name = "byte4", family = "byte", arity = 4, kind = "int", ekind = "byte", has_arith = false, has_bitops = true, size_bytes = 4, align_bytes = 1),
        TypeSpec(name = "byte8", family = "byte", arity = 8, kind = "int", ekind = "byte", has_arith = false, has_bitops = true, size_bytes = 8, align_bytes = 1),
        TypeSpec(name = "byte16", family = "byte", arity = 16, kind = "int", ekind = "byte", has_arith = false, has_bitops = true, size_bytes = 16, align_bytes = 1),
        TypeSpec(name = "ubyte2", family = "ubyte", arity = 2, kind = "uint", ekind = "ubyte", has_arith = false, has_bitops = true, size_bytes = 2, align_bytes = 1),
        TypeSpec(name = "ubyte3", family = "ubyte", arity = 3, kind = "uint", ekind = "ubyte", has_arith = false, has_bitops = true, size_bytes = 3, align_bytes = 1),
        TypeSpec(name = "ubyte4", family = "ubyte", arity = 4, kind = "uint", ekind = "ubyte", has_arith = false, has_bitops = true, size_bytes = 4, align_bytes = 1),
        TypeSpec(name = "ubyte8", family = "ubyte", arity = 8, kind = "uint", ekind = "ubyte", has_arith = false, has_bitops = true, size_bytes = 8, align_bytes = 1),
        TypeSpec(name = "ubyte16", family = "ubyte", arity = 16, kind = "uint", ekind = "ubyte", has_arith = false, has_bitops = true, size_bytes = 16, align_bytes = 1)
    ]
    for (spec in specs) {
        emit_test_file(spec)
    }
    to_log(LOG_INFO, "[gen_type_conformance] generated {length(specs)} file(s)\n")
    delete specs
    return true
}
