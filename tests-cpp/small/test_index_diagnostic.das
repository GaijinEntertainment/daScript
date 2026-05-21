options gen2

// idx passed as arg so daslang's compile-time const-fold can't pre-resolve
// the access -- we want the int32 runtime indexing path actually executed.
//
// arr[idx] with idx<0 must panic with a diagnostic that names the signed
// index value (-2), not the unsigned wrap (4294967294).

[export]
def neg_int_array_at(idx : int) : int {
    var arr : array<int>
    arr |> resize(10)
    return arr[idx]
}

// `var x = arr[idx]` triggers the Op2ArrayAt fusion path
// (SimNode_Op2ArrayAt in simulate_fusion_at_array.cpp).
[export]
def neg_int_array_at_fused(idx : int) : int {
    var arr : array<int>
    arr |> resize(10)
    let x = arr[idx]
    return x
}

// Dim (fixed-size array) variant -- exercises simulate_fusion_at.cpp / SimNode_At.
[export]
def neg_int_dim_at(idx : int) : int {
    let arr : int[5]
    return arr[idx]
}
