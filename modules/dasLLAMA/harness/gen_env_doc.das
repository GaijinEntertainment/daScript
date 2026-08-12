options gen2

// Writes modules/dasLLAMA/ENVIRONMENT.md from the [EnvConfig] declarations in dasllama_env. The
// doc is generated so it cannot drift from the code: tests/test_env_registry.das re-renders and
// compares, and separately fails on any env read that bypasses the declarations.
//
//   daslang modules/dasLLAMA/harness/gen_env_doc.das

require dasllama/dasllama_env

let OUT_PATH = "modules/dasLLAMA/ENVIRONMENT.md"

[export]
def main : int {
    return save_generated_doc("{get_das_root()}/{OUT_PATH}", env_markdown()) ? 0 : 1
}
