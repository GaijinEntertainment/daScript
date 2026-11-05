options gen2
options max_infer_passes = 6
options no_coverage = true

require dastest/testing_boost public

enum Color {
    red
    green
}

struct Point {
    x : int
}

def classify(v : auto(T)) : int {
    static_if (typeinfo is_enum(v)) {
        return 1
    } static_elif (typeinfo is_string(v)) {
        return 2
    } static_elif (typeinfo is_pointer(v)) {
        return 3
    } static_elif (typeinfo is_array(v)) {
        return 4
    } static_elif (typeinfo is_table(v)) {
        return 5
    } static_elif (typeinfo stripped_typename(v) == "bool") {
        return 6
    } static_elif (typeinfo stripped_typename(v) == "float") {
        return 7
    } static_elif (typeinfo stripped_typename(v) == "int") {
        return 8
    } else {
        return 99
    }
}

[test]
def test_static_elif_chain(t : T?) {
    t |> equal(classify(Color.red), 1)
    t |> equal(classify("s"), 2)
    t |> equal(classify(1.0), 7)
    t |> equal(classify(1), 8)
    t |> equal(classify(Point()), 99)
}
