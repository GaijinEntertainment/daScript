options gen2
options no_aot
options stack = 1_048_576

require daslib/c_api_header
require daslib/ast_boost
require daslib/fio
require daslib/rtti
require strings
require dastest/testing_boost public


struct private Emitted {
    ok : bool
    header : string
    rejections : array<string>
}


def private emit_for(fixture : string; export_all : bool) : Emitted {
    var result : Emitted
    let input = path_join(get_das_root(), "tests/aot/{fixture}.das")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_public_functions = export_all
            var inscope access <- make_file_access("")
            compile_file(input, access, unsafe(addr(mg)), cop) $(compiled; var program : smart_ptr<Program>; issues) {
                if (!compiled || program == null) {
                    result.rejections |> push("compile failed: {issues}")
                    return
                }
                let names = CNames(prefix = lib_prefix_from_path(input), this_module = program.getThisModule)
                var selected <- collect_c_exports(program.get_ptr(), names, export_all)
                for (r in selected.errors) {
                    result.rejections |> push(r.message)
                }
                result.header = build_c_header(selected.exports, names, input, "out/{fixture}", "LINKNOTE")
                result.ok = true
            }
        }
    }
    return <- result
}


def private expect_in(t : T?; text, needle, what : string) {
    t |> success(find(text, needle) >= 0, what)
    if (find(text, needle) < 0) {
        to_log(LOG_ERROR, "missing from the generated header: {needle}\n")
    }
}


def private expect_not_in(t : T?; text, needle, what : string) {
    t |> success(find(text, needle) < 0, what)
}


def private expect_before(t : T?; text, first, second, what : string) {
    let a = find(text, first)
    let b = find(text, second)
    t |> success(a >= 0 && b >= 0 && a < b, what)
}


def private any_rejection_says(rejections : array<string>; needle : string) : bool {
    for (r in rejections) {
        if (find(r, needle) >= 0) {
            return true
        }
    }
    return false
}


[test]
def test_c_header_prologue(t : T?) {
    let e <- emit_for("_c_header_basic", false)
    t |> success(e.ok, "the fixture compiles and emits")
    t |> success(e.rejections |> empty(), "no [export_c] is refused")
    expect_in(t, e.header, "#pragma once", "the header is include-guarded")
    expect_in(t, e.header, "#include <stdint.h>", "fixed-width integers are included")
    expect_in(t, e.header, "#include <stdbool.h>", "bool is included")
    expect_in(t, e.header, "#include <stddef.h>", "offsetof is included")
    expect_in(t, e.header, "extern \"C\" \{", "a C++ TU gets C linkage")
    expect_in(t, e.header, "#define _C_HEADER_BASIC_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)",
        "C11 gets _Static_assert")
    expect_in(t, e.header, "#define _C_HEADER_BASIC_STATIC_ASSERT(cond, msg) static_assert(cond, msg)",
        "C++ gets static_assert")
    expect_in(t, e.header, "// LINKNOTE", "the link note is carried through")
    expect_in(t, e.header, "typedef struct _c_header_basic_ctx _c_header_basic_ctx;", "the context is opaque")
    expect_in(t, e.header, "_c_header_basic_ctx * _c_header_basic_create(void);", "create takes nothing")
    expect_in(t, e.header, "void _c_header_basic_destroy(_c_header_basic_ctx * ctx);", "destroy takes the context")
    expect_in(t, e.header, "const char * _c_header_basic_last_error(_c_header_basic_ctx * ctx);", "last_error is declared")
    expect_in(t, e.header, "void _c_header_basic_shutdown_runtime(void);", "shutdown_runtime is declared")
}


[test]
def test_c_header_signatures(t : T?) {
    let e <- emit_for("_c_header_basic", false)
    let p = "_c_header_basic"
    expect_in(t, e.header, "float {p}_dot({p}_ctx * ctx, const {p}_Vec3 * a, const {p}_Vec3 * b);",
        "a scalar result returns directly, a struct argument by const pointer")
    expect_in(t, e.header, "void {p}_scale_by({p}_ctx * ctx, const {p}_Vec3 * v, float k, {p}_Vec3 * out);",
        "a struct result travels through a trailing out pointer, under the [export_c] name")
    expect_in(t, e.header, "void {p}_bump({p}_ctx * ctx, {p}_Vec3 * v);", "a var argument drops const")
    expect_in(t, e.header, "void {p}_stretch({p}_ctx * ctx, const {p}_float3 * v, float k, {p}_float3 * out);",
        "a vector crosses by pointer both ways")
    expect_in(t, e.header, "void {p}_spread({p}_ctx * ctx, const {p}_range * r, {p}_range * out);",
        "a range crosses by pointer both ways")
    expect_in(t, e.header, "const char * {p}_greet({p}_ctx * ctx, const char * who);", "a string is a const char *")
    expect_in(t, e.header, "bool {p}_flip({p}_ctx * ctx, bool b);", "bool stays bool")
    expect_in(t, e.header, "uint64_t {p}_widen({p}_ctx * ctx, int64_t n);", "64-bit scalars keep their width")
    expect_in(t, e.header, "const {p}_Vec3 * {p}_raw({p}_ctx * ctx, void * p);", "a void? is a void *")
    expect_in(t, e.header, "float {p}_pick({p}_ctx * ctx, const {p}_Vec3 * v, {p}_Axis8 ax);", "an enum passes by value")
    expect_in(t, e.header, "int32_t {p}_first({p}_ctx * ctx, const int32_t * d);", "a fixed_array argument decays")
    expect_in(t, e.header, "int32_t {p}_depth({p}_ctx * ctx, {p}_Node * n);", "a pointer to a struct is a struct pointer")
    expect_in(t, e.header, "void {p}_nothing({p}_ctx * ctx);", "a void function takes only the context")
    expect_in(t, e.header, "// def dot(a : Vec3; b : Vec3) : float", "each prototype records its das signature")
}


[test]
def test_c_header_types(t : T?) {
    let e <- emit_for("_c_header_basic", false)
    let p = "_c_header_basic"
    expect_in(t, e.header, "typedef int8_t {p}_Axis8;", "an enum is a typedef of its base integer")
    expect_in(t, e.header, "{p}_Axis8_lo = -3", "a negative enumerator survives")
    expect_in(t, e.header, "{p}_Axis8_hi = 7", "an explicit enumerator survives")
    expect_in(t, e.header, "typedef struct \{ float x, y, z; \} {p}_float3;", "float3 is three floats")
    expect_in(t, e.header, "typedef struct \{ int32_t from, to; \} {p}_range;", "range is from/to")
    expect_in(t, e.header, "_STATIC_ASSERT(sizeof({p}_float3) == 12", "float3 is asserted at 12 bytes")
    expect_in(t, e.header, "_STATIC_ASSERT(sizeof({p}_Vec3) == 12", "the struct size is asserted")
    expect_in(t, e.header, "_STATIC_ASSERT(offsetof({p}_Vec3, z) == 8", "every field offset is asserted")
    expect_in(t, e.header, "{p}_Node * next;", "a self-referential field is a pointer")
    expect_before(t, e.header, "typedef struct {p}_Node {p}_Node;", "struct {p}_Node \{",
        "the forward typedef precedes the definition")
}


[test]
def test_c_header_nested_types(t : T?) {
    let e <- emit_for("_c_header_nested", false)
    let p = "_c_header_nested"
    t |> success(e.rejections |> empty(), "a nested POD closure is representable")
    expect_before(t, e.header, "struct {p}_Inner \{", "struct {p}_Outer \{",
        "a by-value field's structure is defined first")
    expect_in(t, e.header, "float taps[3];", "a fixed_array field keeps its dimension")
    expect_in(t, e.header, "{p}_Inner * p;", "a pointer field is a pointer")
    expect_in(t, e.header, "typedef int16_t {p}_Kind;", "a 16-bit enum keeps its width")
    expect_in(t, e.header, "_STATIC_ASSERT(sizeof({p}_Inner) == 16", "alignment padding is asserted")
    expect_in(t, e.header, "_STATIC_ASSERT(offsetof({p}_Inner, b) == 8", "a padded field's offset is asserted")
    expect_in(t, e.header, "_STATIC_ASSERT(offsetof({p}_Outer, taps) == 24", "an array field's offset is asserted")
}


[test]
def test_c_header_refuses_what_c_cannot_spell(t : T?) {
    let e <- emit_for("_c_header_unrepresentable", false)
    t |> success(e.ok, "the fixture itself compiles")
    t |> success(any_rejection_says(e.rejections, "parameter `a` has type array<int>"),
        "an array parameter is refused, named")
    t |> success(any_rejection_says(e.rejections, "result has type tuple<int;int>"), "a tuple result is refused")
    t |> success(any_rejection_says(e.rejections, "result has type int[3]"), "a fixed_array result is refused")
    t |> success(any_rejection_says(e.rejections, "no C representation"), "each refusal says why")
    expect_not_in(t, e.header, "takes_array", "a refused function is not declared")
}


[test]
def test_c_header_needs_the_annotation_module_without_jit(t : T?) {
    let e <- emit_for("_c_header_no_require", false)
    t |> success(!e.ok, "a non-JIT compile cannot see [export_c] on its own")
    t |> success(any_rejection_says(e.rejections, "export_c"),
        "the failure names the annotation, so the reader knows to require daslib/export_c")
}


[test]
def test_c_header_refuses_name_clashes(t : T?) {
    let e <- emit_for("_c_header_clash", false)
    t |> success(any_rejection_says(e.rejections, "C has no overloading"), "two exports cannot share one C symbol")
    t |> success(any_rejection_says(e.rejections, "_c_header_clash_create` is already taken by the fixed library API"),
        "an export cannot take a fixed API name")
    expect_in(t, e.header, "int32_t _c_header_clash_twin(", "the first claim on a symbol is kept")
}


[test]
def test_c_header_export_all(t : T?) {
    let e <- emit_for("_c_header_export_all", true)
    let p = "_c_header_export_all"
    t |> success(e.rejections |> empty(), "export-all skips rather than refuses")
    expect_in(t, e.header, "int32_t {p}_public_ok({p}_ctx * ctx, int32_t x);", "a public function is exported")
    expect_in(t, e.header, "void {p}_main({p}_ctx * ctx);", "an [export] function is exported too")
    expect_not_in(t, e.header, "{p}_public_array", "a public function C cannot spell is skipped")
    expect_not_in(t, e.header, "{p}_hidden", "a private function is never offered")
    expect_not_in(t, e.header, "{p}_generic_one", "a generic function is never offered")
    expect_not_in(t, e.header, "{p}_setup", "an [init] function is never offered")

    let only_explicit <- emit_for("_c_header_export_all", false)
    expect_not_in(t, only_explicit.header, "{p}_public_ok", "without export-all a bare public function is not exported")
    expect_in(t, only_explicit.header, "{p}_create(void);", "the fixed API is emitted even with no exports")
}


[test]
def test_c_header_prefix_sanitizing(t : T?) {
    t |> success(lib_prefix_from_path("a/b/3d-math.das") == "_3d_math", "a leading digit and a dash are fixed")
    t |> success(lib_prefix_from_path("x/int.das") == "int_", "a C keyword gains a suffix")
    t |> success(lib_prefix_from_path("/tmp/plain.das") == "plain", "an ordinary stem is kept")
    t |> success(c_ident("with space") == "with_space", "a space becomes an underscore")
    t |> success(c_ident("ok`tick") == "ok_tick", "a backtick becomes an underscore")
}
