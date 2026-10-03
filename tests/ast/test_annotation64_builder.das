options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require daslib/ast_boost
require daslib/rtti

require _annotation64_builder

[wide_field_arguments(copied=4102444800l)]
struct BuiltWideArguments { field : int }
[test]
def macro_built_and_copied_annotation_values_keep_all_bits(t : T?) {
    let info = (typeinfo rtti_typeinfo(type<BuiltWideArguments>)).structType
    var seen = 0
    unsafe {
        for (field in *info) {
            for (arg in each_annotation_argument(field)) {
                let value = get_annotation_argument_value(arg)
                if (arg.name == "signed") { t |> equal(value as tInt64, -9007199254740991l); seen++ }
                elif (arg.name == "unsigned") { t |> equal(value as tUInt64, 18446744073709551615ul); seen++ }
                elif (arg.name == "copied") { t |> equal(value as tInt64, 4102444800l); seen++ }
            }
        }
    }
    t |> equal(seen, 3)
}
