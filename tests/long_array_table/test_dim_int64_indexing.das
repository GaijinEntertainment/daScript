options gen2
require dastest/testing_boost public

// 64-bit index types (int64/uint64) on fixed arrays (T[N]) — mirrors
// test_arr_int64_indexing.das / test_fusion_arr_i64.das for array<T>.
// Covers const-literal indices (compile-time fold), variable indices
// (SimNode_At_I64/U64 + fused variants), r2v reads, writes, argument
// passing, and safe-at.

[test(description = "a fixed array can be read with an int64 index")]
def test_dim_i64_index_read(t : T?) {
    var arr : int[5]
    for (i in range(5)) {
        arr[i] = i * 10
    }
    let i64_idx : int64 = 2_l
    t |> equal(arr[i64_idx], 20)
}

[test(description = "a fixed array can be read with a uint64 index")]
def test_dim_u64_index_read(t : T?) {
    var arr : int[5]
    for (i in range(5)) {
        arr[i] = i * 10
    }
    let u64_idx : uint64 = 3_ul
    t |> equal(arr[u64_idx], 30)
}

[test(description = "a fixed array element can be written through an int64 index")]
def test_dim_i64_index_write(t : T?) {
    var arr : int[3]
    let i64_idx : int64 = 1_l
    arr[i64_idx] = 42
    t |> equal(arr[1], 42)
}

[test(description = "int, int64 and uint64 indices address the same fixed-array elements")]
def test_dim_mixed_width_indexing(t : T?) {
    var arr : int[4]
    arr[0] = 10
    arr[1_l] = 20
    arr[2_ul] = 30
    arr[uint(3)] = 40
    t |> equal(arr[0] + arr[int64(1)] + arr[uint64(2)] + arr[uint(3)], 100)
}

[test(description = "a constant int64 index into a fixed array is folded and reads the right element")]
def test_dim_i64_const_idx(t : T?) {
    // const literal 64-bit indices fold at compile time (sv_trySimulate const path)
    var arr : int[10]
    for (i in range(10)) {
        arr[i] = i * 7
    }
    t |> equal(arr[0_l], 0)
    t |> equal(arr[3_l], 21)
    t |> equal(arr[9_l], 63)
    t |> equal(arr[0_ul], 0)
    t |> equal(arr[4_ul], 28)
    t |> equal(arr[9_ul], 63)
}

[test(description = "a fixed array passed as an argument can be indexed with an int64")]
def test_dim_i64_via_argument(t : T?) {
    var arr : int[8]
    for (i in range(8)) {
        arr[i] = i + 1000
    }
    t |> equal(read_at(arr, 0_l), 1000)
    t |> equal(read_at(arr, 5_l), 1005)
    t |> equal(read_at(arr, 7_l), 1007)
    t |> equal(read_at_u(arr, 6_ul), 1006)
}

def read_at(arr : int[8]; idx : int64) : int => arr[idx]

def read_at_u(arr : int[8]; idx : uint64) : int => arr[idx]

[test(description = "int64 indexing works on a fixed array of floats")]
def test_dim_i64_float_value_type(t : T?) {
    // float element exercises the AtR2V_I64 value-node path
    var arr : float[4]
    arr[0_l] = 1.5f
    arr[1_l] = 2.5f
    arr[2_l] = 3.5f
    arr[3_l] = 4.5f
    t |> equal(arr[0_l], 1.5f)
    t |> equal(arr[3_l], 4.5f)
}

[test(description = "a uint64 index works as an lvalue write into a fixed array")]
def test_dim_u64_lvalue_write(t : T?) {
    var arr : float[3]
    arr[0_ul] = 1.0f
    arr[1_ul] = 2.0f
    arr[2_ul] = 3.0f
    t |> equal(arr[0_ul] + arr[1_ul] + arr[2_ul], 6.0f)
}

[test(description = "int64 indices work on a multi-dimensional fixed array")]
def test_dim_i64_multi_dim(t : T?) {
    // one-peel rule: each [] peels one fixed-array level; 64-bit index at both levels
    var arr : int[2][3]
    for (i in range(2)) {
        for (j in range(3)) {
            arr[i][j] = i * 100 + j
        }
    }
    let ii : int64 = 1_l
    let jj : uint64 = 2_ul
    t |> equal(arr[ii][jj], 102)
    t |> equal(arr[1_l][0], 100)
    t |> equal(arr[0][2_ul], 2)
}

[test(description = "int64 indexing works on a fixed array of int64 values")]
def test_dim_i64_int64_value_type(t : T?) {
    // 64-bit element type + 64-bit index
    var arr : int64[4]
    let idx : int64 = 2_l
    arr[idx] = 5_000_000_000_l
    t |> equal(arr[2], 5_000_000_000_l)
    t |> equal(arr[idx], 5_000_000_000_l)
}

[test(description = "safe-at with an int64 index returns the element in range and the default out of range")]
def test_dim_i64_safe_at(t : T?) {
    var arr : int[5]
    for (i in range(5)) {
        arr[i] = i * 10
    }
    let good : int64 = 3_l
    let bad : int64 = 99_l
    let neg : int64 = -1_l
    var val = arr?[good] ?? -1
    t |> equal(val, 30)
    val = arr?[bad] ?? -1
    t |> equal(val, -1)
    val = arr?[neg] ?? -1
    t |> equal(val, -1)
}

[test(description = "safe-at with a uint64 index returns the element in range and the default out of range")]
def test_dim_u64_safe_at(t : T?) {
    var arr : int[5]
    for (i in range(5)) {
        arr[i] = i * 10
    }
    let good : uint64 = 4_ul
    let bad : uint64 = 100_ul
    var val = arr?[good] ?? -1
    t |> equal(val, 40)
    val = arr?[bad] ?? -1
    t |> equal(val, -1)
}
