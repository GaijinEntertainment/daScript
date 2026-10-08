options gen2
require dastest/testing_boost public
require dasllama/dasllama
require daslib/rtti

// dasllama/dasllama_core is the engine without the file decoders: no module it depends on is
// `audio` or `stbimage`, while dasllama/dasllama, core plus the decoders, depends on both.

def private collect_dependencies(mod : Module?; var seen : table<string; bool>) {
    module_for_each_dependency(mod) $(dep, _public) {
        let name = string(dep.name)
        if (!key_exists(seen, name)) {
            seen[name] = true
            collect_dependencies(dep, seen)
        }
    }
}

def private dependencies(name : string) : table<string; bool> {
    var seen : table<string; bool>
    let mod = get_module(name)
    if (mod != null) {
        collect_dependencies(mod, seen)
    }
    return <- seen
}

[test]
def test_dasllama_core_requires_no_decoder(t : T?) {
    let core <- dependencies("dasllama_core")
    t |> success(key_exists(core, "dasllama_tts"), "the walk reaches the engine")
    t |> success(!key_exists(core, "audio") && !key_exists(core, "stbimage"), "dasllama_core needs no decoder module")
    let facade <- dependencies("dasllama")
    t |> success(key_exists(facade, "audio") && key_exists(facade, "stbimage"), "dasllama/dasllama brings both decoders")
}
