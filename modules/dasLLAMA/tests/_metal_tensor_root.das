options gen2
options _dasllama_internal = true

require dasllama/dasllama_common
require llvm/daslib/llvm_tune   // tune_box_identity: the fixture is this box's, so the verdict reads it
require daslib/fio

// the box-profile fixture test_box_profile.das spawns: a sidecar of this box crowning mulmm_q8, applied through the
// checked route, and the crowns that apply left - empty where DASLLAMA_METAL_TENSOR=0 keeps them off

[export]
def main() {
    var terr = ""
    let path = path_join(temp_directory(terr), "dasllama_test_metal_tensor_root.json")
    fwrite(path, "\{\"kernels\":\{\},\"runtime\":\{\"metal_tensor\":\"mulmm_q8\"\},\"provenance\":\{\"box\":\"{tune_box_identity()}\"\}\}")
    apply_box_profile_runtime_checked(path)
    print("CROWNS=[{get_metal_tensor_crowns()}]\n")
    remove(path)
}
