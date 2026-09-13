options gen2
options indenting = 4
options no_aot = true

require dastest/testing_boost public

require daslib/rtti
require daslib/ast
require daslib/fio
require _mc_common

// the rtti serializer API round-trips a whole program in process, and the reader's phase timers
// account for the read: finalize and setup are the read's parts, not more than the read
[test]
def test_rtti_serializer_round_trip(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.version_2_syntax = true
            compile_file(path_join(get_das_root(), "examples/hello_world.das"), access, unsafe(addr(mg)), cop) $(ok, program, issues) {
                if (!ok) {
                    t |> failure("the fixture does not compile: {issues}")
                    return
                }
                var bytes : array<uint8>
                var ser = create_ast_serializer()
                t |> success(serialize_program(ser, program), "the program serializes")
                ast_serializer_get_data(ser) $(data) {
                    bytes := data
                }
                delete_ast_serializer(ser)
                t |> success(length(bytes) > 1000, "the stream carries the program's modules ({length(bytes)} bytes)")
                var restored = false
                let t0 = ref_time_ticks()
                var deser = create_ast_deserializer(bytes)
                deserialize_program(deser) $(dok, _prog, err) {
                    restored = dok
                    if (!dok) {
                        t |> failure("deserialize_program: {err}")
                    }
                }
                let readUsec = int64(get_time_usec(t0))
                let finalizeUsec = ast_serializer_finalize_usec(deser)
                let setupUsec = ast_serializer_setup_usec(deser)
                delete_ast_serializer(deser)
                t |> success(restored, "the stream deserializes")
                t |> success(finalizeUsec >= 0l && setupUsec >= 0l, "the phase timers are non-negative ({finalizeUsec}, {setupUsec})")
                t |> success(finalizeUsec + setupUsec <= readUsec, "finalize {finalizeUsec} + setup {setupUsec} fit inside the read {readUsec} us")
            }
        }
    }
}
