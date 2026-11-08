options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/ast_boost
require daslib/rtti
require _annotation64_builder
require daslib/ast
require daslib/ast_verify

[wide_field_arguments(copied=4102444800l)]
struct BuiltWideArguments { field : int }
[test(description = "annotation arguments built by a macro or copied keep all 64 bits of signed and unsigned values")]
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

// Consumer test for the 64-bit size accessors: validates the any_*_long_size
// bindings end-to-end from daslang. The accessors take a pointer to the
// array/table header (same idiom as convert_to_expression in ast_boost).

[test(description = "the 64-bit size accessor reports the element count of an array inside an any value")]
def test_any_array_long_size(t : T?) {
    var arr <- [10, 20, 30, 40, 50]
    let p = unsafe(addr<uint8 const?>(arr))
    t |> equal(any_array_long_size(p), int64(5))
}

[test(description = "the 64-bit size accessor reports the entry count of a table inside an any value")]
def test_any_table_long_size(t : T?) {
    var tab <- {1 => 100, 2 => 200, 3 => 300}
    let p = unsafe(addr<uint8 const?>(tab))
    t |> equal(any_table_long_size(p), int64(3))
}

[test(description = "the AST verifier reports a child expression whose parsed range lies outside its parent or starts before its previous sibling ends")]
def test_parsed_ranges(t : T?) {
    ast_gc_guard() {
        var clean = quote(a + b * c)
        t |> equal(0, verify_parsed_expression(clean), "a parsed expression has no misplaced range")

        var escaped = quote(a + b) as ExprOp2
        escaped.right.at.last_line += 5u
        t |> success(verify_parsed_expression(escaped) > 0,
            "a child past its parent's end is not reported")

        var swapped = quote(a * b) as ExprOp2
        let left = swapped.left.at
        swapped.left.at = swapped.right.at
        swapped.right.at = left
        t |> success(verify_parsed_expression(swapped) > 0,
            "siblings out of source order are not reported")
    }
}
