options gen2

// Writes modules/dasLLAMA/ENVIRONMENT.md from env_registry(). The doc is generated so it cannot
// drift from the code: tests/test_env_registry.das re-renders and compares, and separately fails
// on any env read in the tree whose name is not registered.
//
//   daslang modules/dasLLAMA/harness/gen_env_doc.das

require dasllama/dasllama_env
require daslib/fio

let OUT_PATH = "modules/dasLLAMA/ENVIRONMENT.md"

[export]
def main : int {
    let body = env_markdown()
    let path = "{get_das_root()}/{OUT_PATH}"
    var f = fopen(path, "wb")
    if (f == null) {
        to_log(LOG_ERROR, "gen_env_doc: cannot write {path}\n")
        return 1
    }
    fwrite(f, body)
    fclose(f)
    to_log(LOG_INFO, "gen_env_doc: wrote {OUT_PATH} ({long_length(body)} bytes)\n")
    return 0
}
