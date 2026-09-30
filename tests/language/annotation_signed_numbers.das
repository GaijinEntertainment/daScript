options gen2
require dastest/testing_boost public
require daslib/rtti

[comment(min=-2, offset=-1.5, zero=-0)]
struct SignedAnnotation {
    @min = -2 @offset = -1.5
    value : int
}

[test]
def signed_annotation_values_survive_reflection(t : T?) {
    t |> equal(typeinfo struct_get_annotation_argument<comment; min>(type<SignedAnnotation>), -2)
    t |> equal(typeinfo struct_get_annotation_argument<comment; offset>(type<SignedAnnotation>), -1.5)
    t |> equal(typeinfo struct_get_annotation_argument<comment; zero>(type<SignedAnnotation>), 0)
    let info = typeinfo rtti_typeinfo(type<SignedAnnotation>)
    var checked = 0
    unsafe { for (field in *info.structType) {
        for (arg in each_annotation_argument(field)) {
            checked++
            if (arg.name == "min") { t |> equal(get_annotation_argument_value(arg) as tInt, -2) }
            if (arg.name == "offset") { t |> equal(get_annotation_argument_value(arg) as tFloat, -1.5) }
        }
    } }
    t |> equal(checked, 2)
}
