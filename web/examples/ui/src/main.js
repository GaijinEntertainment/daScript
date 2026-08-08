
var editorCode;
var editorOutput;
var samplesData;

var code;
var runHostEl;   // where run frames (interpreter) and page frames (wasm pages) stand

var sampleList = {"examples":null};


// (the AudioContext autoplay unlock moved into run-frame.html — the wasm that
// constructs the context lives there now, and the wrapper has to be installed in
// that window before daslang_static.js loads)


pageInit = function () {

    editorCode = document.getElementById("code");
    editorOutput = document.getElementById("output");

    // The runtime lives in run-frame.html, one frame per program — this page no
    // longer loads daslang_static.js or owns a canvas. See playground-runner.js
    // for why (a failed run used to poison every later run in the session).
    // The frame reports its own canvas visibility, so the getContext hook and
    // the graphics/text auto-detect moved in there with it.
    // Take the canvas's PLACE, not just its parent: the column is a flex row, so
    // a frame merely appended lands past #output on the far right and gets
    // shrunk. The host stands where the canvas stood and holds every frame.
    var oldCanvas = document.getElementById("canvas");
    var runHost = document.createElement("div");
    runHost.className = "pg-run-host";
    if (oldCanvas) oldCanvas.parentNode.replaceChild(runHost, oldCanvas);
    else editorOutput.parentNode.insertBefore(runHost, editorOutput);
    runHostEl = runHost;   // page-shape wasm artifacts (runWasmPage) share the canvas's place
    PlaygroundRunner.init(runHost, function (text, color) { printOutput(text, color); });
    initFpsToggle();

    sampleList["examples"] = document.getElementById("examples");




    code = CodeMirror( editorCode, window.FORGE_PLAYGROUND_OPTS || {
        lineNumbers: true, matchBrackets: true, indentWithTabs: false, styleActiveLine: true,
        theme:'eclipse', mode:"application/javascript",
        tabSize: 4, indentUnit: 4, highlightSelectionMatches: {showToken: /\w/}
    });


     // Offer the wasm engine only where it can actually work. Asked once per
     // page, not per sample: builds are content-addressed, so availability is
     // a property of the browser and the service, not of the loaded code.
     updateEngineAvailability();

     // Switching engines changes what Run needs, so re-gate on the switch.
     for (const radio of document.querySelectorAll('input[name=engine]')) {
         radio.addEventListener('change', updateButtonStates);
     }

     // "new" — one click back to a pristine sample. Drops the autosaved session
     // (playground-tabs.js owns the storage), then reloads the last selected
     // sample — or the default one when nothing was ever picked (a restored
     // autosave buffer never goes through selectSample).
     const newBtn = document.getElementById('new');
     if (newBtn) newBtn.addEventListener('click', function () {
         if (typeof window.pgResetSession === 'function') window.pgResetSession();
         clearOutput();
         const last = window.__pgLastSample;
         if (last && samplesData && samplesData[last.type] && samplesData[last.type][last.id]) {
             selectSample(last.type, last.id);
         } else {
             selectSample("examples", 0);
         }
     });

     // The curated list comes from the sample service (store-fed, phase 2 of
     // plans/dasweb_backend.md); the committed data.json remains the fallback
     // for the GH Pages mirror / an empty store on first boot.
     fetch('/api/samples/listed')
        .then(function(r) { if (!r.ok) throw new Error('listed ' + r.status); return r.json(); })
        .then(function(listed) {
            if (!Array.isArray(listed) || !listed.length) throw new Error('listed empty');
            const byCat = {};
            for (const e of listed) {
                const cat = e.category || 'examples';
                (byCat[cat] = byCat[cat] || []).push({
                    name: e.title,
                    files: [e.path || (e.hash + '.das')],
                    slug: e.slug || undefined,
                    hash: e.hash,
                    path: e.path || '',
                });
            }
            initSampleUi(byCat);
        })
        .catch(function() { $.getJSON('./samples/data.json', initSampleUi); });

     function initSampleUi(res) {
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
         //
         // Deferred to DOMContentLoaded because pgRestoredFromState is set by
         // playground-tabs.js from ITS DOMContentLoaded handler: this list
         // arrives over the network, and when it arrives first (a warm cache, a
         // local service, an empty store answering instantly) the default
         // sample would load over the user's restored buffer. tabs.js registers
         // its handler at parse time, so it always runs before this one; if the
         // event already fired, tabs.js has already run and the flag is final.
         whenTabsReady(function () {
             applyInitialSelection();
             // The buffer has now settled into whatever this visit should show.
             // Everything above is asynchronous, so without a signal there is no
             // way to tell "still loading" from "loaded and this is the content"
             // — which is exactly what a test, or anything scripting the page,
             // needs to know before it touches the editor.
             window.pgSamplesReady = true;
             document.dispatchEvent(new CustomEvent('pg-samples-ready'));
         });

         function applyInitialSelection() {
             if (window.pgRestoredFromState || editorHasContent()) {
                 // A restored buffer (autosave, share link, Back) never went
                 // through selectSample, so its asset manifest was never
                 // derived. Adopt the one saved beside the buffer — otherwise
                 // every asset-loading sample runs against an empty MEMFS and
                 // panics on the first file it opens, but only on a return
                 // visit, which reads as a random failure.
                 if (window.pgRestoredAssetsUrl) {
                     currentAssetsUrl = window.pgRestoredAssetsUrl;
                 }
                 return;
             }
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

    }


}

// Is there anything in the editor worth protecting? The sample list arrives
// over the network, so its default selection can land after someone has
// already started typing — sample zero must not overwrite their work. An
// untouched playground holds one empty buffer, which is what lets the default
// through on a first visit.
function editorHasContent() {
    try {
        const files = window.pgState && window.pgState.files;
        if (!files) return false;
        const names = Object.keys(files);
        if (names.length > 1) return true;
        return names.some(n => (files[n].getValue() || '').trim() !== '');
    } catch (e) {
        return false;   // no tab state yet — nothing to protect
    }
}

// Run fn once playground-tabs.js has decided the buffer's starting content
// (restored from a link, from autosave, or left empty for a default sample).
// Not DOMContentLoaded: tabs init waits for CodeMirror and can finish after
// it, and selecting a sample fetches asynchronously — so starting before tabs
// has spoken means finishing after it, on top of what it restored.
function whenTabsReady(fn) {
    if (window.pgTabsReady) {
        fn();
    } else {
        document.addEventListener('pg-tabs-ready', fn, { once: true });
    }
}

// Derive a URL-friendly slug from a sample entry. Used by the ?example=
// query-string deep-link (daslang.io's § 01 bench cycler, and the /examples
// page's embedded player). An explicit "slug" field wins — multi-file samples
// (the games) all share a main.das basename, so they MUST carry a slug to be
// addressable. Otherwise fall back to the first file's basename without `.das`
// (so `examples/sha256.das` → `sha256`, matching the dasProfile bench name).
function slugForSample(entry) {
    if (!entry) return null;
    if (entry.slug) return entry.slug;
    if (!entry.files || !entry.files.length) return null;
    const f = entry.files[0];
    const base = f.split('/').pop();
    return base.replace(/\.das$/, '');
}

// Some samples (e.g. the OpenGL deferred-shading tutorial) load external assets
// — a mesh + PBR texture set — via plain fopen(). On the web fopen() reads
// MEMFS, which starts empty, so the bytes must be fetched and written first.
// A sample opts in with a "<file>.das.assets.json" sidecar next to it: an array
// of repo-relative paths. We fetch each and write it into MEMFS at "/<path>",
// the exact cwd-relative path the tutorial's fopen() expects — the one and only
// web-vs-desktop delta (desktop fopen hits real disk). Mirrors the standalone
// gl_tutorial.html harness used to develop the rung.
var currentAssetsUrl = null;

function deriveAssetsUrl(files) {
    if (!files || files.length !== 1) return null;
    return './samples/' + files[0] + '.assets.json';
}

// Fetch the current sample's asset manifest (if any) and populate MEMFS. Idempotent
// per manifest URL — MEMFS persists for the page session, so the (potentially large)
// fetch happens once, not on every Run. Absent sidecar / a 404 is the normal case.
// Read the sidecar and hand the parsed list to the run frame, which does the
// fetching — MEMFS lives in the frame now, and it is born empty with every run,
// so the assets go in per run rather than once per page. The browser's HTTP
// cache absorbs the repeat cost.
async function collectAssetUrls() {
    if (!currentAssetsUrl) return [];
    try {
        const r = await fetch(currentAssetsUrl);
        if (!r.ok) return [];   // absent sidecar is the normal case
        const assets = await r.json();
        return assets.map(rel => './' + rel);
    } catch (e) {
        return [];  // transient network error — the next Run retries
    }
}

// Built artifacts are wasm64 (memory64) — browsers without memory64
// (Safari/iOS) cannot instantiate them, so the wasm engine stays disabled
// there no matter what the build service says. Detected once at load.
const WASM64_SUPPORTED = (() => {
    try {
        // Validate a minimal module declaring a 64-bit (memory64) memory:
        // \0asm | version | memory section {count=1, flags=0x04 (memory64), min=1}.
        // More robust than `new WebAssembly.Memory({index:'i64'})`, which can
        // false-positive on engines that silently ignore the unknown descriptor
        // field and hand back a wasm32 memory. validate() parses the memory64
        // flag, so it is true only where the engine truly supports it.
        return WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1]));
    } catch (e) {
        return false;
    }
})();

// Whether the wasm engine can be offered at all. Unlike the retired
// precompiled-artifact path, this no longer depends on which sample is loaded:
// builds are content-addressed, so any editor state — including a multi-file
// one — is buildable. What it does depend on is this browser (wasm64 artifacts
// need memory64) and the service having a live toolchain to build against.
function updateEngineAvailability() {
    const wasmRadio = document.querySelector('input[name=engine][value=wasm]');
    if (!wasmRadio) return;
    // Leaving the radio merely `disabled` but still `checked` would keep
    // selectedEngine() returning 'wasm' and send Run down a dead path.
    const disableWasm = () => {
        wasmRadio.disabled = true;
        if (wasmRadio.checked) {
            wasmRadio.checked = false;
            const interpRadio = document.querySelector('input[name=engine][value=interpreter]');
            if (interpRadio) interpRadio.checked = true;
        }
    };
    if (!WASM64_SUPPORTED || !window.pgWasmBuild) { disableWasm(); return; }
    window.pgWasmBuild.buildInfo()
        .then(info => { if (info.enabled) wasmRadio.disabled = false; else disableWasm(); })
        .catch(() => disableWasm());
}

// The fps badge is on by default: comparing the two engines is the reason it exists,
// and a counter you have to go and enable is one nobody turns on. A visitor who opts
// out keeps that across visits — a preference, not per-run state.
const FPS_PREF_KEY = 'pg-show-fps';
function initFpsToggle() {
    const box = document.getElementById('show-fps');
    if (!box) return;
    let on = true;
    try { on = localStorage.getItem(FPS_PREF_KEY) !== '0'; } catch (e) { /* storage blocked */ }
    box.checked = on;
    if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.setFpsVisible(on);
    box.addEventListener('change', function () {
        if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.setFpsVisible(box.checked);
        try { localStorage.setItem(FPS_PREF_KEY, box.checked ? '1' : '0'); } catch (e) { /* storage blocked */ }
    });
}

// Hide the run frame. Revealing and sizing it is the runner's own job — the
// frame posts canvas-visible the instant a program asks for a WebGL context,
// which is program-driven and so works for pasted code, not just samples
// flagged graphics in data.json.
function showCanvas(show) {
    if (!show) {
        const f = document.querySelector(".pg-run-frame");
        if (f) f.style.display = "none";
        destroyPageFrame();   // a page-shape wasm run goes with the canvas
    }
    const o = document.getElementById("output");
    if (o) o.classList.toggle("with-canvas", show);
}

// Page-shape wasm artifacts (GL/audio programs) run as their own document from
// the run origin — a full emscripten page with its js glue and canvas. The
// iframe is deliberately NOT sandboxed: the `sandbox` attribute makes its
// origin opaque, which breaks emscripten's same-origin worker spawning — the
// separate, cookie-less run origin is the isolation boundary. The allow list
// delegates cross-origin isolation so -pthread programs get SharedArrayBuffer.
var pageFrame = null;
var pageFrameFit = null;
var pageFrameMsg = null;
function destroyPageFrame() {
    if (pageFrameFit) {
        window.removeEventListener('resize', pageFrameFit);
        pageFrameFit = null;
    }
    if (pageFrameMsg) {
        window.removeEventListener('message', pageFrameMsg);
        pageFrameMsg = null;
    }
    if (pageFrame && pageFrame.parentNode) pageFrame.parentNode.removeChild(pageFrame);
    pageFrame = null;
}
function runWasmPage(files) {
    let htmlUrl = null;
    for (const u of files) if (/\.html$/.test(u)) htmlUrl = u;
    if (!htmlUrl) {
        printOutput('page build reported no html artifact', '#ff2d2d');
        return;
    }
    // Same clean-slate rule as the interpreter: the previous run's canvas and
    // page frame go before this one stands.
    if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.reset();
    destroyPageFrame();
    pageFrame = document.createElement('iframe');
    pageFrame.className = 'pg-page-frame';
    pageFrame.setAttribute('title', 'daslang wasm program');
    pageFrame.setAttribute('allow', 'cross-origin-isolated; autoplay; fullscreen');
    // Geometry is PlaygroundRunner.fitElement's job, not a percentage: this
    // frame stands in the run host, and the host sizes to its content — so a
    // width:100% here resolves against nothing and collapses to a tiny box.
    // The run frame has always been sized off the COLUMN for that reason.
    pageFrame.style.cssText =
        'display:block;border:1px solid #444;' +
        'background:#000;margin-top:20px;margin-bottom:6px;';
    pageFrame.src = htmlUrl;
    (runHostEl || editorOutput.parentNode).appendChild(pageFrame);
    // 3/4 is the run frame's starting aspect, used until the page reports its
    // real drawing buffer — the shell posts canvas-visible exactly as
    // run-frame.html does, which is what lets the canvas fill without bars.
    let pageAspect = 3 / 4;
    pageFrameFit = function () {
        if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.fitElement(pageFrame, pageAspect);
    };
    // Accept that report only from the frame we created, and only from the
    // origin its artifact came from.
    const pageOrigin = new URL(htmlUrl, location.href).origin;
    pageFrameMsg = function (ev) {
        if (!pageFrame || ev.source !== pageFrame.contentWindow || ev.origin !== pageOrigin) return;
        const m = ev.data;
        if (!m) return;
        // The artifact is cross-origin by design, so its own fps meter reporting in is
        // the only way this side can know the frame rate. The badge lives in the runner.
        if (m.type === 'fps') {
            if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.noteFps(m.value);
            return;
        }
        if (m.type !== 'canvas-visible' || !(m.width > 0) || !(m.height > 0)) return;
        pageAspect = m.height / m.width;
        pageFrameFit();
    };
    window.addEventListener('message', pageFrameMsg);
    pageFrameFit();
    window.addEventListener('resize', pageFrameFit);
    const o = document.getElementById('output');
    if (o) o.classList.add('with-canvas');
}

selectSample = function(type, id) {
    const sel = sampleList[type];
    if (!sel && id === undefined) return;  // dropdown was removed; nothing to read
    let vv = id !== undefined ? id : parseInt(sel.value);
    // samplesData arrives over the network - a "new" click can land before it does
    if (!Number.isNaN(vv) && samplesData && samplesData[type] && samplesData[type][vv]) {
        // Remembered so the "new" button can reload THIS sample fresh — a visitor
        // who deep-linked into f2s and hacked on it expects new = fresh f2s, not
        // the default sample.
        window.__pgLastSample = { type: type, id: vv };
        // Multi-file samples ship as files[] — load all in parallel, then hand
        // the bundle to the loader (single editor today, tab strip in phase 3).
        // Hide the canvas on every sample switch; a graphics program re-reveals
        // it on Run when it creates a WebGL context (the getContext hook).
        showCanvas(false);
        const entry = samplesData[type][vv];
        const files = entry.files;
        currentAssetsUrl = deriveAssetsUrl(files);
        // Static fallback only exists for entries that name a real file in the
        // shipped samples tree. A promoted user sample has no such path (the
        // store synthesized "<hash>.das"), so fetching it is a guaranteed 404.
        const hasStaticFiles = !entry.hash || !!entry.path;
        const loadFromStaticFiles = () => hasStaticFiles ? Promise.all(files.map(f =>
            $.ajax({ url: './samples/' + f, dataType: 'text' })
                .then(text => ({ name: f.split('/').pop(), text }))
        )).then(loaded => {
            const byName = Object.fromEntries(loaded.map(({ name, text }) => [name, text]));
            loadSample(byName);
        }) : Promise.resolve();
        if (entry.hash) {
            // Store-fed entry: one fetch; multi-file samples arrive as the
            // same {files, active} bundle user shares store. Static tree is
            // the fallback (mirror / store hiccup).
            fetch('/api/samples/' + entry.hash)
                .then(r => { if (!r.ok) throw new Error('sample ' + r.status); return r.text(); })
                .then(text => {
                    let bundle = null;
                    try { const o = JSON.parse(text); if (o && o.files) bundle = o.files; } catch (e) { /* single file */ }
                    loadSample(bundle || { 'main.das': text });
                })
                .catch(loadFromStaticFiles);
        } else {
            loadFromStaticFiles();
        }
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

// (stale-file tracking is gone: every run gets a fresh MEMFS in a fresh frame,
// so a deleted or renamed tab simply isn't written the next time)

// Sync the URL hash with the current playground state. Run is the natural
// "snapshot" moment — the address bar then IS the share link (no need to hunt
// for the share button), and Back/Forward navigate a real history of code
// changes (see the popstate listener in playground-init.js).
function syncUrlToState() {
    if (!window.pgBuildShareUrl) return;
    const url = window.pgBuildShareUrl();
    if (url && url !== location.href) history.pushState(null, '', url);
}

// Snapshot the program as a {name: text} map for the run frame. Every run gets
// a brand-new MEMFS inside a brand-new frame, so there is nothing stale to
// unlink — deleting a tab simply means the file is absent from the next run.
function collectProgramFiles() {
    if (window.pgState && window.pgState.files) {
        const out = {};
        for (const [name, doc] of Object.entries(window.pgState.files)) {
            out[name] = doc.getValue();
        }
        if (Object.keys(out).length) return out;
    }
    return { 'main.das': code.getValue() };
}

// WASM readiness: callMain + FS both exposed by the Emscripten module. False
// during the ~3-5s page-load window while daslang_static.{js,wasm} streams in,
// or indefinitely if the load fails. Both Run and Test buttons stay disabled
// until this flips to true (see updateButtonStates + Module.onRuntimeInitialized).
function isWasmReady() {
    return typeof PlaygroundRunner !== 'undefined' && PlaygroundRunner.isReady();
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
    // Run gates on what the SELECTED engine needs. The interpreter needs the
    // local runtime loaded; the wasm engine does not — it compiles remotely and
    // instantiates the artifact itself, and its radio is only selectable once
    // the build service has answered. Gating both on the local runtime left Run
    // dead for an engine that never uses it.
    if (runBtn) runBtn.disabled = selectedEngine() === 'wasm' ? false : !ready;
    // Test always runs interpreted, through the local runtime.
    if (testBtn) testBtn.disabled = !ready || !hasTestAnnotation();
}
// Kept under the old name so playground-tabs.js's existing autosave hook
// still works without churn — it triggers a full refresh.
window.updateTestButtonState = updateButtonStates;

function selectedEngine() {
    const el = document.querySelector('input[name=engine]:checked');
    return el ? el.value : 'interpreter';
}

// (stdout flushing moved into run-frame.html, where FS now lives)

runCode = async function() {
    syncUrlToState();
    if (selectedEngine() === 'wasm') {
        await runWasm();
        return;
    }
    if (!isWasmReady()) {
        printOutput('daslang is still loading, please wait…', '#ff9393');
        return;
    }
    // Each run gets a fresh frame, so the previous program's canvas, GL context,
    // module registry and MEMFS are gone before this one starts.
    showCanvas(false);
    PlaygroundRunner.run(collectProgramFiles(), ['main.das'], await collectAssetUrls());
}

// Invoke dastest against the current main.das. `[test]` functions in the file
// are discovered + run by dastest/suite.das; the existing Module.print hook
// routes stdout into the output panel. No --color: Emscripten has no TERM and
// our printOutput doesn't render ANSI escapes. --timeout=0 disables dastest's
// wall-clock thread (suite.das wraps each file in new_thread when timeout>0),
// keeping the run single-threaded in the WASM build.
runTests = async function() {
    if (!isWasmReady()) {
        printOutput('daslang is still loading, please wait…', '#ff9393');
        return;
    }
    syncUrlToState();
    showCanvas(false);
    PlaygroundRunner.run(
        collectProgramFiles(),
        ['/dastest/dastest.das', '--', '--test', '/main.das', '--timeout=0'],
        await collectAssetUrls());
}

// Minimal wasi_snapshot_preview1 shim — daslang STANDALONE_WASM output only
// touches stdout (fd_write), proc_exit, clock, args/environ stubs, and a few
// fd_* no-ops emscripten's libc emits at link time. See
// modules/dasLLVM/README.md "Cross-compilation" for the import surface.
//
// Built artifacts are compiled to wasm64 (memory64). Under memory64 every
// pointer/size argument arrives as a BigInt (not a Number), the iovec struct
// fields are 8 bytes (not 4), and `__wasi_size_t` outputs are 8 bytes — so the
// shim must coerce BigInt offsets for DataView and write 64-bit sizes. The
// helpers below keep it correct for both wasm64 and (legacy) wasm32 modules.
function makeWasiShim(memoryRef) {
    let stdoutBuf = '';
    const decoder = new TextDecoder('utf-8');

    function mem() { return new DataView(memoryRef.buffer); }
    function u8() { return new Uint8Array(memoryRef.buffer); }

    // memory64 passes pointer/size args as BigInt; wasm32 passes Number. Coerce
    // to a Number for DataView offsets and array slicing — playground linear
    // memory is far below 2^53, so the narrowing is lossless.
    const N = (x) => (typeof x === 'bigint' ? Number(x) : x);

    // A WASI `__wasi_size_t` output is 4 bytes under wasm32 and 8 bytes under
    // wasm64 (it is size_t). Pick the width from whether the destination
    // pointer arrived as a BigInt (memory64).
    function putSize(dv, ptr, val) {
        if (typeof ptr === 'bigint') dv.setBigUint64(Number(ptr), BigInt(val), true);
        else dv.setUint32(ptr, val, true);
    }

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
            const wide = typeof iovsPtr === 'bigint';
            const base = N(iovsPtr);
            const count = N(iovsLen);
            // iovec = { buf, buf_len }: two pointer-sized fields — 4 bytes each
            // under wasm32, 8 bytes each under wasm64.
            const fsz = wide ? 8 : 4;
            let total = 0;
            for (let i = 0; i < count; i++) {
                const off = base + i * fsz * 2;
                const bufPtr = wide ? Number(dv.getBigUint64(off, true)) : dv.getUint32(off, true);
                const bufLen = wide ? Number(dv.getBigUint64(off + fsz, true)) : dv.getUint32(off + fsz, true);
                const bytes = u8().subarray(bufPtr, bufPtr + bufLen);
                stdoutBuf += decoder.decode(bytes, { stream: true });
                total += bufLen;
            }
            putSize(dv, nWrittenPtr, total);
            flushStdout(false);
            return 0;
        },
        fd_read() { return 0; },
        fd_close() { return 0; },
        fd_seek(fd, offset, whence, newOffPtr) {
            // __wasi_filesize_t result is u64 on both wasm32 and wasm64.
            mem().setBigUint64(N(newOffPtr), 0n, true);
            return 0;
        },
        fd_fdstat_get() { return 0; },
        fd_fdstat_set_flags() { return 0; },
        fd_prestat_get() { return 8; /* BADF — stop probing preopens */ },
        fd_prestat_dir_name() { return 8; },
        args_sizes_get(argcPtr, argvBufSizePtr) {
            const dv = mem();
            putSize(dv, argcPtr, 0);
            putSize(dv, argvBufSizePtr, 0);
            return 0;
        },
        args_get() { return 0; },
        environ_sizes_get(envcPtr, envBufSizePtr) {
            const dv = mem();
            putSize(dv, envcPtr, 0);
            putSize(dv, envBufSizePtr, 0);
            return 0;
        },
        environ_get() { return 0; },
        clock_time_get(id, precision, timePtr) {
            const ns = BigInt(Date.now()) * 1000000n;
            mem().setBigUint64(N(timePtr), ns, true);
            return 0;
        },
        clock_res_get(id, resPtr) {
            mem().setBigUint64(N(resPtr), 1000000n, true);
            return 0;
        },
        random_get(bufPtr, bufLen) {
            const view = u8().subarray(N(bufPtr), N(bufPtr) + N(bufLen));
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

// Compile the editor's code on the build service, then run what came back.
// The first build of a given source pays a real compile; the same source
// afterwards is a cache hit, because the build is keyed by content hash.
async function runWasm() {
    if (!window.pgWasmBuild) {
        printOutput('wasm engine unavailable: build client not loaded', '#ff9393');
        return;
    }
    // A build is not instant, so the button must not invite a second one.
    const runBtn = document.getElementById('run');
    const wasBusy = runBtn ? runBtn.disabled : false;
    if (runBtn) runBtn.disabled = true;
    try {
        const result = await window.pgWasmBuild.build(line => printOutput(line, '#9aa0a6'));
        if (!result.ok) {
            // A compiler error is the user's own code talking — show it as-is,
            // not decorated as an infrastructure failure.
            printOutput(result.error, result.compileError ? '#ff9393' : '#ff2d2d');
            return;
        }
        // The build's kind decides the delivery: a bare wasi module instantiates
        // right here; a page-shape artifact (GL/audio) runs as its own document.
        if (result.kind === 'page') runWasmPage(result.files);
        else runWasmArtifact(result.files[0]);
    } catch (e) {
        printOutput('wasm build error: ' + (e && e.message ? e.message : e), '#ff2d2d');
    } finally {
        if (runBtn) runBtn.disabled = wasBusy;
    }
}

function runWasmArtifact(url) {
    // Every run starts from a clean slate, page frames included: a sample edited
    // from graphics to compute-only builds as a module, and without this the
    // previous run's page sits there rendering while this one prints — the same
    // "my run drew nothing" confusion the run frame was introduced to end.
    destroyPageFrame();
    // Shim's DataView is rebuilt per-call from memRef.buffer, so we can
    // create the shim before instantiation and wire the buffer once memory
    // is exported. The wire must be a LIVE getter, not a snapshot: artifacts
    // link with -sALLOW_MEMORY_GROWTH, and the first grow detaches the old
    // ArrayBuffer — a cached reference then throws "Cannot perform DataView
    // constructor on a detached ArrayBuffer" on the next fd_write.
    let wasmMemory = null;
    const memRef = { get buffer() { return wasmMemory ? wasmMemory.buffer : new ArrayBuffer(0); } };
    const shim = makeWasiShim(memRef);
    fetch(url)
        .then(r => {
            if (!r.ok) throw new Error('HTTP ' + r.status + ' fetching the build artifact');
            return r.arrayBuffer();
        })
        .then(bytes => WebAssembly.instantiate(bytes, {
            wasi_snapshot_preview1: shim,
            env: new Proxy({}, { get: (_, name) => name === 'memory' ? undefined : () => -1 }),
        }))
        .then(({ instance }) => {
            wasmMemory = instance.exports.memory;
            const exports = instance.exports;
            try {
                if (typeof exports._initialize === 'function') exports._initialize();
                if (typeof exports._start === 'function') exports._start();
                else if (typeof exports.main === 'function') exports.main();
                else printOutput('wasm error: no entry point exported', '#ff2d2d');
            } catch (e) {
                if (!(e instanceof WasiExit)) {
                    printOutput('wasm error: ' + (e && e.message ? e.message : e), '#ff2d2d');
                }
            }
        })
        .catch(e => printOutput('wasm load error: ' + (e && e.message ? e.message : e), '#ff2d2d'));
}

clearOutput = function() {

    while (editorOutput.firstChild) {
        editorOutput.removeChild(editorOutput.lastChild);
    }
    // Clear means clear: drop the frame so the previous program's canvas and its
    // WebGL context go with it. Previously the canvas stuck around showing a dead
    // program's last frame, which reads as "my run drew nothing".
    if (typeof PlaygroundRunner !== 'undefined') PlaygroundRunner.reset();
    showCanvas(false);
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
    PlaygroundRunner.run({ 'main.das': text }, ['main.das'], []);

    if (onComplete!==undefined)
        onComplete();
}







// No Module here any more: the wasm runtime, its stdout hooks and its readiness
// signal all live in run-frame.html. playground-runner.js calls
// updateButtonStates() when a frame reports ready.

window.onerror = function(message)
{

    printOutput(message,'#ff2d2d');


    printOutput("An error occurred, you may need to reload the page",'#ff9393');
}

