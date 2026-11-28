options gen2
options rtti

module uncategorized_rtti shared public

require dastest/testing_boost public
require daslib/rtti
require _init_seed_helper

struct public RttiInitProbe {
    value : int = 7
}

[test(description = "in a named shared module, a structure's rtti init_mnh equals the hash of its generated initializer function")]
def test_named_module_structure_init_mnh(t : T?) {
    let initializer = @@RttiInitProbe
    t |> success(initializer != null)

    var initializerHash : uint64
    context_for_each_function() $(info : FuncInfo) {
        if (info.name == "RttiInitProbe" && info.count == 0u) {
            initializerHash = info.hash
        }
    }
    t |> success(initializerHash != 0ul)

    let typeInfo = typeinfo rtti_typeinfo(type<RttiInitProbe>)
    t |> equal(typeInfo.structType.init_mnh, initializerHash)
}

[test(description = "a test file that declares a module name runs the [init] of the modules it requires")]
def test_named_root_runs_required_init(t : T?) {
    t |> equal(seed_set_by_init(), 7, "the required module's [init] ran")
}
