// Run the little wasm boy (examples/fatman/wasmboy.das) inside the WASM build by
// mounting the host repo via NODEFS — the require-smoke for the imgui family
// statically linked into daslang_static (see .github/workflows/wasmboy.yml).
// Usage: node wasmboy_node.js <repo-root> <output-dir>
//   repo-root  — path to daslang repo root (contains examples/, daslib/)
//   output-dir — path containing daslang_static.js + daslang_static.wasm
const path = require('path');

const repoRoot  = path.resolve(process.argv[2]);
const outputDir = path.resolve(process.argv[3]);

if (!repoRoot || !outputDir) {
    console.error('usage: node wasmboy_node.js <repo-root> <output-dir>');
    process.exit(1);
}

// Emscripten implements exit() by throwing ExitStatus, which propagates as an
// unhandled promise rejection. Intercept it to forward the exit code correctly.
process.on('unhandledRejection', (reason) => {
    if (reason && reason.name === 'ExitStatus') {
        process.exit(reason.status);
    }
    console.error('WASM error:', reason);
    process.exit(1);
});

const Module = require(outputDir + '/daslang_static.js');
Module.onRuntimeInitialized = function() {
    Module.FS.mkdir('/repo');
    Module.FS.mount(Module.FS.filesystems.NODEFS, { root: repoRoot }, '/repo');

    // daslib + the imgui-family .das halves are embedded by --embed-file at build
    // time; only the smoke script itself comes from the host repo mount.
    Module.callMain(['/repo/examples/fatman/wasmboy.das']);
};
