options gen2

require _llvm_tune_fat_lib
require daslib/export_c

// C-library fixture for the fat-mode test of llvm_tune.das: built
// with `-lib`, it has no `main`, so the class pick has to run from
// the library's own `[init]`.

[export_c]
def fat_result(n : int) : int => fat_add(n, 3)

[export_c]
def fat_mask_now() : int => tune_fat_mask()

[export_c]
def is_library() : bool => tune_library_built()
