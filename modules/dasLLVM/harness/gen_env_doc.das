options gen2

// Writes modules/dasLLVM/ENVIRONMENT.md from the [EnvConfig] declarations in llvm_env. The doc
// is generated so it cannot drift from the code: tests/llvm_env_registry.das re-renders and
// compares, and separately fails on any env read that bypasses the declarations.
//
//   daslang modules/dasLLVM/harness/gen_env_doc.das

require llvm/daslib/llvm_env

let OUT_PATH = "modules/dasLLVM/ENVIRONMENT.md"

[export]
def main : int {
    return save_generated_doc("{get_das_root()}/{OUT_PATH}", env_markdown()) ? 0 : 1
}
