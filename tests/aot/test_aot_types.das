options gen2

require dastest/testing_boost public
require UnitTest

// Regression #3269 (bug F): an empty struct is 0 bytes standalone but occupies >=1 byte in C++
// (no zero-size member). Embedding one by value used to make the AOT-emitted sizeof/offsetof
// static_asserts diverge from the interpreter layout. Structure size/offset now count an
// empty-struct field as >=1, so the emitted C++ matches. The empty field is placed first to
// exercise the offsets of the fields that follow it.

struct Marker {}

struct EmptyFieldHolder {
    m : Marker
    x : int
    y : int
}

[test(description = "A by-value empty struct field keeps the sizes and offsets of the fields after it identical in the AOT C++")]
def test_empty_struct_field_aot(t : T?) {
    t |> equal(typeinfo sizeof(type<Marker>), 0)
    let h = EmptyFieldHolder(x = 7, y = 9)
    t |> equal(h.x, 7)
    t |> equal(h.y, 9)
}

tuple EmptyTuple {}

struct EmptyTupleHolder {
    e : EmptyTuple
    x : int
}

[test(description = "An empty tuple embedded by value in a struct compiles in AOT and leaves the other fields intact")]
def test_empty_tuple_aot(t : T?) {
    t |> equal(typeinfo sizeof(type<EmptyTuple>), 0)
    let h = EmptyTupleHolder(x = 13)
    t |> equal(h.x, 13)
}

// Regression #3269 (bug B): an empty variant has no alternatives, only its 4-byte index tag.
// The AOT emitter used to write TVariant<0,4,> (trailing comma + size 0 -> char data[-4]). It now
// emits TVariant<4,4> (the variant size counts the always-present index tag) and a dedicated
// empty-variant C++ specialization compiles it without a zero-length data array. The empty variant
// is also embedded by value to exercise the containing struct's layout.

variant EmptyVariant {}

struct EmptyVariantHolder {
    e : EmptyVariant
    x : int
}

[test(description = "An empty variant, which has only an index tag, compiles in AOT and leaves the other fields intact")]
def test_empty_variant_aot(t : T?) {
    t |> equal(typeinfo sizeof(type<EmptyVariant>), 4)
    let h = EmptyVariantHolder(x = 13)
    t |> equal(h.x, 13)
}

// Regression for #3151: a tuple with >256 elements overflowed the C++ fold-expression
// nesting limit (256) in tuple_size<>()/tuple_offset<>() in include/daScript/simulate/aot.h.
// Rewriting those folds as a brace-init pack + constexpr loop removes the per-element
// nesting. The tests/aot harness compiles the emitted C++, so a 275-element tuple's
// size/offset templates must instantiate. One int64 element forces non-trivial alignment.

[test(description = "A tuple with 275 elements instantiates its size and offset templates in the AOT C++")]
def test_big_tuple_aot(t : T?) {
    t |> run("275-element tuple AOT-compiles") @(t : T?) {
        let big = (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 100l)
        t |> equal(big._0, 0)
        t |> equal(big._273, 273)
        t |> equal(big._274, 100l)
    }
}

// Regression for #3151 (variant arm): variant_sizeof<>() in
// include/daScript/simulate/aot.h used a per-element comma fold that overflowed the
// C++ 256 expression-nesting limit for a variant with >256 alternatives, the same way
// tuple_size<>()/tuple_offset<>() did for tuples. Rewritten as a brace-init pack +
// constexpr loop. The tests/aot harness compiles the emitted C++, so a 275-alternative
// variant's TVariant<> template must instantiate. One int64 arm forces non-trivial align.

variant BigV {
    f0 : int
    f1 : int
    f2 : int
    f3 : int
    f4 : int
    f5 : int
    f6 : int
    f7 : int
    f8 : int
    f9 : int
    f10 : int
    f11 : int
    f12 : int
    f13 : int
    f14 : int
    f15 : int
    f16 : int
    f17 : int
    f18 : int
    f19 : int
    f20 : int
    f21 : int
    f22 : int
    f23 : int
    f24 : int
    f25 : int
    f26 : int
    f27 : int
    f28 : int
    f29 : int
    f30 : int
    f31 : int
    f32 : int
    f33 : int
    f34 : int
    f35 : int
    f36 : int
    f37 : int
    f38 : int
    f39 : int
    f40 : int
    f41 : int
    f42 : int
    f43 : int
    f44 : int
    f45 : int
    f46 : int
    f47 : int
    f48 : int
    f49 : int
    f50 : int
    f51 : int
    f52 : int
    f53 : int
    f54 : int
    f55 : int
    f56 : int
    f57 : int
    f58 : int
    f59 : int
    f60 : int
    f61 : int
    f62 : int
    f63 : int
    f64 : int
    f65 : int
    f66 : int
    f67 : int
    f68 : int
    f69 : int
    f70 : int
    f71 : int
    f72 : int
    f73 : int
    f74 : int
    f75 : int
    f76 : int
    f77 : int
    f78 : int
    f79 : int
    f80 : int
    f81 : int
    f82 : int
    f83 : int
    f84 : int
    f85 : int
    f86 : int
    f87 : int
    f88 : int
    f89 : int
    f90 : int
    f91 : int
    f92 : int
    f93 : int
    f94 : int
    f95 : int
    f96 : int
    f97 : int
    f98 : int
    f99 : int
    f100 : int
    f101 : int
    f102 : int
    f103 : int
    f104 : int
    f105 : int
    f106 : int
    f107 : int
    f108 : int
    f109 : int
    f110 : int
    f111 : int
    f112 : int
    f113 : int
    f114 : int
    f115 : int
    f116 : int
    f117 : int
    f118 : int
    f119 : int
    f120 : int
    f121 : int
    f122 : int
    f123 : int
    f124 : int
    f125 : int
    f126 : int
    f127 : int
    f128 : int
    f129 : int
    f130 : int
    f131 : int
    f132 : int
    f133 : int
    f134 : int
    f135 : int
    f136 : int
    f137 : int
    f138 : int
    f139 : int
    f140 : int
    f141 : int
    f142 : int
    f143 : int
    f144 : int
    f145 : int
    f146 : int
    f147 : int
    f148 : int
    f149 : int
    f150 : int
    f151 : int
    f152 : int
    f153 : int
    f154 : int
    f155 : int
    f156 : int
    f157 : int
    f158 : int
    f159 : int
    f160 : int
    f161 : int
    f162 : int
    f163 : int
    f164 : int
    f165 : int
    f166 : int
    f167 : int
    f168 : int
    f169 : int
    f170 : int
    f171 : int
    f172 : int
    f173 : int
    f174 : int
    f175 : int
    f176 : int
    f177 : int
    f178 : int
    f179 : int
    f180 : int
    f181 : int
    f182 : int
    f183 : int
    f184 : int
    f185 : int
    f186 : int
    f187 : int
    f188 : int
    f189 : int
    f190 : int
    f191 : int
    f192 : int
    f193 : int
    f194 : int
    f195 : int
    f196 : int
    f197 : int
    f198 : int
    f199 : int
    f200 : int
    f201 : int
    f202 : int
    f203 : int
    f204 : int
    f205 : int
    f206 : int
    f207 : int
    f208 : int
    f209 : int
    f210 : int
    f211 : int
    f212 : int
    f213 : int
    f214 : int
    f215 : int
    f216 : int
    f217 : int
    f218 : int
    f219 : int
    f220 : int
    f221 : int
    f222 : int
    f223 : int
    f224 : int
    f225 : int
    f226 : int
    f227 : int
    f228 : int
    f229 : int
    f230 : int
    f231 : int
    f232 : int
    f233 : int
    f234 : int
    f235 : int
    f236 : int
    f237 : int
    f238 : int
    f239 : int
    f240 : int
    f241 : int
    f242 : int
    f243 : int
    f244 : int
    f245 : int
    f246 : int
    f247 : int
    f248 : int
    f249 : int
    f250 : int
    f251 : int
    f252 : int
    f253 : int
    f254 : int
    f255 : int
    f256 : int
    f257 : int
    f258 : int
    f259 : int
    f260 : int
    f261 : int
    f262 : int
    f263 : int
    f264 : int
    f265 : int
    f266 : int
    f267 : int
    f268 : int
    f269 : int
    f270 : int
    f271 : int
    f272 : int
    f273 : int
    f274 : int64
}

[test(description = "A variant with 275 alternatives instantiates its TVariant template in the AOT C++")]
def test_big_variant_aot(t : T?) {
    t |> run("275-alternative variant AOT-compiles") @(t : T?) {
        let v : BigV
        t |> equal(v is f0, true)
    }
}

// Regression for #3065 / PR #3145 (AOT emitter stack bump). describeCppTypeEx recurses
// once per nesting level when emitting a type, so a deeply-nested function type overflows
// the emitter's default ~16KB macro-context stack (overflow threshold ~13 levels). That
// used to silently drop the field and emit uncompilable C++ (a Func deep; with an
// unbalanced DAS_COMMENT, plus a dangling offsetof). With options stack bumped in
// daslib/aot_cpp.das the ~40-level field below emits a complete, balanced field; the
// tests/aot harness compiles the emitted C++, so removing the bump would fail this AOT
// build. 40 is ~3x the overflow threshold yet well within the 1MB bump -- the daslang
// context stack is a platform-independent managed buffer, so depth behaves identically
// across CI targets.

struct NestedHolder {
    deep : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : function<(a : int) : int>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

[test(description = "A function type nested 40 levels deep emits a complete field in the AOT C++")]
def test_deep_nested_type_aot(t : T?) {
    t |> run("deeply-nested function type AOT-emits a complete field") @(t : T?) {
        let h = NestedHolder()
        t |> equal(h.deep == null, true)
    }
}

// Regression for #3269 (fuzzer AOT codegen). The AOT C++ emitter wrapped a call argument in
// das_alias<Alias>::to(...) whenever the callee parameter was an alias type. That is only correct
// for extern/bound (builtIn) callees whose C++ signature spells the alias name; emitter-generated
// callees always spell parameters structurally, so the wrap mismatched the parameter and the
// generated C++ failed to compile. bug C: delete of a handled variant alias (SampleVariant) ->
// generated finalize(). bug G: builtin push/to_array_move of a variant element -> generic
// placeholder name leaked. Both go through the now builtIn-gated alias wrap in daslib/aot_cpp.das.

variant U_F {
    i_value : uint
    f_value : float
}

[sideeffects]
def test_delete_handled_variant_alias(t : T?) {
    var s = makeSampleS()
    t |> success(s is s_value)
    delete s
    t |> success(s is i_value)
}

def test_variant_element_array(t : T?) {
    var v : U_F
    unsafe {
        set_variant_index(v, 0)
        v.i_value = 7u
    }
    var arr : array<U_F>
    arr |> push(v)
    t |> equal(length(arr), 1)
    t |> success(arr[0] is i_value)
    t |> equal(arr[0] as i_value, 7u)
}

[test(description = "Deleting a handled variant alias resets it to its first alternative, and variant elements push into arrays in AOT")]
def test_variant_alias_aot(t : T?) {
    test_delete_handled_variant_alias(t)
    test_variant_element_array(t)
}
