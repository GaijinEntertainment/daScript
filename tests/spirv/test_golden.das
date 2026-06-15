options gen2
options indenting = 4

// Phase-6 behavior-preservation gate. Each fixture's symbolic disassembly (spirv_dis::dump) is frozen
// in tests/spirv/_golden/<name>.txt (generated from the pre-port hand-walker emitter). This asserts the
// post-port AstVisitor emitter produces the IDENTICAL dump — i.e. byte-identical blobs (same opcodes,
// operands, and id allocation order). If the visitor port legitimately renumbers ids, regenerate the
// goldens (daslang tests/spirv/_gen_golden.das) and confirm the diff is id-renumbering only; the census
// + spirv-val + GPU gates remain the semantic proof. Goldens live under get_das_root() so the test is
// cwd-independent (isolated mode / CI).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require daslib/fio
require daslib/strings_boost

def private read_golden(name : string) : string {
    let path = "{get_das_root()}/tests/spirv/_golden/{name}.txt"
    var content = ""
    var ok = false
    fopen(path, "rb") $(f) {
        if (f != null) {
            content = fread(f)
            ok = true
        }
    }
    if (!ok) {
        return "MISSING GOLDEN: {path}"
    }
    return content
}

// Compare EOL-agnostically: goldens are stored -text (see _golden/.gitattributes), but strip any \r
// so the gate never breaks on an autocrlf checkout (dump() always emits \n).
def private nocr(s : string) : string {
    return replace(s, "\r", "")
}

def private check(t : T?; name : string; var words : array<uint>) {
    let got = nocr(dump(words))
    let want = nocr(read_golden(name))
    let same = got == want
    t |> success(same, "{name}: emitted disassembly matches golden")
    if (!same) {
        to_log(LOG_ERROR, "GOLDEN MISMATCH {name}:\n--- want ---\n{want}\n--- got ---\n{got}\n")
    }
    delete words
}

[test]
def test_golden_disassembly(t : T?) {
    t |> run("emitter disassembly == frozen golden (all fixtures)") <| @(t : T?) {
        check(t, "square", square_words())
        check(t, "uarith", uarith_words())
        check(t, "iarith", iarith_words())
        check(t, "farith", farith_words())
        check(t, "uctrl", uctrl_words())
        check(t, "ictrl", ictrl_words())
        check(t, "fctrl", fctrl_words())
        check(t, "wloop", wloop_words())
        check(t, "floop", floop_words())
        check(t, "nloop", nloop_words())
        check(t, "echain", echain_words())
        check(t, "eret", eret_words())
        check(t, "rfor", rfor_words())
        check(t, "tri_vert", tri_vert_words())
        check(t, "tri_frag", tri_frag_words())
        check(t, "vindex", vindex_words())
        check(t, "raster_math", raster_math_words())
        check(t, "catri", catri_words())
        check(t, "ubo_frag", ubo_frag_words())
        check(t, "mvp_vert", mvp_vert_words())
        check(t, "pc_frag", pc_frag_words())
        check(t, "tex_frag", tex_frag_words())
    }
}
