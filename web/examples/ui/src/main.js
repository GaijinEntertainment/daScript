
var editorCode;
var editorOutput;
var samplesData;

var code;

var sampleList = {"examples":null};


pageInit = function () {

    $.getScript("daslang_static.js")


    editorCode = document.getElementById("code");
    editorOutput = document.getElementById("output");

    sampleList["examples"] = document.getElementById("examples");




    code = CodeMirror( editorCode, window.FORGE_PLAYGROUND_OPTS || {
        lineNumbers: true, matchBrackets: true, indentWithTabs: false, styleActiveLine: true,
        theme:'eclipse', mode:"application/javascript",
        tabSize: 4, indentUnit: 4, highlightSelectionMatches: {showToken: /\w/}
    });


     $.getJSON( "./samples/data.json", function(res) {
        samplesData = res;



         (function populateExamples() {
             const ll = document.getElementById("examples");
             if (!ll) return;
             while (ll.firstChild) ll.removeChild(ll.lastChild);
             const entries = samplesData["examples"] || [];
             for (let i = 0; i < entries.length + 1; i++) {
                 const opt = document.createElement("option");
                 if (i === 0) {
                     opt.innerText = "Select example";
                     opt.value = "init";
                 } else {
                     opt.innerText = entries[i - 1].name;
                     opt.value = i - 1;
                 }
                 ll.appendChild(opt);
             }
         })();

         // Selection priority:
         //   1. URL hash (#code= or #z=) restored by playground-tabs.js — wins.
         //   2. URL query (?example=<slug>) — slug = files[0]'s basename
         //      without the `.das` suffix (so `examples/sha256.das` → `sha256`).
         //      Used by the daslang.io § 01 cycler's "try <test> on the
         //      playground →" deep-link.
         //   3. Autosave from localStorage — handled by playground-tabs.js.
         //   4. Default first sample.
         if (!window.pgRestoredFromState) {
             const params = new URLSearchParams(window.location.search);
             const wanted = params.get("example");
             if (wanted) {
                 const entries = samplesData["examples"] || [];
                 const idx = entries.findIndex(function(e) {
                     return slugForSample(e) === wanted;
                 });
                 if (idx >= 0) {
                     selectSample("examples", idx);
                     return;
                 }
                 // Unknown slug — silently fall through to default.
             }
             selectSample("examples", 0);
         }

    });


}

// Derive a URL-friendly slug from a sample entry. Used by the ?example=
// query-string deep-link from daslang.io's § 01 bench cycler. The slug is
// the first file's basename without the `.das` suffix, which by convention
// matches the dasProfile bench test name (`sha256.das` → `sha256`, etc.).
function slugForSample(entry) {
    if (!entry || !entry.files || !entry.files.length) return null;
    const f = entry.files[0];
    const base = f.split('/').pop();
    return base.replace(/\.das$/, '');
}

// Current sample's JIT-wasm basename (null for multi-file samples or when the
// .wasm artifact is not present). Drives runJit() and the engine radio's
// disabled state.
var currentJitName = null;

function deriveJitName(files) {
    if (!files || files.length !== 1) return null;
    return files[0].split('/').pop().replace(/\.das$/, '');
}

function updateEngineAvailability(name) {
    const jitRadio = document.querySelector('input[name=engine][value=jit]');
    if (!jitRadio) return;
    // When JIT becomes unavailable for the new sample, fall back to the
    // interpreter — leaving the radio merely `disabled` but still `checked`
    // would keep selectedEngine() returning 'jit' and runCode() would 404
    // on the missing .wasm.
    const disableJit = () => {
        jitRadio.disabled = true;
        if (jitRadio.checked) {
            jitRadio.checked = false;
            const interpRadio = document.querySelector('input[name=engine][value=interpreter]');
            if (interpRadio) interpRadio.checked = true;
        }
    };
    if (!name) { disableJit(); return; }
    // Rapid sample-switching can land HEAD-fetch responses out of order
    // (HTTP/2). Gate the late .then/.catch on the sample still being current
    // — currentJitName is the source of truth for "what sample is loaded".
    fetch('./samples/examples/' + name + '.wasm', { method: 'HEAD' })
        .then(r => {
            if (name !== currentJitName) return;
            if (r.ok) jitRadio.disabled = false; else disableJit();
        })
        .catch(() => {
            if (name !== currentJitName) return;
            disableJit();
        });
}

selectSample = function(type, id) {
    const sel = sampleList[type];
    if (!sel && id === undefined) return;  // dropdown was removed; nothing to read
    let vv = id !== undefined ? id : parseInt(sel.value);
    if (!Number.isNaN(vv) && samplesData[type] && samplesData[type][vv]) {
        // Multi-file samples ship as files[] — load all in parallel, then hand
        // the bundle to the loader (single editor today, tab strip in phase 3).
        const files = samplesData[type][vv].files;
        currentJitName = deriveJitName(files);
        updateEngineAvailability(currentJitName);
        Promise.all(files.map(f =>
            $.ajax({ url: './samples/' + f, dataType: 'text' })
                .then(text => ({ name: f.split('/').pop(), text }))
        )).then(loaded => {
            const byName = Object.fromEntries(loaded.map(({ name, text }) => [name, text]));
            loadSample(byName);
        });
    }
    if (sel) sel.value = "init";
}

// Apply a {filename: content} bundle. Once the tab strip is mounted, route
// through pgLoadFiles so every file gets its own Doc. Before then (during
// pageInit's initial selectSample call), stash the bundle for pgInit to pick
// up when it polls in.
//
// Single-file samples are normalized to main.das (the entry callMain runs).
// Multi-file samples are expected to ship a main.das themselves.
loadSample = function(filesByName) {
    const names = Object.keys(filesByName);
    if (!names.length) return;
    let bundle = filesByName;
    if (names.length === 1 && names[0] !== 'main.das') {
        bundle = { 'main.das': filesByName[names[0]] };
    }
    if (typeof window.pgLoadFiles === 'function') {
        window.pgLoadFiles(bundle);
        return;
    }
    const active = bundle['main.das'] !== undefined ? 'main.das' : Object.keys(bundle)[0];
    code.setValue(bundle[active]);
    window.__pendingSampleBundle = bundle;
}

// Names of files we wrote to MEMFS on the previous run. Tracked so each new
// run can unlink files the user has since deleted or renamed — otherwise
// `require utils` would resolve stale code from the prior run, and the
// executed program no longer matches the visible tab state.
var __lastWrittenFiles = new Set();

// Sync the URL hash with the current playground state. Run is the natural
// "snapshot" moment — the address bar then IS the share link (no need to hunt
// for the share button), and Back/Forward navigate a real history of code
// changes (see the popstate listener in playground-init.js).
function syncUrlToState() {
    if (!window.pgBuildShareUrl) return;
    const url = window.pgBuildShareUrl();
    if (url && url !== location.href) history.pushState(null, '', url);
}

// Multi-file MEMFS sync: unlink stale, write current pgState files. Returns
// true when both prerequisites are ready (pgState mounted AND Emscripten's
// FS is up); false when either is still pending. Callers that fall back to
// the single-buffer path must check WASM readiness themselves before touching
// FS — see isWasmReady() and the runCode/runTests guards below.
function syncMemFsFromState() {
    if (!window.pgState || typeof FS === 'undefined') return false;
    const current = new Set(Object.keys(window.pgState.files));
    for (const stale of __lastWrittenFiles) {
        if (!current.has(stale)) {
            try { FS.unlink(stale); } catch (e) { /* ENOENT — ignore */ }
        }
    }
    for (const [name, doc] of Object.entries(window.pgState.files)) {
        FS.writeFile(name, doc.getValue());
    }
    __lastWrittenFiles = current;
    return true;
}

// WASM readiness: callMain + FS both exposed by the Emscripten module. False
// during the ~3-5s page-load window while daslang_static.{js,wasm} streams in,
// or indefinitely if the load fails. Both Run and Test buttons stay disabled
// until this flips to true (see updateButtonStates + Module.onRuntimeInitialized).
function isWasmReady() {
    return typeof FS !== 'undefined'
        && typeof Module === 'object' && Module !== null
        && typeof Module.callMain === 'function';
}

// Toggle Run + Test buttons. Run requires WASM ready; Test additionally
// requires a [test] annotation in any open buffer. Called from autosave
// (every buffer/state mutation) and from Module.onRuntimeInitialized (WASM
// finishes loading) so both signals refresh the gating.
var __TEST_ANNOT_RE = /\[\s*test\b/m;
function hasTestAnnotation() {
    if (window.pgState) {
        for (const doc of Object.values(window.pgState.files)) {
            if (__TEST_ANNOT_RE.test(doc.getValue())) return true;
        }
        return false;
    }
    if (typeof code === 'object' && code) {
        return __TEST_ANNOT_RE.test(code.getValue());
    }
    return false;
}
function updateButtonStates() {
    const ready = isWasmReady();
    const runBtn = document.getElementById('run');
    const testBtn = document.getElementById('test');
    if (runBtn) runBtn.disabled = !ready;
    if (testBtn) testBtn.disabled = !ready || !hasTestAnnotation();
}
// Kept under the old name so playground-tabs.js's existing autosave hook
// still works without churn — it triggers a full refresh.
window.updateTestButtonState = updateButtonStates;

function selectedEngine() {
    const el = document.querySelector('input[name=engine]:checked');
    return el ? el.value : 'interpreter';
}

runCode = function() {
    syncUrlToState();
    if (selectedEngine() === 'jit') {
        if (!currentJitName) {
            printOutput("JIT unavailable: no precompiled .wasm for this sample", '#ff9393');
            return;
        }
        runJit(currentJitName);
        return;
    }
    // Interpreter path. WASM readiness gate first — both Module.callMain and
    // the single-buffer fallback need FS up.
    if (!isWasmReady()) {
        printOutput('daslang is still loading, please wait…', '#ff9393');
        return;
    }
    if (syncMemFsFromState()) {
        Module.callMain(['main.das']);
        return;
    }
    runScript(code.getValue());
}

// Invoke dastest against the current main.das. `[test]` functions in the file
// are discovered + run by dastest/suite.das; the existing Module.print hook
// routes stdout into the output panel. No --color: Emscripten has no TERM and
// our printOutput doesn't render ANSI escapes. --timeout=0 disables dastest's
// wall-clock thread (suite.das wraps each file in new_thread when timeout>0),
// keeping the run single-threaded in the WASM build.
runTests = function() {
    if (!isWasmReady()) {
        printOutput('daslang is still loading, please wait…', '#ff9393');
        return;
    }
    if (!syncMemFsFromState()) {
        FS.writeFile('main.das', code.getValue());
    }
    syncUrlToState();
    Module.callMain(['/dastest/dastest.das', '--', '--test', '/main.das', '--timeout=0']);
}

// Minimal wasi_snapshot_preview1 shim — daslang STANDALONE_WASM output only
// touches stdout (fd_write), proc_exit, clock, args/environ stubs, and a few
// fd_* no-ops emscripten's libc emits at link time. See
// modules/dasLLVM/README.md "Cross-compilation" for the import surface.
function makeWasiShim(memoryRef) {
    let stdoutBuf = '';
    const decoder = new TextDecoder('utf-8');

    function mem() { return new DataView(memoryRef.buffer); }
    function u8() { return new Uint8Array(memoryRef.buffer); }

    function flushStdout(force) {
        // Flush by newline so each printed line gets its own output row.
        let nl;
        while ((nl = stdoutBuf.indexOf('\n')) >= 0) {
            printOutput(stdoutBuf.slice(0, nl), '#ffffff');
            stdoutBuf = stdoutBuf.slice(nl + 1);
        }
        if (force && stdoutBuf.length) {
            printOutput(stdoutBuf, '#ffffff');
            stdoutBuf = '';
        }
    }

    return {
        fd_write(fd, iovsPtr, iovsLen, nWrittenPtr) {
            const dv = mem();
            let total = 0;
            for (let i = 0; i < iovsLen; i++) {
                const off = iovsPtr + i * 8;
                const bufPtr = dv.getUint32(off, true);
                const bufLen = dv.getUint32(off + 4, true);
                const bytes = u8().subarray(bufPtr, bufPtr + bufLen);
                stdoutBuf += decoder.decode(bytes, { stream: true });
                total += bufLen;
            }
            dv.setUint32(nWrittenPtr, total, true);
            flushStdout(false);
            return 0;
        },
        fd_read() { return 0; },
        fd_close() { return 0; },
        fd_seek(fd, offLo, offHi, whence, newOffPtr) {
            const dv = mem();
            dv.setUint32(newOffPtr, 0, true);
            dv.setUint32(newOffPtr + 4, 0, true);
            return 0;
        },
        fd_fdstat_get() { return 0; },
        fd_fdstat_set_flags() { return 0; },
        fd_prestat_get() { return 8; /* BADF — stop probing preopens */ },
        fd_prestat_dir_name() { return 8; },
        args_sizes_get(argcPtr, argvBufSizePtr) {
            const dv = mem();
            dv.setUint32(argcPtr, 0, true);
            dv.setUint32(argvBufSizePtr, 0, true);
            return 0;
        },
        args_get() { return 0; },
        environ_sizes_get(envcPtr, envBufSizePtr) {
            const dv = mem();
            dv.setUint32(envcPtr, 0, true);
            dv.setUint32(envBufSizePtr, 0, true);
            return 0;
        },
        environ_get() { return 0; },
        clock_time_get(id, precision, timePtr) {
            const ns = BigInt(Date.now()) * 1000000n;
            mem().setBigUint64(timePtr, ns, true);
            return 0;
        },
        clock_res_get(id, resPtr) {
            mem().setBigUint64(resPtr, 1000000n, true);
            return 0;
        },
        random_get(bufPtr, bufLen) {
            const view = u8().subarray(bufPtr, bufPtr + bufLen);
            crypto.getRandomValues(view);
            return 0;
        },
        path_open() { return 8; },
        path_readlink() { return 8; },
        path_filestat_get() { return 8; },
        proc_exit(code) {
            flushStdout(true);
            throw new WasiExit(code);
        },
    };
}

function WasiExit(code) { this.code = code; }

function runJit(name) {
    // Shim's DataView is rebuilt per-call from memRef.buffer, so we can
    // create the shim before instantiation and patch the buffer once memory
    // is exported.
    const memRef = { buffer: new ArrayBuffer(0) };
    const shim = makeWasiShim(memRef);
    fetch('./samples/examples/' + name + '.wasm')
        .then(r => {
            if (!r.ok) throw new Error('HTTP ' + r.status + ' fetching ' + name + '.wasm');
            return r.arrayBuffer();
        })
        .then(bytes => WebAssembly.instantiate(bytes, {
            wasi_snapshot_preview1: shim,
            env: new Proxy({}, { get: (_, name) => name === 'memory' ? undefined : () => -1 }),
        }))
        .then(({ instance }) => {
            memRef.buffer = instance.exports.memory.buffer;
            const exports = instance.exports;
            try {
                if (typeof exports._initialize === 'function') exports._initialize();
                if (typeof exports._start === 'function') exports._start();
                else if (typeof exports.main === 'function') exports.main();
                else printOutput('JIT error: no entry point exported in wasm', '#ff2d2d');
            } catch (e) {
                if (!(e instanceof WasiExit)) {
                    printOutput('JIT error: ' + (e && e.message ? e.message : e), '#ff2d2d');
                }
            }
        })
        .catch(e => printOutput('JIT load error: ' + (e && e.message ? e.message : e), '#ff2d2d'));
}

clearOutput = function() {

    while (editorOutput.firstChild) {
        editorOutput.removeChild(editorOutput.lastChild);
    }
}


printOutput = function(text,color) {

    var currentdate = new Date();


    let out = document.createElement("div");
    out.classList.add("output_line");
    out.style.backgroundColor = color;


    let outTime = document.createElement("div");
    outTime.classList.add("output_line_time");
    outTime.classList.add("unselectable");
    outTime.innerText = currentdate.toISOString().substr(11, 12);

    out.appendChild(outTime);


    let outP = document.createElement("p");
    outP.classList.add("output_line_text");
    outP.innerText = text;

    out.appendChild(outP);


    editorOutput.appendChild(out);


    editorOutput.scrollTop = editorOutput.scrollHeight;



}


runScript = function(text,onComplete)
{


    FS.writeFile('main.das',text);
    Module.callMain(['main.das']);




    //printOutput(text,"#fff2d2");

    if (onComplete!==undefined)
        onComplete();
}







var Module = {
        preRun: [],
        postRun: [],
        print: (function() {

            return function(text) {

                if (arguments.length > 1)
                    text = Array.prototype.slice.call(arguments).join(' ');
                console.log(text);
                printOutput(text,'#ffffff');
            };
        })(),
        // Re-enable the Run / Test buttons once Emscripten has finished
        // loading daslang_static.wasm and exposing FS + callMain. Both buttons
        // ship disabled in index.html to avoid the early-click ReferenceError.
        onRuntimeInitialized: function() {
            if (typeof updateButtonStates === 'function') updateButtonStates();
        },
    }

window.onerror = function(message)
{

    printOutput(message,'#ff2d2d');


    printOutput("An error occurred, you may need to reload the page",'#ff9393');
}

