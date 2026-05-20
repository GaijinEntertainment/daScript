options gen2
require dastest/testing_boost public

[test]
def test_resize_int64(t : T?) {
    var arr : array<int>
    arr |> resize(int64(7))
    t |> equal(length(arr), 7)
    t |> equal(long_length(arr), 7_l)
}

[test]
def test_reserve_int64(t : T?) {
    var arr : array<int>
    arr |> reserve(int64(16))
    t |> success(long_capacity(arr) >= 16_l)
}

[test]
def test_erase_int64(t : T?) {
    var arr : array<int>
    arr |> resize(int64(5))
    for (i in iter_range(arr)) {
        arr[i] = i + 1
    }
    arr |> erase(int64(2))
    t |> equal(length(arr), 4)
    t |> equal(arr[2], 4)
}

[test]
def test_erase_range_int64(t : T?) {
    var arr : array<int>
    arr |> resize(int64(5))
    for (i in iter_range(arr)) {
        arr[i] = i + 1
    }
    arr |> erase(int64(1), int64(2))
    t |> equal(length(arr), 3)
    t |> equal(arr[0], 1)
    t |> equal(arr[1], 4)
}

[test]
def test_table_reserve_int64(t : T?) {
    var tab : table<int; int>
    tab |> reserve(int64(64))
    t |> success(long_capacity(tab) >= 64_l)
}
