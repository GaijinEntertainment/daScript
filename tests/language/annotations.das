// Annotation reflection through context debug info (AnnotationInfo /
// AnnotationArgumentInfo) is deliberately compiled WITHOUT `options
// rtti` - annotation data is deep-copied into the context debug heap
// and must not depend on the Program staying alive.

options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/rtti
require strings
require _helper_double_patch public
require _field_decl_flags_helper

[comment(v_int = 13, v_float = 1.5, v_bool = true, v_str = "hello", v_esc = "q\"uote\\back\nline")]
struct AnnotatedStruct {
    @v_int = 13 a : int
    @v_float = 1.5 b : float
    @v_bool = true c : bool
    @v_str = "hello" d : string
    e : int     // no annotations
}

enum PlainEnum {
    one
    two
}

[export]
def annotated_fn() {
    feint("annotated\n")
}

[test(description = "a struct's annotation and its int, float, bool and string arguments read back through context debug info, with only the escaped quote unescaped in a string argument")]
def test_struct_annotations(t : T?) {
    let s = AnnotatedStruct()
    let info = typeinfo rtti_typeinfo(s)
    let sinfo = info.structType
    t |> success(sinfo != null, "AnnotatedStruct should have StructInfo")
    t |> equal(int(sinfo.annotation_count), 1)
    var seen_args = 0
    for (ann in each_annotation(*sinfo)) {
        // direct const char*-backed field pass - pins the AOT (char*)(const
        // char*) cast
        t |> equal(ann.name, "comment")
        t |> equal(int(ann.count), 5)
        for (arg in each_annotation_argument(ann)) {
            let value = get_annotation_argument_value(arg)
            if (arg.name == "v_int") {
                t |> success(value is tInt, "v_int should be int")
                t |> equal(value as tInt, 13)
            } elif (arg.name == "v_float") {
                t |> success(value is tFloat, "v_float should be float")
                t |> equal(value as tFloat, 1.5)
            } elif (arg.name == "v_bool") {
                t |> success(value is tBool, "v_bool should be bool")
                t |> equal(value as tBool, true)
            } elif (arg.name == "v_str") {
                t |> success(value is tString, "v_str should be string")
                t |> equal(value as tString, "hello")
            } elif (arg.name == "v_esc") {
                t |> success(value is tString, "v_esc should be string")
                // annotation string_constant unescapes only \" - backslashes stay
                // literal; the expected value is built from chars to stay independent
                // of string escape rules
                let expected = "q" + to_char(34) + "uote" + to_char(92) + to_char(92) + "back" + to_char(92) + "nline"
                t |> equal(value as tString, expected)
            }
            seen_args ++
        }
    }
    t |> equal(seen_args, 5)
}

[test(description = "each struct field's own annotation arguments read back through debug info, and an unannotated field reports none")]
def test_field_annotations(t : T?) {
    let s = AnnotatedStruct()
    let sinfo = (typeinfo rtti_typeinfo(s)).structType
    var checked = 0
    unsafe {
        for (fld in *sinfo) {
            if (fld.name == "a") {
                t |> equal(int(fld.annotation_argument_count), 1)
                for (arg in each_annotation_argument(fld)) {
                    t |> equal(arg.name, "v_int")
                    t |> equal(get_annotation_argument_value(arg) as tInt, 13)
                }
                checked ++
            } elif (fld.name == "b") {
                for (arg in each_annotation_argument(fld)) {
                    t |> equal(get_annotation_argument_value(arg) as tFloat, 1.5)
                }
                checked ++
            } elif (fld.name == "c") {
                for (arg in each_annotation_argument(fld)) {
                    t |> equal(get_annotation_argument_value(arg) as tBool, true)
                }
                checked ++
            } elif (fld.name == "d") {
                for (arg in each_annotation_argument(fld)) {
                    t |> equal(get_annotation_argument_value(arg) as tString, "hello")
                }
                checked ++
            } elif (fld.name == "e") {
                t |> equal(int(fld.annotation_argument_count), 0)
                checked ++
            }
        }
    }
    t |> equal(checked, 5)
}

[test(description = "a function's [export] annotation is visible in the context's function debug info")]
def test_function_annotations(t : T?) {
    var found_fn = false
    var found_export = false
    context_for_each_function() $(finfo) {
        if (finfo.name == "annotated_fn") {
            found_fn = true
            for (ann in each_annotation(finfo)) {
                if (ann.name == "export") {
                    found_export = true
                }
            }
        }
    }
    t |> success(found_fn, "annotated_fn should be in context debug info")
    t |> success(found_export, "annotated_fn should carry the [export] annotation")
}

[test(description = "an enum with no annotations has EnumInfo with an annotation count of zero")]
def test_enum_annotations(t : T?) {
    let info = typeinfo rtti_typeinfo(PlainEnum.one)
    let einfo = info.enumType
    t |> success(einfo != null, "PlainEnum should have EnumInfo")
    t |> equal(int(einfo.annotation_count), 0)
}

[test(description = "resolve_annotation turns a debug-info annotation back into the live structure annotation object")]
def test_resolve_annotation(t : T?) {
    let s = AnnotatedStruct()
    let sinfo = (typeinfo rtti_typeinfo(s)).structType
    for (ann in each_annotation(*sinfo)) {
        let live = resolve_annotation(ann)
        t |> success(live != null, "comment annotation should resolve to the live object")
        if (live != null) {
            t |> equal("{live.name}", "comment")
            t |> success(live.isStructureAnnotation, "comment is a structure annotation")
        }
    }
}

[test(description = "the deprecated structure_for_each_annotation still yields the env-resolved annotation with its reconstructed arguments")]
def test_deprecated_structure_for_each_annotation(t : T?) {
    let s = AnnotatedStruct()
    let sinfo = (typeinfo rtti_typeinfo(s)).structType
    var seen = 0
    structure_for_each_annotation(*sinfo) $(ann; args) {
        t |> equal("{ann.name}", "comment")
        t |> equal(length(args), 5)
        for (arg in args) {
            if (arg.name == "v_int") {
                t |> equal(arg.iValue, 13)
            }
        }
        seen ++
    }
    t |> equal(seen, 1)
}

[ comment(large=9007199254740991l, negative=-9007199254740991l, unsigned_max=18446744073709551615ul, signed_min=-9223372036854775808l, signed_max=9223372036854775807l)]
struct WideAnnotation {
    @limit = 4102444800l
    value : int64
}

[test(description = "typeinfo struct_get_annotation_argument returns full 64-bit signed and unsigned annotation literals unchanged")]
def annotation_int64_typeinfo_retains_full_values(t : T?) {
    t |> equal(typeinfo struct_get_annotation_argument<comment; signed_min>(type<WideAnnotation>),
               int64(0x8000000000000000ul))
    t |> equal(typeinfo struct_get_annotation_argument<comment; signed_max>(type<WideAnnotation>),
               9223372036854775807l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; large>(type<WideAnnotation>),
               9007199254740991l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; negative>(type<WideAnnotation>),
               -9007199254740991l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; unsigned_max>(type<WideAnnotation>),
               18446744073709551615ul)
}

[test(description = "runtime reflection returns 64-bit struct and field annotation arguments as tInt64 / tUInt64 with their full values")]
def annotation_int64_runtime_reflection_retains_full_values(t : T?) {
    let value = WideAnnotation()
    let info = (typeinfo rtti_typeinfo(value)).structType
    var seen = 0
    for (annotation in each_annotation(*info)) {
        for (arg in each_annotation_argument(annotation)) {
            let result = get_annotation_argument_value(arg)
            if (arg.name == "large") { t |> equal(result as tInt64, 9007199254740991l); seen++ }
            elif (arg.name == "negative") { t |> equal(result as tInt64, -9007199254740991l); seen++ }
            elif (arg.name == "signed_min") { t |> equal(result as tInt64,
                                                         int64(0x8000000000000000ul)); seen++ }
            elif (arg.name == "signed_max") { t |> equal(result as tInt64,
                                                         9223372036854775807l); seen++ }
            elif (arg.name == "unsigned_max") { t |> equal(result as tUInt64,
                                                           18446744073709551615ul); seen++ }
        }
    }
    for (field in *info) {
        for (arg in each_annotation_argument(field)) { t |> equal(get_annotation_argument_value(arg) as tInt64, 4102444800l); seen++ }
    }
    t |> equal(seen, 6)
}

[comment(hex=0xD0, hex_max=0xFFFFFFFF, unsigned=7u, byte=0xFFu8, values=(0xD0, 7u, 0xFFu8))]
struct HexAnnotation {
    @offset = 0x2A
    value : uint16
}

[test(description = "hex and unsigned annotation literals of every width come back as uint64, through typeinfo and through runtime field reflection")]
def annotation_unsigned_literals_are_uint64(t : T?) {
    let hex = typeinfo struct_get_annotation_argument<comment; hex>(type<HexAnnotation>)
    let hex_max = typeinfo struct_get_annotation_argument<comment; hex_max>(type<HexAnnotation>)
    let unsigned = typeinfo struct_get_annotation_argument<comment; unsigned>(type<HexAnnotation>)
    let byte = typeinfo struct_get_annotation_argument<comment; byte>(type<HexAnnotation>)
    t |> equal(hex, 0xD0ul)
    t |> equal(hex_max, 0xFFFFFFFFul)
    t |> equal(unsigned, 7ul)
    t |> equal(byte, 0xFFul)
    let value = HexAnnotation()
    let info = (typeinfo rtti_typeinfo(value)).structType
    var seen = 0
    for (field in *info) {
        for (arg in each_annotation_argument(field)) {
            t |> equal(get_annotation_argument_value(arg) as tUInt64, 0x2Aul)
            seen++
        }
    }
    t |> equal(seen, 1)
}

[test(description = "the int64 minimum literal parses in an expression to the bit pattern 0x8000000000000000")]
def int64_minimum_literal_in_expressions(t : T?) {
    let minimum = -9223372036854775808l
    t |> equal(minimum, int64(0x8000000000000000ul))
}

[comment(min=-2, offset=-1.5, zero=-0, values=(-2, -1.5))]
struct SignedAnnotation {
    @min = -2 @offset = -1.5
    value : int
}

[test(description = "negative int and float annotation arguments, and negative zero, keep their sign through typeinfo and runtime field reflection")]
def signed_annotation_values_survive_reflection(t : T?) {
    t |> equal(typeinfo struct_get_annotation_argument<comment; min>(type<SignedAnnotation>), -2)
    t |> equal(typeinfo struct_get_annotation_argument<comment; offset>(type<SignedAnnotation>),
               -1.5)
    t |> equal(typeinfo struct_get_annotation_argument<comment; zero>(type<SignedAnnotation>), 0)
    let info = typeinfo rtti_typeinfo(type<SignedAnnotation>)
    var checked = 0
    unsafe { for (field in *info.structType) {
        for (arg in each_annotation_argument(field)) {
            checked++
            if (arg.name == "min") { t |> equal(get_annotation_argument_value(arg) as tInt, -2) }
            if (arg.name == "offset") { t |> equal(get_annotation_argument_value(arg) as tFloat,
                                                   -1.5) }
        }
    } }
    t |> equal(checked, 2)
}

struct TaiFoo {
    i : int
}

[comment(i=1, f=2.0, s="three", b=false)]
struct TaiEvent {
    i : int
}

[test(description = "typeinfo struct_has_annotation, struct_has_annotation_argument and struct_get_annotation_argument answer for a struct's annotation at compile time")]
def test_typeinfo_annotations(t : T?) {
    t |> run("struct_has_annotation") @(t : T?) {
        t |> success(typeinfo struct_has_annotation < comment > (type<TaiEvent>))
        t |> success(!typeinfo struct_has_annotation < comment > (type<TaiFoo>))
    }
    t |> run("annotation argument queries") @(t : T?) {
        t |> success(typeinfo struct_has_annotation_argument<comment; i>(type<TaiEvent>))
        t |> success(!typeinfo struct_safe_has_annotation_argument < comment; i > (type<TaiFoo>))
        t |> success(typeinfo struct_has_annotation_argument<comment; f>(type<TaiEvent>))
        t |> success(typeinfo struct_has_annotation_argument<comment; s>(type<TaiEvent>))
        t |> success(typeinfo struct_has_annotation_argument<comment; b>(type<TaiEvent>))
        t |> success(!typeinfo struct_has_annotation_argument<comment; q>(type<TaiEvent>))
    }
    t |> run("get annotation argument values") @(t : T?) {
        let i = typeinfo struct_get_annotation_argument<comment; i>(type<TaiEvent>)
        let f = typeinfo struct_get_annotation_argument<comment; f>(type<TaiEvent>)
        let s = typeinfo struct_get_annotation_argument<comment; s>(type<TaiEvent>)
        let b = typeinfo struct_get_annotation_argument<comment; b>(type<TaiEvent>)
        t |> equal(i, 1)
        t |> equal(f, 2.0)
        t |> equal(s, "three")
        t |> equal(b, false)
    }
}

// [arch(at="<doc>.md#<anchor>")] is an inert builtin marker: whether
// a citation RESOLVES is LINT026's question, not the compiler's;
// these are the shapes the compiler must accept
[arch(at="_arch_annotation_doc.md#tokenizer")]
def cited_function : int => 17

[arch(at="_arch_annotation_doc.md#tokenizer"), arch(at="_arch_annotation_doc.md#buffers")]
def twice_cited_function : int => 4

[never_inline, arch(at="_arch_annotation_doc.md#tokenizer")]
def cited_beside_another : int => 9

[arch(at="_arch_annotation_doc.md#token")]
struct CitedStruct {
    value : int
}

[arch(at="_arch_annotation_doc.md#lexer")]
class CitedClass {
    count : int
    def bump {
        count ++
    }
}

[test(description = "an [arch] citation on a function, a struct or a class compiles and changes no behavior, alone, twice, or beside another annotation")]
def test_arch_annotation_compiles(t : T?) {
    t |> run("a cited function is an ordinary function") @(t : T?) {
        t |> equal(cited_function(), 17, "the marker changes no behavior")
        t |> equal(twice_cited_function(), 4, "two citations are legal on one function")
        t |> equal(cited_beside_another(), 9, "the marker composes with other annotations")
    }
    t |> run("a cited structure is an ordinary structure") @(t : T?) {
        var s = CitedStruct(value = 5)
        s.value ++
        t |> equal(s.value, 6, "fields behave as declared")
    }
    t |> run("a cited class is an ordinary class") @(t : T?) {
        var c = new CitedClass()
        c->bump()
        c->bump()
        t |> equal(c.count, 2, "methods behave as declared")
        unsafe {
            delete c
        }
    }
}

[sideeffects, unused_argument(b, c)]
def ua_haha(a, b, c) => a * 2

[test(description = "[unused_argument(b, c)] lets a function leave those arguments unread without a warning, and it still computes from the rest")]
def test_unused_arguments_annotation(t : T?) {
    t |> run("unused_argument annotation suppresses warning") @(t : T?) {
        let v = ua_haha(1, 2.0, "blah")
        t |> equal(v, 2)
    }
}

[count_patch(tag = "first"), count_patch(tag = "second")]
def probe_twice() {
    feint("probe\n")
}

[test(description = "two instances of one patching function annotation on a function each apply their patch once")]
def test_each_annotation_instance_patches_once(t : T?) {
    t |> run("two [count_patch] on one function both patch") @(t : T?) {
        double_patch_marks = 0
        probe_twice()
        t |> equal(double_patch_marks, 2)
    }
}

// [check_add_field_flags] asserts the FieldDeclaration bits of `add`
// at compile time (a mismatch is a macro_error): declared abstract ->
// abstract, not inherited; cloned -> abstract, inherited; overridden
// with a body -> neither
[check_add_field_flags(is_abstract = true, is_inherited = false)]
class FdfFoo {
    def abstract add(a, b : int) : int
}

[check_add_field_flags(is_abstract = true, is_inherited = true)]
class FdfBar : FdfFoo {
}

[check_add_field_flags(is_abstract = false, is_inherited = false)]
class FdfFar : FdfBar {
    def override add(a, b : int) : int => a + b
}

[test(description = "an abstract method's field carries the abstract and inherited bits through clone and override, and the overriding body dispatches")]
def test_far_overrides_abstract(t : T?) {
    var f = new FdfFar()
    t |> equal(f.add(2, 3), 5)
    unsafe {
        delete f
    }
}
