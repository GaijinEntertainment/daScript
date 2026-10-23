options gen2
options indenting = 4
options no_aot = true

require dastest/testing_boost public

require daslib/rtti
require daslib/ast
require daslib/fio

struct TypeSharing {
    holders : int
    nodes : int
    fromModuleCache : bool
}

//! counts the type slots of one module's structure fields and
//! function signatures, and the distinct nodes they point at -
//! fewer nodes than slots is the sharing the finalize left
def type_sharing(prog : ProgramPtr; name : string) : TypeSharing {
    var res : TypeSharing
    var seen : table<void?>
    program_for_each_module(prog) $(mod) {
        if (mod.name != name) return
        res.fromModuleCache = mod.moduleFlags.fromModuleCache
        for_each_structure(mod) $(st) {
            for (fld in st.fields) {
                res.holders++
                seen |> insert(unsafe(reinterpret<void?>(fld._type)))
            }
        }
        for_each_function(mod, "") $(fn) {
            res.holders++
            seen |> insert(unsafe(reinterpret<void?>(fn.result)))
            for (arg in fn.arguments) {
                res.holders++
                seen |> insert(unsafe(reinterpret<void?>(arg._type)))
            }
        }
    }
    res.nodes = length(seen)
    return res
}

//! a module read back from a serialized program keeps the type
//! sharing its compile left, and is marked as read from the cache.
//! ast_verify is the module under test: its own macro module is
//! simulated again on the read, and its simulate macro must skip a
//! module read from the cache, where it would report each shared
//! type as an alias and un-share it
[test]
def test_served_module_keeps_shared_types(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.version_2_syntax = true
            let fixture = path_join(get_das_root(),
                "tests/module_cache/_fixtures/mc_ast_verify_drv.das")
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(ok, program, issues) {
                if (!ok) {
                    t |> failure("the fixture does not compile: {issues}")
                    return
                }
                let cold = type_sharing(program, "ast_verify")
                t |> success(cold.nodes < cold.holders,
                    "the compiled ast_verify shares types " +
                    "({cold.nodes} nodes for {cold.holders} slots)")
                t |> success(!cold.fromModuleCache,
                    "a compiled module is not marked as read from the cache")
                var bytes : array<uint8>
                var ser = create_ast_serializer()
                t |> success(serialize_program(ser, program), "the program serializes")
                ast_serializer_get_data(ser) $(data) {
                    bytes := data
                }
                delete_ast_serializer(ser)
                var deser = create_ast_deserializer(bytes)
                deserialize_program(deser, unsafe(addr(mg))) $(dok, restored, err) {
                    if (!dok) {
                        t |> failure("deserialize_program: {err}")
                        return
                    }
                    let warm = type_sharing(restored, "ast_verify")
                    t |> equal(warm.holders, cold.holders,
                        "the read module has the compiled module's type slots")
                    t |> equal(warm.nodes, cold.nodes,
                        "the read module shares its types as the compiled one did")
                    t |> success(warm.fromModuleCache,
                        "a module read back is marked as read from the cache")
                }
                delete_ast_serializer(deser)
            }
        }
    }
}
