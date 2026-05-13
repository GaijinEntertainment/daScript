// Mini-playground WASM runner for the hero panel.
//
// Loads daslang_static.{js,wasm} from ./files/wasm/ (vendored by pages.yml
// from the wasm_build.yml artifact) and exposes window.daslangRun(source)
// returning {stdout, stderr, durationMs}.
//
// Gracefully degrades: if the WASM artifact is missing (local-dev without a
// build), daslangRun returns an explanatory message and the ▶ Run button
// stays in a degraded "open full playground" state.

(function () {
    const WASM_DIR = './files/wasm/';

    let stdoutBuf = [];
    let stderrBuf = [];
    let runtimeReady = false;
    let runtimeBroken = false;
    let runtimePromise = null;

    // Pre-define Module so daslang_static.js sees our overrides at startup.
    window.Module = {
        preRun: [],
        postRun: [],
        print: (text) => { stdoutBuf.push(text); },
        printErr: (text) => { stderrBuf.push(text); },
        onRuntimeInitialized: () => { runtimeReady = true; },
        onAbort: (err) => { runtimeBroken = true; console.error('[daslang] runtime aborted:', err); },
        locateFile: (path) => WASM_DIR + path,
        noExitRuntime: true,
    };

    function loadRuntime() {
        if (runtimePromise) return runtimePromise;
        runtimePromise = new Promise((resolve, reject) => {
            const s = document.createElement('script');
            s.src = WASM_DIR + 'daslang_static.js';
            s.async = true;
            s.onerror = () => {
                runtimeBroken = true;
                reject(new Error('daslang_static.js not found at ' + s.src));
            };
            // Resolve once the runtime is ready — onRuntimeInitialized fires
            // after the wasm has loaded and exports are wired.
            const tick = () => {
                if (runtimeReady) return resolve();
                if (runtimeBroken) return reject(new Error('runtime aborted'));
                setTimeout(tick, 50);
            };
            document.head.appendChild(s);
            tick();
        });
        return runtimePromise;
    }

    async function daslangRun(source) {
        const t0 = performance.now();
        try {
            await loadRuntime();
        } catch (e) {
            return {
                stdout: '',
                stderr: '',
                durationMs: 0,
                exitCode: -1,
                unavailable: true,
                error: e.message,
            };
        }
        stdoutBuf = [];
        stderrBuf = [];
        let exitCode = 0;
        // FS lives on Module in modern Emscripten modularized builds, not as
        // a global. The web/ui IDE happens to work because its build exposes
        // FS through the script's IIFE; ours doesn't.
        const M = window.Module;
        try {
            M.FS.writeFile('main.das', source);
        } catch (e) {
            stderrBuf.push('FS error: ' + (e.message || e));
        }
        try {
            M.callMain(['main.das']);
        } catch (e) {
            stderrBuf.push(String(e.message || e));
            exitCode = -1;
        }
        const durationMs = performance.now() - t0;
        return {
            stdout: stdoutBuf.join('\n'),
            stderr: stderrBuf.join('\n'),
            durationMs,
            exitCode,
        };
    }

    window.daslangRun = daslangRun;
    window.daslangRuntimeReady = () => runtimeReady;
})();
