options gen2
// options log_infer_passes = true
// options log_generics = true
options rtti

require daslib/fio
require daslib/ast_boost
require daslib/cpp_bind
require dashv/dashv_boost

[export]
def main {
    let root = get_das_root() + "/modules/dasHV/src/"
    fopen(root + "dashv_gen.inc", "wb") $(cpp_file) {
        log_cpp_class_adapter(cpp_file, "HvWebSocketClient_Adapter", typeinfo ast_typedecl(type<HvWebSocketClient>))
        log_cpp_class_adapter(cpp_file, "HvWebServer_Adapter", typeinfo ast_typedecl(type<HvWebServer>))
    }
    return true
}

