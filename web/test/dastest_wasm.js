// Run dastest inside the WASM build by mounting the host repo via NODEFS.
// Usage: node dastest_wasm.js <repo-root> <output-dir>
//   repo-root  — path to daslang repo root (contains dastest/, tests/, daslib/)
//   output-dir — path containing daslang_static.js + daslang.wasm
const path = require('path');

const repoRoot  = path.resolve(process.argv[2]);
const outputDir = path.resolve(process.argv[3]);

if (!repoRoot || !outputDir) {
    console.error('usage: node dastest_wasm.js <repo-root> <output-dir>');
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

    // daslib is already embedded at /daslib by --embed-file at build time.
    // div_by_zero and try_recover use fatal C++ exceptions that escape the WASM
    // callMain boundary before daslang's setjmp-based TryCatch can intercept them.
    Module.callMain([
        '/repo/dastest/dastest.das', '--',
        '--color', '--timeout=0',
        // Excluded a bunch of tests with try/catch
        '--exclude', 'div_by_zero',
        '--exclude', 'try_recover',
        '--exclude', 'func_addr',
        '--exclude', 'move_and_return_move',
        '--exclude', 'resize_locked',
        '--exclude', 'strict_smart_ptr',
        '--exclude', 'string_ops',
        '--exclude', 'table_operations',
        '--exclude', 'variant',
        '--exclude', 'vec_index',
        '--test', '/repo/tests/language'
    ]);
};
