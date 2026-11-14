options gen2

require _fatclib_c

// Host fixture for the fat-mode test of llvm_tune.das: loads the
// fat library through its generated bindings and prints which clone
// the library's own `[init]` picked (10+3+k).

[export]
def main() : int {
    var ctx : fatclib_ctx? = fatclib_create()
    if (ctx == null) {
        print("create failed: {fatclib_last_error(null)}\n")
        return 1
    }
    let result = fatclib_fat_result(ctx, 10)
    let mask = fatclib_fat_mask_now(ctx)
    print("LIBRESULT {result} MASK {mask} LIBRARY {fatclib_is_library(ctx)}\n")
    fatclib_destroy(ctx)
    return 0
}
