options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_math   // the `[tuned]` kernels the table names
require dasllama/dasllama_tune   // [dasllama_fallbacks]

// The fixture test_f32_batch_tile_perms.das compiles twice over one module cache record, the second copy one byte longer:
// the root reparses while the cache serves the kernels' modules. Every entry of the table prints as one
// `FB_ROW <kernel> <perm>` record.

[dasllama_fallbacks]
struct private Fallbacks {}

[export]
def main {
    var inscope bank <- dasllama_tuned_fallbacks()
    for (k, v in keys(bank), values(bank)) {
        to_log(LOG_INFO, "FB_ROW {k} {v}\n")
    }
}
