
var editorCode;
var editorOutput;
var samplesData;

var code;

var sampleList = {"examples":null, "tests":null};


pageInit = function () {

    $.getScript("daslang_static.js")


    editorCode = document.getElementById("code");
    editorOutput = document.getElementById("output");

    sampleList["examples"] = document.getElementById("examples");
    sampleList["tests"] = document.getElementById("tests");




    code = CodeMirror( editorCode, window.FORGE_PLAYGROUND_OPTS || {
        lineNumbers: true, matchBrackets: true, indentWithTabs: false, styleActiveLine: true,
        theme:'eclipse', mode:"application/javascript",
        tabSize: 4, indentUnit: 4, highlightSelectionMatches: {showToken: /\w/}
    });


     $.getJSON( "./samples/data.json", function(res) {
        samplesData = res;



         ["example","test"].forEach(function (n) {

             let ll = document.getElementById(n+"s");
             while (ll.firstChild) {
                 ll.removeChild(ll.lastChild);
             }

             for (let i=0;i<samplesData[n+"s"].length+1;i++)
             {
                 let newO = document.createElement("option");
                 if (i===0)
                 {

                     newO.innerText = "Select "+n;
                     newO.value = "init";
                 }
                 else
                 {
                     newO.innerText = samplesData[n+"s"][i-1].name;
                     newO.value = i-1;

                 }
                 ll.appendChild(newO);
             }
         });

         selectSample("examples",0);

    });


}

selectSample = function(type, id) {
    let vv = id !== undefined ? id : parseInt(sampleList[type].value);
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
    sampleList[type].value = "init";
}

// Apply a {filename: content} bundle to the editor. Today: surfaces only the
// main.das (or the first file) in the single CM instance. Phase 3 routes this
// through pgState + the tab strip so all files are addressable from the UI.
loadSample = function(filesByName) {
    const names = Object.keys(filesByName);
    if (!names.length) return;
    const active = filesByName['main.das'] !== undefined ? 'main.das' : names[0];
    code.setValue(filesByName[active]);
    // Stash the full bundle so phase 3 can pick it up post-tab-strip.
    window.__pendingSampleBundle = filesByName;
}

runCode = function() {

    runScript(code.getValue());



}


runTests = function() {


    runTest(0);

}

var outputPool = [];

runTest = function(i) {

    $.get('./samples/'+samplesData["tests"][i].files[0], function(res) {

        printOutput("Running Test "+(i+1)+"/"+samplesData["tests"].length+": "+samplesData["tests"][i].name,"#bec7b6");

        outputPool = [];

        runScript(res,function () {


            let ok = true;



            if (outputPool.length<samplesData["tests"][i].correct_output.length)
                ok = false;
            else
                for (let o=0;o<samplesData["tests"][i].correct_output.length;o++) {
                    let correct = samplesData["tests"][i].correct_output[o];

                    if (Array.isArray(correct))
                    {
                        let outp = outputPool[o].split(' ');

                        if (outp.length<correct.length)
                            ok = false;
                        else
                            for (let k=0;k<correct.length;k++)
                                if (correct[k] !== null && outp[k] !== correct[k])
                                    ok = false;


                    }
                    else
                    {
                        if (correct !== null && outputPool[o] !== correct)
                            ok = false;
                    }

                }


            printOutput(samplesData["tests"][i].name+" Test "+(i+1)+"/"+samplesData["tests"].length+": "+(ok ? "SUCCESS" : "FAIL"),ok ? "#89db4a": '#ff9393');

            if (i<samplesData["tests"].length-1)
                runTest(i+1);
        });
    }, 'text');
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

                outputPool.push(text);
            };
        })(),
    }

window.onerror = function(message)
{

    printOutput(message,'#ff2d2d');


    printOutput("An error occurred, you may need to reload the page",'#ff9393');
}

