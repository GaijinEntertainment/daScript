options gen2
require dastest/testing_boost public
require _cmres_block_common

struct BlockCounts {
    before : int
    count : int
    after : int
}

[never_inline]
def copied_block_result() : BlockCounts {
    var result = BlockCounts(before = 11, count = 3, after = 17)
    for (_ in range(4)) {
        let reply = call_with_result($(value) {
            result.count += value
        })
        result.after += reply.code
    }
    return result
}

struct BlockValues {
    values : array<int>
}

[never_inline]
def moved_block_result() : BlockValues {
    var result : BlockValues
    call_with_result($(value) {
        result.values |> push(value)
    })
    return <- result
}

[never_inline]
def read_captured_result(var observed : int&) : BlockCounts {
    let result = BlockCounts(before = 11, count = 3, after = 17)
    call_with_result($(_value) {
        call_with_result($(_nested) {
            observed = result.before + result.count + result.after
        })
    })
    return result
}

[test]
def test_copied_local_captured_in_block(t : T?) {
    let result = copied_block_result()
    t |> equal(result.before, 11)
    t |> equal(result.count, 31)
    t |> equal(result.after, 93)
}

[test]
def test_moved_local_captured_in_block(t : T?) {
    let result <- moved_block_result()
    t |> equal(length(result.values), 1)
    if (length(result.values) == 1) {
        t |> equal(result.values[0], 7)
    }
}

[test]
def test_return_local_read_by_nested_block(t : T?) {
    var observed = 0
    let result = read_captured_result(observed)
    t |> equal(observed, 31)
    t |> equal(result.before, 11)
    t |> equal(result.count, 3)
    t |> equal(result.after, 17)
}
