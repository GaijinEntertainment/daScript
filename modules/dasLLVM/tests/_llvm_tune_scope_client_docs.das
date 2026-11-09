options gen2

require _llvm_tune_scope_client_lib
require _llvm_tune_scope_client_lib2 // nolint:STYLE029,STYLE030,LINT019 - loads scope 2 on purpose

// A documentation root compiles the source API but neither stamps winners nor
// runs tune policy, even when the environment requests the strict error flavor.
[export]
def main {
    let n = 2
    print("RESULT {scope_add(n, 3)}\n")
}
