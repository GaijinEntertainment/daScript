options gen2
options multiple_contexts
options no_aot // CUDA host APIs use late externs; probe their interpreter path.

require dastest/testing_boost public
require daslib/ast
require daslib/rtti
require daslib/debugger
require daslib/fio
require strings

def private cache_probe_source(t : T?; fail_read : bool) : string {
    let file = path_join(get_das_root(), "modules/dasCUDA/cuda/das_cuda_boost.das")
    let text = fread(file)
    let start = find(text, "def public cubin_cached(")
    t |> strictEqual(start >= 0, true)
    let finish = find(text, "\n}", start)
    t |> strictEqual(finish > start, true)
    var body = slice(text, start, finish + 2)
    let key = "cubin_cache_key(source, compile_opts)"
    let reader = "fread(f, image)"
    let compiler = "cuda_compile(source, name, compile_opts, CudaImage.cubin, log)"
    for (site in fixed_array(key, reader, compiler)) {
        t |> strictEqual(find(body, site) >= 0, true, "fault injection site: {site}")
    }
    body = replace(body, key, "\"fixture\"")
    body = replace(body, reader, "(FAIL_READ ? 0 : fread(f, image))")
    body = replace(body, compiler, "default<array<uint8>>")
    return ("options gen2\noptions persistent_heap\n" +
        "options no_unused_function_arguments = false\n" +
        "require daslib/builtin\nrequire daslib/fio\nrequire strings\n" +
        "let CUDA_CACHE_DIR = \"\"\nlet FAIL_READ = {fail_read}\n" + body + "
        [export]
        def probe(folder : string; var retained : uint64&; var size : int&) \{
            let before = heap_bytes_allocated()
            let opts : array<string>
            var error : string
            var image <- cubin_cached(\"\", \"\", opts, error, folder)
            size = length(image)
            delete image
            retained = heap_bytes_allocated() - before
        \}
    ")
}

[test]
def failed_cubin_read_releases_its_buffer_before_compile(t : T?) {
    let folder = test_temp_dir()
    t |> success(fwrite(path_join(folder, "fixture.cubin"), repeat("x", 256)))
    for (fail_read in fixed_array(false, true)) {
        let source = cache_probe_source(t, fail_read)
        using() $(var policies : CodeOfPolicies) {
            policies.threadlock_context = true
            compile("cuda_cache_probe", source, policies) $(ok; program; errors) {
                t |> success(ok, "{errors}")
                if (!ok) { return }
                simulate(program) $(ready; context; simulation_errors) {
                    t |> success(ready, "{simulation_errors}")
                    if (!ready) { return }
                    var retained = 999ul
                    var size = -1
                    unsafe { invoke_in_context(context, "probe", folder, addr(retained), addr(size)) }
                    t |> equal(size, fail_read ? 0 : 256)
                    t |> equal(retained, 0ul, "cache-read buffers must not leak on fallback")
                }
            }
        }
    }
}
