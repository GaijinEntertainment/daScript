// Global serialization of heap allocation on threaded WebAssembly builds.
//
// emscripten's dlmalloc lock only serializes pthreads. The AudioWorklet audio thread runs as a
// Wasm Worker (-sWASM_WORKERS, required by -sAUDIO_WORKLET) and bypasses that lock, so its
// malloc/free races against the pthreads on the shared dlmalloc arena -> arena corruption (a later
// posix_memalign aborts in dispose_chunk; or a contended lock busy-spins forever, since the audio
// thread cannot Atomics.wait). We wrap every allocation entry point in one process-wide spinlock
// (which the Wasm Worker DOES honor) and delegate to the real allocator (emscripten_builtin_*), so
// das, C++ new and miniaudio allocations are mutually exclusive across the pthreads and the worklet.
//
// No-op except on threaded wasm; native and single-threaded wasm compile this to an empty unit.

#if defined(__EMSCRIPTEN__) && defined(__EMSCRIPTEN_SHARED_MEMORY__)

#include <stddef.h>
#include <atomic>

extern "C" {
    void  *emscripten_builtin_malloc(size_t);
    void   emscripten_builtin_free(void *);
    void  *emscripten_builtin_calloc(size_t, size_t);
    void  *emscripten_builtin_realloc(void *, size_t);
    void  *emscripten_builtin_memalign(size_t, size_t);
}

static std::atomic_flag g_das_wasm_malloc_lock = ATOMIC_FLAG_INIT;
static inline void das_wasm_malloc_acquire() { while (g_das_wasm_malloc_lock.test_and_set(std::memory_order_acquire)) {} }
static inline void das_wasm_malloc_release() { g_das_wasm_malloc_lock.clear(std::memory_order_release); }

extern "C" {

void *malloc(size_t n) {
    das_wasm_malloc_acquire(); void *p = emscripten_builtin_malloc(n); das_wasm_malloc_release(); return p;
}
void free(void *p) {
    if (!p) return; das_wasm_malloc_acquire(); emscripten_builtin_free(p); das_wasm_malloc_release();
}
void *calloc(size_t a, size_t b) {
    das_wasm_malloc_acquire(); void *p = emscripten_builtin_calloc(a, b); das_wasm_malloc_release(); return p;
}
void *realloc(void *p, size_t n) {
    das_wasm_malloc_acquire(); void *r = emscripten_builtin_realloc(p, n); das_wasm_malloc_release(); return r;
}
void *memalign(size_t al, size_t n) {
    das_wasm_malloc_acquire(); void *p = emscripten_builtin_memalign(al, n); das_wasm_malloc_release(); return p;
}
void *aligned_alloc(size_t al, size_t n) {
    das_wasm_malloc_acquire(); void *p = emscripten_builtin_memalign(al, n); das_wasm_malloc_release(); return p;
}
int posix_memalign(void **out, size_t al, size_t n) {
    das_wasm_malloc_acquire(); void *p = emscripten_builtin_memalign(al, n); das_wasm_malloc_release();
    if (!p) return 12 /*ENOMEM*/;
    *out = p; return 0;
}

}  // extern "C"

#endif
