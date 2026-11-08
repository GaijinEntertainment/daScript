options gen2
require dastest/testing_boost public

[test(description = "long_length of an empty array is zero")]
def test_long_length_empty(t : T?) {
    let arr : array<int>
    t |> equal(long_length(arr), 0_l)
    t |> equal(long_capacity(arr), 0_l)
}

[test(description = "long_length reports the new size after a resize")]
def test_long_length_after_resize(t : T?) {
    var arr : array<int>
    arr |> resize(int64(10))
    t |> equal(long_length(arr), 10_l)
}

[test(description = "long_length reports the entry count of a table")]
def test_long_length_table(t : T?) {
    let tab <- { 1 => 100, 2 => 200, 3 => 300 }
    t |> equal(long_length(tab), 3_l)
}
