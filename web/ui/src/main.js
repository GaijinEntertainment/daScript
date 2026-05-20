
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

         // Skip the default sample if pgInit already restored state from URL
         // hash or localStorage autosave (otherwise the async fetch overwrites
         // the user's work ~200ms after page load).
         if (!window.pgRestoredFromState) {
             selectSample("examples", 0);
         }

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

runCode = function() {
    if (!isWasmReady()) {
        printOutput('daslang is still loading, please wait…', '#ff9393');
        return;
    }
    if (syncMemFsFromState()) {
        syncUrlToState();
        Module.callMain(['main.das']);
        return;
    }
    syncUrlToState();
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

