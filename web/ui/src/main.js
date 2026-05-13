
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

runCode = function() {
    // Multi-file: sync MEMFS with the current pgState (unlink stale, write
    // current), then run main.das. Falls back to the single-buffer path when
    // pgState isn't up yet.
    if (window.pgState && typeof FS !== 'undefined') {
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
        Module.callMain(['main.das']);
        return;
    }
    runScript(code.getValue());
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
    }

window.onerror = function(message)
{

    printOutput(message,'#ff2d2d');


    printOutput("An error occurred, you may need to reload the page",'#ff9393');
}

