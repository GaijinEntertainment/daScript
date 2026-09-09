options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama          // the facade: the DlimConfiguration contract, and the tiers that register its sources at [init]

// Prints the running build's DlimConfiguration as JSON - the input `dasllama-convert --config`
// bakes a .dlim against. Cross-compiled to wasm64 (`daspkg release wasm --root examples/dasLLAMA/wasm/dlim_config`)
// and run under node, it tells the deploy what identity the browser build wants its images in,
// so the images are minted natively for THAT build and never go stale against it.
//
//   node dlim_config.js > wasm64.json

[export]
def main() {
    print("{dlim_config_json(dlim_config_current())}\n")
}
