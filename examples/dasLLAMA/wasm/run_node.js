// Runs the wasm64 dasllama_aot host under node: the repo mounts at /repo and the model's
// directory at /models through NODEFS, daslib and the engine's das files are embedded in
// the module, and main runs with -use-aot so the engine's AOT stubs bind.
//
//   node examples/dasLLAMA/wasm/run_node.js <output-dir> <script.das> <model.gguf> [args...]
//
//   output-dir  - the directory holding dasllama_aot.js + dasllama_aot.wasm (web/output64)
//   script.das  - a path inside the repo (examples/dasLLAMA/run.das)
//   model.gguf  - a host path; its directory is mounted read-write, so the image rail is
//                 held off (DASLLAMA_IMAGE=0): a mint under the wasm identity would land
//                 beside the model
const path = require('path');

const outputDir = process.argv[2] && path.resolve(process.argv[2]);
const script = process.argv[3];
const model = process.argv[4] && path.resolve(process.argv[4]);
const extra = process.argv.slice(5);
if (!outputDir || !script || !model) {
    console.error('usage: node run_node.js <output-dir> <script.das> <model.gguf> [args...]');
    process.exit(1);
}
const repoRoot = path.resolve(__dirname, '../../..');

// emscripten implements exit() by throwing ExitStatus, which surfaces as an unhandled rejection
process.on('unhandledRejection', (reason) => {
    if (reason && reason.name === 'ExitStatus') {
        process.exit(reason.status);
    }
    console.error('WASM error:', reason);
    process.exit(1);
});

const Module = require(path.join(outputDir, 'dasllama_aot.js'));
Module.onRuntimeInitialized = function() {
    Module.ENV.DASLLAMA_IMAGE = '0';
    Module.ENV.DASLLAMA_ALLOW_UNTUNED = '1';
    Module.FS.mkdir('/repo');
    Module.FS.mount(Module.FS.filesystems.NODEFS, { root: repoRoot }, '/repo');
    Module.FS.mkdir('/models');
    Module.FS.mount(Module.FS.filesystems.NODEFS, { root: path.dirname(model) }, '/models');
    Module.callMain([
        '-use-aot', path.posix.join('/repo', script), '--',
        path.posix.join('/models', path.basename(model)), ...extra
    ]);
};
