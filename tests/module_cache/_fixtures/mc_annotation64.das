options gen2
require daslib/rtti
[comment(wide=9007199254740991l)]
struct CachedWide {
    @wide = 18446744073709551615ul
    value : int64
}
[export]
def main {
    verify(typeinfo struct_get_annotation_argument<comment; wide>(type<CachedWide>) == 9007199254740991l)
    let info = (typeinfo rtti_typeinfo(type<CachedWide>)).structType
    var seen = 0
    for (annotation in each_annotation(*info)) {
        for (arg in each_annotation_argument(annotation)) { verify((get_annotation_argument_value(arg) as tInt64) == 9007199254740991l); seen++ }
    }
    for (field in *info) {
        for (arg in each_annotation_argument(field)) { verify((get_annotation_argument_value(arg) as tUInt64) == 18446744073709551615ul); seen++ }
    }
    verify(seen == 2)
    print("wide annotation cache verified\n")
}
