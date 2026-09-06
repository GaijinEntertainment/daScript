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
//                 beside the model, and the image GC would reap the box's own image
//   args        - the script's own arguments; with none, the mounted model path is passed as
//                 the one positional argument (run.das), otherwise they are passed verbatim
//                 with every `@model` replaced by that path (speak.das takes --model @model)
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

// the module script adopts a pre-existing global Module: preRun is the one hook that runs before
// emscripten builds the C environment from ENV, so the knobs land there, not after init
const Module = {
    preRun: [function() {
        Module.ENV.DASLLAMA_IMAGE = '0';
        Module.ENV.DASLLAMA_ALLOW_UNTUNED = '1';
        Module.FS.mkdir('/repo');
        Module.FS.mount(Module.FS.filesystems.NODEFS, { root: repoRoot }, '/repo');
        Module.FS.mkdir('/models');
        Module.FS.mount(Module.FS.filesystems.NODEFS, { root: path.dirname(model) }, '/models');
    }],
    onRuntimeInitialized: function() {
        const mounted = path.posix.join('/models', path.basename(model));
        const scriptArgs = extra.length ? extra.map((a) => a.replace(/@model/g, mounted)) : [mounted];
        Module.callMain(['-use-aot', path.posix.join('/repo', script), '--', ...scriptArgs]);
    },
};
global.Module = Module;
require(path.join(outputDir, 'dasllama_aot.js'));
