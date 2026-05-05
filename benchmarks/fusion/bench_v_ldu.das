options gen2
options persistent_heap

require dastest/testing_boost

// Micro-benches for the fusion-engine v_ldu/workhorse load path.
// Op2At (array<T>[idx]) at different sizeof(T):
//   int    (4)  — at.cpp scalar path, *(int*)slot, no v_ldu
//   int64  (8)  — at.cpp scalar path, *(int64*)slot, no v_ldu
//   float3 (12) — at.cpp VECTOR path, v_ldu_p3_safe target
//   float4 (16) — at.cpp VECTOR path, v_ldu target
//
// int/int64 benches act as control variables (unaffected by the macro
// choice); float3/float4 are the actual targets. Compare four builds:
//   DAS_FUSION=0 (no fusion floor)
//   raw v_ldu (current/unsafe baseline)
//   DAS_LDU_WORKHORSE if-constexpr ladder
//   v_zero+memcpy(sizeof(CTYPE))

let private N = 100000

[benchmark]
def at_int_4(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = i
    }
    var sum = 0
    b |> run("at_int", N) {
        for (i in range(N)) {
            sum += arr[i]
        }
    }
    if (sum == 0) {
        b->failNow()
    }
}

[benchmark]
def at_int64_8(b : B?) {
    var arr : array<int64>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = int64(i)
    }
    var sum = 0l
    b |> run("at_int64", N) {
        for (i in range(N)) {
            sum += arr[i]
        }
    }
    if (sum == 0l) {
        b->failNow()
    }
}

[benchmark]
def at_float3_12(b : B?) {
    var arr : array<float3>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = float3(i, i + 1, i + 2)
    }
    var sum = float3(0.0, 0.0, 0.0)
    b |> run("at_float3", N) {
        for (i in range(N)) {
            sum += arr[i]
        }
    }
    if (sum.x == 0.0) {
        b->failNow()
    }
}

[benchmark]
def at_float4_16(b : B?) {
    var arr : array<float4>
    arr |> resize(N)
    for (i in range(N)) {
        arr[i] = float4(i, i + 1, i + 2, i + 3)
    }
    var sum = float4(0.0, 0.0, 0.0, 0.0)
    b |> run("at_float4", N) {
        for (i in range(N)) {
            sum += arr[i]
        }
    }
    if (sum.x == 0.0) {
        b->failNow()
    }
}
