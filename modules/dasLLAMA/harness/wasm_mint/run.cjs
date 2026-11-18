// node run.cjs <release.js> <host-dir> [args...]
// Runs a wasm_mint release with <host-dir> mounted at /out and the tune sidecar at
// /out/wasm.tune.json; every DAS* variable of the shell reaches the program.
const path = require('path');

const appJs = process.argv[2] && path.resolve(process.argv[2]);
const outDir = process.argv[3] && path.resolve(process.argv[3]);
if (!appJs || !outDir) {
    console.error('usage: node run.cjs <release.js> <host-dir> [args...]');
    process.exit(1);
}

process.on('unhandledRejection', (reason) => {
    if (reason && reason.name === 'ExitStatus') {
        process.exit(reason.status);
    }
    console.error('WASM error:', reason);
    process.exit(1);
});

const Module = {
    arguments: process.argv.slice(4),
    preRun: [function() {
        for (const k of Object.keys(process.env)) {
            if (k.startsWith('DAS')) {
                Module.ENV[k] = process.env[k];
            }
        }
        Module.FS.mkdir('/out');
        Module.FS.mount(Module.FS.filesystems.NODEFS, { root: outDir }, '/out');
        Module.ENV.DAS_TUNE_MANIFEST = '/out/wasm.tune.json';
    }],
};
global.Module = Module;
require(appJs);
