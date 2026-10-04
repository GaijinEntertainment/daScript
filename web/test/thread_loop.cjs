// The native/LLVM fixture also runs in real WASM pthreads.
require("./wasm_assertions.cjs")(process.argv[2], "thread_loop", "THREAD_LOOP_COLLECT_OK");
