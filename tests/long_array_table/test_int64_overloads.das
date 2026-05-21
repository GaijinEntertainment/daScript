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

[test]
def test_push_at_int64(t : T?) {
    var arr : array<int>
    arr |> reserve(4)
    arr |> push(10, 0)
    arr |> push(20, 1)
    arr |> push(30, 0_l)  // int64 'at' -- requires int|int64 overload
    t |> equal(length(arr), 3)
    t |> equal(arr[0], 30)
    t |> equal(arr[1], 10)
    t |> equal(arr[2], 20)
}

[test]
def test_push_clone_at_int64(t : T?) {
    var arr : array<int>
    arr |> reserve(4)
    arr |> push_clone(10, 0)
    arr |> push_clone(20, 1)
    arr |> push_clone(30, 0_l)
    t |> equal(length(arr), 3)
    t |> equal(arr[0], 30)
    t |> equal(arr[1], 10)
    t |> equal(arr[2], 20)
}

[test]
def test_emplace_at_int64(t : T?) {
    var arr : array<int>
    arr |> reserve(4)
    var a = 10
    var b = 20
    var c = 30
    arr |> emplace(a, 0)
    arr |> emplace(b, 1)
    arr |> emplace(c, 0_l)
    t |> equal(length(arr), 3)
    t |> equal(arr[0], 30)
    t |> equal(arr[1], 10)
    t |> equal(arr[2], 20)
}

[test]
def test_resize_and_init_int64(t : T?) {
    var arr : array<int>
    arr |> resize_and_init(int64(5))
    t |> equal(length(arr), 5)
    for (i in iter_range(arr)) {
        t |> equal(arr[i], 0)
    }
}

[test]
def test_resize_and_init_initvalue_int64(t : T?) {
    var arr : array<int>
    arr |> resize_and_init(int64(5), 42)
    t |> equal(length(arr), 5)
    for (i in iter_range(arr)) {
        t |> equal(arr[i], 42)
    }
}
