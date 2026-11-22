options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The CUDA home's kernel prewarm as a step of its own: arms the home once, loads every kernel the engine links - each
// text NVRTC compiles lands in the cubin cache (`.cache/daslang/cuda` under the working directory, or
// DASLLAMA_CUDA_CACHE_DIR) - shuts the home down, and prints one line: `PREWARM compiled=<n> cached=<n> failed=<n>
// refused=<n> in <s> s`, or `PREWARM none: <why>` where the build has no CUDA module or no card arms. Exit 1 where a
// kernel did not compile or load, else 0. The pod fleet runs it ahead of the suites (`pod_fleet.das -- run`), so no
// test pays a fresh cache's compile inside its wall. Kernels a test file stamps for itself are not the engine's and
// compile in their test.
//
//   bin/daslang -jit modules/dasLLAMA/harness/cuda_prewarm.das

require dasllama/dasllama   // every CUDA kernel the engine runs: the facade links the CUDA home's modules
require ../tests/_child_cuda_cache.das   // cuda_warm: the arm the cache cells' child runs

[export]
def main() : int {
    var inscope w <- cuda_warm()
    print("{cuda_warm_line(w)}\n")
    cuda_warm_end()
    return cuda_warm_exit(w)
}
