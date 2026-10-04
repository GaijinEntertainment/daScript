options gen2
require dastest/testing_boost public
require daslib/rtti

[comment(large=9007199254740991l, negative=-9007199254740991l, unsigned_max=18446744073709551615ul, signed_min=-9223372036854775808l, signed_max=9223372036854775807l)]
struct WideAnnotation {
    @limit = 4102444800l
    value : int64
}
[test]
def annotation_int64_typeinfo_retains_full_values(t : T?) {
    t |> equal(typeinfo struct_get_annotation_argument<comment; signed_min>(type<WideAnnotation>), int64(0x8000000000000000ul))
    t |> equal(typeinfo struct_get_annotation_argument<comment; signed_max>(type<WideAnnotation>), 9223372036854775807l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; large>(type<WideAnnotation>), 9007199254740991l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; negative>(type<WideAnnotation>), -9007199254740991l)
    t |> equal(typeinfo struct_get_annotation_argument<comment; unsigned_max>(type<WideAnnotation>), 18446744073709551615ul)
}
[test]
def annotation_int64_runtime_reflection_retains_full_values(t : T?) {
    let value = WideAnnotation()
    let info = (typeinfo rtti_typeinfo(value)).structType
    var seen = 0
    for (annotation in each_annotation(*info)) {
        for (arg in each_annotation_argument(annotation)) {
            let result = get_annotation_argument_value(arg)
            if (arg.name == "large") { t |> equal(result as tInt64, 9007199254740991l); seen++ }
            elif (arg.name == "negative") { t |> equal(result as tInt64, -9007199254740991l); seen++ }
            elif (arg.name == "signed_min") { t |> equal(result as tInt64, int64(0x8000000000000000ul)); seen++ }
            elif (arg.name == "signed_max") { t |> equal(result as tInt64, 9223372036854775807l); seen++ }
            elif (arg.name == "unsigned_max") { t |> equal(result as tUInt64, 18446744073709551615ul); seen++ }
        }
    }
    for (field in *info) {
        for (arg in each_annotation_argument(field)) { t |> equal(get_annotation_argument_value(arg) as tInt64, 4102444800l); seen++ }
    }
    t |> equal(seen, 6)
}
[test]
def int64_minimum_literal_in_expressions(t : T?) {
    let minimum = -9223372036854775808l
    t |> equal(minimum, int64(0x8000000000000000ul))
}
